
#include "../../MoX/src/MOX_TYPE.h"

#include "RACETYPE.h"


// drake178: struct RACE_DATA 
// drake178: TBL_Races


char race_type_name_000[] = "Barbarian";
char race_type_name_001[] = "Beastmen";
char race_type_name_002[] = "Dark Elf";
char race_type_name_003[] = "Draconian";
char race_type_name_004[] = "Dwarven";
char race_type_name_005[] = "Gnoll";
char race_type_name_006[] = "Halfling";
char race_type_name_007[] = "High Elf";
char race_type_name_008[] = "High Men";
char race_type_name_009[] = "Klackon";
char race_type_name_010[] = "Lizardman";
char race_type_name_011[] = "Nomad";
char race_type_name_012[] = "Orc";
char race_type_name_013[] = "Troll";
char race_type_name_014[] = "";
char race_type_name_015[] = "Arcane";
char race_type_name_016[] = "Nature";
char race_type_name_017[] = "Sorcery";
char race_type_name_018[] = "Chaos";
char race_type_name_019[] = "Life";
char race_type_name_020[] = "Death";


char * _race_type_names[21] =
{
    race_type_name_000,
    race_type_name_001,
    race_type_name_002,
    race_type_name_003,
    race_type_name_004,
    race_type_name_005,
    race_type_name_006,
    race_type_name_007,
    race_type_name_008,
    race_type_name_009,
    race_type_name_010,
    race_type_name_011,
    race_type_name_012,
    race_type_name_013,
    race_type_name_014,
    race_type_name_015,
    race_type_name_016,
    race_type_name_017,
    race_type_name_018,
    race_type_name_019,
    race_type_name_020
};

// sizeof:  18h  24d
struct s_RACE_TYPE _race_type_table[14] =
{
    /*                D9 21   03 00  14 00  0A 00  19 00  00 00  00 00  00 00  00 00  0F 00  02 00  00 00 */
    { &_race_type_names[ 0],      3,    { 20,    10,    25,     0,     0,     0,     0 },    15,     2,     0 },
    /*                81 26   02 00  0B 00  0D 00  00 00  00 00  00 00  00 00  00 00  05 00  00 00  00 00 */
    { &_race_type_names[ 1],      2,    { 11,    13,     0,     0,     0,     0,     0 } ,     5,     0,     0 },
    /*                8A 26   02 00  19 00  0E 00  00 00  00 00  00 00  00 00  00 00  02 00  FE FF  01 00 */
    { &_race_type_names[ 2],      2,    { 25,    14,     0,     0,     0,     0,     0 },     2,    -2,     1 },
    /*                62 22   03 00  21 00  0B 00  0E 00  00 00  00 00  00 00  00 00  05 00  FF FF  00 00 */
    { &_race_type_names[ 3],      3,    { 33,    11,    14,     0,     0,     0,     0 },     5,    -1,     0 },
    /*                93 26   04 00  09 00  0D 00  14 00  18 00  00 00  00 00  00 00  07 00  FE FF  00 00 */
    { &_race_type_names[ 4],      4,    {  9,    13,    20,    24,     0,     0,     0 },     7,    -2,     0 },
    /*                9B 26   05 00  0A 00  11 00  18 00  06 00  0E 00  00 00  00 00  05 00  FF FF  02 00 */
    { &_race_type_names[ 5],      5,    { 10,    17,    24,     6,    14,     0,     0 },     5,    -1,     2 },
    /*                A1 26   04 00  14 00  06 00  09 00  0D 00  00 00  00 00  00 00  0F 00  00 00  00 00 */
    { &_race_type_names[ 6],      4,    { 20,     6,     9,    13,     0,     0,     0 },    15,     0,     0 },
    /*                AA 26   02 00  18 00  0E 00  00 00  00 00  00 00  00 00  00 00  05 00  FE FF  01 00 */
    { &_race_type_names[ 7],      2,    { 24,    14,     0,     0,     0,     0,     0 },     5,    -2,     1 },
    /*                B3 26   01 00  0B 00  00 00  00 00  00 00  00 00  00 00  00 00  0A 00  00 00  00 00 */
    { &_race_type_names[ 8],      1,    { 11,     0,     0,     0,     0,     0,     0 },    10,     0,     0 },
    /*                BC 26   03 00  17 00  11 00  0D 00  00 00  00 00  00 00  00 00  05 00  FF FF  02 00 */
    { &_race_type_names[ 9],      3,    { 23,    11,    13,     0,     0,     0,     0 },     5,    -1,     2 },
    /*                C4 26   07 00  07 00  0A 00  11 00  18 00  22 00  0C 00  0F 00  0A 00  01 00  02 00 */
    { &_race_type_names[10],      7,    {  7,    10,    17,    18,    34,    12,    15 },    10,     1,     2 },
    /*                CE 26   02 00  0E 00  15 00  00 00  00 00  00 00  00 00  00 00  0A 00  FF FF  00 00 */
    { &_race_type_names[11],      2,    { 14,    21,     0,     0,     0,     0,     0 },    10,    -1,     0 },
    /*                D4 26   00 00  00 00  00 00  00 00  00 00  00 00  00 00  00 00  0A 00  00 00  00 00 */
    { &_race_type_names[12],      0,    {  0,     0,     0,     0,     0,     0,     0 },    10,     0,     0 },
    /*                D8 26   04 00  0B 00  11 00  22 00  0D 00  00 00  00 00  00 00  03 00  FE FF  02 00 */
    { &_race_type_names[13],      4,    { 11,    17,    34,    13,     0,     0,     0 },     3,    -2,     1 }
};



// dseg:2089 00                                              cnst_ZeroString_3 db 0                  ; DATA XREF: _main+226o ...

// dseg:21D9 42 61 72 62 61 72 69 61 6E 00                   cnst_Barbarian db 'Barbarian',0         ; DATA XREF: dseg:_unit_type_tableo ...

// dseg:2262 44 72 61 63 6F 6E 69 61 6E 00                   cnst_Draconian db 'Draconian',0         ; DATA XREF: dseg:_unit_type_tableo ...

// dseg:2681 42 65 61 73 74 6D 65 6E 00                      cnst_Beastmen db 'Beastmen',0           ; DATA XREF: dseg:race_tableo
// dseg:268A 44 61 72 6B 20 45 6C 66 00                      cnst_DarkElf db 'Dark Elf',0            ; DATA XREF: dseg:race_tableo
// dseg:2693 44 77 61 72 76 65 6E 00                         cnst_Dwarven db 'Dwarven',0             ; DATA XREF: dseg:race_tableo
// dseg:269B 47 6E 6F 6C 6C 00                               cnst_Gnoll db 'Gnoll',0                 ; DATA XREF: dseg:race_tableo
// dseg:26A1 48 61 6C 66 6C 69 6E 67 00                      cnst_Halfling db 'Halfling',0           ; DATA XREF: dseg:race_tableo
// dseg:26AA 48 69 67 68 20 45 6C 66 00                      cnst_HighElf db 'High Elf',0            ; DATA XREF: dseg:race_tableo
// dseg:26B3 48 69 67 68 20 4D 65 6E 00                      cnst_HighMen db 'High Men',0            ; DATA XREF: dseg:race_tableo
// dseg:26BC 4B 6C 61 63 6B 6F 6E 00                         cnst_Klackon db 'Klackon',0             ; DATA XREF: dseg:race_tableo
// dseg:26C4 4C 69 7A 61 72 64 6D 61 6E 00                   cnst_Lizardman db 'Lizardman',0         ; DATA XREF: dseg:race_tableo
// dseg:26CE 4E 6F 6D 61 64 00                               cnst_Nomad db 'Nomad',0                 ; DATA XREF: dseg:race_tableo
// dseg:26D4 4F 72 63 00                                     cnst_Orc db 'Orc',0                     ; DATA XREF: dseg:race_tableo
// dseg:26D8 54 72 6F 6C 6C 00                               cnst_Troll db 'Troll',0                 ; DATA XREF: dseg:race_tableo

// dseg:26DE 41 72 63 61 6E 65 00                            cnst_Rlm5_Arcane db 'Arcane',0          ; DATA XREF: dseg:race_table_15o
// dseg:26E5 4E 61 74 75 72 65 00                            cnst_Rlm0_Nature db 'Nature',0          ; DATA XREF: dseg:rtn2o
// dseg:26EC 53 6F 72 63 65 72 79 00                         cnst_Rlm1_Sorcery db 'Sorcery',0        ; DATA XREF: dseg:rtn3o
// dseg:26F4 43 68 61 6F 73 00                               cnst_Rlm2_Chaos db 'Chaos',0            ; DATA XREF: dseg:rtn4o
// dseg:26FA 44 65 61 74 68 00                               cnst_Rlm4_Death db 'Death',0            ; DATA XREF: dseg:rtn6o






// D9 21 03 00 14 00 0A 00 19 00 00 00 00 00 00 00 00 00 0F 00 02 00 00 00 
// 81 26 02 00 0B 00 0D 00 00 00 00 00 00 00 00 00 00 00 05 00 00 00 00 00
// 8A 26 02 00 19 00 0E 00 00 00 00 00 00 00 00 00 00 00 02 00 FE FF 01 00
// 62 22 03 00 21 00 0B 00 0E 00 00 00 00 00 00 00 00 00 05 00 FF FF 00 00
// 93 26 04 00 09 00 0D 00 14 00 18 00 00 00 00 00 00 00 07 00 FE FF 00 00
// 9B 26 05 00 0A 00 11 00 18 00 06 00 0E 00 00 00 00 00 05 00 FF FF 02 00
// A1 26 04 00 14 00 06 00 09 00 0D 00 00 00 00 00 00 00 0F 00 00 00 00 00
// AA 26 02 00 18 00 0E 00 00 00 00 00 00 00 00 00 00 00 05 00 FE FF 01 00
// B3 26 01 00 0B 00 00 00 00 00 00 00 00 00 00 00 00 00 0A 00 00 00 00 00
// BC 26 03 00 17 00 11 00 0D 00 00 00 00 00 00 00 00 00 05 00 FF FF 02 00
// C4 26 07 00 07 00 0A 00 11 00 18 00 22 00 0C 00 0F 00 0A 00 01 00 02 00
// CE 26 02 00 0E 00 15 00 00 00 00 00 00 00 00 00 00 00 0A 00 FF FF 00 00
// D4 26 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 0A 00 00 00 00 00
// D8 26 04 00 0B 00 11 00 22 00 0D 00 00 00 00 00 00 00 03 00 FE FF 02 00


// WZD dseg:1ED6 89 20                                           STR_USW_NoRace@ dw offset cnst_ZeroString_3
// WZD dseg:1ED8 DE 26 E5 26 EC 26 F4 26 9C 21 FA 26             STR_USW_MagicRaces@ dw offset cnst_Rlm5_Arcane, offset cnst_Rlm0_Nature, offset cnst_Rlm1_Sorcery, offset cnst_Rlm2_Chaos, offset cnst_Rlm3_Life, offset cnst_Rlm4_Death
// WZD dseg:1ED8                                                                                         ; DATA XREF: USW_DrawView+453t ...
// WZD dseg:1ED8                                                                                         ; "Life"
