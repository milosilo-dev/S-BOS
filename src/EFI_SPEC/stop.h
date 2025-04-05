#ifndef STOP_H
#define STOP_H

#include "text_defs.h"
#include "efi.h"

#define EFI_TEXT_ATTR(Foreground,Background) \
    ((Foreground) | ((Background) << 4))

// SIMPLE_TEXT_OUTPUT_PROTOCOL
// https://uefi.org/specs/UEFI/2.10_A/12_Protocols_Console_Support.html#simple-text-output-protocol
typedef struct {
    INT32                              MaxMode;
    INT32                              Mode;
    INT32                              Attribute;
    INT32                              CursorColumn;
    INT32                              CursorRow;
    BOOLEAN                            CursorVisible;
} SIMPLE_TEXT_OUTPUT_MODE;

// Define an empty version of the struct that can be passed to the functions,
// Wich can then be refrenced in the struct
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;

// Functions defined in the SIMPLE_TEXT_OUTPUT_PROTOCOL

// Text Reset
typedef EFI_STATUS(EFIAPI *EFI_TEXT_RESET) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *This,
    IN BOOLEAN                               ExtendedVerification
);

// Text String
typedef EFI_STATUS(EFIAPI *EFI_TEXT_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL    *This,
    IN CHAR16                             *String
);

// Test String
typedef EFI_STATUS(EFIAPI *EFI_TEXT_TEST_STRING) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL       *This,
    IN CHAR16                                *String
);

// Text Query Mode
typedef EFI_STATUS(EFIAPI *EFI_TEXT_QUERY_MODE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL          *This,
    IN UINTN                                    ModeNumber,
    OUT UINTN                                   *Columns,
    OUT UINTN                                   *Rows
);

// Set Text Mode
typedef EFI_STATUS(* EFIAPI EFI_TEXT_SET_MODE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL          *This,
    IN UINTN                                    ModeNumber
);

// Set Text Attribute
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_ATTRIBUTE) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL        *This,
    IN UINTN                                  Attribute
);

// Clear the screen
typedef EFI_STATUS(EFIAPI *EFI_TEXT_CLEAR_SCREEN) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL             *This
);

// Set the cursor position
typedef EFI_STATUS(EFIAPI *EFI_TEXT_SET_CURSOR_POSITION) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL             *This,
    IN UINTN                                       Column,
    IN UINTN                                       Row
);

// Enable the cursor
typedef EFI_STATUS(EFIAPI *EFI_TEXT_ENABLE_CURSOR) (
    IN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL             *This,
    IN BOOLEAN                                     Visible
);

typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL {
    EFI_TEXT_RESET                           Reset;
    EFI_TEXT_STRING                          OutputString;
    EFI_TEXT_TEST_STRING                     TestString;
    EFI_TEXT_QUERY_MODE                      QueryMode;
    EFI_TEXT_SET_MODE                        SetMode;
    EFI_TEXT_SET_ATTRIBUTE                   SetAttribute;
    EFI_TEXT_CLEAR_SCREEN                    ClearScreen;
    EFI_TEXT_SET_CURSOR_POSITION             SetCursorPosition;
    EFI_TEXT_ENABLE_CURSOR                   EnableCursor;
    SIMPLE_TEXT_OUTPUT_MODE                  *Mode;
} EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
#endif