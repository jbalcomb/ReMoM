// _s09p03c.c lbx_seek
// ST_LBX.H

#include <io.h>         /* filelength(); SEEK_CUR, SEEK_END, SEEK_SET */


int lbx_seek(long foffset, int fhandle)
{
    int st_status;

    if ( lseek(fhandle, foffset, SEEK_SET) == -1L )  //  long lseek(int handle, long offset, int fromwhere);  returns FAILURE: -1L
    {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}
