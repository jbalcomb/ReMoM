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
#include <stdio.h>
#include <string.h>

/* reverse:  reverse string s in place */
void mox_reverse(char s[])
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
     }
}  

 /* itoa:  convert n to characters in s */
void mox_itoa(int n, char s[])
{
    int i, sign;

    if ((sign = n) < 0)  /* record sign */
        n = -n;          /* make n positive */
    i = 0;
    do {       /* generate digits in reverse order */
        s[i++] = n % 10 + '0';   /* get next digit */
    } while ((n /= 10) > 0);     /* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    mox_reverse(s);
}  

#ifndef WIN32
void itoa(int value, char * string, int radix)
{
    mox_itoa(value, string);
}

void ltoa(long value, char * string, int radix)
{
    sprintf(string, "%ld", value);
}
#endif
