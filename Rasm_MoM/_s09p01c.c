// _s09p01c.c lbx_open

/*
    the global variable errno is set to one of the following:
        ENOENT  Path or file not found
        EMFILE  Too many open files
        EACCES  Permission denied
        EINVACC Invalid access code
*/

int lbx_open(char *fname)
{
// asm push bp
// asm mov  bp, sp
// asm push si
// asm push di
// asm push es
asm push ds
asm mov  ax, 3D00h
//asm mov  dx, fname
asm lds  dx, fname
asm int  21h
asm jc _openFailed
asm pop  ds
// asm pop  es
// asm pop  di
// asm pop  si
// asm pop  bp
    return _AX;
_openFailed:
asm pop  ds
// asm pop  es
// asm pop  di
// asm pop  si
// asm pop  bp
    return (0);
}
