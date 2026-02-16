#ifndef MOCKLBX_H
#define MOCKLBX_H

#ifdef __cplusplus
extern "C" {
#endif

int LBX_Load_Data_Static(const char *filename, void *dst, int size);
extern char names_lbx_file__ovr132[];

#ifdef __cplusplus
}
#endif

#endif  /* MOCKLBX_H */
