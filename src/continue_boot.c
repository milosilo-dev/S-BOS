#ifndef CONTINUE_BOOT_C
#define CONTINUE_BOOT_C
#include "../main.c"

GOP_OBJ gop_obj;

EFI_STATUS continue_boot(){
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop = get_gop();

    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION gop_mode_info;
    UINTN gop_mode_info_size = sizeof(gop_mode_info);
    gop->QueryMode(gop, gop->Mode->Mode, &gop_mode_info_size, (EFI_GRAPHICS_OUTPUT_MODE_INFORMATION**)&gop_mode_info);

    gop->SetMode(gop, 0);

    UINT32 *framebuffer = (UINT32 *)gop->Mode->FrameBufferBase;
    UINTN width  = gop->Mode->Info->HorizontalResolution;
    UINTN height = gop->Mode->Info->VerticalResolution;

    GOP_OBJ gop_obj = mk_gop_obj(gop, height, width, framebuffer);

    clear_screen(0x00000000, gop_obj);
    draw_image((gop_obj.width / 2) - 75, (gop_obj.height / 2) - 75, gop_obj, L"kiwi.bmp", 0x00000000, TRUE);

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 20, 22);
    printf(EFI_STOP, u"Press ( F11 ) to enter S-BOS boot menu!");
    
    UINTN index = 30000;
    while (index != 0){
        EFI_STIP->ReadKeyStroke(EFI_STIP, &key);
        if (key.ScanCode == 0x15){
            printf(EFI_STOP, u"HI");
            efi_boot_menu();
            continue_boot();
        }
        // | / - \ | 
        EFI_STOP->SetCursorPosition(EFI_STOP, 40, 20);
        UINT8 frame = (UINT8)(index / 1000) % 4;
        if (frame == 0)
            printf(EFI_STOP, u"|");
        else if (frame == 1)
            printf(EFI_STOP, u"/");
        else if (frame == 2)
            printf(EFI_STOP, u"-");
        else if (frame == 3)
            printf(EFI_STOP, u"\\");
        index --;
    }

    st->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    return EFI_SUCCESS; // Shutdown
}

#endif