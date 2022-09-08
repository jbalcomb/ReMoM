// _s10p15c.c LBX_Error

#include "ST_HEAD.H"
#include "ST_EMM.H"
#include "ST_LBX.H"

void LBX_Error(char *name, int errno, int entry, int pages)
{
    char cnv[20];
    char errmsg[120];

    printf("DEBUG: [%s, %d]: BEGIN: LBX_Error(name = %s, errno = %d, entry = %d, pages = %d)\n", __FILE__, __LINE__, name, errno, entry, pages);

    strcpy(errmsg, name);

    itoa(entry, cnv, 10);

    strcat(errmsg, cnst_LBXErr_Common1);

    strcat(errmsg, cnv);

    strcat(errmsg, cnst_LBXErr_Common2);

    
    switch (errno)
    {
        case 1:
            strcat(errmsg, cnst_LBX_Error1);
            break;
        case 2:
            strcat(errmsg, cnst_LBX_Error2);
            break;
        case 3:
            strcpy(errmsg, cnst_LBX_Error31);
            itoa(RAM_Min_KB, cnv, 10);
            strcat(errmsg, cnv);
            strcat(errmsg, cnst_LBX_Error32);
            break;
        case 4:
            strcat(errmsg, cnst_LBX_Error4);
            break;
        case 5:
            strcat(errmsg, cnst_LBX_Error51);
            itoa(pages, cnv, 10);
            strcat(errmsg, cnv);
            strcat(errmsg, cnst_LBX_Error52);
            break;
        //case 6:
        //    break;
        case 7:
            strcat(errmsg, cnst_LBX_Error7);
            break;
        case 8:
            strcat(errmsg, cnst_LBX_Error8);
            break;
        case 9:
            strcat(errmsg, cnst_LBX_Error9);
            break;
        case 10:
            strcat(errmsg, cnst_LBX_ErrorA);
            break;
        case 11:
            strcpy(errmsg, name);
            strcat(errmsg, g_LBX_FileExtension);
            strcat(errmsg, cnst_LBX_ErrorB);
            break;
        case 12:
            strcat(errmsg, cnst_LBX_ErrorC);
            break;
        case 13:
            strcat(errmsg, cnst_LBX_ErrorD);
            break;
        case 14:
            strcat(errmsg, cnst_LBX_ErrorC);
            strcat(errmsg, cnst_LBX_ErrorE);
        case 15:
            strcat(errmsg, cnst_LBX_ErrorF1);
            strcat(errmsg, cnst_LBX_ErrorC);
            strcat(errmsg, cnst_LBX_ErrorE);
            break;
        case 16:
            strcat(errmsg, cnst_LBX_ErrorG);
            break;
    }

    Quit(errmsg);
}
