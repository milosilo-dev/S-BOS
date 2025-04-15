#ifndef GOP_C
#define GOP_C
#include "../main.c"

#pragma pack(push, 1)
typedef struct {
    UINT16 bfType;
    UINT32 bfSize;
    UINT16 bfReserved1;
    UINT16 bfReserved2;
    UINT32 bfOffBits;
} BMP_FILE_HEADER;

typedef struct {
    UINT32 biSize;
    INT32  biWidth;
    INT32  biHeight;
    UINT16 biPlanes;
    UINT16 biBitCount;
    UINT32 biCompression;
    UINT32 biSizeImage;
    INT32  biXPelsPerMeter;
    INT32  biYPelsPerMeter;
    UINT32 biClrUsed;
    UINT32 biClrImportant;
} BMP_INFO_HEADER;
#pragma pack(pop)

typedef struct {
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    UINTN height;
    UINTN width;
    UINT32* framebuffer;
} GOP_OBJ;

EFI_GRAPHICS_OUTPUT_PROTOCOL* get_gop(){
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

    bs->LocateProtocol(&gop_guid, NULL, (VOID **)&gop);
    return gop;
}

GOP_OBJ mk_gop_obj(EFI_GRAPHICS_OUTPUT_PROTOCOL* gop, UINTN height, UINTN width, UINT32* framebuffer){
    GOP_OBJ gop_obj;
    gop_obj.gop = gop;
    gop_obj.height = height;
    gop_obj.width = width;
    gop_obj.framebuffer = framebuffer;
    return gop_obj;
}

VOID clear_screen(UINT32 color, GOP_OBJ gop_obj){
    for (UINTN y = 0; y < gop_obj.height; y++) {
        for (UINTN x = 0; x < gop_obj.width; x++) {
            gop_obj.framebuffer[y * gop_obj.width + x] = color;
        }
    }
}

VOID draw_image(UINT16 x_off, UINT16 y_off, GOP_OBJ gop_ojb, CHAR16* path, UINT32 bg_color, BOOLEAN key){
    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL *lip;
    EFI_STATUS status = 0;

    bs->OpenProtocol(image_handle, &lip_guid, (VOID **)&lip, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    // Open the Simple File System Protocol
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;

    bs->OpenProtocol(lip->DeviceHandle, &sfsp_guid, (VOID **)&sfsp, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    
    // Open root dir, loop until new dir selected
    EFI_FILE_PROTOCOL *current_directory_pointer;
    sfsp->OpenVolume(sfsp, &current_directory_pointer);

    EFI_FILE_PROTOCOL* file;
    status = current_directory_pointer->Open(current_directory_pointer, &file, path, EFI_FILE_MODE_READ, 0);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Can't open the file: %d", status);
        return;
    }

    // Read BMP headers
    BMP_FILE_HEADER fileHeader;
    BMP_INFO_HEADER infoHeader;
    UINTN headerSize = sizeof(fileHeader);
    file->Read(file, &headerSize, &fileHeader);
    
    headerSize = sizeof(infoHeader);
    file->Read(file, &headerSize, &infoHeader);

    if (fileHeader.bfType != 0x4D42) {
        printf(EFI_STOP, u"Not a valid BMP file\n");
        return;
    }

    UINT32 imageWidth = infoHeader.biWidth;
    UINT32 imageHeight = infoHeader.biHeight;
    UINT32 bytesPerPixel = infoHeader.biBitCount / 8;
    UINTN imageSize = imageWidth * imageHeight * bytesPerPixel;

    // Skip to pixel data
    UINTN skipBytes = fileHeader.bfOffBits - sizeof(fileHeader) - sizeof(infoHeader);
    if (skipBytes > 0) {
        UINT8 dummy[skipBytes];
        UINTN skip = skipBytes;
        status = file->Read(file, &skip, dummy);

        if (EFI_ERROR(status)){
            printf(EFI_STOP, u"Can't read the file: %d", status);
            return;
        }
    }

    // Read pixel data
    UINT8 *imageData;
    status = bs->AllocatePool(EfiLoaderData, imageSize, (void**)&imageData);

    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Can't allocate memory for image: %d", status);
        return;
    }

    UINTN readSize = imageSize;
    status = file->Read(file, &readSize, imageData);

    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Can't read the file: %d", status);
        return;
    }

    // Draw to framebuffer
    UINT32 *framebuffer = gop_ojb.framebuffer;
    UINTN fbWidth = gop_ojb.width;

    for (UINTN y = 0; y < imageHeight; y++) {
        for (UINTN x = 0; x < imageWidth; x++) {
            UINTN bmpIndex = ((imageHeight - 1 - y) * imageWidth + x) * bytesPerPixel;
            UINT8 b = imageData[bmpIndex + 0];
            UINT8 g = imageData[bmpIndex + 1];
            UINT8 r = imageData[bmpIndex + 2];

            UINT32 color = (r << 16) | (g << 8) | b;

            if (color == bg_color && key)
                continue;

            if (x < gop_ojb.width && y < gop_ojb.height) {
                framebuffer[(y + y_off) * fbWidth + (x + x_off)] = color;
            }
        }
    }

    // Free memory
    bs->FreePool(imageData);
}

#endif