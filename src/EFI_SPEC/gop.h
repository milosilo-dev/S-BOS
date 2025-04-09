#ifndef GOP_H
#define GOP_H
#include "efi.h"

#define EFI_EDID_OVERRIDE_DONT_OVERRIDE 0x01
#define EFI_EDID_OVERRIDE_ENABLE_HOT_PLUG 0x02

typedef struct _EFI_GRAPHICS_OUTPUT_PROTCOL EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct {
    UINT32              RedMask;
    UINT32              GreenMask;
    UINT32              BlueMask;
    UINT32              ReservedMask;
} EFI_PIXEL_BITMASK;

typedef enum {
    PixelRedGreenBlueReserved8BitPerColor,
    PixelBlueGreenRedReserved8BitPerColor,
    PixelBitMask,
    PixelBltOnly,
    PixelFormatMax
} EFI_GRAPHICS_PIXEL_FORMAT;

typedef struct {
    UINT32                    Version;
    UINT32                    HorizontalResolution;
    UINT32                    VerticalResolution;
    EFI_GRAPHICS_PIXEL_FORMAT PixelFormat;
    EFI_PIXEL_BITMASK         PixelInformation;
    UINT32                    PixelsPerScanLine;
} EFI_GRAPHICS_OUTPUT_MODE_INFORMATION;

typedef struct {
    UINT32                                    MaxMode;
    UINT32                                    Mode;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION      *Info;
    UINTN                                     SizeOfInfo;
    EFI_PHYSICAL_ADDRESS                      FrameBufferBase;
    UINTN                                     FrameBufferSize;
} EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE;

typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL              *This,
    IN UINT32                                    ModeNumber,
    OUT UINTN                                    *SizeOfInfo,
    OUT EFI_GRAPHICS_OUTPUT_MODE_INFORMATION     **Info
);

typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL                *This,
    IN UINT32                                      ModeNumber
);

typedef struct {
    UINT8                        Blue;
    UINT8                        Green;
    UINT8                        Red;
    UINT8                        Reserved;
} EFI_GRAPHICS_OUTPUT_BLT_PIXEL;
   
typedef enum {
    EfiBltVideoFill,
    EfiBltVideoToBltBuffer,
    EfiBltBufferToVideo,
    EfiBltVideoToVideo,
    EfiGraphicsOutputBltOperationMax
} EFI_GRAPHICS_OUTPUT_BLT_OPERATION;
   
typedef EFI_STATUS(EFIAPI *EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT) (
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL                 *This,
    IN OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL            *BltBuffer, OPTIONAL
    IN EFI_GRAPHICS_OUTPUT_BLT_OPERATION            BltOperation,
    IN UINTN                                        SourceX,
    IN UINTN                                        SourceY,
    IN UINTN                                        DestinationX,
    IN UINTN                                        DestinationY,
    IN UINTN                                        Width,
    IN UINTN                                        Height,
    IN UINTN                                        Delta OPTIONAL
);

typedef struct _EFI_GRAPHICS_OUTPUT_PROTCOL {
    EFI_GRAPHICS_OUTPUT_PROTOCOL_QUERY_MODE     QueryMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_SET_MODE       SetMode;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_BLT            Blt;
    EFI_GRAPHICS_OUTPUT_PROTOCOL_MODE           *Mode;
} EFI_GRAPHICS_OUTPUT_PROTOCOL;

typedef struct {
    UINT32                              SizeOfEdid;
    UINT8                               *Edid;
} EFI_EDID_DISCOVERED_PROTOCOL;

typedef struct {
    UINT32                             SizeOfEdid;
    UINT8                              *Edid;
} EFI_EDID_ACTIVE_PROTOCOL;

typedef struct _EFI_EDID_OVERRIDE_PROTOCOL EFI_EDID_OVERRIDE_PROTOCOL;

typedef EFI_STATUS(EFIAPI *EFI_EDID_OVERRIDE_PROTOCOL_GET_EDID) (
    IN   EFI_EDID_OVERRIDE_PROTOCOL                   *This,
    IN   EFI_HANDLE                                   *ChildHandle,
    OUT  UINT32                                       *Attributes,
    OUT UINTN                                         *EdidSize,
    OUT UINT8                                         **Edid
);

typedef struct _EFI_EDID_OVERRIDE_PROTOCOL {
    EFI_EDID_OVERRIDE_PROTOCOL_GET_EDID      GetEdid;
} EFI_EDID_OVERRIDE_PROTOCOL;
#endif