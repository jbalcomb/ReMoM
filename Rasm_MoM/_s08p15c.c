// _s08p15c.c SA_GetFree LBX_Alloc_GetFree

unsigned int SA_Alloc_GetFree(unsigned int Alloc_Seg)
{
    unsigned int tmp_Size_Paras;
    unsigned int tmp_Used_Paras;
    unsigned int tmp_Free_Paras;
    unsigned int Free_Paras;

    tmp_Size_Paras = farpeekw(Alloc_Seg, 0x08);  // SAMB.size

    tmp_Used_Paras = farpeekw(Alloc_Seg, 0x0A);  // SAMB.used

    tmp_Free_Paras = tmp_Size_Paras - tmp_Used_Paras;

    Free_Paras = tmp_Free_Paras;

    return Free_Paras;
}
