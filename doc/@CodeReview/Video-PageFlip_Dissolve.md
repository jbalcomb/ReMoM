Video-PageFlip_Dissolve.md

Summon_Animation()
    |-> Dissolve_Main_Screen()
        |-> PageFlip_Dissolve()



Mosaic_Rnd50_Tbl[]
Mosaic_Rnd256_Bits[]

void PageFlip_Dissolve(void)
    Toggle_Pages_No_Draw_Fields_Pre();
    Platform_Video_Update();   /* show this pass's grain accumulating */
    ...four passes...
    if(g_pageflip_dissolve_pass_delay > 0) { Platform_Sleep_Millies(g_pageflip_dissolve_pass_delay); }
    Toggle_Pages_No_Draw_Fields();

