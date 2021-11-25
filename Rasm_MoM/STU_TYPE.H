#ifndef STU_TYPES_H
#define STU_TYPES_H


/*
    J:\STU\DBWD\BORLANDC\DOC\HELPME!.DOC

    Q.  What does the _seg modifier do?
    A.  Using _seg causes a pointer to become a storage place for a
        segment value, rather than an offset ( or a segment/offset ).
        For instance, if "int _seg *x" contains the value 0x40,
        then when you use "*x", the value pointed to will be at
        segment 0x40, offset 0. If you add a value to the pointer,
        the value is multiplied by the size of the pointer type. That
        new value is used as an offset, and is combined with the segment
        value contained in the pointer. For instance,

            int _seg *x;
            int value;

            x = (int _seg *)0x40;
            value = *(x + 20);

        value is assigned the value of the integer at 0x40:0x28
        (Remember, 20 * sizeof(int) = 40 = 0x28).

        You can find a more detailed description of _seg in the Borland C++ Programmer's Guide, Chapter 9.

    BCpp31PG, pg 339/354
        
*/
/*
    http://archive.retro.co.za/CDROMs/DrDobbs/CD%20Release%2012/articles/1988/8811/8811e/8811e.htm

*/

// J:\STU\DBWD\developc\Animator\Vpaint\PJ\picdrive\jpeg\LIB\jmemname.c
#ifndef SEEK_SET		/* pre-ANSI systems may not define this; */
#define SEEK_SET  0		/* if not, assume 0 is correct */
#endif


typedef unsigned int sgmt_addr;
typedef unsigned int ofst_addr;
typedef void _seg * sgmt_ptr;
typedef void near * near_ptr;
typedef void far * far_ptr;

typedef void _seg * memptr;  // J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_MM.H


// /* Compile time assertions. */
// #define ASSERT_CONCAT_(a, b)    a##b
// #define ASSERT_CONCAT(a, b)     ASSERT_CONCAT_(a, b)
// #define STATIC_ASSERT(module, e) \
// 	struct ASSERT_CONCAT(static_assert_##module##_line_, __LINE__) \
// 		{ unsigned int bf : !!(e); }


// #define GCC_PACKED

// #if defined(__GNUC__)
// #undef GCC_PACKED
// #define GCC_PACKED  __attribute__((packed))
// #endif


// typedef char BYTE;
// typedef unsigned char UBYTE;
// typedef short WORD;
// typedef unsigned short UWORD;

// #if defined(__TURBOC__)
// typedef long LONG;
// typedef unsigned long ULONG;
// #else
// typedef int LONG;
// typedef unsigned int ULONG;
// #endif

// STATIC_ASSERT(jimk, sizeof( BYTE) == 1);
// STATIC_ASSERT(jimk, sizeof(UBYTE) == 1);
// STATIC_ASSERT(jimk, sizeof( WORD) == 2);
// STATIC_ASSERT(jimk, sizeof(UWORD) == 2);
// STATIC_ASSERT(jimk, sizeof( LONG) == 4);
// STATIC_ASSERT(jimk, sizeof(ULONG) == 4);


// #if defined(__TURBOC__)
// struct byte_regs
// 	{
// 	UBYTE al, ah, bl, bh, cl, ch, dl, dh;
// 	UWORD si, di, ds, es;
// 	};
// struct word_regs
// 	{
// 	UWORD ax, bx, cx, dx;
// 	UWORD si, di, ds, es;
// 	};
// union regs
// 	{
// 	struct byte_regs b;
// 	struct word_regs w;
// 	};
// #endif /* __TURBOC__ */

#endif /* STU_TYPES_H */
