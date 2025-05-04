#ifndef PRINT_C
#define PRINT_C

#include "EFI_SPEC/efi.h"
#include <stdarg.h>

BOOLEAN print_number(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EFI_STOP, UINTN number, UINT8 base, BOOLEAN is_signed) {
    const CHAR16 *digits = u"0123456789ABCDEF";
    CHAR16 buffer[24];
    UINTN i = 0;
    BOOLEAN negative = FALSE;

    if (base > 16) {
        EFI_STOP->OutputString(EFI_STOP, u"Invalid base specified!\r\n");
        return FALSE;
    }

    if (base == 10 && is_signed && (INTN)number < 0) {
       number = -(INTN)number;
       negative = TRUE;
    }

    do {
       buffer[i++] = digits[number % base];
       number /= base;
    } while (number > 0);

    switch (base) {
        case 2:
            buffer[i++] = u'b';
            buffer[i++] = u'0';
            break;

        case 8:
            buffer[i++] = u'o';
            buffer[i++] = u'0';
            break;

        case 10:
            if (negative) buffer[i++] = u'-';
            break;

        case 16:
            buffer[i++] = u'x';
            buffer[i++] = u'0';
            break;

        default:
            break;
    }

    buffer[i--] = u'\0';

    for (UINTN j = 0; j < i; j++, i--) {
        UINTN temp = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = temp;
    }

    EFI_STOP->OutputString(EFI_STOP, buffer);

    return TRUE;
}

void print(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EFI_STOP, CHAR16 *str, UINT8 Color, UINT8 BG){
    EFI_STOP->SetAttribute(EFI_STOP, EFI_TEXT_ATTR(Color, BG));
    EFI_STOP->OutputString(EFI_STOP, str);
}

BOOLEAN printf(EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL *EFI_STOP, CHAR16 *fmt, ...){
    CHAR16 c[2];
    va_list args;
    va_start(args, fmt);

    c[0] = u'\0', c[1] = u'\0';
    for (UINTN i = 0; fmt[i] != u'\0'; i++){
        if (fmt[i] == u'%'){
            i++;

            switch (fmt[i])
            {
                case u'c': {
                    CHAR16 *lchar = va_arg(args, CHAR16*);
                    CHAR16 buffer[2];
                    buffer[1] = u'\0';
                    buffer[0] = *lchar; 

                    EFI_STOP->OutputString(EFI_STOP, buffer);
                }
                break;
                case u's': {
                    CHAR16 *string = va_arg(args, CHAR16*);
                    EFI_STOP->OutputString(EFI_STOP, string);
                }
                break;
                case u'd': {
                    INT32 number = va_arg(args, INT32);
                    print_number(EFI_STOP, number, 10, TRUE);
                }
                break;
                case u'x': {
                    UINTN number = va_arg(args, UINTN);
                    print_number(EFI_STOP, number, 16, FALSE);
                }
                break;
                case u'b': {
                    UINTN number = va_arg(args, UINTN);
                    print_number(EFI_STOP, number, 2, FALSE);
                }
                break;
                case u't': {
                    BOOLEAN b = va_arg(args, int);
                    EFI_STOP->OutputString(EFI_STOP, b ? u"Y" : u"N");
                }
                break;
                default:
                    EFI_STOP->OutputString(EFI_STOP, u"%");
                    EFI_STOP->OutputString(EFI_STOP, &fmt[i]);
                    goto end;
                    break;
            }

        } else{
            c[0] = fmt[i];
            EFI_STOP->OutputString(EFI_STOP, c);
        }
    }
end:
    va_end(args);
    return TRUE;
}

CHAR16 *strcpy_u16(CHAR16* dst, CHAR16* src){
    if (!dst) return NULL;
    if (!src) return dst;

    CHAR16* result = dst;
    while(*src) *dst++ = *src++;
    *dst = u'\0';

    return result;
}
#endif