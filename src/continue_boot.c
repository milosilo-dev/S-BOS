#ifndef CONTINUE_BOOT_C
#define CONTINUE_BOOT_C
#include "EFI_SPEC/gop.h"
#include "../main.c"

VOID continue_boot(){
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL gop;

    bs->OpenProtocol(image_handle, &gop_guid, (VOID**)&gop, image_handle, NULL, EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL);
    return; // Shutdown
}

#endif