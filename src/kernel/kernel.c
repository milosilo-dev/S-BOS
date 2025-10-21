#ifndef KERNEL_C
#define KERNEL_C
#include "../boot/EFI_SPEC/efi.h"
#include <stdint.h>

typedef struct {
    UINT32 width;
    UINT32 height;
    UINT32* fb;
} GOP_OBJ;

GOP_OBJ gop_obj = {0};

void EFIAPI kernel_main(Kernel_Params kargs){
    UINT32 width = kargs.gop_mode->Info->PixelsPerScanLine;
    UINT32 height = kargs.gop_mode->Info->HorizontalResolution;
    UINT32 *fb = (UINT32*)kargs.gop_mode->FrameBufferBase;

    gop_obj.fb = fb;
    gop_obj.width = width;
    gop_obj.height = height;

    for (UINT32 y=0; y < height; y++){
        UINTN line = y % 90;
        for (UINT32 x=0; x < width; x++){
            if (line < 30){
                fb[y * width + x] = 0xFFFF3333;
            } else if (line < 60){ 
                fb[y * width + x] = 0xFF33FF33;
            } else {
                fb[y * width + x] = 0xFF3333FF;
            }
        }
    }

    EFI_TIME old_time = {0}, new_time = {0};
    EFI_TIME_CAPABILITIES time_caps = {0};
    UINT8 i = 0;
    while (i < 3){
        kargs.rs->GetTime(&new_time, &time_caps); 
        if (new_time.Second != old_time.Second){
            old_time = new_time;
            i++;
        }
    }

    kargs.rs->ResetSystem(EfiResetShutdown, EFI_SUCCESS, 0, NULL);
    while (1);
}
#endif