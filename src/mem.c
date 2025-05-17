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

UINTN SplitString(CHAR16* TokenBuffer, CHAR16* Input, CHAR16 Delimiter, CHAR16** OutputTokens) {
    UINTN i = 0;
    UINTN tokenIndex = 0;
    UINTN charIndex = 0;

    while (Input[i] != L'\0' && tokenIndex < 10) {
        if (Input[i] == Delimiter) {
            // End current token
            (&TokenBuffer[tokenIndex])[charIndex] = L'\0';
            OutputTokens[tokenIndex] = &TokenBuffer[tokenIndex];
            tokenIndex++;
            charIndex = 0;
        } else {
            if (charIndex < 100 - 1) {
                (&TokenBuffer[tokenIndex])[charIndex++] = Input[i];
            }
        }
        i++;
    }

    // Add last token
    if (tokenIndex < 10) {
        (&TokenBuffer[tokenIndex])[charIndex] = L'\0';
        OutputTokens[tokenIndex] = &TokenBuffer[tokenIndex];
        tokenIndex++;
    }

    return tokenIndex; // Return number of tokens
}

#endif