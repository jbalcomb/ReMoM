
#include "MOM_DEF.H"


//dseg:87C4 00 00 00 00 00 00 00 00 00 00+Game GAME_DATA <0>
//dseg:87D4 00 00 00 00 00 00 00 00 00 00+Settings SETTINGS <0>

SettingsStruct Settings;


unsigned int g_MemFreeWorst_KB = 64000;         // dseg:3CC2  0xFA00
char *cnst_Quit_Report1 = "Data";               // dseg:3CC4
char *cnst_Quit_Report2 = " Free: ";            // dseg:3CC9
char *cnst_Quit_Report3 = " bytes   Memory";    // dseg:3CD1
char *cnst_Quit_Report4 = "k   Worst";          // dseg:3CE1
char *cnst_Quit_Report5 = "k   EMM: ";          // dseg:3CEB
char *cnst_Quit_Report6 = " blocks";            // dseg:3CF5
