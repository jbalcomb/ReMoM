// _s08p19c.c LBX_Alloc_Error
// in ST_SA.H

#include "ST_SA.H"

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


void SA_Alloc_Error(int errno, int value)
{
    char conv[20];
    char errmsg[120];

    printf("DEBUG: %s %d BEGIN: SA_Alloc_Error(errno = %d, value = %d)\n", __FILE__, __LINE__, errno, value);
    
    if ( DBG_IsDisabled() != 0 )
    {
        errno = errno - 1;
        switch(errno)
        {
            case 0:
                strcpy(errmsg, cnst_Alloc_Error11);
                itoa(value, conv, 10);
                break;
            case 1:
                strcpy(errmsg, cnst_Alloc_Error21);
                strcat(errmsg, cnst_Alloc_Error22);
                strcat(errmsg, cnst_Alloc_Error23);
                itoa(value, conv, 10);
                break;
            case 2:
                strcpy(errmsg, cnst_Alloc_Error3);
                itoa(value, conv, 10);
                strcat(errmsg, cnst_Alloc_Error22);
                strcat(errmsg, cnst_Alloc_Error12);
                break;
            case 3:
                strcpy(errmsg, cnst_Alloc_Error21);
                strcat(errmsg, cnst_Alloc_Error4);
                strcat(errmsg, cnst_Alloc_Error22);
                strcat(errmsg, cnst_Alloc_Error23);
                itoa(value, conv, 10);
                break;
        }
        strcat(errmsg, conv);
        strcat(errmsg, cnst_Alloc_Error13);
        Quit(errmsg);
    }
    else
    {
        strcpy(errmsg, cnst_Alloc_Error51);
        itoa(g_RAM_Min_KB, conv, 10);
        strcat(errmsg, conv);
        strcat(errmsg, cnst_Alloc_Error52);
        Quit(errmsg);
    }
}
