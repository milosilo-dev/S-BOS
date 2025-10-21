#ifndef DISK_IO_H
#define DISK_IO_H
#include "efi.h"

typedef struct _EFI_DISK_IO_PROTOCOL EFI_DISK_IO_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_DISK_READ) (
    IN EFI_DISK_IO_PROTOCOL        *This,
    IN UINT32                      MediaId,
    IN UINT64                      Offset,
    IN UINTN                       BufferSize,
    OUT VOID                       *Buffer
);

typedef EFI_STATUS(EFIAPI *EFI_DISK_WRITE) (
    IN EFI_DISK_IO_PROTOCOL    *This,
    IN UINT32                  MediaId,
    IN UINT64                  Offset,
    IN UINTN                   BufferSize,
    IN VOID                    *Buffer
);

typedef struct _EFI_DISK_IO_PROTOCOL {
  UINT64                         Revision;
  EFI_DISK_READ                  ReadDisk;
  EFI_DISK_WRITE                 WriteDisk;
} EFI_DISK_IO_PROTOCOL;
#endif