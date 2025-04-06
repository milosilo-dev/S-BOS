#ifndef File_Manager_C
#define File_Manager_C
#include "../main.c"

VOID draw_directory(EFI_FILE_PROTOCOL* current_directory_pointer, UINT8 selected_dir)
{
    EFI_STOP->ClearScreen(EFI_STOP);

    CHAR16 current_directory[256];

    strcpy_u16(current_directory, u"/");
    EFI_FILE_INFO file_info;
    UINTN buf_size = sizeof file_info;
    UINT8 index = 0;

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 2, 2);
    printf(EFI_STOP, u"%s: \n\r ", current_directory);

    current_directory_pointer->SetPosition(current_directory_pointer, 0);
    current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);
    while (buf_size > 0){
        if (selected_dir == index)
            EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
        else
            EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));

        printf(EFI_STOP, u"%s %s\r\n ", (file_info.Attribute & EFI_FILE_DIRECTORY) ? u" <DIR> " : u" <FILE>", file_info.FileName);

        buf_size = sizeof file_info;
        current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);
        index++;
    }

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_RED,EFI_BLACK));
    BOOLEAN filled = FALSE;
    for (UINT8 x = 0; x < 60; x++){
        DrawBox(x, 0, filled);
        DrawBox(x, 20, filled);
        filled = !filled;
    }
    for (UINT8 y = 0; y < 21; y++){
        DrawBox(0, y, filled);
        DrawBox(60, y, filled);
        filled = !filled;
    }
}

UINTN get_file_num(EFI_FILE_PROTOCOL* current_directory_pointer){
    EFI_FILE_INFO file_info;
    UINTN buf_size = sizeof file_info;
    UINT8 index = 0;

    current_directory_pointer->SetPosition(current_directory_pointer, 0);
    current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);

    while (buf_size > 0)
    {
        buf_size = sizeof file_info;
        current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);
        index++;
    }
    return index;
}

EFI_FILE_INFO get_file_info(EFI_FILE_PROTOCOL* current_directory_pointer, UINTN selected_item){
    EFI_FILE_INFO file_info;
    UINTN buf_size = sizeof file_info;
    UINT8 index = 0;

    current_directory_pointer->SetPosition(current_directory_pointer, 0);
    current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);

    while (buf_size > 0)
    {
        if (index == selected_item)
            break;

        buf_size = sizeof file_info;
        current_directory_pointer->Read(current_directory_pointer, &buf_size, &file_info);
        index++;
    }
    return file_info;
}

BOOLEAN open_directory(EFI_FILE_PROTOCOL* current_directory_pointer);

UINT8 open_new_directory(EFI_FILE_PROTOCOL* current_directory_pointer, UINTN selected_dir)
{
    EFI_FILE_PROTOCOL *new_dir;
    current_directory_pointer->Open(current_directory_pointer, &new_dir, get_file_info(current_directory_pointer, selected_dir).FileName, EFI_FILE_MODE_READ, 0);
    current_directory_pointer->Close(current_directory_pointer);
    current_directory_pointer = new_dir;

    EFI_GUID FileInfoGuid = EFI_FILE_INFO_ID;
    UINTN InfoSize = 0;
    current_directory_pointer->GetInfo(current_directory_pointer, &FileInfoGuid, &InfoSize, NULL);
    EFI_FILE_INFO *FileInfo;
    bs->AllocatePool(EfiLoaderData, InfoSize, (VOID **)&FileInfo);
    current_directory_pointer->GetInfo(current_directory_pointer, &FileInfoGuid, &InfoSize, FileInfo);
    CHAR16* filename = FileInfo->FileName;

    if (open_directory(current_directory_pointer) == TRUE)
        return 1;

    current_directory_pointer->Open(current_directory_pointer, &new_dir, filename, EFI_FILE_MODE_READ, 0);
    current_directory_pointer->Close(current_directory_pointer);
    current_directory_pointer = new_dir;
    return 0;
}

// 0 -> Drop to next directory
// 1 -> Drop back to main menu
BOOLEAN open_directory(EFI_FILE_PROTOCOL* current_directory_pointer)
{    
    UINT8 return_value = 2;
    UINTN Index;
    UINTN selected_dir = 0;
    UINTN num_files = get_file_num(current_directory_pointer);

    draw_directory(current_directory_pointer, selected_dir);

    while (return_value == 2) 
    {
        EFI_EVENT WaitList[1];
        WaitList[0] = EFI_STIP->WaitForKey;
        bs->WaitForEvent(1, WaitList, &Index);

        EFI_INPUT_KEY l_key;
        EFI_STIP->ReadKeyStroke(EFI_STIP, &l_key);
        switch (l_key.ScanCode){
            case 0x01:
                selected_dir--;
                if (selected_dir > num_files){
                    selected_dir = 0;
                }
                break;
            case 0x02:
                selected_dir++;
                if (selected_dir >= num_files){
                    selected_dir = num_files - 1;
                }
                break;
            case 0x0017:
                return 1;
                break;
        }
        if (l_key.UnicodeChar == 0xD){
            if (open_new_directory(current_directory_pointer, selected_dir) == 1)
            {
                return 1;
            }
        }

        EFI_STIP->Reset(EFI_STIP, FALSE);
        draw_directory(current_directory_pointer, selected_dir);
    }

    return 0;
}

// Entry point
UINT8 file_manager(){
    // Open the Loaded Image Protocol
    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL *lip;

    bs->OpenProtocol(image_handle, &lip_guid, (VOID **)&lip, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    // Open the Simple File System Protocol
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;

    bs->OpenProtocol(lip->DeviceHandle, &sfsp_guid, (VOID **)&sfsp, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    
    // Open root dir, loop until new dir selected
    EFI_FILE_PROTOCOL *current_directory_pointer;
    sfsp->OpenVolume(sfsp, &current_directory_pointer);

    // new dir will be opened in a function
    open_directory(current_directory_pointer);
    
    bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
    bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
    current_directory_pointer->Close(current_directory_pointer);
    return 10;
}
#endif