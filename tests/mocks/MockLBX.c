#include <stdlib.h>
#include <string.h>

// Minimal stub implementing LBX loader functions used by tests.

int LBX_Load_Data_Static(const char *filename, void *dst, int size)
{
    // Very small stub: if filename exists in repo root, pretend success.
    (void)dst; (void)size;
    if(filename == NULL) return 0;
    // Return non-zero to indicate data loaded; tests only check for side effects.
    return 1;
}

// Provide symbol for names file used in Spells132 tests
char names_lbx_file__ovr132[] = "names.lbx";
