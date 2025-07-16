#ifndef MOX_ITOA_H
#define MOX_ITOA_H



#ifdef __cplusplus
extern "C" {
#endif



// void mox_itoa(int n, char s[]);
#ifndef _WIN32
void itoa(int value, char * string, int radix);
void ltoa(long value, char * string, int radix);
#else
// #include <stdlib.h>
#endif
/*
e.g.,
  itoa(  (1400 + WTF__turns_years), GUI_String_2,     10);
itoa(   (1400 + WTF__turns_years), GUI_String_2,     10);
_itoa_s( (1400 + WTF__turns_years), GUI_String_2, 10, 10);
*/


#ifdef __cplusplus
}
#endif



#endif /* MOX_ITOA_H */
