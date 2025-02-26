#ifndef MOX_ITOA_H
#define MOX_ITOA_H



#ifdef __cplusplus
extern "C" {
#endif



// void mox_itoa(int n, char s[]);
#ifndef WIN32
void itoa(int value, char * string, int radix);
void ltoa(long value, char * string, int radix);
#endif



#ifdef __cplusplus
}
#endif



#endif /* MOX_ITOA_H */
