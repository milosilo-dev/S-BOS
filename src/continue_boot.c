#ifndef CONTINUE_BOOT_C
#define CONTINUE_BOOT_C
#include "../main.c"

EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gop_mode_info;
UINT32 *framebuffer;
UINTN width;
UINTN height;

VOID continue_boot(){
    gop = get_gop();

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gop_mode_info;
    UINTN gop_mode_info_size = sizeof(gop_mode_info);
    gop->QueryMode(gop, gop->Mode->Mode, &gop_mode_info_size, (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION**)&gop_mode_info);

    gop->SetMode(gop, 0);

    UINT32 *framebuffer = (UINT32 *)gop->Mode->FrameBufferBase;
    UINTN width  = gop->Mode->Info->HorizontalResolution;
    UINTN height = gop->Mode->Info->VerticalResolution;

    GOP_OBJ gop_obj = mk_gop_obj(gop, height, width, framebuffer);

    clear_screen(0x00FF0000, gop_obj);

    UINTN index = 999999999;
    while (index != 0){
        index --;
    }

    st->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    return; // Shutdown
}

#endif