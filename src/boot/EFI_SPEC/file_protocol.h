#ifndef FILE_PROTOCOL_H
#define FILE_PROTOCOL_H
#include "efi.h"

// File Protocol
// https://uefi.org/specs/UEFI/2.10_A/13_Protocols_Media_Access.html#efi-file-protocol
#define EFI_FILE_PROTOCOL_REVISION           0x00010000
#define EFI_FILE_PROTOCOL_REVISION2          0x00020000
#define EFI_FILE_PROTOCOL_LATEST_REVISION EFI_FILE_PROTOCOL_REVISION2

//******************************************************
// Open Modes
//******************************************************
#define EFI_FILE_MODE_READ       0x0000000000000001
#define EFI_FILE_MODE_WRITE      0x0000000000000002
#define EFI_FILE_MODE_CREATE     0x8000000000000000

//******************************************************
// File Attributes
//******************************************************
#define EFI_FILE_READ_ONLY       0x0000000000000001
#define EFI_FILE_HIDDEN          0x0000000000000002
#define EFI_FILE_SYSTEM          0x0000000000000004
#define EFI_FILE_RESERVED        0x0000000000000008
#define EFI_FILE_DIRECTORY       0x0000000000000010
#define EFI_FILE_ARCHIVE         0x0000000000000020
#define EFI_FILE_VALID_ATTR      0x0000000000000037

typedef struct _EFI_FILE_PROTOCOL EFI_FILE_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_FILE_WRITE) (
    IN EFI_FILE_PROTOCOL              *This,
    IN OUT UINTN                      *BufferSize,
    IN VOID                           *Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_READ) (
    IN EFI_FILE_PROTOCOL           *This,
    IN OUT UINTN                   *BufferSize,
    OUT VOID                       *Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_DELETE) (
    IN EFI_FILE_PROTOCOL                     *This
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_CLOSE) (
    IN EFI_FILE_PROTOCOL                     *This
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_OPEN) (
    IN EFI_FILE_PROTOCOL                  *This,
    OUT EFI_FILE_PROTOCOL                 **NewHandle,
    IN CHAR16                             *FileName,
    IN UINT64                             OpenMode,
    IN UINT64                             Attributes
);

typedef struct {
    EFI_EVENT                         Event;
    EFI_STATUS                        Status;
    UINTN                             BufferSize;
    VOID                              *Buffer;
} EFI_FILE_IO_TOKEN;

typedef EFI_STATUS(EFIAPI *EFI_FILE_SET_POSITION) (
    IN EFI_FILE_PROTOCOL      *This,
    IN UINT64                 Position
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_POSITION) (
    IN EFI_FILE_PROTOCOL                *This,
    OUT UINT64                          *Position
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_GET_INFO) (
    IN EFI_FILE_PROTOCOL             *This,
    IN EFI_GUID                      *InformationType,
    IN OUT UINTN                     *BufferSize,
    OUT VOID                         *Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_SET_INFO) (
    IN EFI_FILE_PROTOCOL                *This,
    IN EFI_GUID                         *InformationType,
    IN UINTN                            BufferSize,
    IN VOID                             *Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_FILE_FLUSH) (
    IN EFI_FILE_PROTOCOL             *This
);

typedef struct {
    UINT64                         Size;
    UINT64                         FileSize;
    UINT64                         PhysicalSize;
    EFI_TIME                       CreateTime;
    EFI_TIME                       LastAccessTime;
    EFI_TIME                       ModificationTime;
    UINT64                         Attribute;
    CHAR16                         FileName [256];
} EFI_FILE_INFO;
  
//******************************************
// File Attribute Bits
//******************************************
  
#define EFI_FILE_READ_ONLY      0x0000000000000001
#define EFI_FILE_HIDDEN         0x0000000000000002
#define EFI_FILE_SYSTEM         0x0000000000000004
#define EFI_FILE_RESERVED       0x0000000000000008
#define EFI_FILE_DIRECTORY      0x0000000000000010
#define EFI_FILE_ARCHIVE        0x0000000000000020
#define EFI_FILE_VALID_ATTR     0x0000000000000037

typedef struct _EFI_FILE_PROTOCOL {
    UINT64                      Revision;
    EFI_FILE_OPEN               Open;
    EFI_FILE_CLOSE              Close;
    EFI_FILE_DELETE             Delete;
    EFI_FILE_READ               Read;
    EFI_FILE_WRITE              Write;
    EFI_FILE_GET_POSITION       GetPosition;
    EFI_FILE_SET_POSITION       SetPosition;
    EFI_FILE_GET_INFO           GetInfo;
    EFI_FILE_SET_INFO           SetInfo;
    EFI_FILE_FLUSH              Flush;
    VOID*                       OpenEx; // Added for revision 2
    VOID*                       ReadEx; // Added for revision 2
    VOID*                       WriteEx; // Added for revision 2
    VOID*                       FlushEx; // Added for revision 2
} EFI_FILE_PROTOCOL;

// Loaded image protocol
// https://uefi.org/specs/UEFI/2.10_A/09_Protocols_EFI_Loaded_Image.html#id3
#define EFI_LOADED_IMAGE_PROTOCOL_REVISION 0x1000

typedef EFI_STATUS(EFIAPI *EFI_IMAGE_UNLOAD) (
    IN EFI_HANDLE               ImageHandle
);

typedef struct {
    UINT32                        Revision;
    EFI_HANDLE                    ParentHandle;
    EFI_SYSTEM_TABLE              *SystemTable;
    EFI_HANDLE                    DeviceHandle;
    VOID                          *FilePath;
    VOID                          *Reserved;
    UINT32                        LoadOptionsSize;
    VOID                          *LoadOptions;
    VOID                          *ImageBase;
    UINT64                        ImageSize;
    EFI_MEMORY_TYPE               ImageCodeType;
    EFI_MEMORY_TYPE               ImageDataType;
    EFI_IMAGE_UNLOAD              Unload;
} EFI_LOADED_IMAGE_PROTOCOL;

// Efi Simple File Protocol
// https://uefi.org/specs/UEFI/2.10_A/13_Protocols_Media_Access.html#simple-file-system-protocol
#define EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION 0x00010000

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME) (
    IN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL                   *This,
    OUT EFI_FILE_PROTOCOL                                **Root
);

typedef struct _EFI_SIMPLE_FILE_SYSTEM_PROTOCOL {
    UINT64                                         Revision;
    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_OPEN_VOLUME    OpenVolume;
} EFI_SIMPLE_FILE_SYSTEM_PROTOCOL;

#endif