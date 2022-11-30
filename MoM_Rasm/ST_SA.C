
#include "MoX_TYPE.H"
#include "ST_DEF.H"     /* PTR_INC_PARAGRAPH() */
#include "ST_SA.H"

#include "MoX_DBG.H"    /* Check_Release_Version() */
#include "MoX_LBX.H"    /* MoX_RAM_Min_KB*/

#include "ST_EXIT.H"    /* Exit() */
#include "ST_DBG.H"     /* DBG_IsDisabled() */
#include "ST_LBX.H"     /* RAM_Min_KB */

#include <ALLOC.H>      /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */    
// #include <STDIO.H>   /* printf() */
#include <STDLIB.H>  /* itoa() */
#include <STRING.H> /* strcat(), strcpy() */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif


//struct ST_Alloc_Space_Header_Struct LBX_Alloc_Space_Header;
//struct s_SAMB SAMB;

/*
    Near Heap
*/

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
//void _FAR * fp_tmpSAMB;                   // MGC dseg:A5C0  WZD dseg:E5CA
SAMB_ptr g_SAMB;                        // MGC dseg:A5C0  WZD dseg:E5CA
// unsigned int g_RAM_Min_KB;               // MGC dseg:A5C4
// MoX_MoM  extern unsigned int RAM_Min_KB;             // MGC dseg:A5C4  ; set to 583 in _main

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
int SA_Alloc_Validate(SAMB_ptr SAMB_head)
{
    int is_valid;
    if (SA_Get_MemSig1(SAMB_head) != SA_MEMSIG1 || SA_Get_MemSig2(SAMB_head) != SA_MEMSIG2)
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
    // asm { mov si, [ofst];  mov ax, [sgmt];  mov ds, ax;  lodsw; }
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
SAMB_ptr SA_Allocate_Space(unsigned int nparas)
{
    SAMB_ptr SAMB_head;
    
    g_SAMB = (SAMB_ptr) malloc( ((nparas + 1) * 16) );
    if ( g_SAMB == NULL ) { SA_Alloc_Error(0x01, nparas); }
    SAMB_head = (SAMB_ptr) PTR_ADD_PARAGRAPH(g_SAMB,1);
    // // // SAMB_SetMemSig1(SAMB_head);
    // // // SAMB_SetMemSig2(SAMB_head);
    // // // SAMB_SetSize(SAMB_head, nparas);
    // // // SAMB_SetUsed(SAMB_head, 1);
    // // *((SAMB_head) +  4) = ((SA_MEMSIG1) & 0xFF);  *((SAMB_head) +  5) = (((SA_MEMSIG1) >> 8) & 0xFF);
    // // *((SAMB_head) +  6) = ((SA_MEMSIG2) & 0xFF);  *((SAMB_head) +  7) = (((SA_MEMSIG2) >> 8) & 0xFF);
    // // *((SAMB_head) +  8) = ((nparas)     & 0xFF);  *((SAMB_head) +  9) = (((nparas)     >> 8) & 0xFF);
    // // *((SAMB_head) + 10) = ((1)          & 0xFF);  *((SAMB_head) + 11) = (((1)          >> 8) & 0xFF);
    // SET_2B_OFS(SAMB_head,  4, SA_MEMSIG1);
    // SET_2B_OFS(SAMB_head,  6, SA_MEMSIG2);
    // SET_2B_OFS(SAMB_head,  8, nparas);
    // SET_2B_OFS(SAMB_head, 10, 1);
    SA_Set_MemSig1(SAMB_head);
    SA_Set_MemSig2(SAMB_head);
    SA_Set_Size(SAMB_head,nparas);
    SA_Set_Used(SAMB_head,1);

    Update_MemFreeWorst_KB();

    return SAMB_head;
}

// s08p08
SAMB_ptr SA_Allocate_MemBlk(unsigned int nparas)
{
    SAMB_ptr SAMB_data;
    // g_SAMB = (SAMB_ptr) malloc( ((nparas + 1 ) * 16) );
    g_SAMB = (SAMB_ptr) farmalloc( ((nparas + 1 ) * 16) );
    if ( g_SAMB == NULL ) { SA_Alloc_Error(0x01, nparas); }
    SAMB_data = (SAMB_ptr) PTR_ADD_PARAGRAPH(g_SAMB,1);
    Update_MemFreeWorst_KB();
    return SAMB_data;
}

// _s08p12
/*
    creates a suballocation
*/
SAMB_ptr SA_Alloc_First(SAMB_ptr pSAMB_head, int nparas)
{
    int paras_total;
    SAMB_ptr pSAMB_sub_head;
    SAMB_ptr pSAMB_sub_data;
    int tmp_nparas;

    tmp_nparas = nparas + 1;

    //pSAMB_sub_data = pSAMB_head + 1;  // + 1 segment / 16 bytes / paragraph / SAMB unit size
    //pSAMB_sub_head = (pSAMB_head + 16);
    pSAMB_sub_head = (SAMB_ptr) MK_FP((FP_SEG(pSAMB_head) + 1),FP_OFF(pSAMB_head));

    //paras_total = farpeekw(SAMB_head, 8);  // s_SAMB.Size_Paras
    paras_total = ((unsigned int)*((unsigned char *)pSAMB_head + 8)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 9) << 8);

    if ( SA_Alloc_Validate(pSAMB_head) == ST_FALSE )
    {
        SA_Alloc_Error(0x03, tmp_nparas);  // Alloc_Space_corrupted
    }

    if ( paras_total < tmp_nparas )
    {
        SA_Alloc_Error(0x02, tmp_nparas);  // Alloc_Next_too_small
    }

    ((unsigned char *)(pSAMB_head))[10] = ( (tmp_nparas + 1) & 0xFF);
    ((unsigned char *)(pSAMB_head))[11] = (((tmp_nparas + 1) >> 8) & 0xFF);

    //farpokew(SAMB_sub_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    //farpokew(SAMB_sub_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    //farpokew(SAMB_sub_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    //farpokew(SAMB_sub_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    //farpokew(SAMB_sub_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    ((unsigned char *)(pSAMB_sub_head))[4] = ( (SA_MEMSIG1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[5] = (((SA_MEMSIG1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[6] = ( (SA_MEMSIG2) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[7] = (((SA_MEMSIG2) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[8] = ( (tmp_nparas - 1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[9] = (((tmp_nparas - 1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[10] = ( (1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[11] = (((1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[14] = ( (1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[15] = (((1) >> 8) & 0xFF);

    //SAMB_sub_data = SAMB_sub_head + 1;
    // pSAMB_sub_data = pSAMB_sub_head + 16;
    pSAMB_sub_data = (SAMB_ptr) MK_FP((FP_SEG(pSAMB_sub_head) + 1),0);

// #ifdef STU_DEBUG
//     // first sub allocation, so sub_head should be head + 1 segment and sub_data should be sub_head + 1 segment
//     // dealing in segment addresses means these are inherently/effectively normalized pointers, so the offset should be 0
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_head: %p\n", __FILE__, __LINE__, pSAMB_head);
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_sub_head: %p\n", __FILE__, __LINE__, pSAMB_sub_head);
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_sub_data: %p\n", __FILE__, __LINE__, pSAMB_sub_data);
// #endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: SA_Alloc_First(pSAMB_head = %p, nparas = %d) { pSAMB_sub_data = %p }\n", __FILE__, __LINE__, pSAMB_head, nparas, pSAMB_sub_data);
// #endif

    return pSAMB_sub_data;
}

// _s08p14
SAMB_ptr SA_Alloc_Next(SAMB_ptr pSAMB_head, int nparas)
{
    SAMB_ptr pSAMB_sub_head;
    SAMB_ptr pSAMB_sub_data;
    int tmp_nparas;
    int tmp_paras_free;
    int tmp_paras_used;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Alloc_Next(pSAMB_head = %p, nparas = %d)\n", __FILE__, __LINE__, pSAMB_head, nparas);
// #endif

    tmp_nparas = nparas + 1;

    // // if ( SA_Alloc_Validate(SAMB_head) == 0 )
    // pSAMB_head = (void *)MK_FP(SAMB_head,0);
    if ( SA_Alloc_Validate(pSAMB_head) == ST_FALSE )
    {
        SA_Alloc_Error(0x03, tmp_nparas);  // Alloc_Space_corrupted
    }

    tmp_paras_free = SA_GetFree(pSAMB_head);

    if ( tmp_paras_free < tmp_nparas )
    {
        SA_Alloc_Error(0x02, tmp_nparas);  // Alloc_Next_too_small
    }

    // tmp_paras_used = farpeekw(SAMB_head, 10);  // s_SAMB.Used_Paras
    tmp_paras_used = ((unsigned int)*((unsigned char *)pSAMB_head + 10)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 11) << 8);
    // pSAMB_sub_head = pSAMB_head + tmp_paras_used;
    // pSAMB_sub_head = MK_FP((FP_SEG(pSAMB_head) + tmp_paras_used),FP_OFF(pSAMB_head));
    pSAMB_sub_head = (SAMB_ptr) MK_FP((FP_SEG(pSAMB_head) + tmp_paras_used),0);

    // tmp_paras_used = farpeekw(SAMB_head, 10);  // s_SAMB.Used_Paras
    tmp_paras_used = ((unsigned int)*((unsigned char *)pSAMB_head + 10)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 11) << 8);
    tmp_nparas = tmp_nparas + tmp_paras_used;

    //farpokew(tmp_SAMB, 10, tmp_nparas);                 // s_SAMB.Used_Paras = previously used + newly requested
    ((unsigned char *)(pSAMB_head))[10] = ( (tmp_nparas) & 0xFF);
    ((unsigned char *)(pSAMB_head))[11] = (((tmp_nparas) >> 8) & 0xFF);

    // farpokew(subSAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    // farpokew(subSAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    // farpokew(subSAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    // farpokew(subSAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    // // DNE !?! farpokew(subSAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    ((unsigned char *)(pSAMB_sub_head))[4] = ( (SA_MEMSIG1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[5] = (((SA_MEMSIG1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[6] = ( (SA_MEMSIG2) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[7] = (((SA_MEMSIG2) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[8] = ( (tmp_nparas - 1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[9] = (((tmp_nparas - 1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_sub_head))[10] = ( (1) & 0xFF);
    ((unsigned char *)(pSAMB_sub_head))[11] = (((1) >> 8) & 0xFF);

    // ((unsigned char *)(pSAMB_sub_head))[14] = ( (1) & 0xFF);
    // ((unsigned char *)(pSAMB_sub_head))[15] = (((1) >> 8) & 0xFF);

    // subSAMB_data = subSAMB_head + 1;  // + 1 segment / 16 bytes / paragraph / SAMB unit size
    // pSAMB_sub_data = pSAMB_sub_head + 16;
    pSAMB_sub_data = (SAMB_ptr) MK_FP((FP_SEG(pSAMB_sub_head) + 1),0);

// #ifdef STU_DEBUG
//     // next sub allocation, so sub_head should be head + used segments and sub_data should be sub_head + 1 segment
//     // dealing in segment addresses means these are inherently/effectively normalized pointers, so the offset should be 0
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_head: %p\n", __FILE__, __LINE__, pSAMB_head);
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_sub_head: %p\n", __FILE__, __LINE__, pSAMB_sub_head);
//     dlvfprintf("DEBUG: [%s, %d]: pSAMB_sub_data: %p\n", __FILE__, __LINE__, pSAMB_sub_data);
// #endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d]: END: SA_Alloc_Next(pSAMB_head = %p, nparas = %d) { pSAMB_sub_data = %p }\n", __FILE__, __LINE__, pSAMB_head, nparas, pSAMB_sub_data);
// #endif

    return pSAMB_sub_data;    
}

// _s08p15
//unsigned int SA_GetFree(SAMB_addr SAMB_head)
//{
//    unsigned int tmp_Size_Paras;
//    unsigned int tmp_Used_Paras;
//    unsigned int tmp_Free_Paras;
//    unsigned int Free_Paras;
//    tmp_Size_Paras = farpeekw(SAMB_head, 0x08);  // SAMB.size
//    tmp_Used_Paras = farpeekw(SAMB_head, 0x0A);  // SAMB.used
//    tmp_Free_Paras = tmp_Size_Paras - tmp_Used_Paras;
//    Free_Paras = tmp_Free_Paras;
//    return Free_Paras;
//}
unsigned int SA_GetFree(SAMB_ptr pSAMB_head)
{
    unsigned int tmp_Size_Paras;
    unsigned int tmp_Used_Paras;
    unsigned int Free_Paras;
    tmp_Size_Paras = ((unsigned int)*((unsigned char *)pSAMB_head +  8)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head +  9) << 8);
    tmp_Used_Paras = ((unsigned int)*((unsigned char *)pSAMB_head + 10)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 11) << 8);
    Free_Paras = tmp_Size_Paras - tmp_Used_Paras;
    return Free_Paras;
}

// _s08p16
SAMB_ptr SA_MK_FP0(SAMB_addr sgmt_addr)
{
    SAMB_ptr fp;
    fp = (SAMB_ptr) MK_FP(sgmt_addr,0);
    return fp;
}


// _s08p19
void SA_Alloc_Error(int SA_Error_Number, int blocks)
{
    int SA_Error_Index;
    char str_itoa[20];
    char str_errmsg[120];

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: SA_Alloc_Error( SA_Error_Number = %d, blocks = %d)\n", __FILE__, __LINE__, SA_Error_Number, blocks);
#endif

#ifdef STU_DEBUG
    // strcpy(str_errmsg, "012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789");  // 120 characters
    strcpy(str_errmsg, "0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_");  // 110 characters
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

    // if ( DBG_IsDisabled() != 0 )
    if ( Check_Release_Version() != ST_TRUE )
    {
        // DLOG("( DBG_IsDisabled() != 0 )");
        SA_Error_Index = SA_Error_Number - 1;
        switch(SA_Error_Index)
        {
            case 0:
                strcpy(str_errmsg, cnst_Alloc_Error11);
                strcat(str_errmsg, cnst_Alloc_Error12);
                itoa(blocks, str_itoa, 10);
                break;
            case 1:
                strcpy(str_errmsg, cnst_Alloc_Error21);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error23);
                itoa(blocks, str_itoa, 10);
                break;
            case 2:
                strcpy(str_errmsg, cnst_Alloc_Error3);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error12);
                itoa(blocks, str_itoa, 10);
                break;
            case 3:
                strcpy(str_errmsg, cnst_Alloc_Error21);
                strcat(str_errmsg, cnst_Alloc_Error4);
                strcat(str_errmsg, cnst_Alloc_Error22);
                strcat(str_errmsg, cnst_Alloc_Error23);
                itoa(blocks, str_itoa, 10);
                break;
        }
        strcat(str_errmsg, str_itoa);
        strcat(str_errmsg, cnst_Alloc_Error13);
        goto Done;
    }
    else
    {
        // DLOG("( DBG_IsDisabled() == 0 )");
        strcpy(str_errmsg, cnst_Alloc_Error51);
        // itoa(RAM_Min_KB, str_itoa, 10);
        itoa(MoX_RAM_Min_KB, str_itoa, 10);
        strcat(str_errmsg, str_itoa);
        strcat(str_errmsg, cnst_Alloc_Error52);
        goto Done;
    }

Done:

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] str_errmsg: %s\n", __FILE__, __LINE__, str_errmsg);
#endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: SA_Alloc_Error( SA_Error_Number = %d, blocks = %d)\n", __FILE__, __LINE__, SA_Error_Number, blocks);
#endif
    Exit(str_errmsg);
}
