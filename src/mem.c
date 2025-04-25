#ifndef MEM_C
#define MEM_C
#include "EFI_SPEC/efi.h"

VOID *memset(VOID *dst, UINT8 c, UINTN len){
    UINT8 *p = dst;
    for (UINTN i = 0; i < len; i++)
        p[i] = c;
    
    return dst;
}

VOID *memcpy(VOID *dst, VOID *src, UINTN len) {
    UINT8 *p = dst;
    UINT8 *q = src;

    for (UINTN i = 0; i < len; i++)
        p[i] = q[i];
    
    return dst;
}

#endif