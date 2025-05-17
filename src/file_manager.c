#ifndef File_Manager_C
#define File_Manager_C
#include "../main.c"
#include "esp_file_manager.c"
#include "block_io_file_manager.c"
#include "read_data_partition.c"

UINT8 menu_options = 3;

UINT8 file_manager_update(){
    EFI_INPUT_KEY key;

    EFI_STIP->ReadKeyStroke(EFI_STIP, &key);

    if (selected_menu_option > menu_options)
        selected_menu_option = menu_options;
    else if (selected_menu_option < 1)
        selected_menu_option = 1;

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_BLACK));
    for (UINT8 i = 1; i <= menu_options; i++){
        if (selected_menu_option != i){
            EFI_STOP->SetCursorPosition(EFI_STOP, 10, 20 - (i * 2));
            EFI_STOP->OutputString(EFI_STOP, u"   ");
        }
    }
    EFI_STOP->SetCursorPosition(EFI_STOP, 10, 20 - (selected_menu_option * 2));
    EFI_STOP->OutputString(EFI_STOP, u" -> ");

    if (key.ScanCode == 0x01)
        selected_menu_option++;
    else if (key.ScanCode == 0x02)
        selected_menu_option--;
    else if (key.UnicodeChar == 0xD){
        return selected_menu_option;
    }
    else if (key.ScanCode == 0x0017){
        efi_boot_menu();
    }
    return 0;
}

UINT8 file_manager(){
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
    EFI_STOP->SetCursorPosition(EFI_STOP, 24, 2);
    EFI_STOP->OutputString(EFI_STOP, u"File Manager");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_WHITE,EFI_BLACK));
    EFI_STOP->SetCursorPosition(EFI_STOP, 14, 4);
    EFI_STOP->OutputString(EFI_STOP, u"Interact with the efi file system,");
    EFI_STOP->SetCursorPosition(EFI_STOP, 7, 5);
    EFI_STOP->OutputString(EFI_STOP, u"Get Partition Info or Read the Data Partition");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 25, 14);
    EFI_STOP->OutputString(EFI_STOP, u"ESP System");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 23, 16);
    EFI_STOP->OutputString(EFI_STOP, u"Partition Info");

    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(EFI_YELLOW,EFI_RED));
    EFI_STOP->SetCursorPosition(EFI_STOP, 20, 18);
    EFI_STOP->OutputString(EFI_STOP, u"Read Data Partition");

    while(TRUE){
        UINT8 value = file_manager_update();

        if (value == 10)
            return 10;
        else if (value == 1){
            read_data_partition();
            return 10;
        }
        else if (value == 2){
            bi_file_manager();
            return 10;
        }
        else if (value == 3){
            esp_file_manager();
            return 10;
        }
    }
}

#endif