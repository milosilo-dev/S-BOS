#ifndef KERNEL_PARAMS_H
#define KERNEL_PARAMS_H
#include "efi.h"

typedef struct {
    EFI_MEMORY_DESCRIPTOR* memory_map;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE* gop_mode;
    EFI_RUNTIME_SERVICES* rs;
    UINTN table_entrys;
    EFI_CONFIGURATION_TABLE* config_table;
} Kernel_Params;
#endif