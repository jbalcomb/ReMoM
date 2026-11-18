/*
    STU_GD -- Game-Data capture.  See STU_GD.h.

    The formatting here was lifted from the per-array dumpers that used to live in MoM/src/INITGAME.c, MAPGEN.c and AIMOVE.c.
    Their output is byte-for-byte what the OG-side probe and compare-gd expect, including the truncation guards, so those guards
    are kept exactly as they were.
*/

#include "../../ext/stu_compat.h"

#include <stdio.h>
#include <stdlib.h>   /* getenv */
#include <string.h>

#include "STU_LOG.h"

#include "STU_GD.h"

#define STU_GD_VALUE_BUF_SIZE   1100   /* record field value -- same size as the original dumpers' val[] */
#define STU_GD_ROW_BUF_SIZE     1100   /* csv row / chunk; the widest is a 100-value chunk of 5-digit values */
#define STU_GD_CHUNK_VALUES      100
#define STU_GD_MAX_POINTS        256   /* distinct STU_GD_ONCE points; there are ~60 */

static int stu_gd_enabled = -1;   /* -1 = not read yet, 0 = off, 1 = on */

static const char * stu_gd_seen_points[STU_GD_MAX_POINTS];
static int stu_gd_seen_point_count = 0;
static int stu_gd_seen_points_full_logged = 0;



int STU_GD_Is_Enabled(void)
{
#if defined(STU_GD_CAPTURE)
    const char * env = NULL;

    if(stu_gd_enabled < 0)
    {
        env = getenv("REMOM_GD_CAPTURE");
        stu_gd_enabled = (env != NULL && env[0] != '\0' && !(env[0] == '0' && env[1] == '\0')) ? 1 : 0;
        if(stu_gd_enabled == 1)
        {
            LOG_INFO(LOG_CAT_GENERAL, "[GD-CAPTURE] enabled (REMOM_GD_CAPTURE=%s)", env);   /* not "[GD] " -- that prefix is reserved for the compare-gd data lines */
        }
    }
    return stu_gd_enabled;
#else
    return 0;
#endif
}


int STU_GD_First_Time(const char * point)
{
    int itr = 0;

    if(point == NULL)
    {
        LOG_WARN(LOG_CAT_GENERAL, "[GD-CAPTURE] STU_GD_First_Time: NULL point; not firing");
        return 0;
    }

    for(itr = 0; itr < stu_gd_seen_point_count; itr++)
    {
        if(strcmp(stu_gd_seen_points[itr], point) == 0)
        {
            return 0;
        }
    }

    if(stu_gd_seen_point_count >= STU_GD_MAX_POINTS)
    {
        if(stu_gd_seen_points_full_logged == 0)
        {
            stu_gd_seen_points_full_logged = 1;
            LOG_WARN(LOG_CAT_GENERAL, "[GD-CAPTURE] STU_GD_First_Time: more than %d distinct points; '%s' and later new points will not fire", STU_GD_MAX_POINTS, point);
        }
        return 0;
    }

    /* point strings are call-site literals, so keeping the pointer is safe */
    stu_gd_seen_points[stu_gd_seen_point_count] = point;
    stu_gd_seen_point_count++;
    return 1;
}


static int STU_GD_Element_Size(int kind)
{
    return (kind == GD_U16 || kind == GD_I16) ? 2 : (kind == GD_U32 || kind == GD_I32) ? 4 : 1;
}


/* little-endian decode, matching the OG-side gd_decode_record (STU-Extras STU-DOSBox/dosbox-0.74-3/src/misc/og_sort_trace.h).
   Decoded through a 64-bit integer so a GD_U32 with bit 31 set prints unsigned on every platform.  The OG probe returns (long)
   and prints %ld, but it is a Linux build (64-bit long), so it prints U32 unsigned; a 32-bit long (MSVC) printed those values
   negative -- hero abilities (e.g. 0xD0000000) and uninitialised _ITEMS[].powers -- and mismatched compare-gd. */
static long long STU_GD_Read_Element(const uint8_t * r, int kind)
{
    switch(kind)
    {
        case GD_I8:  return (long long)(int8_t)r[0];
        case GD_U16: return (long long)(uint16_t)(r[0] | (r[1] << 8));
        case GD_I16: return (long long)(int16_t)(uint16_t)(r[0] | (r[1] << 8));
        case GD_U32: return (long long)((uint32_t)r[0] | ((uint32_t)r[1] << 8) | ((uint32_t)r[2] << 16) | ((uint32_t)r[3] << 24));
        case GD_I32: return (long long)(int32_t)((uint32_t)r[0] | ((uint32_t)r[1] << 8) | ((uint32_t)r[2] << 16) | ((uint32_t)r[3] << 24));
        default:     return (long long)r[0];   /* U8 */
    }
}


void STU_GD_Format_Field(char * val, int val_size, const uint8_t * field_ptr, const gd_field_t * field)
{
    int q = 0;
    int k = 0;
    int es = 0;
    long long v = 0;
    uint8_t c = 0;

    if(field->kind == GD_STR)
    {
        val[q++] = '"';
        for(k = 0; k < field->count && q < val_size - 2; k++)
        {
            c = field_ptr[k];
            if(c == 0)
            {
                break;
            }
            val[q++] = (c >= 32 && c < 127) ? (char)c : '.';
        }
        val[q++] = '"';
        val[q] = 0;
    }
    else if(field->kind == GD_BYTES)
    {
        for(k = 0; k < field->count && q < val_size - 3; k++)
        {
            q += snprintf(val + q, val_size - q, "%02X", field_ptr[k]);
        }
        val[q] = 0;
    }
    else
    {
        val[0] = 0;
        es = STU_GD_Element_Size(field->kind);
        for(k = 0; k < field->count; k++)
        {
            v = STU_GD_Read_Element(field_ptr + (k * es), field->kind);
            q += snprintf(val + q, val_size - q, k ? ",%lld" : "%lld", v);
            if(q > val_size - 16)
            {
                break;
            }
        }
    }
}


int STU_GD_Format_Csv(char * buf, int buf_size, const void * array, int kind, int first, int end)
{
    const uint8_t * bytes = (const uint8_t *)array;
    int es = STU_GD_Element_Size(kind);
    int q = 0;
    int itr = 0;
    int written = 0;
    long long v = 0;

    buf[0] = 0;
    for(itr = first; itr < end; itr++)
    {
        v = STU_GD_Read_Element(bytes + (itr * es), kind);
        written = snprintf(buf + q, buf_size - q, (itr > first) ? ",%lld" : "%lld", v);
        if(written < 0 || written >= buf_size - q)
        {
            buf[q] = 0;   /* snprintf wrote as much of the value as fitted -- drop the partial value so the row ends on a whole one */
            LOG_WARN(LOG_CAT_GENERAL, "[GD-CAPTURE] STU_GD_Format_Csv: row truncated at index %d of [%d,%d) (buf_size %d)", itr, first, end, buf_size);
            break;
        }
        q += written;
    }
    return q;
}


void STU_GD_Dump_Csv(const char * point, const char * name, const void * array, int kind, int count)
{
    char buf[STU_GD_ROW_BUF_SIZE];

    STU_GD_Format_Csv(buf, (int)sizeof(buf), array, kind, 0, count);
    STU_GD_WRITE("[GD] %s %s = %s", point, name, buf);
}


void STU_GD_Dump_Record(const char * point, const char * label, int idx, const void * record, const gd_field_t * fields, int field_count)
{
    const uint8_t * base = (const uint8_t *)record;
    char val[STU_GD_VALUE_BUF_SIZE];
    int itr = 0;

    for(itr = 0; itr < field_count; itr++)
    {
        STU_GD_Format_Field(val, (int)sizeof(val), base + fields[itr].off, &fields[itr]);
        STU_GD_WRITE("[GD] %s %s[%d].%s = %s", point, label, idx, fields[itr].name, val);
    }
    /* a dump is a large burst in a tight loop with no main-loop pump in between, which overruns the async logger's ring and drops messages -- drain per record */
    STU_Log_Flush_All();
}


void STU_GD_Dump_Records(const char * point, const char * label, const void * base, size_t stride, int count, const gd_field_t * fields, int field_count)
{
    const uint8_t * bytes = (const uint8_t *)base;
    int itr = 0;

    if(bytes == NULL)
    {
        LOG_WARN(LOG_CAT_GENERAL, "[GD-CAPTURE] %s %s: NULL base; nothing dumped", point, label);
        return;
    }

    for(itr = 0; itr < count; itr++)
    {
        STU_GD_Dump_Record(point, label, itr, bytes + ((size_t)itr * stride), fields, field_count);
    }
}


void STU_GD_Dump_Grid(const char * point, const char * label, STU_GD_Square_Fn square_fn, int width, int height, int planes)
{
    char row[STU_GD_ROW_BUF_SIZE];
    int plane = 0;
    int y = 0;
    int x = 0;
    int q = 0;
    int written = 0;

    for(plane = 0; plane < planes; plane++)
    {
        for(y = 0; y < height; y++)
        {
            q = 0;
            row[0] = 0;
            for(x = 0; x < width; x++)
            {
                written = snprintf(row + q, sizeof(row) - q, x ? ",%d" : "%d", square_fn(x, y, plane));
                if(written < 0 || written >= (int)sizeof(row) - q)
                {
                    LOG_WARN(LOG_CAT_GENERAL, "[GD-CAPTURE] %s %s[%d].y%02d: row truncated at x=%d", point, label, plane, y, x);
                    break;
                }
                q += written;
            }
            STU_GD_WRITE("[GD] %s %s[%d].y%02d = %s", point, label, plane, y, row);
        }
        STU_Log_Flush_All();   /* drain the async ring between planes */
    }
}


void STU_GD_Dump_Chunked(const char * point, const char * label, int idx, const char * member, const void * array, int kind, int count)
{
    char buf[STU_GD_ROW_BUF_SIZE];
    int base = 0;
    int end = 0;

    for(base = 0; base < count; base += STU_GD_CHUNK_VALUES)
    {
        end = (base + STU_GD_CHUNK_VALUES < count) ? (base + STU_GD_CHUNK_VALUES) : count;
        STU_GD_Format_Csv(buf, (int)sizeof(buf), array, kind, base, end);
        STU_GD_WRITE("[GD] %s %s[%d]%s@%d = %s", point, label, idx, member, base, buf);
    }
}
