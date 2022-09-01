// _s09p07c.c strcpyfar

#include "ST_HEAD.H"

/*
J:\STU\DBWD\BORLANDC\CRTL\CLIB\STRCPY.CAS
    char *_CType _FARFUNC strcpy(char *dest, const char *src)
J:\STU\DBWD\BORLANDC\CRTL\CLIB\STPCPY.C
    char _FAR * _CType stpcpy(char _FAR *to, const char _FAR *from)
J:\STU\DBWD\BORLANDC\CRTL\CLIB\_STPCPY.C
    char *_stpcpy(char *to, const char *from)

*/

//void strcpy(char * dst, const char * src)
void strcpyfar(unsigned int dst_ofst, unsigned int dst_sgmt, unsigned int src_ofst, unsigned int src_sgmt)
{
    char * fp_dst;
    char * fp_src;
    int itr_str;

    if ( dst_sgmt == 0 )
    {
        dst_sgmt = _DS;
    }

    if ( src_sgmt == 0 )
    {
        src_sgmt = _DS;
    }

    fp_dst = MK_FP(dst_sgmt, dst_ofst);
    fp_src = MK_FP(src_sgmt, src_ofst);

    itr_str = 0;
    while( fp_src[itr_str] != '\0' )
    {
        *fp_dst++ = *fp_src++;
        itr_str++;
    }
}
