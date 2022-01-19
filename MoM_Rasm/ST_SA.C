
#include "ST_HEAD.H"

#include "ST_SA.H"


struct ST_Alloc_Space_Header_Struct LBX_Alloc_Space_Header;

/*
    Near Heap
*/
/*
char *cnst_Alloc_Error01 = "Near Allocation too large by ";                     // dseg:3D56
char *cnst_Alloc_Error02 = " bytes";                                            // dseg:3D74
char *cnst_Alloc_Error51 = "Insufficient memory. You need at least ";           // dseg:3D7B
char *cnst_Alloc_Error52 = "K free. Try removing all TSR's.";                   // dseg:3DA3
char *cnst_Alloc_Error11 = "Dynamic allocation too small for Allocate_Space()"; // dseg:3DC3
char *cnst_Alloc_Error12 = " of ";                                              // dseg:3DF5
char *cnst_Alloc_Error13 = " blocks";                                           // dseg:3DFA
char *cnst_Alloc_Error21 = "Failed to reload";                                  // dseg:3E02
char *cnst_Alloc_Error22 = " Allocate_Next_Block()";                            // dseg:3E13
char *cnst_Alloc_Error23 = ": Short by ";                                       // dseg:3E2A
char *cnst_Alloc_Error3 = "Allocation space has been corrupted for";            // dseg:3E36
char *cnst_Alloc_Error4 = " (EMM) ";                                            // dseg:3E5E
*/

char Temp_String[100];                                          // dseg:9490  Temp_String    db 64h dup(0)  ; 100 bytes ~= char TempString[100]
char Tmp_Conv_Str_1[20];                                        // dseg:94F4  Tmp_Conv_Str_1 db 14h dup(0)  ;  20 bytes ~= char Tmp_Conv_Str_1[20]
char Tmp_Conv_Str_2[30];                                        // dseg:9508  Tmp_Conv_Str_2 db 1Eh dup(0)  ;  30 bytes ~= char Tmp_Conv_Str_2[30]
char Tmp_Conv_Str_3[106];                                       // dseg:9526  Tmp_Conv_Str_3 db 6Ah dup(0)  ; 106 bytes ~= char Tmp_Conv_Str_3[106] ; ...maybe sizeof/max set at compile-time?

/*
    Far Heap
*/
void _FAR * gfp_tmpSAMB;                     // dseg:A5C0
unsigned int g_RAM_Min_KB;                  // dseg:A5C4
