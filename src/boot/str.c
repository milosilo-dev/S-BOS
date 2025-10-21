#ifndef STR_H
#define STR_H
#include "EFI_SPEC/efi.h"

UINTN strlen(CHAR16 *s) {
    UINTN len = 0;
    while (*s++) len++;
    return len;
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

CHAR16* stpstr(CHAR16* haystack, CHAR16* needle){
    if (!needle) return haystack;

    CHAR16* p = haystack;
    while (*p) {
        if (*p == *needle && !memcmp(p, needle, strlen(needle))) 
            return p + strlen(needle);
        p++;
    }

    return NULL;
}

BOOLEAN isdigit(char c) {
    return c >= '0' && c <= '9';
}

INTN atoi(CHAR16 *s) {
    INTN result = 0;
    while (*s >= L'0' && *s <= L'9') {
        result = (result * 10) + (*s - L'0');
        s++;
    }
    return result;
}
#endif