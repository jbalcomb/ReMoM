// MOM_DEF.H
#include <stdlib.h>
#include <stdio.h>

// https://www.rosettacode.org/wiki/File_size#C

unsigned int C_GetFileSize(const char *filename)
{
    unsigned int result;
    FILE *fh = fopen(filename, "rb");
    fseek(fh, 0, SEEK_END);
    result = (unsigned int)ftell(fh);
    fclose(fh);
    return result;
}
