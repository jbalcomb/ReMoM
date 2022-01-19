// _s06p04c.c dos_memfree_kb  RAM_GetFreeBlockSize
// in MOM_DEF.H

#include "MOM_HEAD.H"

unsigned int dos_memfree_kb(void)
{
    _BX = 0xFFFF;
    _AH = ALLOCMEM;
    geninterrupt(DOS_INT);
    return( (_BX >> 6) );  // divide by 2^6 = 64  (e.g. 256 pages * 16 = 4096 bytes; 4096 bytes / 64 = 4 KB )
}
