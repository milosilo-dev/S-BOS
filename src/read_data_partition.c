#ifndef READ_DATA_PART_C
#define READ_DATA_PART_C
#include "../main.c"

EFI_STATUS open_data_part_file()
{
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

    return Status;
}

EFI_STATUS read_data_partition(){
    EFI_STOP->ClearScreen(EFI_STOP);
    EFI_STOP->OutputString(EFI_STOP, u"Files in Data part: \n\r");

    EFI_STATUS status;
    status = open_data_part_file();

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
#endif