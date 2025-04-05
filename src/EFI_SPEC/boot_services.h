#ifndef Boot_Services_H
#define Boot_Services_H

#define EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL   0x00000001
#define EFI_OPEN_PROTOCOL_GET_PROTOCOL         0x00000002
#define EFI_OPEN_PROTOCOL_TEST_PROTOCOL        0x00000004
#define EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER  0x00000008
#define EFI_OPEN_PROTOCOL_BY_DRIVER            0x00000010
#define EFI_OPEN_PROTOCOL_EXCLUSIVE            0x00000020

typedef EFI_STATUS(EFIAPI *EFI_LOCATE_PROTOCOL) (
    IN EFI_GUID                            *Protocol,
    IN VOID                                *Registration OPTIONAL,
    OUT VOID                               **Interface
);

typedef EFI_STATUS(EFIAPI *EFI_OPEN_PROTOCOL) (
    IN EFI_HANDLE                    Handle,
    IN EFI_GUID                      *Protocol,
    OUT VOID                         **Interface OPTIONAL,
    IN EFI_HANDLE                    AgentHandle,
    IN EFI_HANDLE                    ControllerHandle,
    IN UINT32                        Attributes
);

typedef
EFI_STATUS
(EFIAPI *EFI_CLOSE_PROTOCOL) (
   IN EFI_HANDLE                 Handle,
   IN EFI_GUID                   *Protocol,
   IN EFI_HANDLE                 AgentHandle,
   IN EFI_HANDLE                 ControllerHandle
);

typedef enum {
    EfiReservedMemoryType,
    EfiLoaderCode,
    EfiLoaderData,
    EfiBootServicesCode,
    EfiBootServicesData,
    EfiRuntimeServicesCode,
    EfiRuntimeServicesData,
    EfiConventionalMemory,
    EfiUnusableMemory,
    EfiACPIReclaimMemory,
    EfiACPIMemoryNVS,
    EfiMemoryMappedIO,
    EfiMemoryMappedIOPortSpace,
    EfiPalCode,
    EfiPersistentMemory,
    EfiUnacceptedMemoryType,
    EfiMaxMemoryType
} EFI_MEMORY_TYPE;

typedef EFI_STATUS(EFIAPI *EFI_WAIT_FOR_EVENT) (
   IN UINTN             NumberOfEvents,
   IN EFI_EVENT         *Event,
   OUT UINTN            *Index
);

typedef EFI_STATUS(EFIAPI  *EFI_ALLOCATE_POOL) (
    IN EFI_MEMORY_TYPE            PoolType,
    IN UINTN                      Size,
    OUT VOID                      **Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_FREE_POOL) (
   IN VOID           *Buffer
);

#define EFI_BOOT_SERVICES_SIGNATURE 0x56524553544f4f42
#define EFI_BOOT_SERVICES_REVISION EFI_SPECIFICATION_VERSION

typedef struct _EFI_BOOT_SERVICES{
    EFI_TABLE_HEADER        Hdr;

    //
    // Task Priority Services
    //
    VOID*                   RaiseTPL;       // EFI 1.0+
    VOID*                   RestoreTPL;     // EFI 1.0+

    //
    // Memory Services
    //
    VOID*                   AllocatePages;  // EFI 1.0+
    VOID*                   FreePages;      // EFI 1.0+
    VOID*                   GetMemoryMap;   // EFI 1.0+
    EFI_ALLOCATE_POOL       AllocatePool;   // EFI 1.0+
    EFI_FREE_POOL*          FreePool;       // EFI 1.0+

    //
    // Event & Timer Services
    //
    VOID*                   CreateEvent;    // EFI 1.0+
    VOID*                   SetTimer;       // EFI 1.0+
    EFI_WAIT_FOR_EVENT      WaitForEvent;   // EFI 1.0+
    VOID*                   SignalEvent;    // EFI 1.0+
    VOID*                   CloseEvent;     // EFI 1.0+
    VOID*                   CheckEvent;     // EFI 1.0+

    //
    // Protocol Handler Services
    //
    VOID*                   InstallProtocolInterface;            // EFI 1.0+
    VOID*                   ReinstallProtocolInterface;          // EFI 1.0+
    VOID*                   UninstallProtocolInterface;          // EFI 1.0+
    VOID*                   HandleProtocol;                      // EFI 1.0+
    VOID*                   Reserved;    // EFI 1.0+
    VOID*                   RegisterProtocolNotify;              // EFI  1.0+
    VOID*                   LocateHandle;                        // EFI 1.0+
    VOID*                   LocateDevicePath;                    // EFI 1.0+
    VOID*                   InstallConfigurationTable;           // EFI 1.0+

    //
    // Image Services
    //
    VOID*                   LoadImage;        // EFI 1.0+
    VOID*                   StartImage;       // EFI 1.0+
    VOID*                   Exit;             // EFI 1.0+
    VOID*                   UnloadImage;      // EFI 1.0+
    VOID*                   ExitBootServices; // EFI 1.0+

    //
    // Miscellaneous Services
    //
    VOID*                   GetNextMonotonicCount; // EFI 1.0+
    VOID*                   Stall;                 // EFI 1.0+
    VOID*                   SetWatchdogTimer;      // EFI 1.0+

    //
    // DriverSupport Services
    //
    VOID*                   ConnectController;     // EFI 1.1
    VOID*                   DisconnectController;  // EFI 1.1+

    //
    // Open and Close Protocol Services
    //
    EFI_OPEN_PROTOCOL       OpenProtocol;           // EFI 1.1+
    EFI_CLOSE_PROTOCOL      CloseProtocol;          // EFI 1.1+
    VOID*                   OpenProtocolInformation;// EFI 1.1+

    //
    // Library Services
    //
    VOID*                   ProtocolsPerHandle;     // EFI 1.1+
    VOID*                   LocateHandleBuffer;     // EFI 1.1+
    EFI_LOCATE_PROTOCOL     LocateProtocol;         // EFI 1.1+
    VOID*                   InstallMultipleProtocolInterfaces;    // EFI 1.1+
    VOID*                   UninstallMultipleProtocolInterfaces;   // EFI 1.1+*

    //
    // 32-bit CRC Services
    //
    VOID*                   CalculateCrc32;     // EFI 1.1+

    //
    // Miscellaneous Services
    //
    VOID*                   CopyMem;        // EFI 1.1+
    VOID*                   SetMem;         // EFI 1.1+
    VOID*                   CreateEventEx;  // UEFI 2.0+
} EFI_BOOT_SERVICES;

#endif