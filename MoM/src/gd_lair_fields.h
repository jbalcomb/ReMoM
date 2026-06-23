#ifndef GD_LAIR_FIELDS_H
#define GD_LAIR_FIELDS_H
/* generated from s_LAIR by gen-fieldmap.py -- do not edit by hand */
static const gd_field_t lair_fields[] = {
/*    offset    kind      count   name */
    { 0x0000, GD_I8   ,   1, "wx" },
    { 0x0001, GD_I8   ,   1, "wy" },
    { 0x0002, GD_I8   ,   1, "wp" },
    { 0x0003, GD_I8   ,   1, "intact" },
    { 0x0004, GD_I8   ,   1, "type" },
    { 0x0005, GD_U8   ,   1, "guard1_unit_type" },
    { 0x0006, GD_U8   ,   1, "guard1_count" },
    { 0x0007, GD_U8   ,   1, "guard2_unit_type" },
    { 0x0008, GD_U8   ,   1, "guard2_count" },
    { 0x0009, GD_I8   ,   1, "pad2B_09h" },
    { 0x000A, GD_I16  ,   1, "Loot_Gold" },
    { 0x000C, GD_I16  ,   1, "Loot_Mana" },
    { 0x000E, GD_I8   ,   1, "Spell_n_Special" },
    { 0x000F, GD_U8   ,   1, "Misc_Flags" },
    { 0x0010, GD_I8   ,   1, "Item_Count" },
    { 0x0011, GD_I8   ,   1, "pad2B_11h" },
    { 0x0012, GD_I16  ,   3, "Item_Values" },
};
#define LAIR_FIELD_COUNT 17
#endif /* GD_LAIR_FIELDS_H */
