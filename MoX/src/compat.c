/*

https://github.com/lattera/glibc/blob/master/string/strcasecmp.c

https://github.com/torvalds/linux/blob/master/lib/string.c

tolower()
C:   #include <ctype.h>
C++: #include <cctype>

*/

/*
https://github.com/lattera/glibc/blob/master/string/strcasecmp.c
*/
// /* Compare S1 and S2, ignoring case, returning less than, equal to or
//    greater than zero if S1 is lexicographically less than,
//    equal to or greater than S2.  */
// int
// __strcasecmp (const char *s1, const char *s2 LOCALE_PARAM)
// {
// #if defined _LIBC && !defined USE_IN_EXTENDED_LOCALE_MODEL
//   locale_t loc = _NL_CURRENT_LOCALE;
// #endif
//   const unsigned char *p1 = (const unsigned char *) s1;
//   const unsigned char *p2 = (const unsigned char *) s2;
//   int result;
// 
//   if (p1 == p2)
//     return 0;
// 
//   while ((result = TOLOWER (*p1) - TOLOWER (*p2++)) == 0)
//     if (*p1++ == '\0')
//       break;
// 
//   return result;
// }



/*
https://github.com/torvalds/linux/blob/master/lib/string.c
*/
// int strcasecmp(const char *s1, const char *s2)
// {
// 	int c1, c2;
// 
// 	do {
// 		c1 = tolower(*s1++);
// 		c2 = tolower(*s2++);
// 	} while (c1 == c2 && c1 != 0);
// 	return c1 - c2;
// }

#include <ctype.h>

int strcasecmp(const char *s1, const char *s2)
{
    int c1;
    int c2;
    do
    {
        c1 = tolower(*s1++);
        c2 = tolower(*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}
