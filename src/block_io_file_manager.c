#ifndef BI_File_Manager_C
#define BI_File_Manager_C
#include "../main.c"

EFI_STATUS bi_file_manager(){
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

    while (TRUE){
        
    }

    return EFI_SUCCESS;
}

#endif