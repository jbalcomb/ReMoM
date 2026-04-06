/**
 * compare_save_text.c — Field-by-field comparison of HeMoM save dump text files.
 *
 * Usage:
 *   compare_save_text <actual.txt> <expected.txt> [max_diffs]
 *
 * Reads both files line by line and reports differences by field name.
 * Exit code: 0 = identical, 1 = differences found, 2 = error.
 */

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    FILE *fp_actual;
    FILE *fp_expected;
    char line_actual[512];
    char line_expected[512];
    int line_num = 0;
    int diff_count = 0;
    int max_diffs = 20;
    int actual_done = 0;
    int expected_done = 0;

    if (argc < 3)
    {
        fprintf(stderr, "Usage: %s <actual.txt> <expected.txt> [max_diffs]\n", argv[0]);
        return 2;
    }

    if (argc >= 4)
    {
        max_diffs = atoi(argv[3]);
        if (max_diffs < 1) { max_diffs = 20; }
    }

    fp_actual = fopen(argv[1], "r");
    if (fp_actual == NULL)
    {
        fprintf(stderr, "FAIL: cannot open actual file: %s\n", argv[1]);
        return 2;
    }

    fp_expected = fopen(argv[2], "r");
    if (fp_expected == NULL)
    {
        fprintf(stderr, "FAIL: cannot open expected file: %s\n", argv[2]);
        fclose(fp_actual);
        return 2;
    }

    while (1)
    {
        char *got_actual = fgets(line_actual, sizeof(line_actual), fp_actual);
        char *got_expected = fgets(line_expected, sizeof(line_expected), fp_expected);

        if (got_actual == NULL && got_expected == NULL)
        {
            break;  /* Both files ended at the same line */
        }

        line_num++;

        if (got_actual == NULL)
        {
            if (!actual_done)
            {
                actual_done = 1;
                diff_count++;
                if (diff_count <= max_diffs)
                {
                    fprintf(stderr, "DIFF line %d: actual file ended early\n", line_num);
                    fprintf(stderr, "  expected: %s", line_expected);
                }
            }
            continue;
        }

        if (got_expected == NULL)
        {
            if (!expected_done)
            {
                expected_done = 1;
                diff_count++;
                if (diff_count <= max_diffs)
                {
                    fprintf(stderr, "DIFF line %d: expected file ended early\n", line_num);
                    fprintf(stderr, "  actual:   %s", line_actual);
                }
            }
            continue;
        }

        if (stu_strcmp(line_actual, line_expected) != 0)
        {
            diff_count++;
            if (diff_count <= max_diffs)
            {
                /* Strip trailing newlines for clean output */
                size_t len_a = strlen(line_actual);
                size_t len_e = strlen(line_expected);
                if (len_a > 0 && line_actual[len_a - 1] == '\n') { line_actual[len_a - 1] = '\0'; }
                if (len_e > 0 && line_expected[len_e - 1] == '\n') { line_expected[len_e - 1] = '\0'; }

                fprintf(stderr, "DIFF line %d:\n", line_num);
                fprintf(stderr, "  actual:   %s\n", line_actual);
                fprintf(stderr, "  expected: %s\n", line_expected);
            }
        }
    }

    fclose(fp_actual);
    fclose(fp_expected);

    if (diff_count > max_diffs)
    {
        fprintf(stderr, "... and %d more differences (%d total)\n", diff_count - max_diffs, diff_count);
    }

    if (diff_count > 0)
    {
        fprintf(stderr, "FAIL: %d field differences found\n", diff_count);
        return 1;
    }

    fprintf(stderr, "PASS: all %d lines match\n", line_num);
    return 0;
}
