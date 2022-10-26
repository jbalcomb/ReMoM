// _s08p12c.c SA_Alloc_First
// ST_SA.H

unsigned int SA_Alloc_First(unsigned int SAMB, int nparas)
{
    int paras_total;
    unsigned int SAMB_head;
    unsigned int SAMB_data;
    int tmp_nparas;

//    printf("DEBUG: %s %d BEGIN: SA_Alloc_First(SAMB = 0x%04X, nparas = %d)\n", __FILE__, __LINE__, SAMB, nparas);
    
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
//        printf("DEBUG: [%s %d]: tmp_nparas >= paras_total: %d >= %d\n", __FILE__, __LINE__, tmp_nparas, paras_total);
        SA_Alloc_Error(0x02, tmp_nparas);  // Alloc_Next_too_small
    }

    farpokew(SAMB_head, 4, 0x12FA);             // s_SAMB.MemSig1 = e_SAMB_MemSig1
    farpokew(SAMB_head, 6, 0x4ECF);             // s_SAMB.MemSig2 = e_SAMB_MemSig2
    farpokew(SAMB_head, 8, (tmp_nparas - 1));   // s_SAMB.Size_Paras = nparas + 1 - 1
    farpokew(SAMB_head, 10, 1);                 // s_SAMB.Used_Paras = 1
    farpokew(SAMB_head, 14, 1);                 // s_SAMB.Marked_Paras = 1

    SAMB_data = SAMB_head + 1;

//    printf("DEBUG: %s %d END: SA_Alloc_First()\n", __FILE__, __LINE__);
    return SAMB_data;    
}
