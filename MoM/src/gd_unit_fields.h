#ifndef GD_UNIT_FIELDS_H
#define GD_UNIT_FIELDS_H
/* generated from s_UNIT by gen-fieldmap.py -- do not edit by hand */
static const gd_field_t unit_fields[] = {
/*    offset    kind      count   name */
    { 0x0000, GD_I8   ,   1, "wx" },
    { 0x0001, GD_I8   ,   1, "wy" },
    { 0x0002, GD_I8   ,   1, "wp" },
    { 0x0003, GD_I8   ,   1, "owner_idx" },
    { 0x0004, GD_I8   ,   1, "moves2_max" },
    { 0x0005, GD_U8   ,   1, "type" },
    { 0x0006, GD_I8   ,   1, "Hero_Slot" },
    { 0x0007, GD_I8   ,   1, "Finished" },
    { 0x0008, GD_I8   ,   1, "moves2" },
    { 0x0009, GD_I8   ,   1, "dst_wx" },
    { 0x000A, GD_I8   ,   1, "dst_wy" },
    { 0x000B, GD_I8   ,   1, "Status" },
    { 0x000C, GD_I8   ,   1, "Level" },
    { 0x000D, GD_U8   ,   1, "pad2B_0Dh" },
    { 0x000E, GD_I16  ,   1, "XP" },
    { 0x0010, GD_I8   ,   1, "Move_Failed" },
    { 0x0011, GD_I8   ,   1, "Damage" },
    { 0x0012, GD_I8   ,   1, "Draw_Priority" },
    { 0x0013, GD_U8   ,   1, "pad2B_13h" },
    { 0x0014, GD_I16  ,   1, "in_tower" },
    { 0x0016, GD_I8   ,   1, "Sight_Range" },
    { 0x0017, GD_I8   ,   1, "mutations" },
    { 0x0018, GD_U32  ,   1, "enchantments" },
    { 0x001C, GD_I8   ,   1, "Rd_Constr_Left" },
    { 0x001D, GD_I8   ,   1, "Rd_From_X" },
    { 0x001E, GD_I8   ,   1, "Rd_From_Y" },
    { 0x001F, GD_U8   ,   1, "pad2B_1Fh" },
};
#define UNIT_FIELD_COUNT 27
#endif /* GD_UNIT_FIELDS_H */
