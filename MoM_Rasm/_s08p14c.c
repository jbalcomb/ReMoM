// _s08p14c.c SA_Alloc_Next
// ST_SA.H

unsigned int SA_Alloc_Next(unsigned int SAMB, int nparas)
{
    unsigned int subSAMB_head;
    unsigned int subSAMB_data;
    unsigned int tmp_SAMB;
    int tmp_nparas;
    int tmp_paras_free;
    int tmp_paras_used;

    // printf("DEBUG: %s %d BEGIN: SA_Alloc_Next(SAMB = 0x%04X, nparas = %d)\n", __FILE__, __LINE__, SAMB, nparas);

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
        // printf("DEBUG: [%s %d]: tmp_nparas >= tmp_paras_free: %d >= %d\n", __FILE__, __LINE__, tmp_nparas, tmp_paras_free);
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

    // printf("DEBUG: %s %d END: SA_Alloc_Next()\n", __FILE__, __LINE__);
    return subSAMB_data;    
}
