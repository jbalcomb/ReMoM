
#include "ST_LBX.H"


unsigned int g_LBX_Header_Allocd = 0;            // dseg:3E66
int g_LBX_FileHandle = 0xFFFF;                       // dseg:3E68
unsigned int UU_g_LBX_HdrFmt = 0;    // dseg:3E6A
char UU_g_LBX_FilePath[50] = {0};                 // dseg:3E6C
char g_LBX_FileExtension[] = ".LBX";             // dseg:3E9E

char *cnst_LBXErr_Common1 = ".LBX [entry ";                                         //dseg:3EA3
char *cnst_LBXErr_Common2 = "] ";                                                   //dseg:3EB0
char *cnst_LBX_Error1 = " could not be found.";                                     //dseg:3EB3
char *cnst_LBX_Error2 = " has been corrupted.";                                     //dseg:3EC8
char *cnst_LBX_Error31 = "Insufficient memory. You need at least ";                 //dseg:3EDD
char *cnst_LBX_Error32 = "K free. Try removing all TSR's.";                         //dseg:3F05
char *cnst_LBX_Error4 = " was not properly allocated or has been corrupted.";       //dseg:3F25
char *cnst_LBX_Error51 = " failed to reload. Allocation too small by ";             //dseg:3F58
char *cnst_LBX_Error52 = " pages";                                                  //dseg:3F84
char *cnst_LBX_Error7 = " is not an LBX file";                                      //dseg:3F8B
char *cnst_LBX_Error8 = " exceeds number of LBX entries";                           //dseg:3F9F
char *cnst_LBX_Error9 = " has an incorrect record size";                            //dseg:3FBE
char *cnst_LBX_ErrorA = " exceeds number of defined records";                       //dseg:3FDC
char *cnst_LBX_ErrorB = " cannot be reloaded into EMS w/o being first released.";   //dseg:3FFF
char *cnst_LBX_ErrorC = " EMM loading error. Insufficient EMM.";                    //dseg:4036
char *cnst_LBX_ErrorD = " Only pictures may be loaded into reserved EMM";           //dseg:405C
char *cnst_LBX_ErrorE = " (Reserved EMM) ";                                         //dseg:408B
char *cnst_LBX_ErrorF1 = " LBX to";                                                 //dseg:409C
char *cnst_LBX_ErrorG = " Vga file animation frames cannot exceed 65536 bytes";     //dseg:40A4

unsigned int g_LBX_EntryCount;              // dseg:A5C6
unsigned int g_LBX_FormatType;              // dseg:A5C8
unsigned int gsa_LBX_Header;                  // dseg:A5CA
char g_LBX_Name[16];                        // dseg:A5CC
unsigned int g_LBX_EmmRsvd;                 // dseg:A5EC
