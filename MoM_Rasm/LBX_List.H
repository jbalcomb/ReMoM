
#include <STRING.H>

#include "STU_DBG.H"


char LBX_Name[101][13];

int LBX_File_Count = 101;

#define HASH_TABLE_SIZE 101
int hashViaAscii(const char * S)
{
    int hashVal = 0;
    int k;
    int S_length;

    S_length = strlen(S);

    for (k = 0; k < S_length; k++)
    {
        hashVal += S[k];  // S[k] returns ASCII value of the kth character of String S
    }

    return (hashVal % HASH_TABLE_SIZE);
}

  /* treat strings as base-256 integers */
  /* with digits in the range 1 to 255 */
  #define BASE (256)
unsigned long hash(const char *s, unsigned long m)
{
    unsigned long h;
    unsigned const char *us;

    /* cast s to (unsigned const char *) */
    /* this ensures that elements of s will be treated as having values >= 0 */
    us = (unsigned const char *) s;

    h = 0;
    while(*us != '\0') {
        h = (h * BASE + *us) % m;
        us++;
    } 

    return h;
}

/*
    e.g.,   "MAINSCRN"
            'M' + 'A' + 'I' + 'N' + 'S' + 'C' + 'R' + 'N'
M : 77
A : 65
I : 73
N : 78
S : 83
C : 67
R : 82
N : 78
LBX_Name_Index: 603
*/
int Make_LBX_Name_Index(const char * LBX_Name)
{
    unsigned const char * rvr_LBX_Name;
    int LBX_Name_Index;

    rvr_LBX_Name = (unsigned const char *) LBX_Name;

    LBX_Name_Index = 0;
    while(*rvr_LBX_Name != '\0')
    {
        dlvfprintf("%c : %u\n", *rvr_LBX_Name, *rvr_LBX_Name);
        LBX_Name_Index = ( LBX_Name_Index + *rvr_LBX_Name);
        rvr_LBX_Name++;
    }

    return LBX_Name_Index;
}

int Get_LBX_Name_Index(const char * LBX_Name)
{
    int LBX_Name_Index;

    if ( strcmp(LBX_Name, "ARMYLIST") ) { LBX_Name_Index = 0; }
    if ( strcmp(LBX_Name, "BACKGRND") ) { LBX_Name_Index = 1; }
    if ( strcmp(LBX_Name, "BOOK") ) { LBX_Name_Index = 2; }
    if ( strcmp(LBX_Name, "BUILDDAT") ) { LBX_Name_Index = 3; }
    if ( strcmp(LBX_Name, "BUILDESC") ) { LBX_Name_Index = 4; }
    if ( strcmp(LBX_Name, "CHRIVER") ) { LBX_Name_Index = 5; }
    if ( strcmp(LBX_Name, "CITYNAME") ) { LBX_Name_Index = 6; }
    if ( strcmp(LBX_Name, "CITYSCAP") ) { LBX_Name_Index = 7; }
    if ( strcmp(LBX_Name, "CITYWALL") ) { LBX_Name_Index = 8; }
    if ( strcmp(LBX_Name, "CMBDESRC") ) { LBX_Name_Index = 9; }
    if ( strcmp(LBX_Name, "CMBDESRT") ) { LBX_Name_Index = 10; }
    if ( strcmp(LBX_Name, "CMBGRASC") ) { LBX_Name_Index = 11; }
    if ( strcmp(LBX_Name, "CMBGRASS") ) { LBX_Name_Index = 12; }
    if ( strcmp(LBX_Name, "CMBMAGIC") ) { LBX_Name_Index = 13; }
    if ( strcmp(LBX_Name, "CMBMOUNC") ) { LBX_Name_Index = 14; }
    if ( strcmp(LBX_Name, "CMBMOUNT") ) { LBX_Name_Index = 15; }
    if ( strcmp(LBX_Name, "CMBTCITY") ) { LBX_Name_Index = 16; }
    if ( strcmp(LBX_Name, "CMBTFX") ) { LBX_Name_Index = 17; }
    if ( strcmp(LBX_Name, "CMBTSND") ) { LBX_Name_Index = 18; }
    if ( strcmp(LBX_Name, "CMBTUNDC") ) { LBX_Name_Index = 19; }
    if ( strcmp(LBX_Name, "CMBTUNDR") ) { LBX_Name_Index = 20; }
    if ( strcmp(LBX_Name, "CMBTWALL") ) { LBX_Name_Index = 21; }
    if ( strcmp(LBX_Name, "COMBAT") ) { LBX_Name_Index = 22; }
    if ( strcmp(LBX_Name, "COMPIX") ) { LBX_Name_Index = 23; }
    if ( strcmp(LBX_Name, "CONQUEST") ) { LBX_Name_Index = 24; }
    if ( strcmp(LBX_Name, "DESC") ) { LBX_Name_Index = 25; }
    if ( strcmp(LBX_Name, "DESERT") ) { LBX_Name_Index = 26; }
    if ( strcmp(LBX_Name, "DIPLOMAC") ) { LBX_Name_Index = 27; }
    if ( strcmp(LBX_Name, "DIPLOMSG") ) { LBX_Name_Index = 28; }
    if ( strcmp(LBX_Name, "EVENTMSG") ) { LBX_Name_Index = 29; }
    if ( strcmp(LBX_Name, "EVENTS") ) { LBX_Name_Index = 30; }
    if ( strcmp(LBX_Name, "FIGURE10") ) { LBX_Name_Index = 31; }
    if ( strcmp(LBX_Name, "FIGURE11") ) { LBX_Name_Index = 32; }
    if ( strcmp(LBX_Name, "FIGURE12") ) { LBX_Name_Index = 33; }
    if ( strcmp(LBX_Name, "FIGURE13") ) { LBX_Name_Index = 34; }
    if ( strcmp(LBX_Name, "FIGURE14") ) { LBX_Name_Index = 35; }
    if ( strcmp(LBX_Name, "FIGURE15") ) { LBX_Name_Index = 36; }
    if ( strcmp(LBX_Name, "FIGURE16") ) { LBX_Name_Index = 37; }
    if ( strcmp(LBX_Name, "FIGURES1") ) { LBX_Name_Index = 38; }
    if ( strcmp(LBX_Name, "FIGURES2") ) { LBX_Name_Index = 39; }
    if ( strcmp(LBX_Name, "FIGURES3") ) { LBX_Name_Index = 40; }
    if ( strcmp(LBX_Name, "FIGURES4") ) { LBX_Name_Index = 41; }
    if ( strcmp(LBX_Name, "FIGURES5") ) { LBX_Name_Index = 42; }
    if ( strcmp(LBX_Name, "FIGURES6") ) { LBX_Name_Index = 43; }
    if ( strcmp(LBX_Name, "FIGURES7") ) { LBX_Name_Index = 44; }
    if ( strcmp(LBX_Name, "FIGURES8") ) { LBX_Name_Index = 45; }
    if ( strcmp(LBX_Name, "FIGURES9") ) { LBX_Name_Index = 46; }
    if ( strcmp(LBX_Name, "FONTS") ) { LBX_Name_Index = 47; }
    if ( strcmp(LBX_Name, "HALOFAM") ) { LBX_Name_Index = 48; }
    if ( strcmp(LBX_Name, "HELP") ) { LBX_Name_Index = 49; }
    if ( strcmp(LBX_Name, "HERODATA") ) { LBX_Name_Index = 50; }
    if ( strcmp(LBX_Name, "HIRE") ) { LBX_Name_Index = 51; }
    if ( strcmp(LBX_Name, "HLPENTRY") ) { LBX_Name_Index = 52; }
    if ( strcmp(LBX_Name, "INTRO") ) { LBX_Name_Index = 53; }
    if ( strcmp(LBX_Name, "INTROSFX") ) { LBX_Name_Index = 54; }
    if ( strcmp(LBX_Name, "INTROSND") ) { LBX_Name_Index = 55; }
    if ( strcmp(LBX_Name, "ITEMDATA") ) { LBX_Name_Index = 56; }
    if ( strcmp(LBX_Name, "ITEMISC") ) { LBX_Name_Index = 57; }
    if ( strcmp(LBX_Name, "ITEMPOW") ) { LBX_Name_Index = 58; }
    if ( strcmp(LBX_Name, "ITEMS") ) { LBX_Name_Index = 59; }
    if ( strcmp(LBX_Name, "LILWIZ") ) { LBX_Name_Index = 60; }
    if ( strcmp(LBX_Name, "LISTDAT") ) { LBX_Name_Index = 61; }
    if ( strcmp(LBX_Name, "LOAD") ) { LBX_Name_Index = 62; }
    if ( strcmp(LBX_Name, "LOSE") ) { LBX_Name_Index = 63; }
    if ( strcmp(LBX_Name, "MAGIC") ) { LBX_Name_Index = 64; }
    if ( strcmp(LBX_Name, "MAIN") ) { LBX_Name_Index = 65; }
    if ( strcmp(LBX_Name, "MAINSCRN") ) { LBX_Name_Index = 66; }
    if ( strcmp(LBX_Name, "MAPBACK") ) { LBX_Name_Index = 67; }
    if ( strcmp(LBX_Name, "MESSAGE") ) { LBX_Name_Index = 68; }
    if ( strcmp(LBX_Name, "MONSTER") ) { LBX_Name_Index = 69; }
    if ( strcmp(LBX_Name, "MOODWIZ") ) { LBX_Name_Index = 70; }
    if ( strcmp(LBX_Name, "MUSIC") ) { LBX_Name_Index = 71; }
    if ( strcmp(LBX_Name, "NAMES") ) { LBX_Name_Index = 72; }
    if ( strcmp(LBX_Name, "NEWGAME") ) { LBX_Name_Index = 73; }
    if ( strcmp(LBX_Name, "NEWSOUND") ) { LBX_Name_Index = 74; }
    if ( strcmp(LBX_Name, "PORTRAIT") ) { LBX_Name_Index = 75; }
    if ( strcmp(LBX_Name, "RELOAD") ) { LBX_Name_Index = 76; }
    if ( strcmp(LBX_Name, "RESOURCE") ) { LBX_Name_Index = 77; }
    if ( strcmp(LBX_Name, "SCROLL") ) { LBX_Name_Index = 78; }
    if ( strcmp(LBX_Name, "SNDDRV") ) { LBX_Name_Index = 79; }
    if ( strcmp(LBX_Name, "SOUNDFX") ) { LBX_Name_Index = 80; }
    if ( strcmp(LBX_Name, "SPECFX") ) { LBX_Name_Index = 81; }
    if ( strcmp(LBX_Name, "SPECIAL") ) { LBX_Name_Index = 82; }
    if ( strcmp(LBX_Name, "SPECIAL2") ) { LBX_Name_Index = 83; }
    if ( strcmp(LBX_Name, "SPELLDAT") ) { LBX_Name_Index = 84; }
    if ( strcmp(LBX_Name, "SPELLOSE") ) { LBX_Name_Index = 85; }
    if ( strcmp(LBX_Name, "SPELLS") ) { LBX_Name_Index = 86; }
    if ( strcmp(LBX_Name, "SPELLSCR") ) { LBX_Name_Index = 87; }
    if ( strcmp(LBX_Name, "SPLMASTR") ) { LBX_Name_Index = 88; }
    if ( strcmp(LBX_Name, "TERRAIN") ) { LBX_Name_Index = 89; }
    if ( strcmp(LBX_Name, "TERRSTAT") ) { LBX_Name_Index = 90; }
    if ( strcmp(LBX_Name, "TERRTYPE") ) { LBX_Name_Index = 91; }
    if ( strcmp(LBX_Name, "TUNDRA") ) { LBX_Name_Index = 92; }
    if ( strcmp(LBX_Name, "UNITS1") ) { LBX_Name_Index = 93; }
    if ( strcmp(LBX_Name, "UNITS2") ) { LBX_Name_Index = 94; }
    if ( strcmp(LBX_Name, "UNITVIEW") ) { LBX_Name_Index = 95; }
    if ( strcmp(LBX_Name, "VORTEX") ) { LBX_Name_Index = 96; }
    if ( strcmp(LBX_Name, "WALLRISE") ) { LBX_Name_Index = 97; }
    if ( strcmp(LBX_Name, "WIN") ) { LBX_Name_Index = 98; }
    if ( strcmp(LBX_Name, "WIZARDS") ) { LBX_Name_Index = 99; }
    if ( strcmp(LBX_Name, "WIZLAB") ) { LBX_Name_Index = 100; }

    return LBX_Name_Index;
}

void List_LBX_Name_Indices(void)
{
    int itr_LBX_Name;

    for ( itr_LBX_Name = 0; itr_LBX_Name < LBX_File_Count; itr_LBX_Name++ )
    {
        dlvfprintf("if ( strcmp(LBX_Name, \"%s\") ) { LBX_Name_Index = %d; }\n", LBX_Name[itr_LBX_Name], itr_LBX_Name);
    }
}

char LBX_Name[101][13] = 
{
    "ARMYLIST",
    "BACKGRND",
    "BOOK",
    "BUILDDAT",
    "BUILDESC",
    "CHRIVER",
    "CITYNAME",
    "CITYSCAP",
    "CITYWALL",
    "CMBDESRC",
    "CMBDESRT",
    "CMBGRASC",
    "CMBGRASS",
    "CMBMAGIC",
    "CMBMOUNC",
    "CMBMOUNT",
    "CMBTCITY",
    "CMBTFX",
    "CMBTSND",
    "CMBTUNDC",
    "CMBTUNDR",
    "CMBTWALL",
    "COMBAT",
    "COMPIX",
    "CONQUEST",
    "DESC",
    "DESERT",
    "DIPLOMAC",
    "DIPLOMSG",
    "EVENTMSG",
    "EVENTS",
    "FIGURE10",
    "FIGURE11",
    "FIGURE12",
    "FIGURE13",
    "FIGURE14",
    "FIGURE15",
    "FIGURE16",
    "FIGURES1",
    "FIGURES2",
    "FIGURES3",
    "FIGURES4",
    "FIGURES5",
    "FIGURES6",
    "FIGURES7",
    "FIGURES8",
    "FIGURES9",
    "FONTS",
    "HALOFAM",
    "HELP",
    "HERODATA",
    "HIRE",
    "HLPENTRY",
    "INTRO",
    "INTROSFX",
    "INTROSND",
    "ITEMDATA",
    "ITEMISC",
    "ITEMPOW",
    "ITEMS",
    "LILWIZ",
    "LISTDAT",
    "LOAD",
    "LOSE",
    "MAGIC",
    "MAIN",
    "MAINSCRN",
    "MAPBACK",
    "MESSAGE",
    "MONSTER",
    "MOODWIZ",
    "MUSIC",
    "NAMES",
    "NEWGAME",
    "NEWSOUND",
    "PORTRAIT",
    "RELOAD",
    "RESOURCE",
    "SCROLL",
    "SNDDRV",
    "SOUNDFX",
    "SPECFX",
    "SPECIAL",
    "SPECIAL2",
    "SPELLDAT",
    "SPELLOSE",
    "SPELLS",
    "SPELLSCR",
    "SPLMASTR",
    "TERRAIN",
    "TERRSTAT",
    "TERRTYPE",
    "TUNDRA",
    "UNITS1",
    "UNITS2",
    "UNITVIEW",
    "VORTEX",
    "WALLRISE",
    "WIN",
    "WIZARDS",
    "WIZLAB"
};

enum e_LBX_Number
{
    ARMYLIST,
    BACKGRND,
    BOOK,
    BUILDDAT,
    BUILDESC,
    CHRIVER,
    CITYNAME,
    CITYSCAP,
    CITYWALL,
    CMBDESRC,
    CMBDESRT,
    CMBGRASC,
    CMBGRASS,
    CMBMAGIC,
    CMBMOUNC,
    CMBMOUNT,
    CMBTCITY,
    CMBTFX,
    CMBTSND,
    CMBTUNDC,
    CMBTUNDR,
    CMBTWALL,
    COMBAT,
    COMPIX,
    CONQUEST,
    DESC,
    DESERT,
    DIPLOMAC,
    DIPLOMSG,
    EVENTMSG,
    EVENTS,
    FIGURE10,
    FIGURE11,
    FIGURE12,
    FIGURE13,
    FIGURE14,
    FIGURE15,
    FIGURE16,
    FIGURES1,
    FIGURES2,
    FIGURES3,
    FIGURES4,
    FIGURES5,
    FIGURES6,
    FIGURES7,
    FIGURES8,
    FIGURES9,
    FONTS,
    HALOFAM,
    HELP,
    HERODATA,
    HIRE,
    HLPENTRY,
    INTRO,
    INTROSFX,
    INTROSND,
    ITEMDATA,
    ITEMISC,
    ITEMPOW,
    ITEMS,
    LILWIZ,
    LISTDAT,
    LOAD,
    LOSE,
    MAGIC,
    MAIN,
    MAINSCRN,
    MAPBACK,
    MESSAGE,
    MONSTER,
    MOODWIZ,
    MUSIC,
    NAMES,
    NEWGAME,
    NEWSOUND,
    PORTRAIT,
    RELOAD,
    RESOURCE,
    SCROLL,
    SNDDRV,
    SOUNDFX,
    SPECFX,
    SPECIAL,
    SPECIAL2,
    SPELLDAT,
    SPELLOSE,
    SPELLS,
    SPELLSCR,
    SPLMASTR,
    TERRAIN,
    TERRSTAT,
    TERRTYPE,
    TUNDRA,
    UNITS1,
    UNITS2,
    UNITVIEW,
    VORTEX,
    WALLRISE,
    WIN,
    WIZARDS,
    WIZLAB
};

/*
ARMYLIST.LBX
BACKGRND.LBX
BOOK.LBX
BUILDDAT.LBX
BUILDESC.LBX
CHRIVER.LBX
CITYNAME.LBX
CITYSCAP.LBX
CITYWALL.LBX
CMBDESRC.LBX
CMBDESRT.LBX
CMBGRASC.LBX
CMBGRASS.LBX
CMBMAGIC.LBX
CMBMOUNC.LBX
CMBMOUNT.LBX
CMBTCITY.LBX
CMBTFX.LBX
CMBTSND.LBX
CMBTUNDC.LBX
CMBTUNDR.LBX
CMBTWALL.LBX
COMBAT.LBX
COMPIX.LBX
CONQUEST.LBX
DESC.LBX
DESERT.LBX
DIPLOMAC.LBX
DIPLOMSG.LBX
EVENTMSG.LBX
EVENTS.LBX
FIGURE10.LBX
FIGURE11.LBX
FIGURE12.LBX
FIGURE13.LBX
FIGURE14.LBX
FIGURE15.LBX
FIGURE16.LBX
FIGURES1.LBX
FIGURES2.LBX
FIGURES3.LBX
FIGURES4.LBX
FIGURES5.LBX
FIGURES6.LBX
FIGURES7.LBX
FIGURES8.LBX
FIGURES9.LBX
FONTS.LBX
HALOFAM.LBX
HELP.LBX
HERODATA.LBX
HIRE.LBX
HLPENTRY.LBX
INTRO.LBX
INTROSFX.LBX
INTROSND.LBX
ITEMDATA.LBX
ITEMISC.LBX
ITEMPOW.LBX
ITEMS.LBX
LILWIZ.LBX
LISTDAT.LBX
LOAD.LBX
LOSE.LBX
MAGIC.LBX
MAIN.LBX
MAINSCRN.LBX
MAPBACK.LBX
MESSAGE.LBX
MONSTER.LBX
MOODWIZ.LBX
MUSIC.LBX
NAMES.LBX
NEWGAME.LBX
NEWSOUND.LBX
PORTRAIT.LBX
RELOAD.LBX
RESOURCE.LBX
SCROLL.LBX
SNDDRV.LBX
SOUNDFX.LBX
SPECFX.LBX
SPECIAL.LBX
SPECIAL2.LBX
SPELLDAT.LBX
SPELLOSE.LBX
SPELLS.LBX
SPELLSCR.LBX
SPLMASTR.LBX
TERRAIN.LBX
TERRSTAT.LBX
TERRTYPE.LBX
TUNDRA.LBX
UNITS1.LBX
UNITS2.LBX
UNITVIEW.LBX
VORTEX.LBX
WALLRISE.LBX
WIN.LBX
WIZARDS.LBX
WIZLAB.LBX
*/
