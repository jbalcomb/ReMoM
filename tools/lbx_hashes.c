/*
    lbx_hashes -- generate known-good SHA-256 manifest rows for ReMoM's runtime
    data-compatibility check.

    Point it at a Master of Magic install directory and give a version tag; it
    prints one C row per *.LBX file (sorted, so diffs are stable) to stdout,
    ready to paste into STU/src/lbx_manifest.c between the GENERATED markers.

        Usage:   lbx_hashes <mom-data-dir> <version-tag>
        Example: lbx_hashes "/games/MoM v1.31 floppy" v1.31-floppy-1995

    Run once per distribution you own; concatenate the outputs.  A tag containing
    "v1.31" is treated as supported by the runtime; anything else triggers the
    "wrong version" warning.  (Progress/notes go to stderr; only rows go to
    stdout, so `lbx_hashes ... > rows.inc` yields a clean paste.)
*/

#include "../STU/src/STU_HASH.h"

#include "../../../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <dirent.h>
#endif

#define MAX_FILES  512
#define NAME_LEN   128
#define PATH_LEN   1024

/* Case-insensitive check that `name` ends in ".LBX". */
static int has_lbx_ext(const char * name)
{
    size_t n = strlen(name);
    const char * ext;
    if(n < 4)
    {
        return 0;
    }
    ext = name + (n - 4);
    return ext[0] == '.' &&
           (ext[1] == 'l' || ext[1] == 'L') &&
           (ext[2] == 'b' || ext[2] == 'B') &&
           (ext[3] == 'x' || ext[3] == 'X');
}

static int cmp_names(const void * a, const void * b)
{
    return strcmp((const char *)a, (const char *)b);
}

int main(int argc, char ** argv)
{
    static char names[MAX_FILES][NAME_LEN];
    int count = 0;
    int i;
    const char * dir;
    const char * tag;

    if(argc != 3)
    {
        fprintf(stderr, "usage: %s <mom-data-dir> <version-tag>\n", argv[0]);
        fprintf(stderr, "  e.g. %s \"/games/MoM v1.31\" v1.31-floppy-1995\n", argv[0]);
        return 2;
    }
    dir = argv[1];
    tag = argv[2];

    /* ---- collect *.LBX filenames in the directory ---- */
#if defined(_WIN32)
    {
        char pattern[PATH_LEN];
        WIN32_FIND_DATAA fd;
        HANDLE h;
        snprintf(pattern, sizeof(pattern), "%s\\*", dir);
        h = FindFirstFileA(pattern, &fd);
        if(h == INVALID_HANDLE_VALUE)
        {
            fprintf(stderr, "lbx_hashes: cannot open directory '%s'\n", dir);
            return 1;
        }
        do
        {
            if(has_lbx_ext(fd.cFileName) && count < MAX_FILES &&
               strlen(fd.cFileName) < NAME_LEN)
            {
                stu_strcpy(names[count++], fd.cFileName);
            }
        } while(FindNextFileA(h, &fd));
        FindClose(h);
    }
#else
    {
        DIR * d = opendir(dir);
        struct dirent * ent;
        if(d == NULL)
        {
            fprintf(stderr, "lbx_hashes: cannot open directory '%s'\n", dir);
            return 1;
        }
        while((ent = readdir(d)) != NULL)
        {
            if(has_lbx_ext(ent->d_name) && count < MAX_FILES &&
               strlen(ent->d_name) < NAME_LEN)
            {
                stu_strcpy(names[count++], ent->d_name);
            }
        }
        closedir(d);
    }
#endif

    if(count == 0)
    {
        fprintf(stderr, "lbx_hashes: no .LBX files found in '%s'\n", dir);
        return 1;
    }

    qsort(names, (size_t)count, NAME_LEN, cmp_names);

    /* ---- hash each and emit a manifest row ---- */
    fprintf(stderr, "lbx_hashes: %d .LBX file(s) in '%s', tag '%s'\n", count, dir, tag);
    printf("    /* %s (%d files) */\n", tag, count);
    for(i = 0; i < count; i++)
    {
        char path[PATH_LEN];
        char hex[65];
        FILE * fp;

        snprintf(path, sizeof(path), "%s/%s", dir, names[i]);
        fp = fopen(path, "rb");
        if(fp == NULL)
        {
            fprintf(stderr, "lbx_hashes: WARNING cannot open '%s' -- skipped\n", path);
            continue;
        }
        if(!STU_SHA256_Stream(fp, hex))
        {
            fprintf(stderr, "lbx_hashes: WARNING read error on '%s' -- skipped\n", path);
            fclose(fp);
            continue;
        }
        fclose(fp);
        printf("    { \"%s\", \"%s\", \"%s\" },\n", names[i], hex, tag);
    }

    return 0;
}
