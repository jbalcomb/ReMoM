/*
    cas_check -- syntax-checks Caster of Magic .CAS scripts and reports which built-ins they use that ReMoM does not
    implement yet.

    Usage:  cas_check [-v] FILE.CAS [FILE.CAS ...]      (-v: list each warning)
    Exit:   0 if every file parses, 1 otherwise.
*/

#include "../MOD/src/CAS.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAMES 256

struct s_USE
{
    char name[32];
    int uses;
    int files;
    int implemented;
};

static struct s_USE uses[MAX_NAMES];
static int use_count;

static void record(const char * name, int count, int implemented, void * user_data)
{
    int itr;
    for(itr = 0; itr < use_count; itr++)
    {
        if(strcmp(uses[itr].name, name) == 0) { uses[itr].uses += count; uses[itr].files++; return; }
    }
    if(use_count < MAX_NAMES)
    {
        strncpy(uses[use_count].name, name, sizeof(uses[use_count].name) - 1);
        uses[use_count].uses = count;
        uses[use_count].files = 1;
        uses[use_count].implemented = implemented;
        use_count++;
    }
}

static int by_uses(const void * a, const void * b)
{
    return ((const struct s_USE *)b)->uses - ((const struct s_USE *)a)->uses;
}

int main(int argc, char ** argv)
{
    int itr;
    int failed = 0;
    int implemented = 0;
    int total_warnings = 0;
    int verbose = 0;

    if(argc < 2)
    {
        fprintf(stderr, "Usage: %s FILE.CAS [FILE.CAS ...]\n", argv[0]);
        return 2;
    }
    for(itr = 1; itr < argc; itr++)
    {
        CAS_Script * script;
        if(strcmp(argv[itr], "-v") == 0) { verbose = 1; continue; }
        script = CAS_Load_File(argv[itr]);
        if(script == NULL || CAS_Check(script) != 0)
        {
            printf("FAIL  %s\n", CAS_Last_Error());
            failed++;
        }
        else
        {
            int warnings;
            const char * text = CAS_Warnings(&warnings);
            printf("ok    %s%s\n", argv[itr], warnings ? "" : "");
            if(warnings) { printf("      %d warning(s):\n", warnings); if(verbose) { printf("%s", text); } }
            total_warnings += warnings;
            CAS_Builtins_Used(script, record, NULL);
        }
        CAS_Free(script);
    }
    qsort(uses, (size_t)use_count, sizeof(uses[0]), by_uses);
    printf("\n%-26s %7s %6s  %s\n", "built-in", "uses", "files", "in ReMoM");
    for(itr = 0; itr < use_count; itr++)
    {
        printf("%-26s %7d %6d  %s\n", uses[itr].name, uses[itr].uses, uses[itr].files, uses[itr].implemented ? "yes" : "not yet");
        implemented += uses[itr].implemented;
    }
    printf("\n%d of %d files parse (%d warnings; -v lists them); %d of %d built-ins used are implemented\n",
        argc - 1 - verbose - failed, argc - 1 - verbose, total_warnings, implemented, use_count);
    return failed ? 1 : 0;
}
