/**
 * check_ai_metrics.c — Automated behavioral evaluation of AI metrics CSVs.
 *
 * Usage:
 *   check_ai_metrics <csv_dir> <assertions.txt>
 *
 * Reads CSV files produced by the AI_METRICS system and evaluates behavioral
 * assertions defined in the assertions file. Follows the check_save_fields.c
 * pattern from the HeMoM test infrastructure.
 *
 * Assertion types:
 *   LIVENESS  <csv> <filter> <column> MIN|MAX <value>
 *   BOUNDS    <csv> <filter> <column> RANGE <lo> <hi>
 *
 * Filter syntax:
 *   *             — all rows
 *   column=value  — rows where column equals value
 *
 * Exit codes:
 *   0 = all assertions passed
 *   1 = one or more assertions failed
 *   2 = usage error or file not found
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE        1024
#define MAX_COLUMNS     32
#define MAX_ROWS        50000
#define MAX_PATH_LEN    512

/* ------------------------------------------------------------------ */
/*  CSV data store                                                     */
/* ------------------------------------------------------------------ */

typedef struct
{
    char * headers[MAX_COLUMNS];
    int    col_count;
    char * cells[MAX_ROWS][MAX_COLUMNS];
    int    row_count;
    /* Flat storage for all strings */
    char   storage[MAX_ROWS * 256];
    int    storage_used;
} CSV_Table;

static CSV_Table g_table;

static char * Store_String(CSV_Table * t, const char * s)
{
    int len = (int)strlen(s) + 1;
    char * dst;
    if (t->storage_used + len > (int)sizeof(t->storage))
    {
        return NULL;
    }
    dst = &t->storage[t->storage_used];
    memcpy(dst, s, len);
    t->storage_used += len;
    return dst;
}

static char * Trim(char * s)
{
    char * end;
    while (*s && isspace((unsigned char)*s)) { s++; }
    if (*s == '\0') { return s; }
    end = s + strlen(s) - 1;
    while (end > s && isspace((unsigned char)*end)) { *end-- = '\0'; }
    return s;
}

/* Parse a single CSV line into tokens, handling no quoting (simple CSV). */
static int Parse_CSV_Line(char * line, char * tokens[], int max_tokens)
{
    int count = 0;
    char * p = line;
    while (*p && count < max_tokens)
    {
        tokens[count] = p;
        while (*p && *p != ',' && *p != '\n' && *p != '\r')
        {
            p++;
        }
        if (*p == ',')
        {
            *p = '\0';
            p++;
        }
        else
        {
            /* End of line — null-terminate */
            while (*p == '\n' || *p == '\r')
            {
                *p = '\0';
                p++;
            }
        }
        count++;
    }
    return count;
}

static int Load_CSV(const char * filepath, CSV_Table * t)
{
    FILE * fp;
    char raw[MAX_LINE];
    char * tokens[MAX_COLUMNS];
    int token_count;
    int col;

    memset(t, 0, sizeof(*t));

    fp = fopen(filepath, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "check_ai_metrics: cannot open CSV: %s\n", filepath);
        return 1;
    }

    /* Read header line */
    if (fgets(raw, sizeof(raw), fp) == NULL)
    {
        fclose(fp);
        return 1;
    }
    token_count = Parse_CSV_Line(raw, tokens, MAX_COLUMNS);
    t->col_count = token_count;
    for (col = 0; col < token_count; col++)
    {
        t->headers[col] = Store_String(t, Trim(tokens[col]));
    }

    /* Read data rows */
    while (fgets(raw, sizeof(raw), fp) != NULL && t->row_count < MAX_ROWS)
    {
        char * line = Trim(raw);
        if (*line == '\0') { continue; }

        token_count = Parse_CSV_Line(raw, tokens, MAX_COLUMNS);
        for (col = 0; col < token_count && col < t->col_count; col++)
        {
            t->cells[t->row_count][col] = Store_String(t, Trim(tokens[col]));
        }
        /* Fill missing columns with empty string */
        for (; col < t->col_count; col++)
        {
            t->cells[t->row_count][col] = Store_String(t, "");
        }
        t->row_count++;
    }

    fclose(fp);
    return 0;
}

static int Find_Column(CSV_Table * t, const char * name)
{
    int i;
    for (i = 0; i < t->col_count; i++)
    {
        if (strcmp(t->headers[i], name) == 0)
        {
            return i;
        }
    }
    return -1;
}


/* ------------------------------------------------------------------ */
/*  Assertion evaluation                                               */
/* ------------------------------------------------------------------ */

/* Parse "column=value" filter. Returns filter_col and filter_val.
   If filter is "*", filter_col = -1 (match all). */
static int Parse_Filter(CSV_Table * t, const char * filter, int * filter_col, const char ** filter_val)
{
    char * eq;
    static char filter_buf[256];

    if (strcmp(filter, "*") == 0)
    {
        *filter_col = -1;
        *filter_val = NULL;
        return 0;
    }

    strncpy(filter_buf, filter, sizeof(filter_buf) - 1);
    filter_buf[sizeof(filter_buf) - 1] = '\0';

    eq = strchr(filter_buf, '=');
    if (eq == NULL)
    {
        return 1;
    }
    *eq = '\0';
    *filter_col = Find_Column(t, Trim(filter_buf));
    *filter_val = Trim(eq + 1);

    if (*filter_col < 0)
    {
        return 1;
    }
    return 0;
}

static int Row_Matches(CSV_Table * t, int row, int filter_col, const char * filter_val)
{
    if (filter_col < 0)
    {
        return 1; /* match all */
    }
    return (strcmp(t->cells[row][filter_col], filter_val) == 0);
}

/* LIVENESS assertion: checks that a column value across filtered rows meets MIN or MAX. */
static int Eval_Liveness(
    CSV_Table * t,
    int filter_col, const char * filter_val,
    const char * column_name, const char * op_str, int threshold,
    const char * assertion_line, int line_num, const char * assert_file
)
{
    int col_idx;
    int row;
    int match_count = 0;
    int pass = 0;

    /* Special case: COUNT MIN/MAX — count matching rows */
    if (strcmp(column_name, "COUNT") == 0)
    {
        for (row = 0; row < t->row_count; row++)
        {
            if (Row_Matches(t, row, filter_col, filter_val))
            {
                match_count++;
            }
        }

        if (strcmp(op_str, "MIN") == 0)
        {
            pass = (match_count >= threshold);
        }
        else
        {
            pass = (match_count <= threshold);
        }

        if (!pass)
        {
            fprintf(stderr, "FAIL %s:%d: LIVENESS COUNT %s %d — got %d matching rows\n",
                assert_file, line_num, op_str, threshold, match_count);
        }
        return pass ? 0 : 1;
    }

    col_idx = Find_Column(t, column_name);
    if (col_idx < 0)
    {
        fprintf(stderr, "FAIL %s:%d: column '%s' not found in CSV\n", assert_file, line_num, column_name);
        return 1;
    }

    /* Check that at least one matching row meets the threshold */
    for (row = 0; row < t->row_count; row++)
    {
        int val;
        if (!Row_Matches(t, row, filter_col, filter_val))
        {
            continue;
        }
        match_count++;
        val = atoi(t->cells[row][col_idx]);

        if (strcmp(op_str, "MIN") == 0)
        {
            if (val >= threshold)
            {
                pass = 1;
            }
        }
        else /* MAX */
        {
            if (val <= threshold)
            {
                pass = 1;
            }
        }
    }

    if (match_count == 0)
    {
        fprintf(stderr, "FAIL %s:%d: LIVENESS — no rows matched filter\n", assert_file, line_num);
        return 1;
    }

    if (!pass)
    {
        fprintf(stderr, "FAIL %s:%d: LIVENESS %s %s %d — no matching row satisfied the condition (%d rows checked)\n",
            assert_file, line_num, column_name, op_str, threshold, match_count);
    }
    return pass ? 0 : 1;
}

/* BOUNDS assertion: checks that every filtered row's column value is within [lo, hi]. */
static int Eval_Bounds(
    CSV_Table * t,
    int filter_col, const char * filter_val,
    const char * column_name, int lo, int hi,
    const char * assertion_line, int line_num, const char * assert_file
)
{
    int col_idx;
    int row;
    int violations = 0;
    int first_bad_turn = -1;
    int first_bad_val = 0;

    col_idx = Find_Column(t, column_name);
    if (col_idx < 0)
    {
        fprintf(stderr, "FAIL %s:%d: column '%s' not found in CSV\n", assert_file, line_num, column_name);
        return 1;
    }

    for (row = 0; row < t->row_count; row++)
    {
        int val;
        if (!Row_Matches(t, row, filter_col, filter_val))
        {
            continue;
        }
        val = atoi(t->cells[row][col_idx]);
        if (val < lo || val > hi)
        {
            if (violations == 0)
            {
                int turn_col = Find_Column(t, "turn");
                first_bad_val = val;
                first_bad_turn = (turn_col >= 0) ? atoi(t->cells[row][turn_col]) : row;
            }
            violations++;
        }
    }

    if (violations > 0)
    {
        fprintf(stderr, "FAIL %s:%d: BOUNDS %s RANGE %d..%d — %d violations (first at turn %d: value=%d)\n",
            assert_file, line_num, column_name, lo, hi, violations, first_bad_turn, first_bad_val);
        return 1;
    }
    return 0;
}


/* ------------------------------------------------------------------ */
/*  Main — parse assertion file and evaluate                           */
/* ------------------------------------------------------------------ */

int main(int argc, char * argv[])
{
    FILE * fp;
    char raw[MAX_LINE];
    int line_num = 0;
    int passed = 0;
    int failed = 0;
    char csv_dir[MAX_PATH_LEN];
    const char * assert_file;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <csv_dir> <assertions.txt>\n", argv[0]);
        return 2;
    }

    strncpy(csv_dir, argv[1], sizeof(csv_dir) - 1);
    csv_dir[sizeof(csv_dir) - 1] = '\0';
    assert_file = argv[2];

    fp = fopen(assert_file, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "check_ai_metrics: cannot open assertions file: %s\n", assert_file);
        return 2;
    }

    while (fgets(raw, sizeof(raw), fp) != NULL)
    {
        char * line;
        char * tokens[16];
        int token_count = 0;
        char * p;
        char csv_path[MAX_PATH_LEN];
        int result = 0;

        line_num++;
        line = Trim(raw);
        if (*line == '\0' || *line == '#') { continue; }

        /* Strip inline comments */
        p = strchr(line, '#');
        if (p != NULL)
        {
            *p = '\0';
            line = Trim(line);
            if (*line == '\0') { continue; }
        }

        /* Tokenize by whitespace */
        p = line;
        while (*p && token_count < 16)
        {
            while (*p && isspace((unsigned char)*p)) { p++; }
            if (*p == '\0') { break; }
            tokens[token_count++] = p;
            while (*p && !isspace((unsigned char)*p)) { p++; }
            if (*p)
            {
                *p = '\0';
                p++;
            }
        }

        if (token_count < 2) { continue; }

        /* Build CSV path */
        snprintf(csv_path, sizeof(csv_path), "%s/%s", csv_dir, tokens[1]);

        /* Load CSV (reuses g_table each assertion) */
        if (Load_CSV(csv_path, &g_table) != 0)
        {
            fprintf(stderr, "FAIL %s:%d: could not load %s\n", assert_file, line_num, csv_path);
            failed++;
            continue;
        }

        if (strcmp(tokens[0], "LIVENESS") == 0 && token_count >= 6)
        {
            /* LIVENESS <csv> <filter> <column> MIN|MAX <value> */
            int filter_col;
            const char * filter_val;
            if (Parse_Filter(&g_table, tokens[2], &filter_col, &filter_val) != 0)
            {
                fprintf(stderr, "FAIL %s:%d: bad filter '%s'\n", assert_file, line_num, tokens[2]);
                failed++;
                continue;
            }
            result = Eval_Liveness(&g_table, filter_col, filter_val,
                tokens[3], tokens[4], atoi(tokens[5]),
                line, line_num, assert_file);
        }
        else if (strcmp(tokens[0], "BOUNDS") == 0 && token_count >= 6)
        {
            /* BOUNDS <csv> <filter> <column> RANGE <lo> <hi> */
            int filter_col;
            const char * filter_val;
            if (Parse_Filter(&g_table, tokens[2], &filter_col, &filter_val) != 0)
            {
                fprintf(stderr, "FAIL %s:%d: bad filter '%s'\n", assert_file, line_num, tokens[2]);
                failed++;
                continue;
            }
            if (strcmp(tokens[4], "RANGE") != 0 || token_count < 7)
            {
                fprintf(stderr, "FAIL %s:%d: BOUNDS expects 'RANGE <lo> <hi>'\n", assert_file, line_num);
                failed++;
                continue;
            }
            result = Eval_Bounds(&g_table, filter_col, filter_val,
                tokens[3], atoi(tokens[5]), atoi(tokens[6]),
                line, line_num, assert_file);
        }
        else
        {
            fprintf(stderr, "SKIP %s:%d: unknown assertion type '%s'\n", assert_file, line_num, tokens[0]);
            continue;
        }

        if (result == 0)
        {
            passed++;
        }
        else
        {
            failed++;
        }
    }

    fclose(fp);

    if (failed > 0)
    {
        fprintf(stderr, "\n%d passed, %d failed\n", passed, failed);
        return 1;
    }

    fprintf(stdout, "PASS: %d assertions matched\n", passed);
    return 0;
}
