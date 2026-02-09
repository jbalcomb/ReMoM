
/*

Debugging Getters and Setters for *Dynamic* Game-Data

_battleunits[]
_CITIES[]
_FORTRESSES[]
_HEROES2[]
_LAIRS[]
_players[]
_TOWERS[]
_UNITS[]

¿ Goals ?
...validate
...log
...profile / catalog / heuristics



jbalcomb@Velociraptor MINGW64 /c/STU/devel/ReMoM/src (current)
$ grep -I -E "_CITIES\[.*\]\.farmer_count += +" -r

grep -I -E "_CITIES\[.*\]\.farmer_count =" -r
...matches "=="
grep -I -E "_CITIES\[.*\]\.farmer_count =([^=]|$)" -r
grep -I -P "_CITIES\[.*\]\.farmer_count =(?!=)" -r
...equivalent, but PCRE doesn't require the EOL special case handling

What about zero or more spaces?
    Not possible. grep does not support variable length look-behinds.   [NOTE: this is refering to -G and -E]
        Note: The nonstandard -P option - for PCRE (Perl-Compatible Regular Expression) support - requires GNU grep.
    grep -oP 'mem\s*=\s*\K[0-9]+' file
    The \K simply drops everything matched so far, printing only the number.
    This alternative to a look-behind assertion (such as (?<=mem=)) bypasses the latter's limitation that Tomalak mentions: look-behind assertions must be fixed-length.
grep -I -P "_CITIES\[.*\]\.farmer_count\s*=\s*" -r
grep -I -P "_CITIES\[.*\]\.farmer_count\s*=\s*(?!=)" -r

grep -I -P "_CITIES\[.*\]\.farmer_count\s*=\s*(?!=)" -r
YAY  _CITIES[city_idx].farmer_count = min_farmer_count;
YAY  _CITIES[itr_cities].farmer_count = min_farmer_count;
YAY  _CITIES[itr].farmer_count = min_farmer_count;
YAY  _CITIES[_city_idx].farmer_count = min_farmer_count;
YAY  _CITIES[city_idx].farmer_count= min_farmer_count;
YAY  _CITIES[city_idx].farmer_count =min_farmer_count;
YAY  _CITIES[city_idx].farmer_count=min_farmer_count;


[x] grep -I -P "_CITIES\[.*\]\.farmer_count\s*=\s*(?!=)" -r
[ ] grep -I -P "_CITIES\[.*\]\.population\s*=\s*(?!=)" -r
[ ] grep -I -P "_CITIES\[.*\]\.size\s*=\s*(?!=)" -r
[ ] grep -I -P "_CITIES\[.*\]\.Pop_10s\s*=\s*(?!=)" -r
*/



/*
    BEGIN:  _CITIES[]
*/

#define CITIES_FARMER_COUNT(_city_idx_,_farmer_count_)  {       \
    do {                                                        \
        _CITIES[(_city_idx_)].farmer_count = (_farmer_count_);  \
    } while(0);                                                 \
}

#define CITIES_POPULATION(_city_idx_,_population_)  {           \
    do {                                                        \
        _CITIES[(_city_idx_)].population = (_population_);      \
    } while(0);                                                 \
}

#define CITIES_SIZE(_city_idx_,_size_)  {                       \
    do {                                                        \
        _CITIES[(_city_idx_)].size = (_size_);                  \
    } while(0);                                                 \
}

#define CITIES_POP_10S(_city_idx_,_Pop_10s_)  {                 \
    do {                                                        \
        _CITIES[(_city_idx_)].Pop_10s = (_Pop_10s_);            \
    } while(0);                                                 \
}

#define CITIES_FOOD_UNITS(_city_idx_,_food_units_)  {           \
    do {                                                        \
        _CITIES[(_city_idx_)].food_units = (_food_units_);      \
    } while(0);                                                 \
}

#define CITIES_PRODUCTION_UNITS(_city_idx_,_production_units_)  {       \
    do {                                                                \
        _CITIES[(_city_idx_)].production_units = (_production_units_);  \
    } while(0);                                                         \
}

#define CITIES_GOLD_UNITS(_city_idx_,_gold_units_)  {           \
    do {                                                        \
        _CITIES[(_city_idx_)].gold_units = (_gold_units_);      \
    } while(0);                                                 \
}

#define CITIES_BUILDING_MAINTENANCE(_city_idx_,_building_maintenance_)  {       \
    do {                                                                        \
        _CITIES[(_city_idx_)].building_maintenance = (_building_maintenance_);  \
    } while(0);                                                                 \
}

#define CITIES_RESEARCH_UNITS(_city_idx_,_research_units_)  {       \
    do {                                                            \
        _CITIES[(_city_idx_)].research_units = (_research_units_);  \
    } while(0);                                                     \
}

#define CITIES_MANA_UNITS(_city_idx_,_mana_units_)  {           \
    do {                                                        \
        _CITIES[(_city_idx_)].mana_units = (_mana_units_);      \
    } while(0);                                                 \
}

#define CITIES_SOLD_BUILDING(_city_idx_,_sold_building_)  {         \
    do {                                                            \
        _CITIES[(_city_idx_)].sold_building = (_sold_building_);    \
    } while(0);                                                     \
}

#define CITIES_CONSTRUCTION(_city_idx_,_construction_)  {       \
    do {                                                        \
        _CITIES[(_city_idx_)].construction = (_construction_);  \
    } while(0);                                                 \
}

/*
    END:  _CITIES[]
*/



/*
    BEGIN:  _UNITS[]
*/

#define UNITS_WX(_unit_idx_,_wx_)  {       \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_WX(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].wx, (_wx_)); */ \
        _UNITS[(_unit_idx_)].wx = (_wx_);  \
    } while(0);                                             \
}

#define UNITS_WY(_unit_idx_,_wy_)  {       \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_WY(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].wy, (_wy_)); */ \
        _UNITS[(_unit_idx_)].wy = (_wy_);  \
    } while(0);                                             \
}

#define UNITS_WP(_unit_idx_,_wp_)  {       \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_WP(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].wp, (_wp_)); */ \
        _UNITS[(_unit_idx_)].wp = (_wp_);  \
    } while(0);                                             \
}

#define UNITS_DRAW_PRIORITY(_unit_idx_,_priority_)  {       \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_DRAW_PRIORITY(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].Draw_Priority, (_priority_)); */ \
        _UNITS[(_unit_idx_)].Draw_Priority = (_priority_);  \
    } while(0);                                             \
}

#define UNITS_IN_TOWER(_unit_idx_,_in_tower_)  {            \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_IN_TOWER(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].in_tower, (_in_tower_)); */ \
        _UNITS[(_unit_idx_)].in_tower = (_in_tower_);       \
    } while(0);                                             \
}

#define UNITS_STATUS(_unit_idx_,_status_)  {                \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_STATUS(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].Status, (_status_)); */ \
        /* Validate_Units_Status() */                       \
        _UNITS[(_unit_idx_)].Status = (_status_);           \
    } while(0);                                             \
}

#define UNITS_FINISHED(_unit_idx_,_finished_)  {            \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_FINISHED(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].Finished, (_finished_)); */ \
        /* Validate_Units_Finished() */                     \
        _UNITS[(_unit_idx_)].Finished = (_finished_);       \
    } while(0);                                             \
}

#define UNITS_OWNER(_unit_idx_,_owner_idx_)  {            \
    do {                                                    \
        /* trc_prn("TRACE: [%s, %d]: UNITS_OWNER(): %d,%d\n", __FILE__, __LINE__, _UNITS[(_unit_idx_)].owner_idx, (_owner_idx_)); */ \
        /* Validate_Units_Owner() */                     \
        _UNITS[(_unit_idx_)].owner_idx = (_owner_idx_);       \
    } while(0);                                             \
}

/*
    END:  _UNITS[]
*/
