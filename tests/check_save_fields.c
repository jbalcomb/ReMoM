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
#define MAX_DUMP_LINES  50000

static char  *dump_keys[MAX_DUMP_LINES];
static char  *dump_vals[MAX_DUMP_LINES];
static char   dump_storage[MAX_DUMP_LINES * MAX_LINE];
static int    dump_count = 0;

static char *Trim(char *s)
{
    char *end;
    while (*s && isspace((unsigned char)*s)) { s++; }
    if (*s == '\0') { return s; }
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) { *end-- = '\0'; }
    return s;
}

/* Parse "key = value" into key and value pointers.  Returns 0 on success. */
static int Parse_Key_Value(char *line, char **key_out, char **val_out)
{
    char *eq = strchr(line, '=');
    char *key;
    char *val;
    char *end;

    if (eq == NULL) { return 1; }

    *eq = '\0';
    key = line;
    val = eq + 1;

    /* Trim trailing whitespace from key. */
    end = eq - 1;
    while (end >= key && isspace((unsigned char)*end)) { *end-- = '\0'; }

    /* Trim leading whitespace from value. */
    while (*val && isspace((unsigned char)*val)) { val++; }

    /* Trim trailing whitespace from value. */
    end = val + strlen(val) - 1;
    while (end >= val && isspace((unsigned char)*end)) { *end-- = '\0'; }

    *key_out = key;
    *val_out = val;
    return 0;
}

/* Load a dump file into the dump_keys/dump_vals arrays. */
static int Load_Dump(const char *filepath)
{
    FILE *fp;
    char  raw[MAX_LINE];
    char *storage_ptr = dump_storage;

    fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "check_save_fields: cannot open dump file: %s\n", filepath);
        return 1;
    }

    while (fgets(raw, sizeof(raw), fp) != NULL && dump_count < MAX_DUMP_LINES)
    {
        char *line = Trim(raw);
        char *key;
        char *val;
        size_t key_len;
        size_t val_len;

        if (*line == '\0' || *line == '#') { continue; }
        if (Parse_Key_Value(line, &key, &val) != 0) { continue; }

        key_len = strlen(key) + 1;
        val_len = strlen(val) + 1;

        /* Bounds check on storage. */
        if ((size_t)((storage_ptr - dump_storage) + key_len + val_len) > sizeof(dump_storage))
        {
            fprintf(stderr, "check_save_fields: dump_storage exhausted\n");
            fclose(fp);
            return 1;
        }

        memcpy(storage_ptr, key, key_len);
        dump_keys[dump_count] = storage_ptr;
        storage_ptr += key_len;

        memcpy(storage_ptr, val, val_len);
        dump_vals[dump_count] = storage_ptr;
        storage_ptr += val_len;

        dump_count++;
    }

    fclose(fp);
    return 0;
}

/* Find the value for a given key in the loaded dump, or NULL if not found. */
static const char *Lookup_Dump_Field(const char *key)
{
    int i;
    for (i = 0; i < dump_count; i++)
    {
        if (strcmp(dump_keys[i], key) == 0)
        {
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

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <dump.txt> <assertions.txt>\n", argv[0]);
        return 2;
    }

    if (Load_Dump(argv[1]) != 0) { return 2; }

    fp = fopen(argv[2], "r");
    if (fp == NULL)
    {
        fprintf(stderr, "check_save_fields: cannot open assertions file: %s\n", argv[2]);
        return 2;
    }

    while (fgets(raw, sizeof(raw), fp) != NULL)
    {
        char       *line = Trim(raw);
        char       *key;
        char       *expected;
        const char *actual;

        line_num++;

        if (*line == '\0' || *line == '#') { continue; }
        if (Parse_Key_Value(line, &key, &expected) != 0)
        {
            fprintf(stderr, "%s:%d: bad assertion line: %s\n", argv[2], line_num, line);
            failed++;
            continue;
        }

        actual = Lookup_Dump_Field(key);
        if (actual == NULL)
        {
            fprintf(stderr, "FAIL %s:%d: field not found in dump: %s\n", argv[2], line_num, key);
            missing++;
            failed++;
            continue;
        }

        if (strcmp(actual, expected) != 0)
        {
            fprintf(stderr, "FAIL %s:%d: %s\n  actual:   %s\n  expected: %s\n", argv[2], line_num, key, actual, expected);
            failed++;
            continue;
        }

        passed++;
    }

    fclose(fp);

    if (failed > 0)
    {
        fprintf(stderr, "\n%d passed, %d failed (%d missing)\n", passed, failed, missing);
        return 1;
    }

    fprintf(stdout, "PASS: %d assertions matched\n", passed);
    return 0;
}
