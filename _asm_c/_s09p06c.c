// lbx_read_ofst
// ST_LBX.H

#include "ST_HEAD.H"

int  lbx_read_ofst(unsigned int dst_ofst, int nbytes, int fhandle)
{
    int bytes;
    //void * buf;
    //buf = malloc();
    //read(fhandle, buf, 10)
    void _FAR * buf;
    //unsigned int ofst;
    int st_status;

    buf = MK_FP(_DS, dst_ofst);
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
