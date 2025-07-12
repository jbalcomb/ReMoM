/*
    MAGIC.EXE
        seg011
    WIZARDS.EXE
        seg011

*/

#include "GFILE.H"

#include "MOX_TYPE.H"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */



// WZD dseg:7602                                                 BEGIN:  seg011 - Initialized Data
// WZD dseg:7602 72 00                                           str_r__WZD_seg011 db 'r',0              ; DATA XREF: gfopen+7o
// WZD dseg:7604 72 62 00                                        str_rb__WZD_seg011 db 'rb',0            ; DATA XREF: gfopen+17o
// WZD dseg:7607 77 00                                           str_w__WZD_seg011 db 'w',0              ; DATA XREF: gfopen:loc_169D5o
// WZD dseg:7609 77 62 00                                        str_wb__WZD_seg011 db 'wb',0            ; DATA XREF: gfopen+44o
// WZD dseg:7609                                                 END:  seg011 - Initialized Data

// WZD dseg:E5E6                                                 BEGIN:  seg011 - Unitialized Data
// WZD dseg:E5E6
int16_t _gf_bsize;
// WZD dseg:E5E8
FILE * _gf_fhandle;
// WZD dseg:E5E8                                                 END:  seg011 - Unitialized Data




/*
    MGC  seg011
    WZD  seg011
*/

// TODO  #define gfrewind(_fhandle_)  gfseek(_fhandle_, 0L, SEEK_SET)

// WZD s11p1
FILE * gfopen(const char * filename, const char * mode)
{
    // TODO  if if if if read, write, read & write
    _gf_fhandle = fopen(filename, mode);
    return _gf_fhandle;
}

// WZD s11p2
void gfread(void * ptr, int size, int nitems, FILE * fhandle)
{
    _gf_bsize = (size * nitems);
    fread(ptr, _gf_bsize, 1, _gf_fhandle);
}

// WZD s11p3
void gfwrite(void * ptr, int size, int nitems, FILE * fhandle)
{
    _gf_bsize = (size * nitems);
    fwrite(ptr, _gf_bsize, 1, _gf_fhandle);
}

// WZD s11p4
void gfseek(FILE * fhandle, int offset, int whence)
{
    fseek(_gf_fhandle, offset, whence);
}

// WZD s11p5
void gfclose(FILE * fhandle)
{
    fclose(_gf_fhandle);
}

