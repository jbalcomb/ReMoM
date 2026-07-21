/**
 * gtl2op2.c — Convert a Miles/AIL Global Timbre Library (FAT.OPL / FAT.AD) into a DMX GENMIDI (.op2) FM bank.
 *
 * Usage:
 *   gtl2op2 [options] <input-gtl> <output.op2>
 *
 * Examples:
 *   gtl2op2 assets/__FAT/FAT.OPL out/xmi/fatman.op2
 *   gtl2op2 --single assets/__FAT/FAT.OPL out/xmi/fatman_single.op2
 *   gtl2op2 assets/__FAT/FAT.AD  out/xmi/fatman_ad.op2
 *
 * Then render with it:
 *   wildmidi -c out/xmi/fatman.op2 -r 44100 -o track.wav MUSIC_100.XMI
 *
 * WHY: Master of Magic's music is Miles/AIL XMI addressing bank 55 (melodic) and bank 127 (percussion).  WildMIDI's only FM bank format is the DMX GENMIDI lump, so the timbres The Fat Man authored have to be transcoded before
 * WildMIDI can voice them.  Both sides are OPL register dumps; the difference is field order and how KSL/level are packed.
 *
 * --- Source format: Miles Global Timbre Library -----------------------------
 * Index records, 6 bytes each, terminated by bank == 0xFF.  Layout is taken verbatim from the game's own reader, MoX/src/SOUND.c:242-248:
 *     struct { char patch; char bank; unsigned long offset; }
 * At `offset`: uint16 total_length (INCLUDING these 2 bytes), then total_length - 2 payload bytes.
 *
 * Payload is a mode byte followed by one or two 11-byte OPL voices:
 *     mode, [ mod_chr, mod_lvl, mod_ad, mod_sr, mod_ws, feedback, car_chr, car_lvl, car_ad, car_sr, car_ws ] x N
 *   FAT.AD  payload = 12 bytes -> 1 voice   (OPL2)
 *   FAT.OPL payload = 23 bytes -> 2 voices  (OPL3)
 * Verified against assets/__FAT/FAT.OPL and FAT.AD: 181 timbres each, bank 55 patches 0..127 and bank 127 patches 35..87.
 *
 * --- Target format: DMX GENMIDI ---------------------------------------------
 * "#OPL_II#" + 175 x 36-byte records + 175 x 32-byte names.  Record and voice layout per WildMIDI's reader (src/synth.c:77-123):
 *     record: uint16 flags, uint8 finetune, uint8 fixed_note, voice[16], voice[16]
 *     voice:  mod_chr, mod_ad, mod_sr, mod_ws, mod_ksl, mod_lvl, feedback,
 *             car_chr, car_ad, car_sr, car_ws, car_ksl, car_lvl, unused, int16 note_offset
 * Records 0-127 are GM melodic programs; records 128-174 are percussion keys 35-81.
 *
 * KSL/level: Miles packs the OPL 0x40 register as one byte (KSL in bits 6-7, total level in bits 0-5).  GENMIDI splits it across two bytes, and WildMIDI recombines them as (ksl & 0xC0) | (lvl & 0x3F).  Writing the Miles byte into
 * BOTH slots therefore round-trips the register exactly.
 *
 * --- Known lossy points, by design ------------------------------------------
 *  1. Percussion keys 82-87 exist in the GTL but GENMIDI has no slot for them.  They are dropped; the count is reported.
 *  2. The GTL mode byte (payload[0]) is not carried across; GENMIDI has no equivalent field.
 *  3. UNVERIFIED SEMANTICS: a two-voice Miles OPL3 timbre is not necessarily the same thing as a GENMIDI double-voice
 *     instrument.  GENMIDI's OP2_FLAG_DOUBLE layers two detuned 2-op voices; the Miles pair may instead be the two
 *     halves of a 4-operator voice.  This tool emits both voices with the DOUBLE flag set by default and offers
 *     --single to emit voice 1 only.  Which is correct is an open question -- render both and compare by ear.
 *
 * Standalone: no engine dependencies, portable C, host endianness irrelevant (all multi-byte fields assembled bytewise).
 */

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


/* ========================================================================= */
/*  Source: Miles Global Timbre Library                                      */
/* ========================================================================= */

#define GTL_INDEX_RECORD_SIZE      6
#define GTL_BANK_TERMINATOR        0xFF
#define GTL_VOICE_SIZE             11
#define GTL_PAYLOAD_ONE_VOICE      (1 + GTL_VOICE_SIZE)        /* 12 — FAT.AD  */
#define GTL_PAYLOAD_TWO_VOICE      (1 + (2 * GTL_VOICE_SIZE))  /* 23 — FAT.OPL */

/* Byte positions inside one 11-byte Miles voice */
#define GTL_V_MOD_CHR   0
#define GTL_V_MOD_LVL   1
#define GTL_V_MOD_AD    2
#define GTL_V_MOD_SR    3
#define GTL_V_MOD_WS    4
#define GTL_V_FEEDBACK  5
#define GTL_V_CAR_CHR   6
#define GTL_V_CAR_LVL   7
#define GTL_V_CAR_AD    8
#define GTL_V_CAR_SR    9
#define GTL_V_CAR_WS   10


/* ========================================================================= */
/*  Target: DMX GENMIDI                                                      */
/* ========================================================================= */

#define OP2_MAGIC              "#OPL_II#"
#define OP2_HDRSIZE            8
#define OP2_RECORDS            175
#define OP2_RECSIZE            36
#define OP2_NAMESIZE           32
#define OP2_VOICE_SIZE         16

#define OP2_FLAG_FIXED         0x01
#define OP2_FLAG_DOUBLE        0x04

#define OP2_MELODIC_RECORDS    128   /* records 0..127   = GM programs 0..127 */
#define OP2_PERCUSSION_FIRST   128   /* record 128       = percussion key 35  */
#define OP2_PERC_KEY_LOW       35
#define OP2_PERC_KEY_HIGH      81    /* record 174                            */

/* Defaults observed in FAT.OPL / FAT.AD */
#define DEFAULT_MELODIC_BANK      55
#define DEFAULT_PERCUSSION_BANK  127


/* ========================================================================= */
/*  Helpers                                                                  */
/* ========================================================================= */

static uint16_t Get_U16_LE(const uint8_t * buffer, size_t offset)
{
    return (uint16_t)((uint16_t)buffer[offset] | ((uint16_t)buffer[offset + 1] << 8));
}

static uint32_t Get_U32_LE(const uint8_t * buffer, size_t offset)
{
    return (uint32_t)buffer[offset] | ((uint32_t)buffer[offset + 1] << 8) | ((uint32_t)buffer[offset + 2] << 16) | ((uint32_t)buffer[offset + 3] << 24);
}

static uint8_t * Read_Whole_File(const char * path, size_t * out_size)
{
    FILE * file_ptr;
    uint8_t * buffer;
    long file_size;
    size_t bytes_read;

    file_ptr = fopen(path, "rb");
    if(file_ptr == NULL)
    {
        fprintf(stderr, "gtl2op2: cannot open '%s' for reading\n", path);
        return NULL;
    }

    if(fseek(file_ptr, 0, SEEK_END) != 0 || (file_size = ftell(file_ptr)) < 0)
    {
        fprintf(stderr, "gtl2op2: cannot determine size of '%s'\n", path);
        fclose(file_ptr);
        return NULL;
    }
    rewind(file_ptr);

    buffer = (uint8_t *)malloc((size_t)file_size);
    if(buffer == NULL)
    {
        fprintf(stderr, "gtl2op2: out of memory reading '%s' (%ld bytes)\n", path, file_size);
        fclose(file_ptr);
        return NULL;
    }

    bytes_read = fread(buffer, 1, (size_t)file_size, file_ptr);
    fclose(file_ptr);

    if(bytes_read != (size_t)file_size)
    {
        fprintf(stderr, "gtl2op2: short read on '%s' (got %lu of %ld bytes)\n", path, (unsigned long)bytes_read, file_size);
        free(buffer);
        return NULL;
    }

    *out_size = (size_t)file_size;
    return buffer;
}


/* ========================================================================= */
/*  GTL lookup                                                               */
/* ========================================================================= */

/*
    Locates timbre (bank, patch) and returns a pointer to its payload (past the 2-byte length field), with the payload length in *out_payload_length.  Mirrors load_global_timbre() in MoX/src/SOUND.c:237-285, but bounds-checked
    against the in-memory image instead of trusting the file.
*/
static const uint8_t * GTL_Find_Timbre(const uint8_t * gtl, size_t gtl_size, unsigned bank, unsigned patch, size_t * out_payload_length)
{
    size_t index_offset;
    uint32_t payload_offset;
    uint16_t total_length;

    for(index_offset = 0; index_offset + GTL_INDEX_RECORD_SIZE <= gtl_size; index_offset += GTL_INDEX_RECORD_SIZE)
    {
        uint8_t record_patch = gtl[index_offset + 0];
        uint8_t record_bank  = gtl[index_offset + 1];

        if(record_bank == GTL_BANK_TERMINATOR)
        {
            return NULL;
        }
        if(record_bank != (uint8_t)bank || record_patch != (uint8_t)patch)
        {
            continue;
        }

        payload_offset = Get_U32_LE(gtl, index_offset + 2);
        if((size_t)payload_offset + 2u > gtl_size)
        {
            fprintf(stderr, "gtl2op2: timbre (bank %u, patch %u) points at offset 0x%lX, past end of file\n", bank, patch, (unsigned long)payload_offset);
            return NULL;
        }

        total_length = Get_U16_LE(gtl, payload_offset);
        if(total_length < 3 || (size_t)payload_offset + total_length > gtl_size)
        {
            fprintf(stderr, "gtl2op2: timbre (bank %u, patch %u) declares length %u, which does not fit in the file\n", bank, patch, (unsigned)total_length);
            return NULL;
        }

        *out_payload_length = (size_t)total_length - 2u;
        return gtl + payload_offset + 2;
    }

    return NULL;
}


/* ========================================================================= */
/*  Voice transcode                                                          */
/* ========================================================================= */

/*
    Writes one 16-byte GENMIDI voice from one 11-byte Miles voice.

    Miles order : chr, lvl, ad, sr, ws (per operator) with feedback between them.
    GENMIDI order: chr, ad, sr, ws, ksl, lvl (per operator) with feedback between them.

    The Miles level byte IS the OPL 0x40 register (KSL in bits 6-7, total level in bits 0-5).  WildMIDI recombines GENMIDI's split pair as (ksl & 0xC0) | (lvl & 0x3F), so writing the Miles byte into both slots reproduces the
    register exactly (src/synth.c:115,121).
*/
static void Transcode_Voice(const uint8_t * gtl_voice, uint8_t * op2_voice)
{
    memset(op2_voice, 0, OP2_VOICE_SIZE);

    op2_voice[0]  = gtl_voice[GTL_V_MOD_CHR];
    op2_voice[1]  = gtl_voice[GTL_V_MOD_AD];
    op2_voice[2]  = gtl_voice[GTL_V_MOD_SR];
    op2_voice[3]  = gtl_voice[GTL_V_MOD_WS];
    op2_voice[4]  = gtl_voice[GTL_V_MOD_LVL];   /* KSL bits 6-7 read from here */
    op2_voice[5]  = gtl_voice[GTL_V_MOD_LVL];   /* level bits 0-5 read from here */
    op2_voice[6]  = gtl_voice[GTL_V_FEEDBACK];
    op2_voice[7]  = gtl_voice[GTL_V_CAR_CHR];
    op2_voice[8]  = gtl_voice[GTL_V_CAR_AD];
    op2_voice[9]  = gtl_voice[GTL_V_CAR_SR];
    op2_voice[10] = gtl_voice[GTL_V_CAR_WS];
    op2_voice[11] = gtl_voice[GTL_V_CAR_LVL];
    op2_voice[12] = gtl_voice[GTL_V_CAR_LVL];
    op2_voice[13] = 0;                          /* unused */
    op2_voice[14] = 0;                          /* note offset, low byte  */
    op2_voice[15] = 0;                          /* note offset, high byte */
}

/*
    Fills one 36-byte GENMIDI record from a GTL payload.  `fixed_note` is 0 for melodic records, or the percussion key for percussion records.  Returns the number of voices emitted (1 or 2).
*/
static int Build_Record(const uint8_t * payload, size_t payload_length, int force_single, uint8_t fixed_note, uint8_t * record)
{
    const uint8_t * voice_1;
    const uint8_t * voice_2;
    uint16_t flags = 0;
    int voices_emitted = 1;

    memset(record, 0, OP2_RECSIZE);

    voice_1 = payload + 1;  /* skip the mode byte */
    voice_2 = NULL;

    if(payload_length >= GTL_PAYLOAD_TWO_VOICE && !force_single)
    {
        voice_2 = payload + 1 + GTL_VOICE_SIZE;
        flags |= OP2_FLAG_DOUBLE;
        voices_emitted = 2;
    }

    if(fixed_note != 0)
    {
        flags |= OP2_FLAG_FIXED;
    }

    record[0] = (uint8_t)(flags & 0x00FF);
    record[1] = (uint8_t)((flags >> 8) & 0x00FF);
    record[2] = 0;            /* finetune */
    record[3] = fixed_note;

    Transcode_Voice(voice_1, record + 4);
    if(voice_2 != NULL)
    {
        Transcode_Voice(voice_2, record + 4 + OP2_VOICE_SIZE);
    }

    return voices_emitted;
}


/* ========================================================================= */
/*  main                                                                     */
/* ========================================================================= */

static void Print_Usage(const char * argv0)
{
    fprintf(stderr, "usage: %s [options] <input-gtl> <output.op2>\n", argv0);
    fprintf(stderr, "\n");
    fprintf(stderr, "  Converts a Miles/AIL Global Timbre Library (FAT.OPL, FAT.AD) to a DMX GENMIDI FM bank\n");
    fprintf(stderr, "  that WildMIDI can load with -c / WildMidi_Init().\n");
    fprintf(stderr, "\n");
    fprintf(stderr, "  --melodic-bank N      GTL bank holding GM programs 0-127   (default %d)\n", DEFAULT_MELODIC_BANK);
    fprintf(stderr, "  --percussion-bank N   GTL bank holding percussion keys     (default %d)\n", DEFAULT_PERCUSSION_BANK);
    fprintf(stderr, "  --single              emit only voice 1 of two-voice timbres (no DOUBLE flag)\n");
    fprintf(stderr, "  -v, --verbose         list every timbre as it is converted\n");
}

int main(int argc, char ** argv)
{
    const char * input_path = NULL;
    const char * output_path = NULL;
    unsigned melodic_bank = DEFAULT_MELODIC_BANK;
    unsigned percussion_bank = DEFAULT_PERCUSSION_BANK;
    int force_single = 0;
    int verbose = 0;
    int arg_index;

    uint8_t * gtl;
    size_t gtl_size;
    uint8_t * bank_image;
    FILE * output_file;

    int melodic_found = 0;
    int melodic_missing = 0;
    int percussion_found = 0;
    int percussion_missing = 0;
    int double_voice_count = 0;
    int dropped_high_keys = 0;
    int record_index;
    size_t image_size;
    size_t bytes_written;

    for(arg_index = 1; arg_index < argc; arg_index++)
    {
        const char * arg = argv[arg_index];

        if(strcmp(arg, "--single") == 0)
        {
            force_single = 1;
        }
        else if(strcmp(arg, "-v") == 0 || strcmp(arg, "--verbose") == 0)
        {
            verbose = 1;
        }
        else if(strcmp(arg, "--melodic-bank") == 0 || strcmp(arg, "--percussion-bank") == 0)
        {
            long value;
            char * end_ptr = NULL;

            arg_index++;
            if(arg_index >= argc)
            {
                fprintf(stderr, "gtl2op2: %s requires a bank number\n", arg);
                return 2;
            }
            value = strtol(argv[arg_index], &end_ptr, 10);
            if(end_ptr == argv[arg_index] || *end_ptr != '\0' || value < 0 || value > 255)
            {
                fprintf(stderr, "gtl2op2: '%s' is not a bank number in 0..255\n", argv[arg_index]);
                return 2;
            }
            if(strcmp(arg, "--melodic-bank") == 0)
            {
                melodic_bank = (unsigned)value;
            }
            else
            {
                percussion_bank = (unsigned)value;
            }
        }
        else if(arg[0] == '-' && arg[1] != '\0')
        {
            fprintf(stderr, "gtl2op2: unknown option '%s'\n", arg);
            Print_Usage(argv[0]);
            return 2;
        }
        else if(input_path == NULL)
        {
            input_path = arg;
        }
        else if(output_path == NULL)
        {
            output_path = arg;
        }
        else
        {
            fprintf(stderr, "gtl2op2: unexpected extra argument '%s'\n", arg);
            Print_Usage(argv[0]);
            return 2;
        }
    }

    if(input_path == NULL || output_path == NULL)
    {
        Print_Usage(argv[0]);
        return 2;
    }

    gtl = Read_Whole_File(input_path, &gtl_size);
    if(gtl == NULL)
    {
        return 1;
    }

    image_size = (size_t)OP2_HDRSIZE + ((size_t)OP2_RECORDS * OP2_RECSIZE) + ((size_t)OP2_RECORDS * OP2_NAMESIZE);
    bank_image = (uint8_t *)calloc(1, image_size);
    if(bank_image == NULL)
    {
        fprintf(stderr, "gtl2op2: out of memory allocating %lu-byte bank image\n", (unsigned long)image_size);
        free(gtl);
        return 1;
    }

    memcpy(bank_image, OP2_MAGIC, OP2_HDRSIZE);

    /* --- melodic: GTL bank `melodic_bank` patches 0..127 -> records 0..127 --- */
    for(record_index = 0; record_index < OP2_MELODIC_RECORDS; record_index++)
    {
        const uint8_t * payload;
        size_t payload_length = 0;
        uint8_t * record = bank_image + OP2_HDRSIZE + ((size_t)record_index * OP2_RECSIZE);

        payload = GTL_Find_Timbre(gtl, gtl_size, melodic_bank, (unsigned)record_index, &payload_length);
        if(payload == NULL || payload_length < GTL_PAYLOAD_ONE_VOICE)
        {
            if(payload != NULL)
            {
                fprintf(stderr, "gtl2op2: bank %u patch %d has a %lu-byte payload, too short for a voice — skipped\n", melodic_bank, record_index, (unsigned long)payload_length);
            }
            melodic_missing++;
            continue;
        }

        if(Build_Record(payload, payload_length, force_single, 0, record) == 2)
        {
            double_voice_count++;
        }
        melodic_found++;

        if(verbose)
        {
            printf("melodic  record %3d  <- bank %u patch %3d  (%lu-byte payload)\n", record_index, melodic_bank, record_index, (unsigned long)payload_length);
        }
    }

    /* --- percussion: GTL bank `percussion_bank` keys 35..81 -> records 128..174 --- */
    {
        int key;
        for(key = OP2_PERC_KEY_LOW; key <= OP2_PERC_KEY_HIGH; key++)
        {
            const uint8_t * payload;
            size_t payload_length = 0;
            int target_record = OP2_PERCUSSION_FIRST + (key - OP2_PERC_KEY_LOW);
            uint8_t * record = bank_image + OP2_HDRSIZE + ((size_t)target_record * OP2_RECSIZE);

            payload = GTL_Find_Timbre(gtl, gtl_size, percussion_bank, (unsigned)key, &payload_length);
            if(payload == NULL || payload_length < GTL_PAYLOAD_ONE_VOICE)
            {
                percussion_missing++;
                continue;
            }

            if(Build_Record(payload, payload_length, force_single, (uint8_t)key, record) == 2)
            {
                double_voice_count++;
            }
            percussion_found++;

            if(verbose)
            {
                printf("percuss  record %3d  <- bank %u patch %3d  (key %d)\n", target_record, percussion_bank, key, key);
            }
        }
    }

    /* --- report percussion timbres above key 81, which GENMIDI cannot hold --- */
    {
        size_t index_offset;
        for(index_offset = 0; index_offset + GTL_INDEX_RECORD_SIZE <= gtl_size; index_offset += GTL_INDEX_RECORD_SIZE)
        {
            uint8_t record_patch = gtl[index_offset + 0];
            uint8_t record_bank  = gtl[index_offset + 1];

            if(record_bank == GTL_BANK_TERMINATOR)
            {
                break;
            }
            if(record_bank == (uint8_t)percussion_bank && record_patch > OP2_PERC_KEY_HIGH)
            {
                dropped_high_keys++;
            }
        }
    }

    output_file = fopen(output_path, "wb");
    if(output_file == NULL)
    {
        fprintf(stderr, "gtl2op2: cannot open '%s' for writing (does the output directory exist?)\n", output_path);
        free(bank_image);
        free(gtl);
        return 1;
    }

    bytes_written = fwrite(bank_image, 1, image_size, output_file);
    if(fclose(output_file) != 0 || bytes_written != image_size)
    {
        fprintf(stderr, "gtl2op2: failed writing '%s' (wrote %lu of %lu bytes)\n", output_path, (unsigned long)bytes_written, (unsigned long)image_size);
        free(bank_image);
        free(gtl);
        return 1;
    }

    printf("wrote %s  (%lu bytes)\n", output_path, (unsigned long)image_size);
    printf("  melodic    : %d converted, %d missing   (bank %u, GM programs 0-127)\n", melodic_found, melodic_missing, melodic_bank);
    printf("  percussion : %d converted, %d missing   (bank %u, keys %d-%d)\n", percussion_found, percussion_missing, percussion_bank, OP2_PERC_KEY_LOW, OP2_PERC_KEY_HIGH);
    printf("  two-voice  : %d records%s\n", double_voice_count, force_single ? " (--single: forced to one voice)" : " flagged DOUBLE");

    if(dropped_high_keys > 0)
    {
        printf("  DROPPED    : %d percussion timbre(s) above key %d — GENMIDI has no slot for them\n", dropped_high_keys, OP2_PERC_KEY_HIGH);
    }
    if(melodic_found == 0 && percussion_found == 0)
    {
        fprintf(stderr, "gtl2op2: nothing was converted — are --melodic-bank / --percussion-bank right for this file?\n");
        free(bank_image);
        free(gtl);
        return 1;
    }

    free(bank_image);
    free(gtl);
    return 0;
}
