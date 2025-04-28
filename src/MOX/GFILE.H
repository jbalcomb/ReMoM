/*
    MAGIC.EXE
        seg011
    WIZARDS.EXE
        seg011

*/
#ifndef GFILE_H
#define GFILE_H

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */



#ifdef __cplusplus
extern "C" {
#endif

// WZD s11p1
FILE * gfopen(const char * filename, const char * mode);

// WZD s11p2
void gfread(void * ptr, int size, int nitems, FILE * fhandle);

// WZD s11p3
void gfwrite(void * ptr, int size, int nitems, FILE * fhandle);

// WZD s11p4
void gfseek(FILE * fhandle, int offset, int whence);

// WZD s11p5
void gfclose(FILE * fhandle);


#ifdef __cplusplus
}
#endif

#endif  /* GFILE_H */
