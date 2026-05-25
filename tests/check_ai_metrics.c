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
 *   TREND     <csv> <filter> <column> INCREASES|DECREASES|NONZERO
 *   RATE      <csv> <filter> PER <turns> MIN|MAX <count>
 *   BASELINE  <csv> <filter> <column> <baseline_path> TOLERANCE <pct>
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
static CSV_Table g_baseline;

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

/* TREND assertion: checks how a column value changes across the filtered rows.
   INCREASES — value at end is greater than at start
   DECREASES — value at end is less than at start
   NONZERO   — at least one filtered row has a non-zero value (catches "accumulator never moved") */
static int Eval_Trend(
    CSV_Table * t,
    int filter_col, const char * filter_val,
    const char * column_name, const char * direction,
    const char * assertion_line, int line_num, const char * assert_file
)
{
    int col_idx;
    int row;
    int match_count = 0;
    int first_val = 0;
    int last_val = 0;
    int max_val = 0;
    int min_val = 0;
    int nonzero_seen = 0;

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
        if (match_count == 0)
        {
            first_val = val;
            min_val = val;
            max_val = val;
        }
        last_val = val;
        if (val < min_val) { min_val = val; }
        if (val > max_val) { max_val = val; }
        if (val != 0) { nonzero_seen = 1; }
        match_count++;
    }

    if (match_count == 0)
    {
        fprintf(stderr, "FAIL %s:%d: TREND — no rows matched filter\n", assert_file, line_num);
        return 1;
    }

    if (strcmp(direction, "INCREASES") == 0)
    {
        /* Pass if the max seen exceeded the first value (allows non-monotonic growth
           like accumulators that reset on threshold). */
        if (max_val > first_val) { return 0; }
        fprintf(stderr, "FAIL %s:%d: TREND %s INCREASES — first=%d max=%d last=%d (%d rows)\n",
            assert_file, line_num, column_name, first_val, max_val, last_val, match_count);
        return 1;
    }
    else if (strcmp(direction, "DECREASES") == 0)
    {
        if (min_val < first_val) { return 0; }
        fprintf(stderr, "FAIL %s:%d: TREND %s DECREASES — first=%d min=%d last=%d (%d rows)\n",
            assert_file, line_num, column_name, first_val, min_val, last_val, match_count);
        return 1;
    }
    else if (strcmp(direction, "NONZERO") == 0)
    {
        if (nonzero_seen) { return 0; }
        fprintf(stderr, "FAIL %s:%d: TREND %s NONZERO — all %d matching rows were 0\n",
            assert_file, line_num, column_name, match_count);
        return 1;
    }
    else
    {
        fprintf(stderr, "FAIL %s:%d: TREND direction must be INCREASES|DECREASES|NONZERO (got '%s')\n",
            assert_file, line_num, direction);
        return 1;
    }
}

/* RATE assertion: counts filtered rows per N-turn window, checks MIN or MAX.
   Pass = at least one window of `turns` size satisfies the count threshold. */
static int Eval_Rate(
    CSV_Table * t,
    int filter_col, const char * filter_val,
    int turns_per_window, const char * op_str, int threshold,
    const char * assertion_line, int line_num, const char * assert_file
)
{
    int turn_col;
    int row;
    int max_turn = 0;
    int min_turn = 0x7FFFFFFF;
    int total_count = 0;
    int windows;
    int total_turns;
    double rate_per_window;

    turn_col = Find_Column(t, "turn");
    if (turn_col < 0)
    {
        fprintf(stderr, "FAIL %s:%d: RATE requires a 'turn' column\n", assert_file, line_num);
        return 1;
    }

    for (row = 0; row < t->row_count; row++)
    {
        int turn;
        if (!Row_Matches(t, row, filter_col, filter_val))
        {
            continue;
        }
        turn = atoi(t->cells[row][turn_col]);
        if (turn > max_turn) { max_turn = turn; }
        if (turn < min_turn) { min_turn = turn; }
        total_count++;
    }

    /* Total turn span — if no rows matched, fall back to span from first to last turn in table */
    if (total_count == 0)
    {
        min_turn = 0;
        max_turn = 0;
        for (row = 0; row < t->row_count; row++)
        {
            int turn = atoi(t->cells[row][turn_col]);
            if (turn > max_turn) { max_turn = turn; }
        }
        total_turns = max_turn + 1;
    }
    else
    {
        total_turns = (max_turn - min_turn) + 1;
    }

    if (total_turns < 1) { total_turns = 1; }

    windows = total_turns / turns_per_window;
    if (windows < 1) { windows = 1; }

    rate_per_window = (double)total_count / (double)windows;

    if (strcmp(op_str, "MIN") == 0)
    {
        if (rate_per_window >= (double)threshold) { return 0; }
        fprintf(stderr, "FAIL %s:%d: RATE PER %d MIN %d — got %.2f/window (%d events over %d turns)\n",
            assert_file, line_num, turns_per_window, threshold, rate_per_window, total_count, total_turns);
        return 1;
    }
    else if (strcmp(op_str, "MAX") == 0)
    {
        if (rate_per_window <= (double)threshold) { return 0; }
        fprintf(stderr, "FAIL %s:%d: RATE PER %d MAX %d — got %.2f/window (%d events over %d turns)\n",
            assert_file, line_num, turns_per_window, threshold, rate_per_window, total_count, total_turns);
        return 1;
    }
    else
    {
        fprintf(stderr, "FAIL %s:%d: RATE op must be MIN|MAX (got '%s')\n", assert_file, line_num, op_str);
        return 1;
    }
}

/* Sum a column across filtered rows. Returns 0 on success, -1 if column missing. */
static int Sum_Column(
    CSV_Table * t, int filter_col, const char * filter_val,
    const char * column_name, long * sum_out, int * count_out
)
{
    int col_idx;
    int row;
    long sum = 0;
    int count = 0;

    col_idx = Find_Column(t, column_name);
    if (col_idx < 0)
    {
        return -1;
    }

    for (row = 0; row < t->row_count; row++)
    {
        if (!Row_Matches(t, row, filter_col, filter_val))
        {
            continue;
        }
        sum += atol(t->cells[row][col_idx]);
        count++;
    }

    *sum_out = sum;
    *count_out = count;
    return 0;
}

/* BASELINE assertion: compares the sum of a filtered column against the same
   sum computed from a baseline CSV. Pass if |actual - baseline| / baseline <= tolerance%.
   When baseline sum is 0, requires actual sum to also be 0.
   Baseline path resolution: absolute paths used as-is; otherwise tries
   relative to the assertions file directory first, then to csv_dir. */
static int Eval_Baseline(
    CSV_Table * t,
    const char * csv_dir,
    const char * assert_file,
    int filter_col, const char * filter_val,
    const char * column_name,
    const char * baseline_path,
    int tolerance_pct,
    const char * assertion_line, int line_num
)
{
    char full_baseline_path[MAX_PATH_LEN];
    char assert_dir[MAX_PATH_LEN];
    char * last_slash;
    long actual_sum = 0;
    long baseline_sum = 0;
    int actual_count = 0;
    int baseline_count = 0;
    int baseline_filter_col = -1;
    const char * baseline_filter_val = NULL;
    double drift_pct;
    long abs_drift;
    FILE * test_fp;

    /* Build full baseline path: absolute → use as-is; otherwise try
       relative to the assertions file directory first, then csv_dir. */
    if (baseline_path[0] == '/')
    {
        strncpy(full_baseline_path, baseline_path, sizeof(full_baseline_path) - 1);
        full_baseline_path[sizeof(full_baseline_path) - 1] = '\0';
    }
    else
    {
        strncpy(assert_dir, assert_file, sizeof(assert_dir) - 1);
        assert_dir[sizeof(assert_dir) - 1] = '\0';
        last_slash = strrchr(assert_dir, '/');
        if (last_slash != NULL)
        {
            *last_slash = '\0';
            snprintf(full_baseline_path, sizeof(full_baseline_path), "%s/%s", assert_dir, baseline_path);
        }
        else
        {
            snprintf(full_baseline_path, sizeof(full_baseline_path), "%s", baseline_path);
        }
        /* If not found alongside assertion file, try csv_dir */
        test_fp = fopen(full_baseline_path, "r");
        if (test_fp == NULL)
        {
            snprintf(full_baseline_path, sizeof(full_baseline_path), "%s/%s", csv_dir, baseline_path);
        }
        else
        {
            fclose(test_fp);
        }
    }

    if (Load_CSV(full_baseline_path, &g_baseline) != 0)
    {
        fprintf(stderr, "FAIL %s:%d: BASELINE could not load baseline '%s'\n",
            assert_file, line_num, full_baseline_path);
        return 1;
    }

    /* Re-parse filter against baseline table (column indices may differ) */
    if (filter_col >= 0)
    {
        const char * filter_col_name = t->headers[filter_col];
        baseline_filter_col = Find_Column(&g_baseline, filter_col_name);
        if (baseline_filter_col < 0)
        {
            fprintf(stderr, "FAIL %s:%d: BASELINE filter column '%s' missing in baseline\n",
                assert_file, line_num, filter_col_name);
            return 1;
        }
        baseline_filter_val = filter_val;
    }

    if (Sum_Column(t, filter_col, filter_val, column_name, &actual_sum, &actual_count) != 0)
    {
        fprintf(stderr, "FAIL %s:%d: BASELINE column '%s' not in actual CSV\n",
            assert_file, line_num, column_name);
        return 1;
    }
    if (Sum_Column(&g_baseline, baseline_filter_col, baseline_filter_val,
        column_name, &baseline_sum, &baseline_count) != 0)
    {
        fprintf(stderr, "FAIL %s:%d: BASELINE column '%s' not in baseline CSV\n",
            assert_file, line_num, column_name);
        return 1;
    }

    abs_drift = (actual_sum > baseline_sum) ? (actual_sum - baseline_sum) : (baseline_sum - actual_sum);

    if (baseline_sum == 0)
    {
        if (actual_sum == 0) { return 0; }
        fprintf(stderr, "FAIL %s:%d: BASELINE %s — baseline sum was 0 but actual sum is %ld\n",
            assert_file, line_num, column_name, actual_sum);
        return 1;
    }

    drift_pct = (100.0 * (double)abs_drift) / (double)baseline_sum;

    if (drift_pct <= (double)tolerance_pct) { return 0; }

    fprintf(stderr, "FAIL %s:%d: BASELINE %s drift %.1f%% > tolerance %d%% (actual_sum=%ld over %d rows, baseline_sum=%ld over %d rows)\n",
        assert_file, line_num, column_name, drift_pct, tolerance_pct,
        actual_sum, actual_count, baseline_sum, baseline_count);
    return 1;
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
        else if (strcmp(tokens[0], "TREND") == 0 && token_count >= 5)
        {
            /* TREND <csv> <filter> <column> INCREASES|DECREASES|NONZERO */
            int filter_col;
            const char * filter_val;
            if (Parse_Filter(&g_table, tokens[2], &filter_col, &filter_val) != 0)
            {
                fprintf(stderr, "FAIL %s:%d: bad filter '%s'\n", assert_file, line_num, tokens[2]);
                failed++;
                continue;
            }
            result = Eval_Trend(&g_table, filter_col, filter_val,
                tokens[3], tokens[4],
                line, line_num, assert_file);
        }
        else if (strcmp(tokens[0], "RATE") == 0 && token_count >= 6)
        {
            /* RATE <csv> <filter> PER <turns> MIN|MAX <count> */
            int filter_col;
            const char * filter_val;
            if (Parse_Filter(&g_table, tokens[2], &filter_col, &filter_val) != 0)
            {
                fprintf(stderr, "FAIL %s:%d: bad filter '%s'\n", assert_file, line_num, tokens[2]);
                failed++;
                continue;
            }
            if (strcmp(tokens[3], "PER") != 0 || token_count < 7)
            {
                fprintf(stderr, "FAIL %s:%d: RATE expects 'PER <turns> MIN|MAX <count>'\n", assert_file, line_num);
                failed++;
                continue;
            }
            result = Eval_Rate(&g_table, filter_col, filter_val,
                atoi(tokens[4]), tokens[5], atoi(tokens[6]),
                line, line_num, assert_file);
        }
        else if (strcmp(tokens[0], "BASELINE") == 0 && token_count >= 7)
        {
            /* BASELINE <csv> <filter> <column> <baseline_path> TOLERANCE <pct> */
            int filter_col;
            const char * filter_val;
            if (Parse_Filter(&g_table, tokens[2], &filter_col, &filter_val) != 0)
            {
                fprintf(stderr, "FAIL %s:%d: bad filter '%s'\n", assert_file, line_num, tokens[2]);
                failed++;
                continue;
            }
            if (strcmp(tokens[5], "TOLERANCE") != 0)
            {
                fprintf(stderr, "FAIL %s:%d: BASELINE expects 'TOLERANCE <pct>'\n", assert_file, line_num);
                failed++;
                continue;
            }
            result = Eval_Baseline(&g_table, csv_dir, assert_file,
                filter_col, filter_val,
                tokens[3], tokens[4], atoi(tokens[6]),
                line, line_num);
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
