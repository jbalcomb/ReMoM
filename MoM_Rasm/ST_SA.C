
#include "ST_HEAD.H"

#include "ST_SA.H"


//struct ST_Alloc_Space_Header_Struct LBX_Alloc_Space_Header;
//struct s_SAMB SAMB;

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
//unsigned char SA_NearHeap_Buffer[];          // dseg:9590 SA_NearHeap_Buffer db 4144 dup(0)
//looks like 'Temp_String' should be something else as char [4400] then pointers to offsets from that pointer
//code shows checking NearAlloc(nbytes) aginst 4,144 and then setting mark, used, and return value offset by 256
/*
    Far Heap
*/
void _FAR * fp_tmpSAMB;                    // MGC dseg:A5C0  WZD dseg:E5CA
unsigned int g_RAM_Min_KB;                  // MGC dseg:A5C4

/*
##### seg007
_s07p04     SA_Alloc_Validate
_s07p05     farpeekb
_s07p06     farpeekw
_s07p07     farpeekdw
_s07p08     farpokeb
_s07p09     farpokew
UU _s07p10     farpokedw
##### seg008
_s08p07c.c      SA_Allocate_Space
_s08p08c.c      SA_Allocate_MemBlk
_s08p12c.c      SA_Alloc_First
_s08p14c.c      SA_Alloc_Next
_s08p15c.c      SA_GetFree
_s08p16.asm     SA_MK_FP0
_s08p19c.c      SA_Alloc_Error
*/

/*
    MAGIC.EXE  seg007
*/

// s07p04
int SA_Alloc_Validate(sgmt_addr SAMB_head)
{
    // SAMB * pSAMB_head;
    unsigned int memsig1;
    unsigned int memsig2;
    int is_valid;

    memsig1 = farpeekw(SAMB_head, 4);
    memsig2 = farpeekw(SAMB_head, 6);
    // memsig1 = *((unsigned int *)SAMB_head + 4);
    // memsig2 = *((unsigned int *)SAMB_head + 6);
    // pSAMB_head = (SAMB _FAR *)MK_FP(SAMB_head, 0);

    //if (pSAMB_head->MemSig1 != LBX_MemSig1 || pSAMB_head->MemSig2 != LBX_MemSig2)
    if (memsig1 != LBX_MemSig1 || memsig2 != LBX_MemSig2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

    return is_valid;
}

// _s07p05
unsigned char farpeekb(unsigned int sgmt, unsigned int ofst)
{
    return(*( (char _FAR * )MK_FP(sgmt, ofst)));
}

// _s07p06
unsigned int farpeekw(unsigned int sgmt, unsigned int ofst)
{
    return(*( (unsigned int _FAR * )MK_FP(sgmt, ofst)));
}

// _s07p07
unsigned long farpeekdw(unsigned int sgmt, unsigned int ofst)
{
    unsigned int tmp_AX;
    unsigned int tmp_DX;
    unsigned long tmpDoubleWord;

    tmp_AX = *( (unsigned int _FAR * )MK_FP(sgmt, ofst));
    tmp_DX = *( (unsigned int _FAR * )MK_FP(sgmt, ofst+2));

    tmpDoubleWord = (unsigned long) ( ((unsigned long)tmp_DX) << 16 ) + tmp_AX;

    return tmpDoubleWord;
}

// _s07p08
void farpokeb(unsigned int sgmt, unsigned int ofst, unsigned char val)
{
    ( *( (unsigned char _FAR * )MK_FP(sgmt, ofst)) = val );
}

// _s07p09
void farpokew(unsigned int sgmt, unsigned int ofst, unsigned short val)
{
    ( *( (unsigned short _FAR * )MK_FP(sgmt, ofst)) = val );
}


/*
    MAGIC.EXE  seg008
*/

// _s08p07
sgmt_addr SA_Allocate_Space(unsigned int nparas)
{
    sgmt_addr SAMB_head;
    SAMB * pSAMB_head;

    fp_tmpSAMB = (char *) malloc(((unsigned long)nparas * 16) + 16);

    if ( fp_tmpSAMB == NULL )
    {
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
    
    SAMB_head = FP_SEG(fp_tmpSAMB) + 1;
    //farpokew(SAMB_head, 4, SA_MEMSIG1);  // SAMB.MemSig1
    //farpokew(SAMB_head, 6, SA_MEMSIG2);  // SAMB.MemSig2
    //farpokew(SAMB_head, 8, nparas);      // SAMB.Size
    //farpokew(SAMB_head, 10, 1);          // SAMB.Used
    pSAMB_head = (SAMB _FAR *)MK_FP(SAMB_head, 0);
    pSAMB_head->MemSig1 = SA_MEMSIG1;
    pSAMB_head->MemSig2 = SA_MEMSIG2;
    pSAMB_head->Size_Paras = nparas;
    pSAMB_head->Used_Paras = 1;
    
    //Update_MemFreeWorst_KB();

    return SAMB_head;
}

// _s08p08
unsigned int SA_Allocate_MemBlk(unsigned int nparas)
{
    sgmt_addr SAMB_data;

    fp_tmpSAMB = (char _FAR *) farmalloc(((unsigned long)nparas * 16) + 16);

    if ( fp_tmpSAMB == NULL )
    {
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
    
    SAMB_data = FP_SEG(fp_tmpSAMB) + 1;

    //Update_MemFreeWorst_KB();

    return SAMB_data;
}

// _s08p12
unsigned int SA_Alloc_First(unsigned int SAMB, int nparas)
{
    int paras_total;
    unsigned int SAMB_head;
    unsigned int SAMB_data;
    int tmp_nparas;
    
    tmp_nparas = nparas;
    tmp_nparas = tmp_nparas + 1;

    SAMB_data = SAMB + 1;

    paras_total = farpeekw(SAMB, 8);  // s_SAMB.Size_Paras

    if ( SA_Alloc_Validate(SAMB) == 0 )
    {
        SA_Alloc_Error(0x03, tmp_nparas);  // Alloc_Space_corrupted
    }

    if ( paras_total < tmp_nparas  )
    {
        SA_Alloc_Error(0x02, tmp_nparas);  // Alloc_Next_too_small
    }

    farpokew(SAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(SAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(SAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(SAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    farpokew(SAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    SAMB_data = SAMB_head + 1;

    return SAMB_data;    
}

// _s08p14
unsigned int SA_Alloc_Next(unsigned int SAMB, int nparas)
{
    unsigned int subSAMB_head;
    unsigned int subSAMB_data;
    unsigned int tmp_SAMB;
    int tmp_nparas;
    int tmp_paras_free;
    int tmp_paras_used;

    tmp_SAMB = SAMB;

    tmp_nparas = nparas;
    tmp_nparas = tmp_nparas + 1;

    if ( SA_Alloc_Validate(SAMB) == 0 )
    {
        SA_Alloc_Error(0x03, tmp_nparas);  // Alloc_Space_corrupted
    }

    tmp_paras_free = SA_GetFree(tmp_SAMB);

    if ( tmp_paras_free < tmp_nparas )
    {
        SA_Alloc_Error(0x02, tmp_nparas);  // Alloc_Next_too_small
    }

    tmp_paras_used = farpeekw(SAMB, 10);  // s_SAMB.Used_Paras
    subSAMB_head = tmp_paras_used + tmp_SAMB;

    tmp_paras_used = farpeekw(SAMB, 10);  // s_SAMB.Used_Paras
    tmp_nparas = tmp_nparas + tmp_paras_used;

    farpokew(tmp_SAMB, 10, tmp_nparas);                 // s_SAMB.Used_Paras = previously used + newly requested

    farpokew(subSAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(subSAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(subSAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(subSAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    // DNE !?! farpokew(subSAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    subSAMB_data = subSAMB_head + 1;

    return subSAMB_data;    
}

// _s08p15
unsigned int SA_GetFree(unsigned int SAMB)
{
    unsigned int tmp_Size_Paras;
    unsigned int tmp_Used_Paras;
    unsigned int tmp_Free_Paras;
    unsigned int Free_Paras;

    tmp_Size_Paras = farpeekw(SAMB, 0x08);  // SAMB.size

    tmp_Used_Paras = farpeekw(SAMB, 0x0A);  // SAMB.used

    tmp_Free_Paras = tmp_Size_Paras - tmp_Used_Paras;

    Free_Paras = tmp_Free_Paras;

    return Free_Paras;
}

// _s08p16
void _FAR * SA_MK_FP0(unsigned int sgmt_addr)
{
    void _FAR * fp;
    fp = (void _FAR *)MK_FP(sgmt_addr,0);
    return fp;
}

// _s08p19
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
