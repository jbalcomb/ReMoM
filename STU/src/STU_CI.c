/*
    STU_CI -- Capture/Inject.  See STU_CI.h.

    Moved from MoM/src/INITGAME.c (gd_ci_load / gd_ci_get).  Changes in the move: the names, the developer-machine fallback path
    was dropped, and the whole thing compiles in only with STU_GD_CAPTURE.
*/

#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>   /* getenv, strtol */
#include <string.h>

#include "STU_LOG.h"
#include "STU_GD.h"   /* STU_GD_CAPTURE */

#include "STU_CI.h"

#if defined(STU_GD_CAPTURE)

#define STU_CI_MAX_VALS  512   /* longest record is the 240-int16 world overrun */
#define STU_CI_MAX_RECS   16

typedef struct
{
    char key[32];
    char site[24];
    int  count;
    long vals[STU_CI_MAX_VALS];
} stu_ci_rec_t;

static stu_ci_rec_t stu_ci_recs[STU_CI_MAX_RECS];
static int stu_ci_nrecs  = 0;
static int stu_ci_loaded = 0;   /* 0 = not tried, 1 = loaded, -1 = tried + failed */

#endif  /* STU_GD_CAPTURE */



int STU_CI_Load(void)
{
#if defined(STU_GD_CAPTURE)
    const char * cands[2];
    const char * env = NULL;
    const char * used = NULL;
    FILE * f = NULL;
    char line[8192];
    int nc = 0;
    int i = 0;

    if(stu_ci_loaded != 0)
    {
        return (stu_ci_loaded == 1) ? stu_ci_nrecs : -1;
    }

    env = getenv("OG_CI_FWV");
    if(env && *env) { cands[nc++] = env; }
    cands[nc++] = "og-game-data-capture.fwv";   /* cwd (build dir, if run there) */

    for(i = 0; i < nc; i++)
    {
        f = fopen(cands[i], "r");
        if(f)
        {
            used = cands[i];
            break;
        }
    }
    if(!f)
    {
        LOG_INFO(LOG_CAT_GENERAL, "[CI] load FAILED: og-game-data-capture.fwv not found (OG_CI_FWV=%s, or cwd). No injection.", (env && *env) ? env : "(unset)");
        stu_ci_loaded = -1;
        return -1;
    }

    stu_ci_nrecs = 0;
    while(fgets(line, sizeof(line), f))
    {
        char key[32], site[24];
        int count = 0, consumed = 0, n = 0;
        char * p = line;
        char * endp;
        stu_ci_rec_t * r;

        while(*p == ' ' || *p == '\t') { p++; }
        if(*p == '#' || *p == '\n' || *p == '\r' || *p == 0) { continue; }
        if(stu_ci_nrecs >= STU_CI_MAX_RECS)
        {
            LOG_INFO(LOG_CAT_GENERAL, "[CI] WARNING: >%d records in %s; rest ignored.", STU_CI_MAX_RECS, used);
            break;
        }
        /* sscanf widths kept in sync with r->key[32] / r->site[24] so the subsequent copies can't possibly truncate (GCC -Wformat-truncation). */
        if(sscanf(p, "%31s %23s %d%n", key, site, &count, &consumed) != 3)
        {
            LOG_INFO(LOG_CAT_GENERAL, "[CI] WARNING: malformed record skipped in %s.", used);
            continue;
        }
        r = &stu_ci_recs[stu_ci_nrecs];
        snprintf(r->key,  sizeof(r->key),  "%s", key);
        snprintf(r->site, sizeof(r->site), "%s", site);

        p += consumed;
        for(;;)
        {
            long v = strtol(p, &endp, 10);
            if(endp == p) { break; }       /* no more numbers on the line */
            if(n < STU_CI_MAX_VALS) { r->vals[n] = v; }
            n++;
            p = endp;
        }
        if(n != count)
        {
            LOG_INFO(LOG_CAT_GENERAL, "[CI] WARNING: %s %s declared %d values but found %d.", r->key, r->site, count, n);
        }
        r->count = (n < STU_CI_MAX_VALS) ? n : STU_CI_MAX_VALS;
        stu_ci_nrecs++;
    }
    fclose(f);

    LOG_INFO(LOG_CAT_GENERAL, "[CI] loaded %d record(s) from %s", stu_ci_nrecs, used);
    stu_ci_loaded = 1;
    return stu_ci_nrecs;
#else
    return -1;
#endif
}


int STU_CI_Get(const char * key, const char * site, long * out, int max)
{
#if defined(STU_GD_CAPTURE)
    int i = 0;
    int k = 0;
    int n = 0;

    if(stu_ci_loaded == 0) { STU_CI_Load(); }
    for(i = 0; i < stu_ci_nrecs; i++)
    {
        if(strcmp(stu_ci_recs[i].key, key) == 0 && strcmp(stu_ci_recs[i].site, site) == 0)
        {
            n = stu_ci_recs[i].count;
            if(n > max) { n = max; }
            for(k = 0; k < n; k++) { out[k] = stu_ci_recs[i].vals[k]; }
            return n;
        }
    }
    return -1;
#else
    (void)key; (void)site; (void)out; (void)max;
    return -1;
#endif
}
