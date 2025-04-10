#ifndef CONTINUE_BOOT_C
#define CONTINUE_BOOT_C
#include "EFI_SPEC/gop.h"
#include "../main.c"

VOID continue_boot(){
    EFI_GUID gop_guid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;

    bs->LocateProtocol(&gop_guid, NULL, (VOID **)&gop);

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gop_mode_info;
    UINTN gop_mode_info_size = sizeof(gop_mode_info);
    gop->QueryMode(gop, gop->Mode->Mode, &gop_mode_info_size, (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION**)&gop_mode_info);

    st->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    return; // Shutdown
}

#endif