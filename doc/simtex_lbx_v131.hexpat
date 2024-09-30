#pragma author JimBalcomb
#pragma description SimTex LBX game resource archive file

#pragma MIME application/x-lbx

struct LBX_Header {
    s16 entries;
    u16 magic_number;
    s16 file_type;
    s16 dummy;
    u32 offsets[134];
};

LBX_Header lbx_header_at_0x00 @ 0x00;

struct LBX_DATA_Header {
    s16 num_recs;
    s16 rec_size;
};


LBX_DATA_Header lbx_data_header_at_0x220 @ 0x220;

struct SPELL_DATA
{
    char name[19];
    s8 AI_Group;
    s8 AI_Value;
    s8 type;
    s8 spell_book_category;
    s8 magic_realm;
    s8 Eligibility;
    s8 align2B_19h;
    s16 Casting_Cost;
    s16 research_cost;
    s8 Sound;
    s8 align2B_1Fh;
    s8 Param0;
    s8 Param1;
    s16 Params2_3;
};

struct SPELL_DATA_TABLE
{
    SPELL_DATA spell_data_table[215];
};

SPELL_DATA_TABLE spell_data_table_at_0x224 @ 0x224;