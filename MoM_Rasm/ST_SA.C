
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */    

#include "ST_HEAD.H"
#include "ST_TYPE.H"

#include "ST_SA.H"

#include "STU_DBG.H"


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
//void _FAR * fp_tmpSAMB;                   // MGC dseg:A5C0  WZD dseg:E5CA
SAMB_ptr pTmpSAMB;                        // MGC dseg:A5C0  WZD dseg:E5CA
// unsigned int g_RAM_Min_KB;               // MGC dseg:A5C4
extern unsigned int RAM_Min_KB;             // MGC dseg:A5C4  ; set to 583 in _main

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
// int SA_Alloc_Validate(sgmt_addr SAMB_head)
int SA_Alloc_Validate(SAMB_ptr pSAMB_head)
{
    unsigned int memsig1;
    unsigned int memsig2;
    int is_valid;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Alloc_Validate(pSAMB_head = %p)\n", __FILE__, __LINE__, pSAMB_head);
#endif

    memsig1 = ((unsigned int)*((unsigned char *)pSAMB_head + 4)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 5) << 8);
    memsig2 = ((unsigned int)*((unsigned char *)pSAMB_head + 6)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 7) << 8);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: memsig1: 0x%04X\n", __FILE__, __LINE__, memsig1);
    dlvfprintf("DEBUG: [%s, %d]: memsig2: 0x%04X\n", __FILE__, __LINE__, memsig2);
#endif

    if (memsig1 != SA_MEMSIG1 || memsig2 != SA_MEMSIG2)
    {
        is_valid = ST_FAILURE;
    }
    else
    {
        is_valid = ST_SUCCESS;
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Alloc_Validate(pSAMB_head = %p) { is_valid = %d }\n", __FILE__, __LINE__, pSAMB_head, is_valid);
#endif

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
//SAMB_ptr SA_Allocate_Space(unsigned int nparas)
SAMB_ptr SA_Allocate_Space(unsigned int nparas)
{
    SAMB_ptr pSAMB_head;
    unsigned int memsig1;
    unsigned int memsig2;
    unsigned int size_paras;
    unsigned int used_paras;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Allocate_Space(nparas = %u)\n", __FILE__, __LINE__, nparas);
#endif

    pTmpSAMB = (SAMB_ptr) malloc(((unsigned long)nparas * 16) + 16);

    if ( pTmpSAMB == NULL )
    {
        SA_Alloc_Error(1, nparas); // Alloc Error #1: Allocation Too Small
    }
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: pTmpSAMB: %p\n", __FILE__, __LINE__, pTmpSAMB);
    dlvfprintf("DEBUG: [%s, %d]: pTmpSAMB: %Fp\n", __FILE__, __LINE__, pTmpSAMB);
#endif
    
    //pSAMB_head = (pTmpSAMB + 16);
    //pSAMB_head = MK_FP((FP_SEG(pTmpSAMB) + 1),FP_OFF(pTmpSAMB));
    pSAMB_head = MK_FP((FP_SEG(pTmpSAMB) + 1),0);

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: pSAMB_head: %p\n", __FILE__, __LINE__, pSAMB_head);
    dlvfprintf("DEBUG: [%s, %d]: pSAMB_head: %Fp\n", __FILE__, __LINE__, pSAMB_head);
#endif

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: FP_SEG(pTmpSAMB): 0x%04X\n", __FILE__, __LINE__, FP_SEG(pTmpSAMB));
    dlvfprintf("DEBUG: [%s, %d]: FP_SGMT(pTmpSAMB): 0x%04X\n", __FILE__, __LINE__, FP_SGMT(pTmpSAMB));
    dlvfprintf("DEBUG: [%s, %d]: FP_SEG(pSAMB_head): 0x%04X\n", __FILE__, __LINE__, FP_SEG(pSAMB_head));
    dlvfprintf("DEBUG: [%s, %d]: FP_SGMT(pSAMB_head): 0x%04X\n", __FILE__, __LINE__, FP_SGMT(pSAMB_head));
#endif

//#define SET_LE_16(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu) )
//#define SET_LE_32(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))

    ((unsigned char *)(pSAMB_head))[4] = ((SA_MEMSIG1) & 0xFF);
    ((unsigned char *)(pSAMB_head))[5] = (((SA_MEMSIG1) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_head))[6] = ((SA_MEMSIG2) & 0xFF);
    ((unsigned char *)(pSAMB_head))[7] = (((SA_MEMSIG2) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_head))[8] = ((nparas) & 0xFF);
    ((unsigned char *)(pSAMB_head))[9] = (((nparas) >> 8) & 0xFF);

    ((unsigned char *)(pSAMB_head))[10] = ((1) & 0xFF);
    ((unsigned char *)(pSAMB_head))[11] = (((1) >> 8) & 0xFF);

    memsig1 =    ((unsigned int)*((unsigned char *)pSAMB_head +  4)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head +  5) << 8);
    memsig2 =    ((unsigned int)*((unsigned char *)pSAMB_head +  6)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head +  7) << 8);
    size_paras = ((unsigned int)*((unsigned char *)pSAMB_head +  8)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head +  9) << 8);
    used_paras = ((unsigned int)*((unsigned char *)pSAMB_head + 10)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 11) << 8);
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: memsig1: 0x%04X\n", __FILE__, __LINE__, memsig1);
    dlvfprintf("DEBUG: [%s, %d]: memsig2: 0x%04X\n", __FILE__, __LINE__, memsig2);
    dlvfprintf("DEBUG: [%s, %d]: size_paras: %u\n", __FILE__, __LINE__, size_paras);
    dlvfprintf("DEBUG: [%s, %d]: used_paras: %u\n", __FILE__, __LINE__, used_paras);
#endif

    //Update_MemFreeWorst_KB();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: SA_Allocate_Space(nparas = %u) { pSAMB_head = %p }\n", __FILE__, __LINE__, nparas, pSAMB_head);
#endif

    return pSAMB_head;
}

// _s08p08
SAMB_ptr SA_Allocate_MemBlk(unsigned int nparas)
{
    SAMB_ptr pSAMB_data;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Allocate_MemBlk(nparas = %u)\n", __FILE__, __LINE__, nparas);
#endif

    pTmpSAMB = (SAMB_ptr) malloc(((unsigned long)nparas * 16) + 16);

    if ( pTmpSAMB == NULL )
    {
        SA_Alloc_Error(0x01, nparas); // Alloc Error #1: Allocation Too Small
    }
    
    pSAMB_data = (pTmpSAMB + 16);

    //Update_MemFreeWorst_KB();

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: SA_Allocate_MemBlk(nparas = %u) { pSAMB_data = %p }\n", __FILE__, __LINE__, nparas, pSAMB_data);
#endif

    return pSAMB_data;
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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Alloc_First(pSAMB_head = %p, nparas = %d)\n", __FILE__, __LINE__, pSAMB_head, nparas);
#endif

    tmp_nparas = nparas + 1;

    //pSAMB_sub_data = pSAMB_head + 1;  // + 1 segment / 16 bytes / paragraph / SAMB unit size
    //pSAMB_sub_head = (pSAMB_head + 16);
    pSAMB_sub_head = MK_FP((FP_SEG(pSAMB_head) + 1),FP_OFF(pSAMB_head));

    //paras_total = farpeekw(SAMB_head, 8);  // s_SAMB.Size_Paras
    paras_total = ((unsigned int)*((unsigned char *)pSAMB_head + 8)) | (unsigned int)((unsigned int)*((unsigned char *)pSAMB_head + 9) << 8);

    // // if ( SA_Alloc_Validate(SAMB_head) == 0 )
    // pSAMB_head = (void *)MK_FP(SAMB_head,0);
    if ( SA_Alloc_Validate(pSAMB_head) == 0 )
    {
        HERE("Allocation space has been corrupted"); \
        SA_Alloc_Error(0x03, tmp_nparas);  // Alloc_Space_corrupted
    }

    if ( paras_total < tmp_nparas  )
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
    pSAMB_sub_data = pSAMB_sub_head + 16;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: SA_Alloc_First(pSAMB_head = %p, nparas = %d) { pSAMB_sub_data = %p }\n", __FILE__, __LINE__, pSAMB_head, nparas, pSAMB_sub_data);
#endif

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

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: BEGIN: SA_Alloc_Next(pSAMB_head = %p, nparas = %d)\n", __FILE__, __LINE__, pSAMB_head, nparas);
#endif

    tmp_nparas = nparas + 1;

    // // if ( SA_Alloc_Validate(SAMB_head) == 0 )
    // pSAMB_head = (void *)MK_FP(SAMB_head,0);
    if ( SA_Alloc_Validate(pSAMB_head) == 0 )
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
    pSAMB_sub_head = pSAMB_head + tmp_paras_used;

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
    pSAMB_sub_data = pSAMB_sub_head + 16;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d]: END: SA_Alloc_Next(pSAMB_head = %p, nparas = %d) { pSAMB_sub_data = %p }\n", __FILE__, __LINE__, pSAMB_head, nparas, pSAMB_sub_data);
#endif

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
        itoa(RAM_Min_KB, conv, 10);
        strcat(errmsg, conv);
        strcat(errmsg, cnst_Alloc_Error52);
        Quit(errmsg);
    }
}
