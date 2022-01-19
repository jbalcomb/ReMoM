// _s09p01c.c lbx_open
// ST_LBX.H

/*
    the global variable errno is set to one of the following:
        ENOENT  Path or file not found
        EMFILE  Too many open files
        EACCES  Permission denied
        EINVACC Invalid access code
*/

#include <DOS.H>

#define CF 1 /* Carry flag */

int lbx_open(char *fname)
{
    // int fhandle = -1;
    // printf("DEBUG: [%s, %d]: BEGIN: lbx_open(fname = %s)\n", __FILE__, __LINE__, fname);
    // _AH = 0x3D;
    // _AL = 0x00;
    // _DS = FP_SEG(fname);
    // _DX = FP_OFF(fname);
    // geninterrupt(0x21);
    // if ( (_FLAGS & CF) == 0 )
    // {
    //     fhandle = 0;
    //     printf("DEBUG: [%s, %d]: FAILURE: fhandle: %d\n", __FILE__, __LINE__, fhandle);
    // }
    // else
    // {
    //     fhandle = _AX;
    //     printf("DEBUG: [%s, %d]: SUCCESS: fhandle: %d\n", __FILE__, __LINE__, fhandle);
    // }
    // printf("DEBUG: [%s, %d]: END: lbx_open()\n", __FILE__, __LINE__);
    // return fhandle;

// asm push bp
// asm mov  bp, sp
// asm push si
// asm push di
// asm push es
asm push ds
asm mov ax, 3D00h
//asm mov dx, fname
asm lds dx, fname
asm int 21h
asm jc _openFailed
asm pop ds
// asm pop  es
// asm pop  di
// asm pop  si
// asm pop  bp
    return _AX;
_openFailed:
asm pop ds
// asm pop es
// asm pop di
// asm pop si
// asm pop bp
    return (0);
}
