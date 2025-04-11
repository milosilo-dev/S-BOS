#ifndef GOP_C
#define GOP_C
#include "../main.c"

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

#endif