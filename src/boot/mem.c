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

INTN memcmp(VOID *m1, VOID *m2, UINTN len) {
    UINT8 *p = m1;
    UINT8 *q = m2;
    for (UINTN i = 0; i < len; i++)
        if (p[i] != q[i]) return (INTN)(p[i]) - (INTN)(q[i]);

    return 0;
}
#endif