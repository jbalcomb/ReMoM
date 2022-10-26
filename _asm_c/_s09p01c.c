// _s09p01c.c lbx_open
// ST_LBX.H

#include <fcntl.h>  /* O_BINARY, O_RDONLY */
#include<sys\stat.h>

int lbx_open(char *fname)
{
    int fhandle;

    fhandle = open(fname, O_RDONLY|O_BINARY);

    if (fhandle == -1)
    {
        fhandle = 0;
    }

    return fhandle;
}
