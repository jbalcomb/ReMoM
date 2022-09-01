// _s09p05c.c lbx_read_sgmt
// ST_LBX.H

#include "ST_HEAD.H"

int lbx_read_sgmt(unsigned int dst_sgmt, int nbytes, int fhandle)
{
    int bytes;
    //void * buf;
    //buf = malloc();
    //read(fhandle, buf, 10)
    void _FAR * buf;
    //unsigned int ofst;
    int st_status;

    buf = MK_FP(dst_sgmt, 0);
    //ofst = 0;
    //buf = (void *)((long)(sgmt << 16) | ofst);

    if ((bytes = read(fhandle, buf, nbytes)) == -1) {
        st_status = 0;  // ST_FAILURE
    }
    else
    {
        st_status = -1;  // ST_SUCCESS
    }

    return st_status;
}
