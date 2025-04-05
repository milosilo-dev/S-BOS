#ifndef BOOTMENU_H
#define BOOTMENU_H
#include "../main.c"

UINT8 selected_menu_option = 0;
const UINT8 main_menu_options = 6;

// Adjust text mode

BOOLEAN ajust_mode(EFI_INPUT_KEY* key){
    if (key->ScanCode == 0x03){
        INT32 new_mode = EFI_STOP->Mode->Mode + 1;
        if (new_mode <= EFI_STOP->Mode->MaxMode){
            EFI_STOP->SetMode(EFI_STOP, new_mode);
        }
        return TRUE;
    }
    else if (key->ScanCode == 0x04){
        INT32 new_mode = EFI_STOP->Mode->Mode - 1;
        if (new_mode <= EFI_STOP->Mode->MaxMode){
            EFI_STOP->SetMode(EFI_STOP, new_mode);
        }
        return TRUE;
    }
    return FALSE;
}

// Main menu

UINT8 menu_main_update(EFI_INPUT_KEY* key){
    if (selected_menu_option > main_menu_options)
        selected_menu_option = main_menu_options;
    else if (selected_menu_option < 1)
        selected_menu_option = 1;

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    for (UINT8 i = 1; i <= main_menu_options; i++){
        if (selected_menu_option != i){
            EFI_STOP->SetCursorPosition(EFI_STOP, 10, 20 - (i * 2));
            EFI_STOP->OutputString(EFI_STOP, u"   ");
        }
    }
    EFI_STOP->SetCursorPosition(EFI_STOP, 10, 20 - (selected_menu_option * 2));
    EFI_STOP->OutputString(EFI_STOP, u" -> ");

    if (selected_menu_option == 5){
        if (ajust_mode(key)){
            return 10;
        }
    }

    if (key->ScanCode == 0x01)
        selected_menu_option++;
    else if (key->ScanCode == 0x02)
        selected_menu_option--;
    else if (key->UnicodeChar == 0xD)
        return selected_menu_option;
    return 0;
}

UINT8 menu_main(){
    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_BLUE,EFI_BLACK));
    EFI_STOP->ClearScreen(EFI_STOP);
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

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 22, 2);
    EFI_STOP->OutputString(EFI_STOP, u"Welcome To SBOS");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 14, 4);
    EFI_STOP->OutputString(EFI_STOP, u"(Stupidly Basic Operating System)");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 25, 6);
    EFI_STOP->OutputString(EFI_STOP, u"by Miles ;)");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 24, 8);
    EFI_STOP->OutputString(EFI_STOP, u"File Manager");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 24, 10);
    printf(EFI_STOP, u"Text Mode: %d", (INT32)EFI_STOP->Mode->Mode);

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 26, 12);
    EFI_STOP->OutputString(EFI_STOP, u"Shutdown");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 26, 14);
    EFI_STOP->OutputString(EFI_STOP, u"Open BIOS");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 25, 16);
    EFI_STOP->OutputString(EFI_STOP, u"System Info");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 24, 18);
    EFI_STOP->OutputString(EFI_STOP, u"Continue Boot");

    UINT8 option;
    while (TRUE){
        EFI_STIP->ReadKeyStroke(EFI_STIP, &key);
        option = menu_main_update(&key);
        if (option != 0)
            break;
    }
    return option;
}

// System info

UINT8 system_menu_update(){
    if (EFI_STIP->ReadKeyStroke(EFI_STIP, &key) == EFI_SUCCESS)
        return 10;
    return 2;
}

UINT8 system_menu(){
    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_BLUE,EFI_BLACK));
    EFI_STOP->ClearScreen(EFI_STOP);

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 25, 1);
    EFI_STOP->OutputString(EFI_STOP, u"System Info");
    EFI_STOP->SetCursorPosition(EFI_STOP, 2, 3);

    UINTN Columns;
    UINTN Rows;
    EFI_STOP->QueryMode(EFI_STOP, EFI_STOP->Mode->Mode, &Columns, &Rows);

    printf(EFI_STOP, u"Width: %d \n\r ", Columns);
    printf(EFI_STOP, u" Height: %d \n\r ", Rows);
    for (UINT8 x = 0; x < 60; x++){
        EFI_STOP->OutputString(EFI_STOP, u"-");
    }
    printf(EFI_STOP, u"\n\r  Text Mode: %d \n\r  ", EFI_STOP->Mode->Mode);
    CHAR16* buffer[2];
    buffer[0] = (CHAR16 *)BLOCKELEMENT_LIGHT_SHADE;
    buffer[1] = u"\0";
    EFI_STOP->OutputString(EFI_STOP, *buffer);

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

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 15, 20);
    EFI_STOP->OutputString(EFI_STOP, u"(Press any key to continue ...)");

    UINT8 option;
    while (TRUE){
        option = system_menu_update();
        if (option != 2){
            break;
        }
    }
    return option;
}
#endif