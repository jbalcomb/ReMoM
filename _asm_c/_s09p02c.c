// _s09p02c.c lbx_close

int lbx_close(int fhandle)
{
    return close(fhandle);  // returns SUCCESS: 0, FAILURE: -1
}
