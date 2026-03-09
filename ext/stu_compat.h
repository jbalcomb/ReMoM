/*
 * stu_compat.h - STB-style cross-platform compatibility library
 *
 * Provides portable implementations of non-standard C functions used across
 * the ReMoM project. Covers MSVC, GCC, Clang, and legacy Borland idioms.
 *
 * USAGE:
 *   In exactly ONE .c file, before including this header, define:
 *     #define STU_COMPAT_IMPLEMENTATION
 *     #include "stu_compat.h"
 *
 *   In all other files, just include the header normally:
 *     #include "stu_compat.h"
 *
 * WHAT THIS PROVIDES:
 *   - stu_itoa()          portable itoa() (integer to string)
 *   - stu_stricmp()       portable case-insensitive strcmp
 *   - stu_strnicmp()      portable case-insensitive strncmp
 *   - stu_getch()         portable getch() (unbuffered console input)
 *   - stu_getcwd()        portable getcwd()
 *   - stu_putenv()        portable putenv()
 *   - stu_tzset()         portable tzset()
 *   - stu_debugbreak()    portable debug breakpoint
 *
 * COMPATIBILITY MACROS:
 *   Optionally define STU_COMPAT_ALIASES before including to get drop-in
 *   macros that map the original non-standard names (itoa, _stricmp, etc.)
 *   to the stu_ prefixed versions.
 */

#ifndef STU_COMPAT_H
#define STU_COMPAT_H

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Platform detection
 * ========================================================================= */

#if defined(_WIN32) || defined(_WIN64)
#  define STU_PLATFORM_WINDOWS 1
#else
#  define STU_PLATFORM_WINDOWS 0
#endif

#if defined(__linux__)
#  define STU_PLATFORM_LINUX 1
#else
#  define STU_PLATFORM_LINUX 0
#endif

#if defined(__APPLE__)
#  define STU_PLATFORM_MACOS 1
#else
#  define STU_PLATFORM_MACOS 0
#endif

#if defined(_MSC_VER)
#  define STU_COMPILER_MSVC 1
#else
#  define STU_COMPILER_MSVC 0
#endif

#if defined(__GNUC__)
#  define STU_COMPILER_GCC 1
#else
#  define STU_COMPILER_GCC 0
#endif

/* ============================================================================
 * Function declarations
 * ========================================================================= */

/* Convert integer to string with given radix (2-36). Returns dst. */
char *stu_itoa(int value, char *dst, int radix);

/* Case-insensitive string comparison. Returns <0, 0, or >0. */
int stu_stricmp(const char *s1, const char *s2);

/* Case-insensitive string comparison up to n characters. */
int stu_strnicmp(const char *s1, const char *s2, size_t n);

/* Read a single character from console without echo. Returns the character. */
int stu_getch(void);

/* Get current working directory. Returns buf on success, NULL on failure. */
char *stu_getcwd(char *buf, size_t size);

/* Set environment variable from "NAME=VALUE" string. Returns 0 on success. */
int stu_putenv(const char *envstring);

/* Initialize timezone from TZ environment variable. */
void stu_tzset(void);

/* Trigger a debug breakpoint. */
void stu_debugbreak(void);

/* ============================================================================
 * Compatibility aliases (optional, define STU_COMPAT_ALIASES to enable)
 * ========================================================================= */

#ifdef STU_COMPAT_ALIASES

#define itoa        stu_itoa
#define _itoa       stu_itoa
#define stricmp     stu_stricmp
#define _stricmp    stu_stricmp
#define strcasecmp  stu_stricmp
#define strnicmp    stu_strnicmp
#define _strnicmp   stu_strnicmp
#define strncasecmp stu_strnicmp
#define getch       stu_getch
#define _getch      stu_getch
#define getcwd      stu_getcwd
#define _getcwd     stu_getcwd
#define putenv      stu_putenv
#define _putenv     stu_putenv
#define tzset       stu_tzset
#define _tzset      stu_tzset

#endif /* STU_COMPAT_ALIASES */

/* ============================================================================
 * Implementation
 * ========================================================================= */

#ifdef STU_COMPAT_IMPLEMENTATION

/* --------------------------------------------------------------------------
 * stu_itoa - portable integer to string conversion
 * -------------------------------------------------------------------------- */
char *stu_itoa(int value, char *dst, int radix)
{
    char tmp[34]; /* enough for base-2 of a 32-bit int + sign + null */
    char *tp = tmp;
    char *out = dst;
    int sign;
    unsigned int v;

    if (radix < 2 || radix > 36)
    {
        dst[0] = '\0';
        return dst;
    }

    sign = (radix == 10 && value < 0);
    if (sign)
    {
        v = (unsigned int)(-(value + 1)) + 1u; /* handles INT_MIN safely */
    }
    else
    {
        v = (unsigned int)value;
    }

    while (v || tp == tmp)
    {
        unsigned int digit = v % (unsigned int)radix;
        *tp++ = (char)(digit < 10 ? '0' + digit : 'a' + digit - 10);
        v /= (unsigned int)radix;
    }

    if (sign)
    {
        *out++ = '-';
    }

    while (tp > tmp)
    {
        *out++ = *--tp;
    }
    *out = '\0';

    return dst;
}

/* --------------------------------------------------------------------------
 * stu_stricmp - portable case-insensitive string comparison
 * -------------------------------------------------------------------------- */
int stu_stricmp(const char *s1, const char *s2)
{
    int c1, c2;
    do
    {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
    } while (c1 == c2 && c1 != 0);
    return c1 - c2;
}

/* --------------------------------------------------------------------------
 * stu_strnicmp - portable case-insensitive bounded string comparison
 * -------------------------------------------------------------------------- */
int stu_strnicmp(const char *s1, const char *s2, size_t n)
{
    int c1, c2;
    if (n == 0)
    {
        return 0;
    }
    do
    {
        c1 = tolower((unsigned char)*s1++);
        c2 = tolower((unsigned char)*s2++);
        if (c1 != c2)
        {
            return c1 - c2;
        }
        if (c1 == 0)
        {
            break;
        }
    } while (--n > 0);
    return 0;
}

/* --------------------------------------------------------------------------
 * stu_getch - portable unbuffered console character read
 * -------------------------------------------------------------------------- */
#if STU_PLATFORM_WINDOWS
#include <conio.h>
int stu_getch(void)
{
    return _getch();
}
#else
#include <termios.h>
#include <unistd.h>
int stu_getch(void)
{
    struct termios oldt, newt;
    int ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~((tcflag_t)ICANON | (tcflag_t)ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
#endif

/* --------------------------------------------------------------------------
 * stu_getcwd - portable get current working directory
 * -------------------------------------------------------------------------- */
#if STU_PLATFORM_WINDOWS
#include <direct.h>
char *stu_getcwd(char *buf, size_t size)
{
    return _getcwd(buf, (int)size);
}
#else
#include <unistd.h>
char *stu_getcwd(char *buf, size_t size)
{
    return getcwd(buf, size);
}
#endif

/* --------------------------------------------------------------------------
 * stu_putenv - portable environment variable setting
 * -------------------------------------------------------------------------- */
int stu_putenv(const char *envstring)
{
#if STU_COMPILER_MSVC
    return _putenv(envstring);
#else
    return putenv((char *)envstring);
#endif
}

/* --------------------------------------------------------------------------
 * stu_tzset - portable timezone initialization
 * -------------------------------------------------------------------------- */
void stu_tzset(void)
{
#if STU_COMPILER_MSVC
    _tzset();
#else
    tzset();
#endif
}

/* --------------------------------------------------------------------------
 * stu_debugbreak - portable debug breakpoint
 * -------------------------------------------------------------------------- */
void stu_debugbreak(void)
{
#if STU_COMPILER_MSVC
    __debugbreak();
#elif STU_COMPILER_GCC || defined(__clang__)
    __builtin_trap();
#else
    raise(SIGTRAP);
#endif
}

#endif /* STU_COMPAT_IMPLEMENTATION */

#ifdef __cplusplus
}
#endif

#endif /* STU_COMPAT_H */
