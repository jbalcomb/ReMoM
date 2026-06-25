#ifndef GD_ITEM_FIELDS_H
#define GD_ITEM_FIELDS_H
/* generated from s_ITEM by gen-fieldmap.py -- do not edit by hand */
static const gd_field_t item_fields[] = {
/*    offset    kind      count   name */
    { 0x0000, GD_STR  ,  30, "name" },
    { 0x001E, GD_I8   ,   1, "icon_idx" },
    { 0x001F, GD_I8   ,   1, "IDK_icon_idx" },
    { 0x0020, GD_I8   ,   1, "Slot_Type" },
    { 0x0021, GD_I8   ,   1, "type" },
    { 0x0022, GD_I16  ,   1, "cost" },
    { 0x0024, GD_I8   ,   1, "attack" },
    { 0x0025, GD_I8   ,   1, "tohit" },
    { 0x0026, GD_I8   ,   1, "defense" },
    { 0x0027, GD_I8   ,   1, "moves2" },
    { 0x0028, GD_I8   ,   1, "resistance" },
    { 0x0029, GD_I8   ,   1, "spell_skill" },
    { 0x002A, GD_I8   ,   1, "spell_save" },
    { 0x002B, GD_I8   ,   1, "embed_spell_idx" },
    { 0x002C, GD_I16  ,   1, "embed_spell_cnt" },
    { 0x002E, GD_U32  ,   1, "Powers" },
};
#define ITEM_FIELD_COUNT 16
#endif /* GD_ITEM_FIELDS_H */
