/**
 * lbx_extract.c — Extract entries from a Master of Magic .LBX archive to standalone files.
 *
 * Usage:
 *   lbx_extract --list <file.lbx>
 *   lbx_extract [-o DIR] [--raw] <file.lbx> <entry> [entry ...]
 *   lbx_extract [-o DIR] [--raw] --all <file.lbx>
 *
 * Examples:
 *   lbx_extract --list assets/MUSIC.LBX
 *   lbx_extract -o out assets/MUSIC.LBX 0          -> out/MUSIC_000.XMI
 *   lbx_extract -o out assets/SOUNDFX.LBX 0        -> out/SOUNDFX_000.VOC
 *   lbx_extract -o out --raw assets/MUSIC.LBX 0    -> out/MUSIC_000.BIN  (0xDEAF wrapper kept)
 *
 * LBX container layout (verified against assets/MUSIC.LBX, SOUNDFX.LBX, SNDDRV.LBX):
 *   0x0000  uint16  num_entries
 *   0x0002  uint16  magic == 0xFEAD                    (LBX_MAGSIG, MoX/src/LBX_Load.h:34)
 *   0x0004  uint16  file_type
 *   0x0006  uint16  dummy
 *   0x0008  uint32  offset[num_entries + 1]            (LBX_GET_ENTRY_START/END, LBX_Load.h:50-51)
 *   0x0200  name table, 32 bytes per entry: char name[9]; char description[23];
 *
 * The name table is NOT reliably present.  SNDDRV.LBX has one that is entirely NUL, and SOUNDFX.LBX declares 119 entries but its first data offset (0x1080) leaves room for only 116 name records.  So the table is read only where it
 * provably fits below offset[0], and names are used for the listing only — never for output filenames.  Names are also 8.3 truncated to 8 characters ("MOM81.XM" is really MOM81.XMI), which would make them a poor basis for a
 * filename anyway.
 *
 * Sound entries (MUSIC.LBX, SOUNDFX.LBX) carry a 16-byte wrapper ahead of the payload (LEN_LBX_SND_HDR, platform/sdl2/sdl2_Audio.c:132):
 *   +0  uint16  0xDEAF                                 (SNDMAGSIG, platform/sdl2/PFL_Audio_Internal.h:42)
 *   +2  uint16  subtype: 1 = XMI (music), 2 = VOC (sfx)
 * By default the wrapper is stripped so the output is a standalone .XMI / .VOC that external tools accept.  --raw keeps it.
 *
 * Standalone: no engine dependencies, no STU, portable C89-ish.  All multi-byte reads are assembled from bytes, so host endianness does not matter.
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/* ========================================================================= */
/*  Container constants (mirror MoX/src/LBX_Load.h)                          */
/* ========================================================================= */

#define LBX_MAGSIG                    0xFEAD
#define SZ_LBX_HDR_B                  512
#define LBX_HDR_POS_NUM_ENTRIES       0x00
#define LBX_HDR_POS_MAGIC_NUMBER      0x02
#define LBX_HDR_POS_FILE_TYPE         0x04
#define LBX_HDR_POS_OFFSETS           0x08

#define LBX_NAME_TABLE_RECORD_SIZE    32
#define LBX_NAME_LEN                  9
#define LBX_DESCRIPTION_LEN           23

/* Sound-entry wrapper (mirror platform/sdl2/sdl2_Audio.c) */
#define LEN_LBX_SND_HDR               16
#define SNDMAGSIG                     0xDEAF
#define LBX_ENTRY_TYPE_SOUND_XMI      0x0001
#define LBX_ENTRY_TYPE_SOUND_VOC      0x0002

#define MAX_PATH_LEN                  1024


/* ========================================================================= */
/*  Little-endian byte readers                                               */
/* ========================================================================= */

static uint16_t Get_U16_LE(const uint8_t * buffer, size_t offset)
{
    return (uint16_t)((uint16_t)buffer[offset] | ((uint16_t)buffer[offset + 1] << 8));
}

static uint32_t Get_U32_LE(const uint8_t * buffer, size_t offset)
{
    return (uint32_t)buffer[offset] | ((uint32_t)buffer[offset + 1] << 8) | ((uint32_t)buffer[offset + 2] << 16) | ((uint32_t)buffer[offset + 3] << 24);
}


/* ========================================================================= */
/*  File slurp                                                               */
/* ========================================================================= */

static uint8_t * Read_Whole_File(const char * path, size_t * out_size)
{
    FILE * file_ptr;
    uint8_t * buffer;
    long file_size;
    size_t bytes_read;

    file_ptr = fopen(path, "rb");
    if(file_ptr == NULL)
    {
        fprintf(stderr, "lbx_extract: cannot open '%s' for reading\n", path);
        return NULL;
    }

    if(fseek(file_ptr, 0, SEEK_END) != 0)
    {
        fprintf(stderr, "lbx_extract: seek failed on '%s'\n", path);
        fclose(file_ptr);
        return NULL;
    }

    file_size = ftell(file_ptr);
    if(file_size < 0)
    {
        fprintf(stderr, "lbx_extract: ftell failed on '%s'\n", path);
        fclose(file_ptr);
        return NULL;
    }
    rewind(file_ptr);

    buffer = (uint8_t *)malloc((size_t)file_size);
    if(buffer == NULL)
    {
        fprintf(stderr, "lbx_extract: out of memory reading '%s' (%ld bytes)\n", path, file_size);
        fclose(file_ptr);
        return NULL;
    }

    bytes_read = fread(buffer, 1, (size_t)file_size, file_ptr);
    fclose(file_ptr);

    if(bytes_read != (size_t)file_size)
    {
        fprintf(stderr, "lbx_extract: short read on '%s' (got %lu of %ld bytes)\n", path, (unsigned long)bytes_read, file_size);
        free(buffer);
        return NULL;
    }

    *out_size = (size_t)file_size;
    return buffer;
}


/* ========================================================================= */
/*  Archive model                                                            */
/* ========================================================================= */

struct s_LBX
{
    const uint8_t * data;
    size_t          size;
    uint16_t        num_entries;
    uint16_t        file_type;
    uint32_t        first_offset;
    int             name_table_records;  /* how many 32-byte name records provably fit below first_offset */
};

static int LBX_Open(struct s_LBX * lbx, const uint8_t * data, size_t size, const char * path)
{
    uint32_t offset_table_end;
    uint32_t name_table_space;

    memset(lbx, 0, sizeof(*lbx));

    if(size < SZ_LBX_HDR_B)
    {
        fprintf(stderr, "lbx_extract: '%s' is only %lu bytes, smaller than the %d-byte LBX header\n", path, (unsigned long)size, SZ_LBX_HDR_B);
        return 0;
    }

    if(Get_U16_LE(data, LBX_HDR_POS_MAGIC_NUMBER) != LBX_MAGSIG)
    {
        fprintf(stderr, "lbx_extract: '%s' has magic 0x%04X, expected 0x%04X — not an LBX archive\n", path, Get_U16_LE(data, LBX_HDR_POS_MAGIC_NUMBER), LBX_MAGSIG);
        return 0;
    }

    lbx->data        = data;
    lbx->size        = size;
    lbx->num_entries = Get_U16_LE(data, LBX_HDR_POS_NUM_ENTRIES);
    lbx->file_type   = Get_U16_LE(data, LBX_HDR_POS_FILE_TYPE);

    if(lbx->num_entries == 0)
    {
        fprintf(stderr, "lbx_extract: '%s' declares 0 entries\n", path);
        return 0;
    }

    /* offset table holds num_entries + 1 uint32 values, so entry N's end is offset[N + 1] */
    offset_table_end = (uint32_t)LBX_HDR_POS_OFFSETS + ((uint32_t)lbx->num_entries + 1u) * 4u;
    if(offset_table_end > (uint32_t)size)
    {
        fprintf(stderr, "lbx_extract: '%s' offset table for %u entries runs past end of file\n", path, (unsigned)lbx->num_entries);
        return 0;
    }

    lbx->first_offset = Get_U32_LE(data, LBX_HDR_POS_OFFSETS);

    /*
        The name table sits at 0x200 and is nominally num_entries * 32 bytes, but real archives disagree with that: SOUNDFX.LBX declares 119 entries yet its first data offset (0x1080) leaves room for only 116 records.  Trust the
        data offset, not the entry count, and read names only for records that provably fit.
    */
    lbx->name_table_records = 0;
    if(lbx->first_offset > (uint32_t)SZ_LBX_HDR_B)
    {
        name_table_space = lbx->first_offset - (uint32_t)SZ_LBX_HDR_B;
        lbx->name_table_records = (int)(name_table_space / LBX_NAME_TABLE_RECORD_SIZE);
        if(lbx->name_table_records > (int)lbx->num_entries)
        {
            lbx->name_table_records = (int)lbx->num_entries;
        }
    }

    return 1;
}

static int LBX_Get_Entry(const struct s_LBX * lbx, int entry_num, uint32_t * out_start, uint32_t * out_length)
{
    uint32_t start;
    uint32_t end;

    if(entry_num < 0 || entry_num >= (int)lbx->num_entries)
    {
        fprintf(stderr, "lbx_extract: entry %d out of range (archive has %u entries, 0..%u)\n", entry_num, (unsigned)lbx->num_entries, (unsigned)lbx->num_entries - 1u);
        return 0;
    }

    start = Get_U32_LE(lbx->data, (size_t)LBX_HDR_POS_OFFSETS + (size_t)entry_num * 4u);
    end   = Get_U32_LE(lbx->data, (size_t)LBX_HDR_POS_OFFSETS + (size_t)entry_num * 4u + 4u);

    if(end < start)
    {
        fprintf(stderr, "lbx_extract: entry %d has end offset 0x%lX before start offset 0x%lX — corrupt offset table\n", entry_num, (unsigned long)end, (unsigned long)start);
        return 0;
    }
    if((size_t)end > lbx->size)
    {
        fprintf(stderr, "lbx_extract: entry %d ends at 0x%lX, past end of file (0x%lX)\n", entry_num, (unsigned long)end, (unsigned long)lbx->size);
        return 0;
    }

    *out_start  = start;
    *out_length = end - start;
    return 1;
}

/* Copies the entry's name-table name into `out` (size >= LBX_NAME_LEN + 1).  Empty string when there is no usable name. */
static void LBX_Get_Entry_Name(const struct s_LBX * lbx, int entry_num, char * out)
{
    size_t record_offset;
    int i;

    out[0] = '\0';
    if(entry_num >= lbx->name_table_records)
    {
        return;
    }

    record_offset = (size_t)SZ_LBX_HDR_B + (size_t)entry_num * LBX_NAME_TABLE_RECORD_SIZE;
    for(i = 0; i < LBX_NAME_LEN; i++)
    {
        uint8_t c = lbx->data[record_offset + (size_t)i];
        if(c == 0)
        {
            break;
        }
        out[i] = (char)c;
    }
    out[i] = '\0';
}

static void LBX_Get_Entry_Description(const struct s_LBX * lbx, int entry_num, char * out)
{
    size_t record_offset;
    int i;

    out[0] = '\0';
    if(entry_num >= lbx->name_table_records)
    {
        return;
    }

    record_offset = (size_t)SZ_LBX_HDR_B + (size_t)entry_num * LBX_NAME_TABLE_RECORD_SIZE + LBX_NAME_LEN;
    for(i = 0; i < LBX_DESCRIPTION_LEN; i++)
    {
        uint8_t c = lbx->data[record_offset + (size_t)i];
        if(c == 0)
        {
            break;
        }
        out[i] = (char)c;
    }
    out[i] = '\0';
}


/* ========================================================================= */
/*  Sound-wrapper classification                                             */
/* ========================================================================= */

/* Returns the 0xDEAF subtype (1 = XMI, 2 = VOC), or 0 when this entry is not a wrapped sound. */
static uint16_t Sound_Subtype(const uint8_t * entry, uint32_t length)
{
    if(length < LEN_LBX_SND_HDR)
    {
        return 0;
    }
    if(Get_U16_LE(entry, 0) != SNDMAGSIG)
    {
        return 0;
    }
    return Get_U16_LE(entry, 2);
}

static const char * Subtype_Extension(uint16_t subtype)
{
    switch(subtype)
    {
        case LBX_ENTRY_TYPE_SOUND_XMI: return "XMI";
        case LBX_ENTRY_TYPE_SOUND_VOC: return "VOC";
        default:                       return "BIN";
    }
}

static const char * Subtype_Label(uint16_t subtype)
{
    switch(subtype)
    {
        case 0:                        return "-";
        case LBX_ENTRY_TYPE_SOUND_XMI: return "XMI";
        case LBX_ENTRY_TYPE_SOUND_VOC: return "VOC";
        default:                       return "snd?";
    }
}


/* ========================================================================= */
/*  Output                                                                   */
/* ========================================================================= */

/* Derives the archive stem from a path: "assets/MUSIC.LBX" -> "MUSIC". */
static void Archive_Stem(const char * path, char * out, size_t out_size)
{
    const char * base;
    const char * cursor;
    const char * dot;
    size_t len;

    base = path;
    for(cursor = path; *cursor != '\0'; cursor++)
    {
        if(*cursor == '/' || *cursor == '\\')
        {
            base = cursor + 1;
        }
    }

    dot = strrchr(base, '.');
    len = (dot != NULL) ? (size_t)(dot - base) : strlen(base);
    if(len >= out_size)
    {
        len = out_size - 1;
    }
    memcpy(out, base, len);
    out[len] = '\0';

    if(len == 0)
    {
        strcpy(out, "LBX");
    }
}

static int Write_Entry_File(const char * out_dir, const char * stem, int entry_num, const char * extension, const uint8_t * payload, uint32_t payload_length)
{
    char path[MAX_PATH_LEN];
    FILE * file_ptr;
    size_t bytes_written;

    if(out_dir != NULL && out_dir[0] != '\0')
    {
        snprintf(path, sizeof(path), "%s/%s_%03d.%s", out_dir, stem, entry_num, extension);
    }
    else
    {
        snprintf(path, sizeof(path), "%s_%03d.%s", stem, entry_num, extension);
    }

    file_ptr = fopen(path, "wb");
    if(file_ptr == NULL)
    {
        fprintf(stderr, "lbx_extract: cannot open '%s' for writing (does the output directory exist?)\n", path);
        return 0;
    }

    bytes_written = (payload_length > 0) ? fwrite(payload, 1, (size_t)payload_length, file_ptr) : 0;

    if(fclose(file_ptr) != 0)
    {
        fprintf(stderr, "lbx_extract: error closing '%s'\n", path);
        return 0;
    }
    if(bytes_written != (size_t)payload_length)
    {
        fprintf(stderr, "lbx_extract: short write on '%s' (wrote %lu of %lu bytes)\n", path, (unsigned long)bytes_written, (unsigned long)payload_length);
        return 0;
    }

    printf("wrote %s  (%lu bytes)\n", path, (unsigned long)payload_length);
    return 1;
}

static int Extract_One(const struct s_LBX * lbx, int entry_num, const char * out_dir, const char * stem, int keep_raw)
{
    uint32_t start;
    uint32_t length;
    const uint8_t * entry;
    const uint8_t * payload;
    uint32_t payload_length;
    uint16_t subtype;
    const char * extension;

    if(!LBX_Get_Entry(lbx, entry_num, &start, &length))
    {
        return 0;
    }

    entry   = lbx->data + start;
    subtype = Sound_Subtype(entry, length);

    payload        = entry;
    payload_length = length;
    extension      = "BIN";

    if(subtype != 0 && !keep_raw)
    {
        payload        = entry + LEN_LBX_SND_HDR;
        payload_length = length - LEN_LBX_SND_HDR;
        extension      = Subtype_Extension(subtype);
    }

    return Write_Entry_File(out_dir, stem, entry_num, extension, payload, payload_length);
}

static void List_Entries(const struct s_LBX * lbx)
{
    int entry_num;

    printf("entries    : %u\n", (unsigned)lbx->num_entries);
    printf("file_type  : %u\n", (unsigned)lbx->file_type);
    printf("name table : %d of %u records fit below first data offset 0x%lX\n", lbx->name_table_records, (unsigned)lbx->num_entries, (unsigned long)lbx->first_offset);
    printf("\n");
    printf("  idx  offset      length  kind  name       description\n");
    printf("  ---  ----------  ------  ----  ---------  -----------------------\n");

    for(entry_num = 0; entry_num < (int)lbx->num_entries; entry_num++)
    {
        uint32_t start;
        uint32_t length;
        char name[LBX_NAME_LEN + 1];
        char description[LBX_DESCRIPTION_LEN + 1];
        uint16_t subtype;

        if(!LBX_Get_Entry(lbx, entry_num, &start, &length))
        {
            continue;
        }

        LBX_Get_Entry_Name(lbx, entry_num, name);
        LBX_Get_Entry_Description(lbx, entry_num, description);
        subtype = Sound_Subtype(lbx->data + start, length);

        printf("  %3d  0x%08lX  %6lu  %-4s  %-9s  %s\n", entry_num, (unsigned long)start, (unsigned long)length, Subtype_Label(subtype), name, description);
    }
}

static void Print_Usage(const char * argv0)
{
    fprintf(stderr, "usage: %s --list <file.lbx>\n", argv0);
    fprintf(stderr, "       %s [-o DIR] [--raw] <file.lbx> <entry> [entry ...]\n", argv0);
    fprintf(stderr, "       %s [-o DIR] [--raw] --all <file.lbx>\n", argv0);
    fprintf(stderr, "\n");
    fprintf(stderr, "  -l, --list     list entries (index, offset, length, kind, name) and exit\n");
    fprintf(stderr, "  -a, --all      extract every entry\n");
    fprintf(stderr, "  -o, --outdir   write output files here (default: current directory; must already exist)\n");
    fprintf(stderr, "  -r, --raw      keep the 16-byte 0xDEAF sound wrapper instead of stripping it\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  Output is named <archive-stem>_<index>.<ext>, e.g. MUSIC_000.XMI.\n");
    fprintf(stderr, "  Sound entries are written as .XMI or .VOC with the wrapper stripped; everything else as .BIN.\n");
}


/* ========================================================================= */
/*  main                                                                     */
/* ========================================================================= */

int main(int argc, char ** argv)
{
    const char * lbx_path = NULL;
    const char * out_dir = NULL;
    int do_list = 0;
    int do_all = 0;
    int keep_raw = 0;
    int arg_index;
    int first_entry_arg = 0;
    uint8_t * data;
    size_t size;
    struct s_LBX lbx;
    char stem[MAX_PATH_LEN];
    int failures = 0;

    for(arg_index = 1; arg_index < argc; arg_index++)
    {
        const char * arg = argv[arg_index];

        if(strcmp(arg, "-l") == 0 || strcmp(arg, "--list") == 0)
        {
            do_list = 1;
        }
        else if(strcmp(arg, "-a") == 0 || strcmp(arg, "--all") == 0)
        {
            do_all = 1;
        }
        else if(strcmp(arg, "-r") == 0 || strcmp(arg, "--raw") == 0)
        {
            keep_raw = 1;
        }
        else if(strcmp(arg, "-o") == 0 || strcmp(arg, "--outdir") == 0)
        {
            arg_index++;
            if(arg_index >= argc)
            {
                fprintf(stderr, "lbx_extract: %s requires a directory argument\n", arg);
                return 2;
            }
            out_dir = argv[arg_index];
        }
        else if(arg[0] == '-' && arg[1] != '\0' && lbx_path != NULL)
        {
            fprintf(stderr, "lbx_extract: unknown option '%s'\n", arg);
            Print_Usage(argv[0]);
            return 2;
        }
        else if(lbx_path == NULL)
        {
            lbx_path = arg;
        }
        else
        {
            if(first_entry_arg == 0)
            {
                first_entry_arg = arg_index;
            }
        }
    }

    if(lbx_path == NULL)
    {
        Print_Usage(argv[0]);
        return 2;
    }
    if(!do_list && !do_all && first_entry_arg == 0)
    {
        fprintf(stderr, "lbx_extract: nothing to do — give --list, --all, or one or more entry numbers\n");
        Print_Usage(argv[0]);
        return 2;
    }

    data = Read_Whole_File(lbx_path, &size);
    if(data == NULL)
    {
        return 1;
    }

    if(!LBX_Open(&lbx, data, size, lbx_path))
    {
        free(data);
        return 1;
    }

    Archive_Stem(lbx_path, stem, sizeof(stem));

    if(do_list)
    {
        List_Entries(&lbx);
        free(data);
        return 0;
    }

    if(do_all)
    {
        int entry_num;
        for(entry_num = 0; entry_num < (int)lbx.num_entries; entry_num++)
        {
            if(!Extract_One(&lbx, entry_num, out_dir, stem, keep_raw))
            {
                failures++;
            }
        }
    }
    else
    {
        for(arg_index = first_entry_arg; arg_index < argc; arg_index++)
        {
            char * end_ptr = NULL;
            long entry_num;

            if(argv[arg_index][0] == '-')
            {
                continue;  /* already consumed as an option (or its value) in the first pass */
            }

            entry_num = strtol(argv[arg_index], &end_ptr, 10);
            if(end_ptr == argv[arg_index] || *end_ptr != '\0')
            {
                fprintf(stderr, "lbx_extract: '%s' is not an entry number\n", argv[arg_index]);
                failures++;
                continue;
            }

            if(!Extract_One(&lbx, (int)entry_num, out_dir, stem, keep_raw))
            {
                failures++;
            }
        }
    }

    free(data);

    if(failures > 0)
    {
        fprintf(stderr, "lbx_extract: %d entry/entries failed\n", failures);
        return 1;
    }
    return 0;
}
