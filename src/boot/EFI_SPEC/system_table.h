#ifndef SYSTEM_TABLE_H
#define SYSTEM_TABLE_H
#include "efi.h"

// EFI_TABLE_HEADER
// https://uefi.org/specs/UEFI/2.10_A/04_EFI_System_Table.html#id4
typedef struct{
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

// EFI_CONFIGURATION_TABLE
// https://uefi.org/specs/UEFI/2.10/04_EFI_System_Table.html#efi-configuration-table
typedef struct{
  EFI_GUID           VendorGuid;
  VOID               *VendorTable;
} EFI_CONFIGURATION_TABLE;

// EFI_SYSTEM_TABLE
// https://uefi.org/specs/UEFI/2.10_A/04_EFI_System_Table.html#id6
typedef struct _EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL;
typedef struct _EFI_SIMPLE_TEXT_INPUT_PROTOCOL EFI_SIMPLE_TEXT_INPUT_PROTOCOL;
typedef struct s_efi_runtime_service_handle EFI_RUNTIME_SERVICES;
typedef struct _EFI_BOOT_SERVICES EFI_BOOT_SERVICES;

typedef struct {
    EFI_TABLE_HEADER                Hdr;
    CHAR16                          *FirmwareVendor;
    UINT32                          FirmwareRevision;
    EFI_HANDLE                      ConsoleInHandle;
    EFI_SIMPLE_TEXT_INPUT_PROTOCOL  *ConIn;
    EFI_HANDLE                      ConsoleOutHandle;
    EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *ConOut;
    EFI_HANDLE                      StandardErrorHandle;
    VOID                            *StdErr;
    EFI_RUNTIME_SERVICES            *RuntimeServices;
    EFI_BOOT_SERVICES               *BootServices;
    UINTN                           NumberOfTableEntries;
    EFI_CONFIGURATION_TABLE         *ConfigurationTable;
} EFI_SYSTEM_TABLE;

// EFI_IMAGE_ENTRY_POINT
// https://uefi.org/specs/UEFI/2.10_A/07_Services_Boot_Services.html#efi-image-entry-point
typedef EFI_STATUS(EFIAPI *EFI_IMAGE_ENTRY_POINT)(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
);
#endif