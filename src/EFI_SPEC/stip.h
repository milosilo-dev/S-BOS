#ifndef STIP_H
#define STIP_H

#include "text_defs.h"
#include "efi.h"

typedef struct {
    UINT16                             ScanCode;
    CHAR16                             UnicodeChar;
} EFI_INPUT_KEY;

typedef EFI_STATUS(EFIAPI *EFI_INPUT_READ_KEY) (
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL              *This,
    OUT EFI_INPUT_KEY                              *Key
);

typedef EFI_STATUS(EFIAPI *EFI_INPUT_RESET) (
    IN EFI_SIMPLE_TEXT_INPUT_PROTOCOL                 *This,
    IN BOOLEAN                                        ExtendedVerification
);

typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL {
    EFI_INPUT_RESET                       Reset;
    EFI_INPUT_READ_KEY                    ReadKeyStroke;
    EFI_EVENT                             WaitForKey;
} EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
#endif