#ifndef READ_DATA_PART_C
#define READ_DATA_PART_C
#include "../main.c"

EFI_STATUS open_info_file(EFI_HANDLE image_handle, CHAR16* filename[], VOID* buffer, UINT64* buf_size){
    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL *lip;
    EFI_STATUS status;

    status = bs->OpenProtocol(image_handle, &lip_guid, (VOID **)&lip, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if (EFI_ERROR(status)){
        EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
        printf(EFI_STOP, u"Can't open the Load Image Protocol. Error %d\n\r", status);

        bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);

        return status;
    }

    // Open the Simple File System Protocol
    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *sfsp;

    status = bs->OpenProtocol(lip->DeviceHandle, &sfsp_guid, (VOID **)&sfsp, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);

    if (EFI_ERROR(status)){
        EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
        printf(EFI_STOP, u"Can't open the Simple File System Protcol. Error %d\n\r", status);

        bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
        bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);

        return status;
    }
    
    // Open root dir, loop until new dir selected
    EFI_FILE_PROTOCOL *current_directory_pointer;
    status = sfsp->OpenVolume(sfsp, &current_directory_pointer);

    if (EFI_ERROR(status)){
        EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
        printf(EFI_STOP, u"Can't open the root Volume. Error %d\n\r", status);

        bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
        bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
        current_directory_pointer->Close(current_directory_pointer);

        return status;
    }

    EFI_FILE_PROTOCOL* dirp;
    for (UINTN i = 0; i < 3; i++){
        printf(EFI_STOP, filename[i]);
        status = current_directory_pointer->Open(current_directory_pointer, &dirp, filename[i], EFI_FILE_MODE_READ, 0);

        bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
        bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
        current_directory_pointer->Close(current_directory_pointer);
        dirp->Close(dirp);
        if (EFI_ERROR(status)){
            draw_esp_directory(current_directory_pointer, 0);
            EFI_STOP->SetCursorPosition(EFI_STOP, 0, 0);
            EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));

            printf(EFI_STOP, u"Can't open the Disk info file in the esp partition. Error %d\n\r", status);

            bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
            bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
            current_directory_pointer->Close(current_directory_pointer);

            return status;
        }
    }

    EFI_FILE_INFO* file_info = get_current_esp_file_info(dirp);
    buf_size = &file_info->FileSize;
    bs->AllocatePool(EfiLoaderData, *buf_size, &buffer);

    status = dirp->Read(dirp, buf_size, buffer);
    if (EFI_ERROR(status)){
        EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
        printf(EFI_STOP, u"Can't read the data partition info. Error %d\n\r", status);

        bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
        bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
        current_directory_pointer->Close(current_directory_pointer);
        dirp->Close(dirp);

        return status;
    }

    bs->CloseProtocol(image_handle, &lip_guid, image_handle, NULL);
    bs->CloseProtocol(image_handle, &sfsp_guid, image_handle, NULL);
    current_directory_pointer->Close(current_directory_pointer);
    dirp->Close(dirp);
    return EFI_SUCCESS;
}

EFI_STATUS read_data_partition(){
    EFI_STOP->ClearScreen(EFI_STOP);
    EFI_STOP->OutputString(EFI_STOP, u"Files in Data part: \n\r");
    EFI_STATUS status;

    VOID* buffer = NULL;
    UINT64* buf_size = 0;
    CHAR16 filename[3];
    filename[0] = u"EFI";
    filename[1] = u"BOOT";
    filename[2] = u"DISK.INF";
    status = open_info_file(image_handle, &filename, buffer, buf_size);

    if (EFI_ERROR(status)){
        EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
        printf(EFI_STOP, u"Can't open info file. Error %d \n\r", status);
        printf(EFI_STOP, u"Press eny key to return ...\n\r");

        UINTN Index;
        EFI_EVENT WaitList[1];
        WaitList[0] = EFI_STIP->WaitForKey;
        bs->WaitForEvent(1, WaitList, &Index);

        return status;
    }

    char* pos = (char *)buffer;
    for (UINTN bytes = *buf_size; bytes > 0; bytes--){
        CHAR16 str[2];
        str[0] = *pos;
        str[1] = u'\0';
        if (*pos == '\n'){
            printf(EFI_STOP, u"\r\n  ");
        } else {
            printf(EFI_STOP, u"%s", str);
        }

        pos++;
    }

    UINTN Index;
    EFI_EVENT WaitList[1];
    WaitList[0] = EFI_STIP->WaitForKey;
    bs->WaitForEvent(1, WaitList, &Index);

    return EFI_SUCCESS;
}
#endif