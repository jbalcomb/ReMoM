/*
    STU_INI -- keeping a player's settings file up to date.  See STU_INI.h and doc/@AI_Plans/PLAN-ReMoM-Ini.md.
*/

#include "STU_INI.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



/* -------------------------------------------------------------------------- */
/* A growing text buffer.                                                     */
/* -------------------------------------------------------------------------- */

typedef struct
{
    char * data;
    size_t length;
    size_t capacity;
    int    failed;
} ini_text;

static void ini_text_append_n(ini_text * text, const char * s, size_t n)
{
    if(text->failed != 0)
    {
        return;
    }
    if(text->length + n + 1 > text->capacity)
    {
        size_t new_capacity = (text->capacity == 0) ? 1024 : text->capacity;
        char * grown;
        while(text->length + n + 1 > new_capacity)
        {
            new_capacity *= 2;
        }
        grown = (char *)realloc(text->data, new_capacity);
        if(grown == NULL)
        {
            text->failed = 1;
            return;
        }
        text->data = grown;
        text->capacity = new_capacity;
    }
    memcpy(text->data + text->length, s, n);
    text->length += n;
    text->data[text->length] = '\0';
}

static void ini_text_append(ini_text * text, const char * s)
{
    ini_text_append_n(text, s, strlen(s));
}



/* -------------------------------------------------------------------------- */
/* Reading and classifying lines.                                             */
/* -------------------------------------------------------------------------- */

/* The whole file, NUL-terminated, read in binary so the line endings are kept. NULL if it cannot be read. */
static char * ini_read_file(const char * path, size_t * length_out)
{
    FILE * fp;
    ini_text text = { NULL, 0, 0, 0 };
    char chunk[4096];
    size_t n;

    fp = fopen(path, "rb");
    if(fp == NULL)
    {
        return NULL;
    }
    ini_text_append(&text, "");  /* an empty file still yields "" */
    while((n = fread(chunk, 1, sizeof(chunk), fp)) > 0)
    {
        ini_text_append_n(&text, chunk, n);
    }
    fclose(fp);
    if(text.failed != 0)
    {
        free(text.data);
        return NULL;
    }
    if(length_out != NULL)
    {
        *length_out = text.length;
    }
    return text.data;
}

static int ini_is_space(char c)
{
    return ((c == ' ') || (c == '\t'));
}

static char ini_lower(char c)
{
    return ((c >= 'A') && (c <= 'Z')) ? (char)(c - 'A' + 'a') : c;
}

/* The span [s, s + n) equals the NUL-terminated name, ignoring case. */
static int ini_span_is(const char * s, size_t n, const char * name)
{
    size_t i;
    if(strlen(name) != n)
    {
        return 0;
    }
    for(i = 0; i < n; i++)
    {
        if(ini_lower(s[i]) != ini_lower(name[i]))
        {
            return 0;
        }
    }
    return 1;
}

#define INI_LINE_OTHER    0  /* blank, prose comment, anything else */
#define INI_LINE_SECTION  1  /* [name] */
#define INI_LINE_SETTING  2  /* key = value, live or commented out */

/* Classify the line [s, s + n) (no line ending). For a section or a setting, *name / *name_length is the section name or the
   key. A commented-out setting is "; key = value" or "# key = value" (any number of leading ';' / '#'). */
static int ini_classify_line(const char * s, size_t n, const char ** name, size_t * name_length)
{
    const char * end = s + n;
    const char * p = s;
    const char * key_end;

    while((p < end) && ini_is_space(*p))
    {
        p++;
    }
    if((p < end) && (*p == '['))
    {
        const char * close = p + 1;
        const char * name_start = p + 1;
        while((close < end) && (*close != ']'))
        {
            close++;
        }
        if(close >= end)
        {
            return INI_LINE_OTHER;
        }
        while((name_start < close) && ini_is_space(*name_start))
        {
            name_start++;
        }
        key_end = close;
        while((key_end > name_start) && ini_is_space(key_end[-1]))
        {
            key_end--;
        }
        *name = name_start;
        *name_length = (size_t)(key_end - name_start);
        return INI_LINE_SECTION;
    }
    while((p < end) && ((*p == ';') || (*p == '#') || ini_is_space(*p)))
    {
        p++;
    }
    key_end = p;
    while((key_end < end) && (*key_end != '='))
    {
        key_end++;
    }
    if(key_end >= end)
    {
        return INI_LINE_OTHER;
    }
    {
        const char * key_start = p;
        const char * key_last = key_end;
        while((key_last > key_start) && ini_is_space(key_last[-1]))
        {
            key_last--;
        }
        if(key_last == key_start)
        {
            return INI_LINE_OTHER;
        }
        *name = key_start;
        *name_length = (size_t)(key_last - key_start);
    }
    return INI_LINE_SETTING;
}

/* The next line of `text` from offset: its content [*line_start, *line_start + *line_length) without the line ending, and the
   offset just past the line ending. Returns 0 at the end of the text. */
static int ini_next_line(const char * text, size_t text_length, size_t offset, size_t * line_start, size_t * line_length, size_t * next_offset)
{
    size_t i = offset;
    size_t content_end;

    if(offset >= text_length)
    {
        return 0;
    }
    while((i < text_length) && (text[i] != '\n'))
    {
        i++;
    }
    content_end = i;
    if((content_end > offset) && (text[content_end - 1] == '\r'))
    {
        content_end--;
    }
    *line_start = offset;
    *line_length = content_end - offset;
    *next_offset = (i < text_length) ? (i + 1) : i;
    return 1;
}

static int ini_line_is_blank(const char * s, size_t n)
{
    size_t i;
    for(i = 0; i < n; i++)
    {
        if(!ini_is_space(s[i]))
        {
            return 0;
        }
    }
    return 1;
}



/* -------------------------------------------------------------------------- */
/* Public: questions about a file.                                            */
/* -------------------------------------------------------------------------- */

int STU_INI_File_Exists(const char * path)
{
    FILE * fp;
    if((path == NULL) || (path[0] == '\0'))
    {
        return 0;
    }
    fp = fopen(path, "rb");
    if(fp == NULL)
    {
        return 0;
    }
    fclose(fp);
    return 1;
}

int STU_INI_Text_Has_Key(const char * text, const char * section, const char * key)
{
    size_t text_length;
    size_t offset = 0;
    size_t line_start = 0;
    size_t line_length = 0;
    size_t next_offset = 0;
    int in_section = 0;

    if((text == NULL) || (section == NULL) || (key == NULL))
    {
        return 0;
    }
    text_length = strlen(text);
    while(ini_next_line(text, text_length, offset, &line_start, &line_length, &next_offset) != 0)
    {
        const char * name = NULL;
        size_t name_length = 0;
        int kind = ini_classify_line(text + line_start, line_length, &name, &name_length);

        if(kind == INI_LINE_SECTION)
        {
            in_section = ini_span_is(name, name_length, section);
        }
        else if((kind == INI_LINE_SETTING) && (in_section != 0) && (ini_span_is(name, name_length, key) != 0))
        {
            return 1;
        }
        offset = next_offset;
    }
    return 0;
}

int STU_INI_Has_Key(const char * path, const char * section, const char * key)
{
    char * text = ini_read_file(path, NULL);
    int found;
    if(text == NULL)
    {
        return 0;
    }
    found = STU_INI_Text_Has_Key(text, section, key);
    free(text);
    return found;
}

const char * STU_INI_Native_Eol(void)
{
#if defined(_WIN32)
    return "\r\n";
#else
    return "\n";
#endif
}



/* -------------------------------------------------------------------------- */
/* Composing settings.                                                        */
/* -------------------------------------------------------------------------- */

/* Each line of `lines` ('\n'-separated) as a "; " comment line. An empty line is written as a bare ";". */
static void ini_append_comment_lines(ini_text * text, const char * lines, const char * eol)
{
    const char * p = lines;
    if(lines == NULL)
    {
        return;
    }
    while(*p != '\0')
    {
        const char * line_end = strchr(p, '\n');
        size_t n = (line_end != NULL) ? (size_t)(line_end - p) : strlen(p);
        if(n == 0)
        {
            ini_text_append(text, ";");
        }
        else
        {
            ini_text_append(text, "; ");
            ini_text_append_n(text, p, n);
        }
        ini_text_append(text, eol);
        if(line_end == NULL)
        {
            break;
        }
        p = line_end + 1;
    }
}

/* One setting: its explanation, then "key = value", commented out when asked or when the value is only an example. */
static void ini_append_setting(ini_text * text, const STU_INI_Setting * setting, int commented_out, const char * eol)
{
    ini_append_comment_lines(text, setting->explanation, eol);
    if((commented_out != 0) || (setting->value_is_example != 0))
    {
        ini_text_append(text, "; ");
    }
    ini_text_append(text, setting->key);
    ini_text_append(text, " = ");
    ini_text_append(text, (setting->value != NULL) ? setting->value : "");
    ini_text_append(text, eol);
}

char * STU_INI_Compose(const char * header, const STU_INI_Setting * settings, int count, int commented_out, const char * eol)
{
    ini_text text = { NULL, 0, 0, 0 };
    int section_index;
    int setting_index;

    ini_text_append(&text, "");
    if(header != NULL)
    {
        ini_append_comment_lines(&text, header, eol);
    }
    for(section_index = 0; section_index < count; section_index++)
    {
        int seen_before = 0;
        int earlier;
        for(earlier = 0; earlier < section_index; earlier++)
        {
            if(ini_span_is(settings[earlier].section, strlen(settings[earlier].section), settings[section_index].section) != 0)
            {
                seen_before = 1;
                break;
            }
        }
        if(seen_before != 0)
        {
            continue;
        }
        if(text.length > 0)
        {
            ini_text_append(&text, eol);
        }
        ini_text_append(&text, "[");
        ini_text_append(&text, settings[section_index].section);
        ini_text_append(&text, "]");
        ini_text_append(&text, eol);
        for(setting_index = section_index; setting_index < count; setting_index++)
        {
            if(ini_span_is(settings[setting_index].section, strlen(settings[setting_index].section), settings[section_index].section) == 0)
            {
                continue;
            }
            ini_text_append(&text, eol);
            ini_append_setting(&text, &settings[setting_index], commented_out, eol);
        }
    }
    if(text.failed != 0)
    {
        free(text.data);
        return NULL;
    }
    return text.data;
}



/* -------------------------------------------------------------------------- */
/* Writing.                                                                   */
/* -------------------------------------------------------------------------- */

static int ini_write_file(const char * path, const char * data, size_t length)
{
    FILE * fp = fopen(path, "wb");
    size_t written;
    int closed;
    if(fp == NULL)
    {
        return 0;
    }
    written = fwrite(data, 1, length, fp);
    closed = fclose(fp);
    return ((written == length) && (closed == 0)) ? 1 : 0;
}

int STU_INI_Write_If_Different(const char * path, const char * text)
{
    size_t existing_length = 0;
    char * existing;

    if((path == NULL) || (text == NULL))
    {
        return STU_INI_ERROR;
    }
    existing = ini_read_file(path, &existing_length);
    if(existing != NULL)
    {
        int same = ((existing_length == strlen(text)) && (memcmp(existing, text, existing_length) == 0));
        free(existing);
        if(same != 0)
        {
            return STU_INI_UNCHANGED;
        }
    }
    return (ini_write_file(path, text, strlen(text)) != 0) ? STU_INI_CHANGED : STU_INI_ERROR;
}

/* "<folder><stem>_backup_YYYYMMDDHHMMSS<ext>" beside path, not yet taken ("_2", "_3"... added when it is). */
static int ini_backup_path(const char * path, char * out, size_t cap)
{
    const char * slash = strrchr(path, '/');
    const char * backslash = strrchr(path, '\\');
    const char * name = path;
    const char * dot;
    char stamp[32];
    time_t now = time(NULL);
    struct tm * local = localtime(&now);
    int attempt;

    if((slash != NULL) && (slash + 1 > name))
    {
        name = slash + 1;
    }
    if((backslash != NULL) && (backslash + 1 > name))
    {
        name = backslash + 1;
    }
    dot = strrchr(name, '.');
    if(dot == NULL)
    {
        dot = name + strlen(name);
    }
    if((local == NULL) || (strftime(stamp, sizeof(stamp), "%Y%m%d%H%M%S", local) == 0))
    {
        snprintf(stamp, sizeof(stamp), "%lld", (long long)now);
    }
    for(attempt = 1; attempt < 100; attempt++)
    {
        int n;
        if(attempt == 1)
        {
            n = snprintf(out, cap, "%.*s_backup_%s%s", (int)(dot - path), path, stamp, dot);
        }
        else
        {
            n = snprintf(out, cap, "%.*s_backup_%s_%d%s", (int)(dot - path), path, stamp, attempt, dot);
        }
        if((n <= 0) || ((size_t)n >= cap))
        {
            return 0;
        }
        if(STU_INI_File_Exists(out) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Where new settings for `section` go: just past the line ending of the section's last non-blank line (its header if it has
   none). Returns 0 if the text has no such section. */
static int ini_section_insert_offset(const char * text, size_t text_length, const char * section, size_t * insert_offset)
{
    size_t offset = 0;
    size_t line_start = 0;
    size_t line_length = 0;
    size_t next_offset = 0;
    int in_section = 0;
    int found = 0;

    while(ini_next_line(text, text_length, offset, &line_start, &line_length, &next_offset) != 0)
    {
        const char * name = NULL;
        size_t name_length = 0;
        int kind = ini_classify_line(text + line_start, line_length, &name, &name_length);

        if(kind == INI_LINE_SECTION)
        {
            in_section = ini_span_is(name, name_length, section);
            if(in_section != 0)
            {
                found = 1;  /* a repeated section: the last one gets the additions */
                *insert_offset = next_offset;
            }
        }
        else if((in_section != 0) && (ini_line_is_blank(text + line_start, line_length) == 0))
        {
            *insert_offset = next_offset;
        }
        offset = next_offset;
    }
    return found;
}

int STU_INI_Add_Missing(const char * path, const STU_INI_Setting * settings, int count, char * backup_path_out, size_t backup_cap, int * added_out)
{
    size_t text_length = 0;
    char * text;
    const char * eol;
    int * missing;
    size_t * insert_offsets;
    int missing_count = 0;
    int index;
    ini_text result = { NULL, 0, 0, 0 };
    size_t copied_up_to = 0;
    int status = STU_INI_ERROR;

    if((backup_path_out != NULL) && (backup_cap > 0))
    {
        backup_path_out[0] = '\0';
    }
    if(added_out != NULL)
    {
        *added_out = 0;
    }
    if((path == NULL) || (settings == NULL) || (count <= 0))
    {
        return STU_INI_UNCHANGED;
    }
    text = ini_read_file(path, &text_length);
    if(text == NULL)
    {
        return STU_INI_ERROR;
    }
    eol = (strstr(text, "\r\n") != NULL) ? "\r\n" : ((strchr(text, '\n') != NULL) ? "\n" : STU_INI_Native_Eol());

    missing = (int *)calloc((size_t)count, sizeof(int));
    insert_offsets = (size_t *)calloc((size_t)count, sizeof(size_t));
    if((missing == NULL) || (insert_offsets == NULL))
    {
        free(missing);
        free(insert_offsets);
        free(text);
        return STU_INI_ERROR;
    }

    /* Which settings are missing, and where each goes: an offset into the text when its section exists, text_length + 1 when
       the section is not in the file (it is appended at the end). */
    for(index = 0; index < count; index++)
    {
        if(STU_INI_Text_Has_Key(text, settings[index].section, settings[index].key) == 0)
        {
            size_t offset = 0;
            missing[index] = 1;
            missing_count++;
            insert_offsets[index] = (ini_section_insert_offset(text, text_length, settings[index].section, &offset) != 0) ? offset : (text_length + 1);
        }
    }
    if(missing_count == 0)
    {
        free(missing);
        free(insert_offsets);
        free(text);
        return STU_INI_UNCHANGED;
    }

    /* The new text: the file as it is, with each existing section's additions spliced in at its insert offset (in file order,
       settings in table order within an offset), then the sections the file lacks, appended. */
    ini_text_append(&result, "");
    for(;;)
    {
        size_t next_offset = text_length + 1;
        for(index = 0; index < count; index++)
        {
            if((missing[index] != 0) && (insert_offsets[index] >= copied_up_to) && (insert_offsets[index] <= text_length) && (insert_offsets[index] < next_offset))
            {
                next_offset = insert_offsets[index];
            }
        }
        if(next_offset > text_length)
        {
            break;
        }
        ini_text_append_n(&result, text + copied_up_to, next_offset - copied_up_to);
        copied_up_to = next_offset;
        if((result.length > 0) && (result.data[result.length - 1] != '\n'))
        {
            ini_text_append(&result, eol);  /* the section ended the file without a line ending */
        }
        for(index = 0; index < count; index++)
        {
            if((missing[index] != 0) && (insert_offsets[index] == next_offset))
            {
                ini_text_append(&result, eol);
                ini_append_setting(&result, &settings[index], 1, eol);
                missing[index] = 0;
            }
        }
    }
    ini_text_append_n(&result, text + copied_up_to, text_length - copied_up_to);

    for(index = 0; index < count; index++)
    {
        int later;
        if(missing[index] == 0)
        {
            continue;
        }
        if((result.length > 0) && (result.data[result.length - 1] != '\n'))
        {
            ini_text_append(&result, eol);
        }
        if(result.length > 0)
        {
            ini_text_append(&result, eol);
        }
        ini_text_append(&result, "[");
        ini_text_append(&result, settings[index].section);
        ini_text_append(&result, "]");
        ini_text_append(&result, eol);
        for(later = index; later < count; later++)
        {
            if((missing[later] != 0) && (ini_span_is(settings[later].section, strlen(settings[later].section), settings[index].section) != 0))
            {
                ini_text_append(&result, eol);
                ini_append_setting(&result, &settings[later], 1, eol);
                missing[later] = 0;
            }
        }
    }

    if(result.failed == 0)
    {
        char backup_path[1024];
        if((ini_backup_path(path, backup_path, sizeof(backup_path)) != 0) && (ini_write_file(backup_path, text, text_length) != 0))
        {
            if(ini_write_file(path, result.data, result.length) != 0)
            {
                status = STU_INI_CHANGED;
                if(added_out != NULL)
                {
                    *added_out = missing_count;
                }
                if((backup_path_out != NULL) && (backup_cap > 0))
                {
                    snprintf(backup_path_out, backup_cap, "%s", backup_path);
                }
            }
            else
            {
                /* Put the original back: the write may have truncated it. */
                ini_write_file(path, text, text_length);
                remove(backup_path);
            }
        }
    }

    free(result.data);
    free(missing);
    free(insert_offsets);
    free(text);
    return status;
}
