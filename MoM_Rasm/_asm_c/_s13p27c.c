// _s13p27c.c EMM_EMMDATAH_AllocNext
// ST_EMM.H

#include "ST_EMM.H"


unsigned int EMM_EMMDATAH_AllocNext(int nparas)
{
    unsigned int paras_free;
    unsigned int SAMB_head;
    unsigned int SAMB_data;
    int tmp_nparas;
    unsigned int tmp_paras_free;
    unsigned int tmp_g_EMMDATAH_Level;

//    printf("DEBUG: %s %d BEGIN: EMM_EMMDATAH_AllocNext()\n", __FILE__, __LINE__);

    tmp_nparas = nparas;

    EMM_Map4_EMMDATAH();

    SAMB_head = g_EMM_PageFrame_Base_Address;
    SAMB_head = SAMB_head + g_EMMDATAH_Level;

    EMM_Map4_EMMDATAH();

    tmp_nparas = tmp_nparas + 1;

    tmp_paras_free = 4090;  // 0FFAh
    tmp_paras_free = tmp_paras_free - g_EMMDATAH_Level;
    paras_free = tmp_paras_free;

    if ( paras_free < tmp_nparas )
    {
        SA_Alloc_Error(0x04, (tmp_nparas - paras_free));  // Alloc_EMM_too_small
    }

    farpokew(SAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(SAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(SAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(SAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    farpokew(SAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    tmp_g_EMMDATAH_Level = tmp_nparas;
    tmp_g_EMMDATAH_Level = tmp_g_EMMDATAH_Level + 1;
    g_EMMDATAH_Level = g_EMMDATAH_Level + tmp_g_EMMDATAH_Level;

    SAMB_data = SAMB_head + 1;

//    printf("DEBUG: %s %d END: EMM_EMMDATAH_AllocNext()\n", __FILE__, __LINE__);
    return SAMB_data;
}
