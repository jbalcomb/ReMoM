
#include "ST_EMM.H"

// TODO(jbalcomb): move these scraps of notes
//memptr		bufferseg;

/*
https://fresh2refresh.com/c-programming/c-passing-struct-to-function/
struct student 
{
            int id;
            char name[20];
            float percentage;
};
struct student record; // Global declaration of structure
*/


//; seg012:0006  EMM_Logical_Page dw 0                   ; DATA XREF: EMM_MapnRead, EMM_MapnWrite
unsigned int EMM_Logical_Page;

unsigned int g_EMM_PageFrame_Base_Address;   // dseg:40E4
extern char g_EMM_device_name[9] = "EMMXXXX0";                      // dseg:40E6
//dseg:40EF EMM_Log2Phys_Map EMM_L2P_Map_Record <0, 0>
//dseg:40EF                  EMM_L2P_Map_Record <0, 1>
//dseg:40EF                  EMM_L2P_Map_Record <0, 2>
//dseg:40EF                  EMM_L2P_Map_Record <0, 3>
// align 2                                                      // dseg:40FF
unsigned int g_EMM_Pages_Reserved = 40;                          // dseg:4100 ; set to 158 at the start of _main
unsigned int g_EMM_Open_Handles = 0;                             // dseg:4102
char *g_EmmHndlNm_YOMOMA1 = "YO MOMA";                            // dseg:4104 "YO MOMA"
char *g_EmmHndlNm_YOMOMA2 = "YO MOMA";                            // dseg:410C "YO MOMA"
char *g_EmmHndlNm_VGAFILEH = "VGAFILEH";                          // dseg:4116 "VGAFILEH"
char *g_EmmHndlNm_EMMDATAH = "EMMDATAH";                          // dseg:411F "EMMDATAH"
char *cnst_LBX_Ext2 = ".LBX";                                   // dseg:4128
char *cnst_EmmErr_ResOut1 = "EMM reserved exceeded by ";        // dseg:412D
char *cnst_EmmErr_ResOut2 = " blocks [";                        // dseg:4147
char *cnst_EmmErr_ResOut3 = ".LBX]";                            // dseg:4151
char *cnst_EmmErr_Reload1 = " reloaded into EMM, diff size =";  // dseg:4157
char *cnst_EmmErr_Space = "  ";                                 // dseg:4177

unsigned int g_EMM_Overlay_Handle;                               // dseg:A5E2
unsigned int g_EMM_Data_Level;                                    // dseg:A5E4  set to 0 in EMM_Startup
unsigned int g_EmmHndlNbr_VGAFILEH;                               // dseg:A5EA  5 reserved pages
unsigned int g_EmmHndlNbr_EMMDATAH;                               // dseg:A5E6  4 reserved pages
unsigned int g_EmmHndlNbr_YOMOMA;                                 // dseg:A5EE  1 page
unsigned int g_EMM_OK;                                            // dseg:A5F0  set to 1 after creating the YO MOMA handle
//extern EMM_Table EMM_Record 28h dup(<0>)                      // dseg:A5F2  EMM_Table EMM_Record 28h dup(<0>)
EMM_Record g_EMM_Table[MOM_EMM_HANDLES]; // dseg:A5F2 g_EMS_Table EMS_Record 28h dup(<0>)
unsigned int g_EMM_MinKB = 0;                                     // dseg:A7D2  initialized to 0 set to 2700 in _main
