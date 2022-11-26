// _s03p05c.c DISK_GetFileSize
// MOM_DEF.H

// J:\STU\DBWD\BORLANDC\INCLUDE\DOS.H
// The MSC find_t structure corresponds exactly to the ffblk structure */
// struct find_t {
//     char     reserved[21];      /* Microsoft reserved - do not change */
//     char     attrib;            /* attribute byte for matched file */
//     unsigned wr_time;           /* time of last write to file */
//     unsigned wr_date;           /* date of last write to file */
//     long     size;              /* size of file */
//     char     name[13];          /* asciiz name of matched file */
// };


#include <DOS.H>

#include "MOM_DEF.H"

// TODO(JimBalcomb): stop cheating on DISK_GetFileSize()

long DOS_GetFileSize(char *fname)
{
    struct find_t ffblk;
    long tmp_file_size;

    _dos_findfirst(fname, _A_NORMAL, &ffblk);
    // if ( _dos_findfirst(fname, _A_NORMAL, &ffblk) != 0 )
    // {
    //     printf("DEBUG: [%s, %d]: FILE NOT FOUND!! [%s]\n", __FILE__, __LINE__, fname);
    // }
    // tmp_file_size = (long)ffblk.size;
    // printf("DEBUG: [%s, %d]: tmp_file_size = %ld (0x%08X), ffblk.size = %ld (0x%08X)\n", __FILE__, __LINE__, tmp_file_size, tmp_file_size, ffblk.size, ffblk.size);

    tmp_file_size = ffblk.size;
//    printf("DEBUG: [%s, %d]: tmp_file_size = %ld (0x%08X)\n", __FILE__, __LINE__, tmp_file_size, tmp_file_size);
    // return ffblk.ff_fsize;
    // return ffblk.size;
    g_LbxFileSize = tmp_file_size;
//    printf("DEBUG: [%s, %d]: g_LbxFileSize: %ld (0x%08X)\n", __FILE__, __LINE__, g_LbxFileSize, g_LbxFileSize);

    return(tmp_file_size);
}
