#ifndef READ_DATA_PART_C
#define READ_DATA_PART_C
#include "../../main.c"
#include "gop.c"

CHAR16* draw_data_part(VOID* buffer, UINTN buf_size){
    CHAR16* str1;

    char* pos = (char *)buffer;
    for (UINTN bytes = buf_size; bytes > 0; bytes--){
        str1[buf_size - bytes] = *pos;
        pos++;
    }

    return str1;
}

EFI_STATUS open_data_file_to_buffer(CHAR16** p_str, UINTN* p_buf_size){
    EFI_STATUS Status;
    EFI_LOADED_IMAGE_PROTOCOL *LoadedImage;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *SimpleFS;
    EFI_FILE_PROTOCOL *RootDir;

    EFI_GUID im_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    Status = bs->HandleProtocol(
        image_handle,
        &im_guid,
        (VOID **)&LoadedImage);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open the LOADED IMAGE PROTOCOL \n\r");
        return Status;
    };

    EFI_GUID sfsp_guid = EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID;
    Status = bs->HandleProtocol(
        LoadedImage->DeviceHandle,
        &sfsp_guid,
        (VOID **)&SimpleFS);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open the SIMPLE FILE SYSTEM PROTOCOL \n\r");
        return Status;
    };

    Status = SimpleFS->OpenVolume(SimpleFS, &RootDir);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open volume from SimpleFS \n\r");
        return Status;
    };
    
    EFI_FILE_PROTOCOL* dirp;
    Status = RootDir->Open(RootDir, &dirp, u"EFI", EFI_FILE_MODE_READ, 0);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open EFI DIR \n\r");
        return Status;
    };

    Status = dirp->Open(dirp, &dirp, u"BOOT", EFI_FILE_MODE_READ, 0);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open BOOT DIR \n\r");
        return Status;
    };

    EFI_FILE_PROTOCOL* filep;
    Status = dirp->Open(
        dirp,
        &filep,
        u"DISK.INF",
        EFI_FILE_MODE_READ,
    0);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't open DISK INF file \n\r");
        return Status;
    };

    EFI_GUID FileInfoGuid = EFI_FILE_INFO_ID;
    UINTN InfoSize = 0;
    filep->GetInfo(filep, &FileInfoGuid, &InfoSize, NULL);
    EFI_FILE_INFO *FileInfo;
    VOID* buffer = NULL;

    bs->AllocatePool(EfiLoaderData, InfoSize, (VOID **)&FileInfo);
    filep->GetInfo(filep, &FileInfoGuid, &InfoSize, FileInfo);

    UINT64 buf_size = FileInfo->FileSize;
    bs->AllocatePool(EfiLoaderData, buf_size, &buffer);

    Status = filep->Read(filep, &buf_size, buffer);

    if (EFI_ERROR(Status)) {
        printf(EFI_STOP, u"Can't read DISK INF file \n\r");
        return Status;
    };

    *p_str = draw_data_part(buffer, FileInfo->FileSize);
    p_buf_size = &FileInfo->FileSize;
    return EFI_SUCCESS;
}

EFI_STATUS parse_data_file(CHAR16** str, UINTN* buf_size, UINTN file_size, UINTN disk_lba){
    printf(EFI_STOP, u"Data: %s", *str);

    return EFI_SUCCESS;
}

EFI_STATUS read_data_partition(){
    EFI_STOP->ClearScreen(EFI_STOP);
    EFI_STOP->OutputString(EFI_STOP, u"Files in Data part: \n\r");

    EFI_STATUS status;
    CHAR16* str;
    UINTN* buf_size;

    status = open_data_file_to_buffer(&str, buf_size);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Error code: %d \n\r", status);

        printf(EFI_STOP, u"Press any key to return ...\n\r");
        UINTN Index;
        EFI_EVENT WaitList[1];
        WaitList[0] = EFI_STIP->WaitForKey;
        bs->WaitForEvent(1, WaitList, &Index);

        return status;
    }

    UINTN file_size = 0; UINTN disk_lba = 0;
    status = parse_data_file(&str, buf_size, file_size, disk_lba);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Error code: %d \n\r", status);

        printf(EFI_STOP, u"Press any key to return ...\n\r");
        UINTN Index;
        EFI_EVENT WaitList[1];
        WaitList[0] = EFI_STIP->WaitForKey;
        bs->WaitForEvent(1, WaitList, &Index);

        return status;
    }

    printf(EFI_STOP, u"Press any key to return ...\n\r");
    UINTN Index;
    EFI_EVENT WaitList[1];
    WaitList[0] = EFI_STIP->WaitForKey;
    bs->WaitForEvent(1, WaitList, &Index);

    return EFI_SUCCESS;
}

EFI_STATUS get_disk_image_mediaID(UINT32* mediaID){
    EFI_STATUS status;

    EFI_GUID lip_guid = EFI_LOADED_IMAGE_PROTOCOL_GUID;
    EFI_LOADED_IMAGE_PROTOCOL *lip = NULL;
    status = bs->OpenProtocol(image_handle,
                              &lip_guid,
                              (VOID **)&lip,
                              image_handle,
                              NULL,
                              EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if (!EFI_ERROR(status)){
        EFI_GUID bio_guid = EFI_BLOCK_IO_PROTOCOL_GUID;
        EFI_BLOCK_IO_PROTOCOL *biop = NULL;
        status = bs->OpenProtocol(lip->DeviceHandle,
                                &bio_guid,
                                (VOID **)&biop,
                                image_handle,
                                NULL,
                                EFI_OPEN_PROTOCOL_GET_PROTOCOL);
        *mediaID = biop->Media->MediaId;
    }

    return status;
}
EFI_PHYSICAL_ADDRESS read_disk_lbas_to_buffer(
    EFI_LBA disk_lba,
    UINTN data_size,
    UINT32 disk_mediaID,
    BOOLEAN executable,
    EFI_STATUS* r_status
) {
    if (data_size == 0) {
        printf(EFI_STOP, u"Error: data_size is zero — cannot allocate or read\n");
        return 0;
    }

    EFI_PHYSICAL_ADDRESS buffer = 0;
    EFI_STATUS status = EFI_SUCCESS;

    EFI_GUID bio_guid = EFI_BLOCK_IO_PROTOCOL_GUID;
    EFI_BLOCK_IO_PROTOCOL *biop = NULL;
    UINTN num_handles = 0;
    EFI_HANDLE *handle_buffer = NULL;

    // Locate all handles that support Block IO
    status = bs->LocateHandleBuffer(ByProtocol, &bio_guid, NULL, &num_handles, &handle_buffer);
    if (EFI_ERROR(status) || num_handles == 0) {
        if (handle_buffer) bs->FreePool(handle_buffer);
        printf(EFI_STOP, u"Can't locate handles that support Block IO! Error: %d", status);
        *r_status = status;
        return 0;
    }

    BOOLEAN found = FALSE;
    UINTN i = 0;

    // Find the correct device by MediaId (and optionally partition flag)
    for (i = 0; i < num_handles; i++) {
        status = bs->OpenProtocol(
            handle_buffer[i],
            &bio_guid,
            (VOID **)&biop,
            image_handle,
            NULL,
            EFI_OPEN_PROTOCOL_GET_PROTOCOL
        );

        if (EFI_ERROR(status) || biop == NULL)
            continue;

        // Check for matching media and ensure media is present
        if (biop->Media->MediaId == disk_mediaID && biop->Media->MediaPresent) {
            found = TRUE;
            break;
        }
    }

    if (!found || biop == NULL) {
        if (handle_buffer) bs->FreePool(handle_buffer);
        printf(EFI_STOP, u"No matching Block IO device found for MediaID %u", disk_mediaID);
        *r_status = status;
        return 0;
    }

    // Allocate memory for the read buffer
    UINTN block_size = biop->Media->BlockSize;
    UINTN data_size_rounded = (data_size + block_size - 1) & ~(block_size - 1);
    UINTN pages_needed = (data_size_rounded + (PAGE_SIZE - 1)) / PAGE_SIZE;
    status = bs->AllocatePages(
        AllocateAnyPages,
        executable ? EfiLoaderCode : EfiLoaderData,
        pages_needed,
        &buffer
    );

    if (EFI_ERROR(status)) {
        if (handle_buffer) bs->FreePool(handle_buffer);
        printf(EFI_STOP, u"Unable to allocate memory! We need %d pages, Error: %d\r\n", pages_needed, status);
        printf(EFI_STOP, u"Data size: %d, Page sise: %d\r\n", data_size, PAGE_SIZE);
        *r_status = status;
        return 0;
    }

    // Read the data directly using Block IO
    status = biop->ReadBlocks(
        biop,
        biop->Media->MediaId,
        disk_lba,         // LBA number
        data_size_rounded,
        (VOID *)buffer
    );

    if (EFI_ERROR(status)) {
        bs->FreePages(buffer, pages_needed);
        if (handle_buffer) bs->FreePool(handle_buffer);
        printf(EFI_STOP, u"Unable to read disk blocks! Error: %d\r\n", status);
        *r_status = status;
        return 0;
    }

    if (handle_buffer) bs->FreePool(handle_buffer);

    return buffer;
}

VOID *read_file_from_data_part(CHAR16* file_name, BOOLEAN executable, UINTN* file_size, EFI_STATUS* status){
    *status = EFI_SUCCESS;

    UINT32 image_mediaID = 0;
    *status = get_disk_image_mediaID(&image_mediaID);

    CHAR16* file_string;
    UINTN buffer_size;
    open_data_file_to_buffer(&file_string, &buffer_size);

    CHAR16* strpos = stpstr(file_string, file_name);
    if (!strpos){
        *status = ENCODE_ERROR(1);
        printf(EFI_STOP, u"Can't find the file name in the DISK.INF file!\n\r");
        return 0;
    }

    CHAR16* filesize_str = stpstr(strpos, u"FILE_SIZE=");
    if (!filesize_str){
        *status = ENCODE_ERROR(1);
        printf(EFI_STOP, u"Can't find any filesize under the file entry in DISK.INF!\n\r");
        return 0;
    }

    *file_size = (UINTN)atoi(filesize_str);

    CHAR16* lba_string = stpstr(filesize_str, u"DISK_LBA=");
    if (!lba_string){
        *status = ENCODE_ERROR(1);
        return 0;
    }

    UINTN disk_lba = atoi(lba_string);
    VOID* data = (VOID *)read_disk_lbas_to_buffer(disk_lba, *file_size, image_mediaID, executable, status);
    return data;
}
#endif