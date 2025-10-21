#ifndef LOAD_KERNEL_C
#define LOAD_KERNEL_C
#include "read_data_partition.c"

EFI_STATUS load_kernel(GOP_OBJ *gop_obj){
    UINTN filesize = 0;
    EFI_STATUS status;
    VOID* disk_buffer = read_file_from_data_part(u"kernel.bin", TRUE, &filesize, &status);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Could not find kernel!, Error %d\r\n", status);
        return status;
    }
    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_GREEN,EFI_BLACK));
    printf(EFI_STOP, u"Found kernel!\r\n");
    printf(EFI_STOP, u"Loading kernel...\r\n");

    UINT8 *hdr = disk_buffer;
    printf(EFI_STOP, u"Header bytes: [%x][%x][%x][%x]\r\n", 
           hdr[0], hdr[1], hdr[2], hdr[3]);

    // Get memory map
    UINTN mapSize = 0;
    EFI_MEMORY_DESCRIPTOR* memory_map = NULL;
    UINTN mapKey = 0;
    UINTN descriptorSize = 0;
    UINT32 descriptorVersion = 0;

    // First call: get required buffer size (expected EFI_BUFFER_TOO_SMALL)
    status = bs->GetMemoryMap(&mapSize,
        memory_map,
        &mapKey,
        &descriptorSize,
        &descriptorVersion);
    if (status != ENCODE_ERROR(5)){
        printf(EFI_STOP, u"Could not get Memory map size!, Error %d\r\n", status);
        return status;
    }

    // Allocate a buffer a bit larger than required to account for changes
    mapSize += descriptorSize * 2;
    status = bs->AllocatePool(EfiLoaderData, mapSize, (void**)&memory_map);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Could not allocate memory!, Error %d\r\n", status);
        return status;
    }

    // Second call: get the actual memory map into the allocated buffer
    status = bs->GetMemoryMap(&mapSize,
        memory_map,
        &mapKey,
        &descriptorSize,
        &descriptorVersion);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Could not get memory map!, Error %d\r\n", status);
        return status;
    }
    // Get GOP Mode before we loose boot services
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* gop_mode = gop_obj->gop->Mode;

    // Define the kernels params
    Kernel_Params kargs = {0};
    kargs.memory_map = memory_map;
    kargs.gop_mode = gop_mode;
    kargs.rs = st->RuntimeServices;
    kargs.table_entrys = st->NumberOfTableEntries;
    kargs.config_table = st->ConfigurationTable;

    EFI_STOP->ClearScreen(EFI_STOP);
    status = bs->ExitBootServices(image_handle, mapKey);
    if (EFI_ERROR(status)){
        printf(EFI_STOP, u"Could not exit boot services! Error %d\r\n", status);
        return status;
    }
    
    if (!memcmp(hdr, (UINT8[2]){'M', 'Z'}, 2)){
        // PE File
    }  else if (!memcmp(hdr, (UINT8[4]){0x7F, 'E', 'L', 'F'}, 4)){
        // ELF File
    }
    else {
        // BIN File
        void EFIAPI (*entry_point)(Kernel_Params kargs) = NULL;
        entry_point = disk_buffer;
        entry_point(kargs);
    }
}
#endif