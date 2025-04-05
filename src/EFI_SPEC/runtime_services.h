#ifndef RUNTIME_SERVISES_H
#define RUNTIME_SERVISES_H
#include "efi.h"

typedef enum {
	EfiResetCold,
	EfiResetWarm,
	EfiResetShutdown
} Efi_Reset_Type;

typedef
uint64_t (EFIAPI *EFI_RESET_SYSTEM)
(
    Efi_Reset_Type  	        	ResetType,
    uint64_t		              	ResetStatus,
    uint64_t				        DataSize,
    uint16_t                   		*ResetData
);

typedef struct s_efi_runtime_service_handle {
    EFI_TABLE_HEADER Hdr;
    void*                           GetTime; 
    void*                           SetTime; 
    void*                           GetWakeupTime; 
    void*                           SetWakeupTime; 
    void*                           SetVirtualAddressMap;
    void*                           ConvertPointer;
    void*                           GetVariable;
    void*                           GetNextVariableName;
    void*                           SetVariable;
    void*                           GetNextHighMonotonicCount;
    EFI_RESET_SYSTEM                ResetSystem;
    void*                           UpdateCapsule;
    void*                           QueryCapsuleCapabilities;
    void*                           QueryVariableInfo; 
} EFI_RUNTIME_SERVICES;
#endif