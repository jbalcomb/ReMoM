#ifndef GD_CITY_FIELDS_H
#define GD_CITY_FIELDS_H
/* generated from s_CITY by gen-fieldmap.py -- do not edit by hand */
static const gd_field_t city_fields[] = {
/*    offset    kind      count   name */
    { 0x0000, GD_STR  ,  14, "name" },
    { 0x000E, GD_I8   ,   1, "race" },
    { 0x000F, GD_I8   ,   1, "wx" },
    { 0x0010, GD_I8   ,   1, "wy" },
    { 0x0011, GD_I8   ,   1, "wp" },
    { 0x0012, GD_I8   ,   1, "owner_idx" },
    { 0x0013, GD_I8   ,   1, "size" },
    { 0x0014, GD_I8   ,   1, "population" },
    { 0x0015, GD_I8   ,   1, "farmer_count" },
    { 0x0016, GD_I8   ,   1, "sold_building" },
    { 0x0017, GD_I8   ,   1, "pad2B_17h" },
    { 0x0018, GD_I16  ,   1, "Pop_10s" },
    { 0x001A, GD_U8   ,   1, "contacts" },
    { 0x001B, GD_I8   ,   1, "pad2B_1Bh" },
    { 0x001C, GD_I16  ,   1, "construction" },
    { 0x001E, GD_I8   ,   1, "bldg_cnt" },
    { 0x001F, GD_I8   ,  36, "bldg_status" },
    { 0x0043, GD_U8   ,  26, "enchantments" },
    { 0x005D, GD_I8   ,   1, "production_units" },
    { 0x005E, GD_I16  ,   1, "Prod_Accu" },
    { 0x0060, GD_U8   ,   1, "gold_units" },
    { 0x0061, GD_I8   ,   1, "building_maintenance" },
    { 0x0062, GD_U8   ,   1, "mana_units" },
    { 0x0063, GD_I8   ,   1, "research_units" },
    { 0x0064, GD_I8   ,   1, "food_units" },
    { 0x0065, GD_I8   ,  13, "road_connections" },
};
#define CITY_FIELD_COUNT 26
#endif /* GD_CITY_FIELDS_H */
