/*

https://stackoverflow.com/questions/190229/where-is-the-itoa-function-in-linux

itoa is not a standard C function.  
You can implement your own.  
It appeared in the first edition of Kernighan and Ritchie's The C Programming Language, on page 60.  
The second edition of The C Programming Language ("K&R2") contains the following implementation of itoa, on page 64.  
The book notes several issues with this implementation,  
  including the fact that it does not correctly handle the most negative number.  

*/
/*

https://stackoverflow.com/questions/21501815/optimal-base-10-only-itoa-function/21502307#21502307
https://stackoverflow.com/questions/4351371/c-performance-challenge-integer-to-stdstring-conversion
https://stackoverflow.com/questions/10488468/fast-integer-to-decimal-conversion
https://stackoverflow.com/questions/4351371/c-performance-challenge-integer-to-stdstring-conversion/4351465#4351465
https://github.com/fmtlib/format-benchmark/blob/master/src/u2985907.h

*/
/*
https://github.com/PaulBernier/UART_STM32F4/blob/master/ltoa.c

*/
/*
https://www.strudel.org.uk/itoa/
Update: (2006/10/15)
Luiz Gon?lves tells me that although not an ANSI standard, itoa comes in many packages and it is written in many textbooks.
He suggests a version written in pure ANSI C based on a version from Kernighan & Ritchie's Ansi C.
A base error is reported by the return of an empty string but the function does no checks of sizes and no allocations.
This version is provided below along with a slightly modified version (architecture specific tweaks), the std::string version and the C++ char* itoa() version.
*/
#include "MOX_TYPE.h"

#include <stdlib.h>  /* div_t; div(); */
void strreverse(char * begin, char * end)
{
    char aux;
    while(end > begin)
        aux = *end, *end-- = *begin, *begin++ = aux;
}
/*
    Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C":
*/
void itoa1(int value, char * str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char * wstr = str;
    int sign;
    // Validate base
    if(base < 2 || base > 35) { *wstr = '\0'; return; }
    // Take care of sign
    if((sign = value) < 0) value = -value;
    // Conversion. Number is reversed.
    do *wstr++ = num[value % base]; while(value /= base);
    if(sign < 0) *wstr++ = '-';
    *wstr = '\0';
    // Reverse string
    strreverse(str,wstr - 1);
}
/*
    Ansi C "itoa" based on Kernighan & Ritchie's "Ansi C" with slight modification to optimize for specific architecture:
*/
void itoa2(int value, char * str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char* wstr = str;
    int sign;
    div_t res;
    // Validate base
    if(base < 2 || base > 35) { *wstr='\0'; return; }
    // Take care of sign
    if((sign = value) < 0) value = -value;
    // Conversion. Number is reversed.
    do {
        res = div(value,base);
        *wstr++ = num[res.rem];
    } while((value = res.quot));
    if(sign < 0) *wstr++ = '-';
    *wstr = '\0';
    // Reverse string
    strreverse(str,wstr-1);
}

/*
https://forum.microchip.com/s/topic/a5C3l000000MB7pEAG/t272825

Microchip included some useful functions in the tcp/ip stack code in Helpers.c
 void uitoa(WORD Value, BYTE* Buffer)
and void ultoa(DWORD Value, BYTE* Buffer)

*/
// this routine found online somewhere, then tweaked
 // returns pointer to ASCII string in a static buffer
char * i32toa(int32_t value) 
{
    static char buffer[12]; // 12 bytes is big enough for a 4-byte, signed number
    int original = value;   // save original value
    int c;

    c = sizeof(buffer) - 1;

    buffer[c] = 0;                    // write trailing null in last byte of buffer     

    if(value < 0)                     // if it's negative, note that and take the absolute value
        value = -value;

    do                                // write least significant digit of value that's left
    {
        buffer[--c] = (value % 10) + '0';     
        value /= 10;
    } while (value);

    if(original < 0) 
        buffer[--c] = '-';

    return &buffer[c];

}

/*****************************************************************************
   Function:
     void uitoa(WORD Value, BYTE* Buffer)
 
   Summary:
     Converts an unsigned integer to a decimal string.
 
   Description:
     Converts a 16-bit unsigned integer to a null-terminated decimal string.
 
   Precondition:
     None
 
   Parameters:
     Value    - The number to be converted
     Buffer    - Pointer in which to store the converted string
 
   Returns:
       None
   ***************************************************************************/
// void uitoa(WORD Value, BYTE * Buffer)
void uitoa(uint16_t Value, char * Buffer)
{
    // BYTE i;
    // WORD Digit;
    // WORD Divisor;
    // BOOL Printed = FALSE;
    uint8_t i;
    int16_t Digit;
    int16_t Divisor;
    uint8_t Printed;

    Printed = 0;  // FALSE;

    if(Value)
    {
        for(i = 0, Divisor = 10000; i < 5u; i++)
        {
            Digit = Value / Divisor;
            if(Digit || Printed)
            {
                *Buffer++ = '0' + Digit;
                Value -= Digit * Divisor;
                Printed = 1;  // TRUE;
            }
            Divisor /= 10;
        }
    }
    else
    {
        *Buffer++ = '0';
    }

    *Buffer = '\0';
}


/*
AdvsrScr.C
Line 799
    // sprintf: This function formats and writes data to a string.
    // sprintf(str, "%d", num);
    // snprintf: A safer version of sprintf that prevents buffer overflows.
    // snprintf(str, sizeof(str), "%d", num);
    /// itoa((1400 + WTF__turns_years), GUI_String_2, 10);
    // sprintf(GUI_String_2, "%d", (1400 + WTF__turns_years));
    // snprintf(GUI_String_2, sizeof(GUI_String_2), "%d", (1400 + WTF__turns_years));
    mox_itoa((1400 + WTF__turns_years), GUI_String_2);
*/
// // #include <stdio.h>
// // #include <stdlib.h>
// #include <string.h>
// 
// /* reverse:  reverse string s in place */
// void mox_reverse(char s[])
// {
//     int i, j;
//     char c;
// 
//     for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
//         c = s[i];
//         s[i] = s[j];
//         s[j] = c;
//     }
// }  
// 
// /* itoa:  convert n to characters in s */
// void mox_itoa(int n, char s[])
// {
//     int i, sign;
// 
//     if ((sign = n) < 0)  /* record sign */
//         n = -n;          /* make n positive */
//     i = 0;
//     do {       /* generate digits in reverse order */
//         s[i++] = n % 10 + '0';   /* get next digit */
//     } while ((n /= 10) > 0);     /* delete it */
//     if (sign < 0)
//         s[i++] = '-';
//     s[i] = '\0';
//     mox_reverse(s);
// }  

void mox_itoa(int value, char * str, int base)
{
    static char num[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    char * wstr = str;
    int sign;

    // Validate base
    if(base < 2 || base > 35) { *wstr = '\0'; return; }

    // Take care of sign
    if((sign = value) < 0) value = -value;

    // Conversion. Number is reversed.
    do *wstr++ = num[value % base]; while(value /= base);
    if(sign < 0) *wstr++ = '-';
    *wstr = '\0';

    // Reverse string
    strreverse(str,wstr - 1);
}

// #ifndef _WIN32
// void itoa(int value, char * string, int radix)
// {
//     mox_itoa(value, string);
// }
// 
// void ltoa(long value, char * string, int radix)
// {
//     sprintf(string, "%ld", value);
// }
// #endif
