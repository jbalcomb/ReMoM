/**
 * check_save_fields.c — Field-level assertion checker for HeMoM save dumps.
 *
 * Usage:
 *   check_save_fields <dump.txt> <assertions.txt>
 *
 * Reads a HeMoM save dump (key = value lines) and an assertions file (same
 * key = value format, plus comments starting with #).  For each assertion,
 * looks up the matching line in the dump and compares values.
 *
 * Exit codes:
 *   0 = all assertions passed
 *   1 = one or more assertions failed
 *   2 = usage error or file not found
 *
 * Reports every failure with field name, actual value, expected value.
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE        512

/* Grown as the dump is read.  A fixed 50000-line table silently dropped everything past line 50000
   of a full save dump (66086 lines), so those fields always read as "not found". */
static char **dump_keys = NULL;
static char **dump_vals = NULL;
static int    dump_count = 0;
static int    dump_capacity = 0;

static char *Copy_String(const char *s)
{
    size_t len = strlen(s) + 1;
    char  *copy = (char *)malloc(len);
    if(copy != NULL) { memcpy(copy, s, len); }
    return copy;
}

static char *Trim(char *s)
{
    char *end;
    while(*s && isspace((unsigned char)*s)) { s++; }
    if(*s == '\0') { return s; }
    end = s + strlen(s) - 1;
    while(end > s && isspace((unsigned char)*end)) { *end-- = '\0'; }
    return s;
}

/* Parse "key = value" into key and value pointers.  Returns 0 on success. */
static int Parse_Key_Value(char *line, char **key_out, char **val_out)
{
    char *eq = strchr(line, '=');
    char *key;
    char *val;
    char *end;

    if(eq == NULL) { return 1; }

    *eq = '\0';
    key = line;
    val = eq + 1;

    /* Trim trailing whitespace from key. */
    end = eq - 1;
    while(end >= key && isspace((unsigned char)*end)) { *end-- = '\0'; }

    /* Trim leading whitespace from value. */
    while(*val && isspace((unsigned char)*val)) { val++; }

    /* Trim trailing whitespace from value. */
    end = val + strlen(val) - 1;
    while(end >= val && isspace((unsigned char)*end)) { *end-- = '\0'; }

    *key_out = key;
    *val_out = val;
    return 0;
}

/* Load a dump file into the dump_keys/dump_vals arrays. */
static int Load_Dump(const char *filepath)
{
    FILE *fp;
    char  raw[MAX_LINE];

    fp = fopen(filepath, "r");
    if(fp == NULL)
    {
        fprintf(stderr, "check_save_fields: cannot open dump file: %s\n", filepath);
        return 1;
    }

    while(fgets(raw, sizeof(raw), fp) != NULL)
    {
        char *line = Trim(raw);
        char *key;
        char *val;

        if(*line == '\0' || *line == '#') { continue; }
        if(Parse_Key_Value(line, &key, &val) != 0) { continue; }

        if(dump_count == dump_capacity)
        {
            int    new_capacity = (dump_capacity == 0) ? 65536 : (dump_capacity * 2);
            char **new_keys = (char **)realloc(dump_keys, (size_t)new_capacity * sizeof(char *));
            char **new_vals;
            if(new_keys == NULL)
            {
                fprintf(stderr, "check_save_fields: out of memory growing the dump table to %d lines (%s)\n", new_capacity, filepath);
                fclose(fp);
                return 1;
            }
            dump_keys = new_keys;
            new_vals = (char **)realloc(dump_vals, (size_t)new_capacity * sizeof(char *));
            if(new_vals == NULL)
            {
                fprintf(stderr, "check_save_fields: out of memory growing the dump table to %d lines (%s)\n", new_capacity, filepath);
                fclose(fp);
                return 1;
            }
            dump_vals = new_vals;
            dump_capacity = new_capacity;
        }

        dump_keys[dump_count] = Copy_String(key);
        dump_vals[dump_count] = Copy_String(val);
        if((dump_keys[dump_count] == NULL) || (dump_vals[dump_count] == NULL))
        {
            fprintf(stderr, "check_save_fields: out of memory at dump line %d (%s)\n", dump_count + 1, filepath);
            fclose(fp);
            return 1;
        }

        dump_count++;
    }

    fclose(fp);
    return 0;
}

/* Find the value for a given key in the loaded dump, or NULL if not found. */
static const char *Lookup_Dump_Field(const char *key)
{
    /* Assertions usually follow the dump's order (a whole dump used as the assertions file does
       exactly), so search from just after the previous match and wrap around. */
    static int next_start = 0;
    int n;
    int i;
    for (n = 0; n < dump_count; n++)
    {
        i = (next_start + n) % dump_count;
        if(strcmp(dump_keys[i], key) == 0)
        {
            next_start = i + 1;
            return dump_vals[i];
        }
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    FILE *fp;
    char  raw[MAX_LINE];
    int   line_num = 0;
    int   passed = 0;
    int   failed = 0;
    int   missing = 0;

    if(argc < 3)
    {
        fprintf(stderr, "Usage: %s <dump.txt> <assertions.txt>\n", argv[0]);
        return 2;
    }

    if(Load_Dump(argv[1]) != 0) { return 2; }

    fp = fopen(argv[2], "r");
    if(fp == NULL)
    {
        fprintf(stderr, "check_save_fields: cannot open assertions file: %s\n", argv[2]);
        return 2;
    }

    while(fgets(raw, sizeof(raw), fp) != NULL)
    {
        char       *line = Trim(raw);
        char       *key;
        char       *expected;
        const char *actual;

        line_num++;

        if(*line == '\0' || *line == '#') { continue; }
        if(Parse_Key_Value(line, &key, &expected) != 0)
        {
            fprintf(stderr, "%s:%d: bad assertion line: %s\n", argv[2], line_num, line);
            failed++;
            continue;
        }

        actual = Lookup_Dump_Field(key);
        if(actual == NULL)
        {
            fprintf(stderr, "FAIL %s:%d: field not found in dump: %s\n", argv[2], line_num, key);
            missing++;
            failed++;
            continue;
        }

        if(strcmp(actual, expected) != 0)
        {
            fprintf(stderr, "FAIL %s:%d: %s\n  actual:   %s\n  expected: %s\n", argv[2], line_num, key, actual, expected);
            failed++;
            continue;
        }

        passed++;
    }

    fclose(fp);

    if(failed > 0)
    {
        fprintf(stderr, "\n%d passed, %d failed (%d missing)\n", passed, failed, missing);
        return 1;
    }

    fprintf(stdout, "PASS: %d assertions matched\n", passed);
    return 0;
}
