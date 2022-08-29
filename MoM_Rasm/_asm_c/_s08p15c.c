// _s08p15c.c SA_GetFree LBX_Alloc_GetFree

unsigned int SA_GetFree(unsigned int SAMB)
{
    unsigned int tmp_Size_Paras;
    unsigned int tmp_Used_Paras;
    unsigned int tmp_Free_Paras;
    unsigned int Free_Paras;

    // printf("DEBUG: %s %d BEGIN: SA_GetFree(SAMB = 0x%04X)\n", __FILE__, __LINE__, SAMB);

    tmp_Size_Paras = farpeekw(SAMB, 0x08);  // SAMB.size

    tmp_Used_Paras = farpeekw(SAMB, 0x0A);  // SAMB.used

    tmp_Free_Paras = tmp_Size_Paras - tmp_Used_Paras;

    Free_Paras = tmp_Free_Paras;

    // printf("DEBUG: %s %d END: SA_GetFree()\n", __FILE__, __LINE__);
    return Free_Paras;
}
