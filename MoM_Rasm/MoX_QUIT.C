
#include "MoX_QUIT.H"

#if defined(__DOS16__)
#include "DOS.H"  /* geninterrupt() */
#endif


// s06p01
#if defined(__DOS16__)
void VGA_SetTextMode(void)
{
asm mov ax, 0x03
asm int 0x10
}
#endif
#if defined(__WIN32__)
void VGA_SetTextMode(void)
{

}
#endif

// s06p02
#if defined(__DOS16__)
void QuitToDOS(char *quitmsg)
{
    char *ptr_quitmsg;
    printf("%s\n\n", quitmsg);
    _AH = 0x4C;  // INT 21,4C - Terminate Process With Return Code
    geninterrupt(0x21);  // DOS_INT
}
#endif
#if defined(__WIN32__)
void QuitToDOS(char *quitmsg)
{

}
#endif

// s06p04
#if defined(__DOS16__)
unsigned int dos_memfree_kb(void)
{
    _BX = 0xFFFF;
    _AH = 0x48;  // ALLOCMEM
    geninterrupt(0x21);  // DOS_INT
    return( (_BX >> 6) );  // divide by 2^6 = 64  (e.g. 256 pages * 16 = 4096 bytes; 4096 bytes / 64 = 4 KB )
}
#endif
#if defined(__WIN32__)
unsigned int dos_memfree_kb(void)
{
    return 0;
}
#endif

