/**
 * savedump.c — Standalone CLI tool to dump a Master of Magic .GAM save file
 *              to a structured text representation.
 *
 * Usage:
 *   savedump <input.GAM> [output.txt]
 *
 * If output.txt is omitted, the output file is named after the input file with
 * the `.GAM` suffix replaced by `.txt` (or `.txt` appended if no `.GAM` suffix).
 *
 * Exit codes:
 *   0 = success
 *   1 = usage error
 *   2 = file read/write error
 *
 * This tool is independent of HeMoM and the game engine.  It reads the raw
 * 123300-byte save file and interprets the fields using the same layout as
 * Save_SAVE_GAM() / Load_SAVE_GAM() in MoX/src/LOADSAVE.c.
 *
 * Works on .GAM files produced by:
 *   - OG-MoM (MAGIC.EXE / WIZARDS.EXE)
 *   - ReMoMber
 *   - HeMoM
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/Game_Save_Dump.h"



static void Print_Usage(const char *program_name)
{
    fprintf(stderr, "Usage: %s <input.GAM> [output.txt]\n", program_name);
    fprintf(stderr, "\n");
    fprintf(stderr, "Dumps a Master of Magic .GAM save file to a structured text representation.\n");
    fprintf(stderr, "If output.txt is omitted, the output file is named after the input with\n");
    fprintf(stderr, "'.GAM' replaced by '.txt' (or '.txt' appended if no '.GAM' suffix).\n");
}



int main(int argc, char *argv[])
{
    const char *input_path;
    const char *output_path;
    char        derived_path[1024];

    if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0)
    {
        Print_Usage(argv[0]);
        return 1;
    }

    input_path = argv[1];

    if (argc == 3)
    {
        output_path = argv[2];
    }
    else
    {
        size_t len = strlen(input_path);
        if (len >= sizeof(derived_path) - 5)
        {
            fprintf(stderr, "savedump: input path too long\n");
            return 1;
        }
        memcpy(derived_path, input_path, len + 1);
        /* Replace ".GAM" suffix (case-insensitive) with ".txt", else append ".txt". */
        if (len >= 4
            && (derived_path[len - 4] == '.')
            && (derived_path[len - 3] == 'G' || derived_path[len - 3] == 'g')
            && (derived_path[len - 2] == 'A' || derived_path[len - 2] == 'a')
            && (derived_path[len - 1] == 'M' || derived_path[len - 1] == 'm'))
        {
            derived_path[len - 3] = 't';
            derived_path[len - 2] = 'x';
            derived_path[len - 1] = 't';
        }
        else
        {
            memcpy(derived_path + len, ".txt", 5);
        }
        output_path = derived_path;
    }

    if (Game_Save_Dump(input_path, output_path) != 0)
    {
        fprintf(stderr, "savedump: failed\n");
        return 2;
    }

    fprintf(stdout, "savedump: wrote %s\n", output_path);
    return 0;
}
