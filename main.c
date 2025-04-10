#ifndef MAIN_C
#define MAIN_C

#include "src/EFI_SPEC/efi.h"
#include "src/print.c"

EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL*    EFI_STOP;
EFI_SIMPLE_TEXT_INPUT_PROTOCOL*     EFI_STIP;
EFI_HANDLE                          image_handle;
EFI_BOOT_SERVICES*                  bs;
EFI_SYSTEM_TABLE*                   st;

void DrawBox(UINT8 x, UINT8 y, BOOLEAN filled){
    EFI_STOP->SetCursorPosition(EFI_STOP, x, y);
    if (filled)
        EFI_STOP->OutputString(EFI_STOP, u"█");
    else
        EFI_STOP->OutputString(EFI_STOP, u"░");
}

UINT8 menu_main();
UINT8 system_menu();
UINT8 file_manager();
VOID continue_boot();

EFI_INPUT_KEY key;


EFI_STATUS efi_main(EFI_HANDLE ImageHandle, EFI_SYSTEM_TABLE* SystemTable){
    EFI_STOP = SystemTable->ConOut;
    EFI_STIP = SystemTable->ConIn;
    image_handle = ImageHandle;
    bs = SystemTable->BootServices;
    st = SystemTable;

    EFI_STIP->ReadKeyStroke(EFI_STIP, &key);
    EFI_STOP->Reset(EFI_STOP, FALSE);

    UINT8 option = menu_main();
    while(TRUE){
        EFI_STIP->ReadKeyStroke(EFI_STIP, &key);

        switch (option)
        {
        case 10:
            option = menu_main(&key);
            break;
        case 1:
            continue_boot();
            SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;
        case 2:
            option = system_menu();
            break;
        case 3:
            goto quit;
            break;
        case 4:
            SystemTable->RuntimeServices->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
            break;
        case 5:
            option = menu_main(&key); // Dose nothing
            break;
        case 6:
            option = file_manager();
            break;
        default:
            break;
        }
    };
quit:
    return EFI_SUCCESS;
}

#include "src/boot_menu.c"
#include "src/file_manager.c"
#include "src/continue_boot.c"
#endif