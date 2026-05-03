/**
 * Game_Save_Dump.h — Dump a .GAM save file to structured human-readable text.
 *
 * Reads a binary save file (123300 bytes) and writes every field as a named
 * key=value line. The output can be diffed line-by-line against a golden
 * reference to identify exactly which field changed.
 */

#ifndef HEMOM_SAVEDUMP_H
#define HEMOM_SAVEDUMP_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Parse a .GAM save file and write structured text to an output file.
 * @param save_filepath   Path to the binary .GAM file.
 * @param text_filepath   Path to write the structured text output.
 * @return  0 on success, non-zero on failure.
 */
int Game_Save_Dump(const char *save_filepath, const char *text_filepath);

#ifdef __cplusplus
}
#endif

#endif /* HEMOM_SAVEDUMP_H */
