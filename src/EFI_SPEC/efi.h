#ifndef EFI_H
#define EFI_H
#include <stdint.h>
#include <stddef.h>

#if __has_include(<uchar.h>)
    #include <uchar.h> // Check for unicode sopport on build system
#endif

// Empty Datum
#define IN
#define OUT
#define OPTIONAL
#define CONST const
#define TRUE 1
#define FALSE 0

// EFIAPI defines the way of calling the EFI defined functions. It is a simple headerfile and contains no real logic, that is handeled by UEFI.
#define EFIAPI __attribute__((ms_abi))

// Data types: https://uefi.org/specs/UEFI/2.10_A/02_Overview.html#data-types
typedef uint8_t BOOLEAN;
typedef int64_t INTN;       // Largest Int avalibe on target system, 64 bit = int64
typedef uint64_t UINTN;     // Largest Unsigned Int avalibe on target system, 64 bit = uint64
typedef int8_t INT8;
typedef uint8_t UINT8;
typedef int16_t INT16;
typedef uint16_t UINT16;
typedef int32_t INT32;
typedef uint32_t UINT32;
typedef int64_t INT64;
typedef uint64_t UINT64;

typedef char CHAR8;         // 8 Bit Ascii value
typedef UINT64 EFI_PHYSICAL_ADDRESS;

#ifndef _UCHAR_H
    typedef uint_least16_t char16_t; // Define char16_t manulaly is build system does not support it.
#endif
typedef char16_t CHAR16; // 16 Bit Unicode value (If build system supports it)

typedef void VOID;
typedef struct EFIGUID{
    UINT32 TimeLow;
    UINT16 TimeMid;
    UINT16 TimeHighAndVersion;
    UINT8  ClockSeqHighAndReserved;
    UINT8  ClockSeqLow;
    UINT8  Node[6];
} __attribute__ ((packed)) EFI_GUID;

typedef UINTN EFI_STATUS;
typedef VOID* EFI_HANDLE;
typedef VOID* EFI_EVENT;
typedef UINT64 EFI_LBA;
typedef UINTN EFI_TPL;

// EFI_STATUS values
// https://uefi.org/specs/UEFI/2.10_A/Apx_D_Status_Codes.html
#define EFI_SUCCESS 0ULL
#define TOP_BIT 0x8000000000000000ULL
#define ENCODE_ERROR(x) (x | TOP_BIT)

// EFI_TABLE_HEADER
// https://uefi.org/specs/UEFI/2.10_A/04_EFI_System_Table.html#id4
typedef struct{
    UINT64 Signature;
    UINT32 Revision;
    UINT32 HeaderSize;
    UINT32 CRC32;
    UINT32 Reserved;
} EFI_TABLE_HEADER;

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
    VOID                            *ConfigurationTable;
} EFI_SYSTEM_TABLE;

// EFI_IMAGE_ENTRY_POINT
// https://uefi.org/specs/UEFI/2.10_A/07_Services_Boot_Services.html#efi-image-entry-point
typedef
EFI_STATUS
(EFIAPI *EFI_IMAGE_ENTRY_POINT)(
    IN EFI_HANDLE ImageHandle,
    IN EFI_SYSTEM_TABLE *SystemTable
);

#include "runtime_services.h"
#include "boot_services.h"
#include "efi_time.h"

#include "stop.h"
#include "gop.h"
#include "stip.h"
#include "file_protocol.h"

#include "guids.h"
#endif