// ~== ST_FLIC
// seg028

#include "MoX_TYPE.H"    /* word, SAMB_ptr */
#include "ST_DEF.H"
#include "seg028.H"
#include "seg021.H"     /* FLIC_Load_Palette() */

#include "ST_FLIC.H"
#include "ST_SA.H"  /* SAMB_addr; farpokew(); */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <STRING.H>     /* memcpy() */


// // w/ MAINMENU CREDITS  // MGC s28p02
// // s28p02
// // ST_FLIC.H
// /*
//     XREF:
//         GAME_PrepareCredits()
//         GAME_DrawCredits()
// */
// void FLIC_Prepare(int width, int height, SAMB_addr sa_FLIC_Header)
// {
//     SAMB_ptr fp_FLIC_Header;
//     unsigned int length;
//     int itr;
// 
//     fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
// 
//     // farpokew(sa_FLIC_Header, 0x00, width);
//     // farpokew(sa_FLIC_Header, 0x02, height);
//     farpokew(sa_FLIC_Header, 0x04, 0xDE0A);  // DE0Ah 56842d; DEh 222d; 0Ah 10d; 0ADEh 2782d;
//     farpokew(sa_FLIC_Header, 0x06, 0x00);
//     farpokew(sa_FLIC_Header, 0x08, 0x00);
//     FLIC_Set_Width(fp_FLIC_Header, width);
//     FLIC_Set_Height(fp_FLIC_Header, height);
//     
//     length = width * height;
//     
//     fp_FLIC_Header += 0x10;
//     for(itr = 0; itr < length; itr++)
//     {
//         *(fp_FLIC_Header + itr) = 0;
//     }
// 
// }


// s28p11
/*
    e.g.,
        main()
            GAME_LoadMainImages()
                EMM_Load_LBX_File_1(g_LbxNm_MAINSCRN);
                gsa_MAINSCRN_0_AnimatedLogo = LBXE_LoadSingle(g_LbxNm_MAINSCRN, 0);
                    LBXE_LoadSingle(LbxName=MAINSCRN, LbxEntryIndex=0)
                        LBX_Load_Entry(LbxName=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, LbxHdrFmt=0)
                            EMM_LBX_Load_Entry(EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0, FormatType=0)
                                EMM_LBX_HdrOnly(EmmHndl=6, EmmHndlNm=MAINSCRN, LbxEntry=0, SAMB_head=0x0000, LoadType=0)
                                    Allocate_Space_No_Header(nparas=1);
                                        gfp_tmpSAMB = (char _FAR *) farmalloc(((unsigned long)nparas * 16) + 16);
                                        SAMB_data = FP_SEG(gfp_tmpSAMB) + 1;
                                    SAMB_data=0x168C
                                    FlicHdr_Width=320
                                    FlicHdr_Height=41
                                    FlicHdr_CurrentFrame=0
                                    FlicHdr_FrameCount=20
                                    FlicHdr_LoopFrame=0
                                    FlicHdr_EmmHandleNumber=6
                                    FlicHdr_EmmLogicalPageIndex=0
                                    FlicHdr_EmmLogicalPageOffset=704
                                    FlicHdr_PaletteDataOffset=102
                    gsa_MAINSCRN_0_AnimatedLogo=0x168C

        SCREEN_Menu
            FLIC_Reset_CurrentFrame(gsa_MAINSCRN_0_AnimatedLogo);
                farpokew(IMG_Seg, FLIC_HDR.Current_Frame, 0);

        FLIC_Draw_XY(0, 0, gsa_MAINSCRN_0_AnimatedLogo)


*/
// J:\STU\DBWD\developc\flilib\flidoc.txt
// /** Higher-level FLI playing functions **/ 
// /* Decompress a single frame that's in RAM */ 
// void fli_uncomp(Vscreen *f,   /* The screen to update */ 
//    Fli_frame *frame,          /* Header for this frame */ 
//    Cbuf *cbuf,                /* Compressed data for this frame */
//    Boolean see_colors);       /* update the hardware color map? */ 
// /* Read in FLI header, verify that it's a FLI file and return handle. See AAERR.H for negative return values if there are problems. */ 
// Jfile fli_open(char *fliname, Fli_head *fh); 
// /* Read in next frame and uncompress onto screen, optionally updating hardware color palette */ 
// Errval fli_read_display_frame(Jfile ff, Vscreen *v, Boolean see_colors); 
// /* Read and display next frame onto VGA display */ 
// Errval fli_next_frame(Jfile ff); 


// s28p11
// void FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header) { ... };


// s28p14
void FLIC_Set_CurrentFrame(unsigned int sa_FLIC_Header, int Frame_Index)
{
    SAMB_ptr fp_FLIC_Header;
    int Loop_Length;
    int Loop_Frame;
    int Frame_Count;
    fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    Frame_Index = (Frame_Index & 0x7FFF);
    Frame_Count = FLIC_Get_Frame_Count(fp_FLIC_Header);
    Loop_Frame = FLIC_Get_Loop_Frame(fp_FLIC_Header);
    Loop_Length = Frame_Count - Loop_Frame;  // e.g., 20 - 0 = 20
    if ( !(Frame_Index < Frame_Count) )
    {
        Frame_Index = Loop_Frame + ( (Frame_Index - Frame_Count) % Loop_Length );
    }
    FLIC_Set_Current_Frame(fp_FLIC_Header, Frame_Index);
}

// s28p15
void FLIC_Reset_CurrentFrame(SAMB_addr sa_FLIC_Header)
{
    SAMB_ptr fp_FLIC_Header;
    fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    FLIC_Set_Current_Frame(fp_FLIC_Header, 0);
}

// s28p16
word FLIC_Get_CurrentFrame(SAMB_addr sa_FLIC_Header)
{
    SAMB_ptr fp_FLIC_Header;
    word Current_Frame;
    fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    Current_Frame = FLIC_Get_Current_Frame(fp_FLIC_Header);
    return Current_Frame;
}
