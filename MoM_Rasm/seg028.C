// ~== ST_FLIC
// seg028

#include "ST_TYPE.H"    /* word, SAMB_ptr */
#include "ST_DEF.H"
#include "seg028.H"
#include "seg021.H"     /* FLIC_Load_Palette() */

#include "ST_FLIC.H"
#include "ST_SA.H"  /* SAMB_addr; farpokew(); */

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <STRING.H>     /* memcpy() */


// s28p02
// ST_FLIC.H
/*
    XREF:
        GAME_PrepareCredits()
        GAME_DrawCredits()
*/
void FLIC_Prepare(int FlicWidth, int FlicHeight, SAMB_addr FlicHdr_SgmtAddr)
{
    unsigned int FlicSize_B;
    unsigned int Dst_Sgmt;
    unsigned int Dst_Ofst;
    unsigned char * pDst;
    int itr_FlicSize_B;
    farpokew(FlicHdr_SgmtAddr, 0x00, FlicWidth);
    farpokew(FlicHdr_SgmtAddr, 0x02, FlicHeight);
    farpokew(FlicHdr_SgmtAddr, 0x04, 0xDE0A);  // DE0Ah 56842d; DEh 222d; 0Ah 10d; 0ADEh 2782d;
    farpokew(FlicHdr_SgmtAddr, 0x06, 0x00);
    farpokew(FlicHdr_SgmtAddr, 0x08, 0x00);
    FlicSize_B = FlicWidth * FlicHeight;
    Dst_Sgmt = FlicHdr_SgmtAddr;
    Dst_Ofst = 0x10;
    pDst = (unsigned char *) MK_FP(Dst_Sgmt,Dst_Ofst);
    for(itr_FlicSize_B = 0; itr_FlicSize_B < FlicSize_B; itr_FlicSize_B++)
    {
        *pDst = 0;
    }

}


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
                                    SA_Allocate_MemBlk(nparas=1);
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



void DELETE__FLIC_Draw_XY_Redux(int x, int y, SAMB_addr sa_FLIC_Header)
{
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local 
    SAMB_ptr fp_FLIC_Header;
    // unsigned int FLIC_Frame_Sgmt;
    // unsigned int FLIC_Frame_Ofst;
    int Frame_Index;
    int current_frame_index;
    // int Shading;
    // unsigned int Dst_Sgmt;
    // unsigned int Dst_Ofst;
    byte_ptr fp_Dst;
    // word loword;
    // word hiword;
    // word words_shifted;
    unsigned long int flic_frame_offset;
    // unsigned long int FLIC_Frame_Data_sgmt;
    // unsigned long int FLIC_Frame_Data_ofst;
    // word flic_frame_offset_table_position;
    word flic_frame_offset_sgmt;
    word flic_frame_offset_ofst;
    byte_ptr fp_FLIC_Frame;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_XY_Redux(x=%d, y=%d, sa_FLIC_Header=0x%04X)\n", __FILE__, __LINE__, x, y, sa_FLIC_Header);
#endif

    fp_FLIC_Header = (SAMB_ptr)MK_FP(sa_FLIC_Header,0);


    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // memcpy((void *)&PS_FLIC_Header, (const void *)fp_FLIC_Header, sizeof(PS_FLIC_Header));


    Frame_Index = PS_FLIC_Header.Current_Frame;
    current_frame_index = PS_FLIC_Header.Current_Frame;
    PS_FLIC_Header.Current_Frame += 1;
    if ( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )
    {
        DLOG("( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )");
        FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Current_Frame);
    }
    else
    {
        DLOG("( PS_FLIC_Header.Current_Frame >= PS_FLIC_Header.Frame_Count )");
        FLIC_Set_Current_Frame(fp_FLIC_Header, PS_FLIC_Header.Loop_Frame);
    }


    if ( PS_FLIC_Header.Palette_Header_Offset != 0 )
    {
        FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);
        // FLIC_Load_Palette(fp_FLIC_Header, Frame_Index);
    }

    if ( PS_FLIC_Header.EMM_Handle_Number != 0 )
    {
        DLOG("( PS_FLIC_Header.EMM_Handle_Number != 0 )");
        FLIC_Draw_EMM_C(x, y, sa_FLIC_Header, current_frame_index);
        goto Done;
    }

    // FLIC Frame Data Offset Table
    // offset to entry in table = offset to table + (size of entry * number of entries) = 18 + (4 * Frame_Index)

// 
//     Dst_Sgmt = sa_FLIC_Header;
//     // _BX = 18;
//     // _AX = Frame_Index;
//     // _AX = _AX * 2;
//     // _AX = _AX * 2;
//     // _BX = _BX + _AX;
//     Dst_Ofst = 18 + (Frame_Index * 4);
//     // _AX = GET_2B(_ES:_BX);
//     // _DX = GET_2B(_ES:_BX+2);
//     fp_Dst = MK_FP(Dst_Sgmt,Dst_Ofst);
//     loword = GET_2B_OFS(fp_Dst, 0);
//     hiword = GET_2B_OFS(fp_Dst, 2);
//     FLIC_Frame_Ofst = (loword & 0x0F) + 1;
//     FLIC_Frame_Sgmt = sa_FLIC_Header + (hiword / 16) + (loword / 16);
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] loword: 0x%04X\n", __FILE__, __LINE__, loword);
//     dbg_prn("DEBUG: [%s, %d] hiword: 0x%04X\n", __FILE__, __LINE__, hiword);
//     dbg_prn("DEBUG: [%s, %d] FLIC_Frame_Sgmt: 0x%04X\n", __FILE__, __LINE__, FLIC_Frame_Sgmt);
//     dbg_prn("DEBUG: [%s, %d] FLIC_Frame_Ofst: 0x%04X\n", __FILE__, __LINE__, FLIC_Frame_Ofst);
// #endif

    // _BX = sa_FLIC_Header + 0x12 + (4 * Frame_Index);

// asm {
//     push ds
//     push es
// 
//     mov ax, [sa_FLIC_Header]
//     mov es, ax
// 
//     mov bx, 0x12
//     mov ax, [Frame_Index]
//     shl ax, 1
//     shl ax, 1
//     add bx, ax                          //; _BX  = _BX + (Frame_Index * 2^2) .. ; _BX  = 18 + (Frame_Index * 4)
//     
//     mov [flic_frame_offset_table_position], bx
// 
//     mov ax, [es:bx]     // loword
//     mov dx, [es:bx+2]   // hiword
// 
//     mov [loword], ax
//     mov [hiword], dx
// 
//     mov bx, ax
//     and bx, 0Fh     // ?!? ~= xor bh, bh; clear the bits in the high byte of the word BX
//     inc bx
//     mov [FLIC_Frame_Ofst], bx
// 
//     shr dx, 1
//     rcr ax, 1
// 
//     shr dx, 1
//     rcr ax, 1
// 
//     shr dx, 1
//     rcr ax, 1
// 
//     shr dx, 1
//     rcr ax, 1
// 
//     mov [words_shifted], ax
// 
//     add ax, [sa_FLIC_Header]
//     mov [FLIC_Frame_Sgmt], ax
// 
//     pop es
//     pop ds
// }

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] FLIC_Frame_Sgmt: 0x%04X\n", __FILE__, __LINE__, FLIC_Frame_Sgmt);
//     dbg_prn("DEBUG: [%s, %d] FLIC_Frame_Ofst: 0x%04X\n", __FILE__, __LINE__, FLIC_Frame_Ofst);
//     dbg_prn("DEBUG: [%s, %d] (FLIC_Frame_Sgmt - sa_FLIC_Header): 0x%04X\n", __FILE__, __LINE__, (FLIC_Frame_Sgmt - sa_FLIC_Header));
// #endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] flic_frame_offset_table_position: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_table_position);
//     dbg_prn("DEBUG: [%s, %d] ( 0x12 + (4 * Frame_Index) ): 0x%04X\n", __FILE__, __LINE__, ( 0x12 + (4 * Frame_Index) ));
//     dbg_prn("DEBUG: [%s, %d] ( sa_FLIC_Header + 0x12 + (4 * Frame_Index) ): 0x%04X\n", __FILE__, __LINE__, ( sa_FLIC_Header + 0x12 + (4 * Frame_Index) ));
// #endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] fp_FLIC_Header[0x12 + (4 * Frame_Index)]: 0x%08X\n", __FILE__, __LINE__, fp_FLIC_Header[0x12 + (4 * Frame_Index)]);
//     dbg_prn("DEBUG: [%s, %d] fp_FLIC_Header[flic_frame_offset_table_position]: 0x%08X\n", __FILE__, __LINE__, fp_FLIC_Header[flic_frame_offset_table_position]);
// #endif
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] loword: 0x%04X\n", __FILE__, __LINE__, loword);
//     dbg_prn("DEBUG: [%s, %d] hiword: 0x%04X\n", __FILE__, __LINE__, hiword);
//     dbg_prn("DEBUG: [%s, %d] (loword & 0x0F): 0x%04X\n", __FILE__, __LINE__, (loword & 0x0F));
//     dbg_prn("DEBUG: [%s, %d] ((loword + 1) & 0x0F): 0x%04X\n", __FILE__, __LINE__, ((loword + 1) & 0x0F));
//     dbg_prn("DEBUG: [%s, %d] words_shifted: 0x%04X\n", __FILE__, __LINE__, words_shifted);
//     dbg_prn("DEBUG: [%s, %d] (sa_FLIC_Header + words_shifted): 0x%04X\n", __FILE__, __LINE__, (sa_FLIC_Header + words_shifted));
// #endif

    flic_frame_offset = 0x00000000;
    // flic_frame_offset = GET_4B_OFS( fp_FLIC_Header, FLIC_HDR_POS_FRAME_OFFSET_TABLE + (4 * current_frame_index) );
    flic_frame_offset = FLIC_Get_Frame_Offset(fp_FLIC_Header, current_frame_index);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset: 0x%08X\n", __FILE__, __LINE__, flic_frame_offset);
    dbg_prn("DEBUG: [%s, %d] ((flic_frame_offset + 1) & 0x0F): 0x%04X\n", __FILE__, __LINE__, ((flic_frame_offset + 1) & 0x0F));
    dbg_prn("DEBUG: [%s, %d] (flic_frame_offset >> 4): 0x%04X\n", __FILE__, __LINE__, (flic_frame_offset >> 4));
    dbg_prn("DEBUG: [%s, %d] ( sa_FLIC_Header + (flic_frame_offset >> 4) ): 0x%04X\n", __FILE__, __LINE__, ( sa_FLIC_Header + (flic_frame_offset >> 4) ));
#endif
    flic_frame_offset_sgmt = (flic_frame_offset >> 4);          // ~== flic_frame_offset / 16
    flic_frame_offset_ofst = ((flic_frame_offset & 0x0F) + 1);  // ~== flic_frame_offset % 16
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset_sgmt: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_sgmt);
    dbg_prn("DEBUG: [%s, %d] flic_frame_offset_ofst: 0x%04X\n", __FILE__, __LINE__, flic_frame_offset_ofst);
    dbg_prn("DEBUG: [%s, %d] (sa_FLIC_Header + flic_frame_offset_sgmt): 0x%04X\n", __FILE__, __LINE__, (sa_FLIC_Header + flic_frame_offset_sgmt));
#endif

    fp_FLIC_Frame = (byte_ptr)MK_FP(FP_SEG(fp_FLIC_Header)+flic_frame_offset_sgmt,flic_frame_offset_ofst);
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] fp_FLIC_Frame: %p\n", __FILE__, __LINE__, fp_FLIC_Frame);
#endif

    // fp_FLIC_Frame
    // Offset to FLIC File + Offset to FLIC Frame
    // Far Pointer Offset = (FLIC_Frame_Offset + 1) & 0x0F  AKA (FLIC_Frame_Offset + 1) % 16, where the remainder will be {0,...,15}; +1 so 0000 1111 becomes 0001 0000 



    // if ( Shading != 0 )
    if ( FLIC_Get_Remap(fp_FLIC_Header) == 0 )
    {
        DLOG("( FLIC_Get_Remap(fp_FLIC_Header) == 0 )");
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, flic_frame_offset_ofst, (sa_FLIC_Header + flic_frame_offset_sgmt));
        // FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
        FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
    }
    else
    {
        DLOG("( FLIC_Get_Remap(fp_FLIC_Header) != 0 )");
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, flic_frame_offset_ofst, (sa_FLIC_Header + flic_frame_offset_sgmt));
        // FLIC_Draw_A(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
        FLIC_Draw_R(x, y, PS_FLIC_Header.Width, FP_OFF(fp_FLIC_Frame), FP_SEG(fp_FLIC_Frame));
    }

Done:
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_XY_Redux(x=%d, Top=%d, sa_FLIC_Header=0x%04X)\n", __FILE__, __LINE__, x, y, sa_FLIC_Header);
#endif
}



void DELETE__FLIC_Draw_XY(int Left, int Top, SAMB_addr sa_FLIC_Header)
{
    static struct s_FLIC_HDR PS_FLIC_Header;  // persistent, local 
    unsigned int FLIC_Frame_Sgmt;
    unsigned int FLIC_Frame_Ofst;
    int Frame_Index;
    int Shading;

    // unsigned char _FAR *fptr_FlicHdr_SgmtAddr;
    unsigned char alt_Shading;
    //struct s_FLIC_HDR _FAR *fptr_FlicHeader;
    // struct s_FLIC_HDR _FAR *fptr_PS_FLIC_Header;
// #ifdef STU_DEBUG
//     WORD orig_DS;
// #endif

// #ifdef STU_DEBUG
//     orig_DS = _DS;
// #endif

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: FLIC_Draw_XY(Left=%d, Top=%d, sa_FLIC_Header=0x%04X)\n", __FILE__, __LINE__, Left, Top, sa_FLIC_Header);
#endif

    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x00]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x00));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x02]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x02));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x04]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x04));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x06]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x06));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x08]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x08));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x0A]: 0x%02X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x0A));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x0B]: 0x%02X\n", __FILE__, __LINE__, farpeekb(sa_FLIC_Header, 0x0B));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x0C]: 0x%04X\n", __FILE__, __LINE__, farpeekb(sa_FLIC_Header, 0x0C));
    // dbg_prn("DEBUG: [%s, %d] FLIC_HDR[0x0E]: 0x%04X\n", __FILE__, __LINE__, farpeekw(sa_FLIC_Header, 0x0E));

    // // J:\STU\DBWD\developc\_MPL13\SAMPCODE\C\OTHER\MEMORY\MOVEMEM.C
    // // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes)
    // // // // // ST_MoveData((unsigned int)&varFlicHeader, 0, 0, sa_FLIC_Header, 16);
    // // // //ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // // // fptr_FlicHeader = (struct s_FLIC_HDR _FAR *)MK_FP(sa_FLIC_Header, 0);
    // // // ST_MoveData(FP_OFF(fptr_FlicHeader), FP_SEG(fptr_FlicHeader), 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // // // // ? ST_MoveData(MK_FP(FP_SEG(PS_FLIC_Header),FP_OFF(PS_FLIC_Header)), MK_FP(sa_FLIC_Header,0), sizeof(PS_FLIC_Header));
    // // *fptr_PS_FLIC_Header = PS_FLIC_Header;
    // // ST_MoveData(FP_OFF(fptr_PS_FLIC_Header), FP_SEG(fptr_PS_FLIC_Header), 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));
    // dbg_prn("DEBUG: [%s, %d] &PS_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, &PS_FLIC_Header);
    // dbg_prn("DEBUG: [%s, %d] (unsigned int)&PS_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, (unsigned int)&PS_FLIC_Header);
    // // *fptr_PS_FLIC_Header = PS_FLIC_Header;
    // // dbg_prn("DEBUG: [%s, %d] fptr_PS_FLIC_Header: %Fp\n", __FILE__, __LINE__, fptr_PS_FLIC_Header);
    // ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_PS_FLIC_Header, sizeof(PS_FLIC_Header));
    // // //void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, size_t n);
    // // movedata(sa_FLIC_Header, 0, unsigned dstseg, unsigned dstoff, sizeof(PS_FLIC_Header));

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] orig_DS: 0x%04X\n", __FILE__, __LINE__, orig_DS);
//     // dbg_prn("DEBUG: [%s, %d] FP_SEG(PS_FLIC_Header): 0x%04X\n", __FILE__, __LINE__, FP_SEG(PS_FLIC_Header));  // Error: Incompatible type conversion
//     // dbg_prn("DEBUG: [%s, %d] FP_OFF(PS_FLIC_Header): 0x%04X\n", __FILE__, __LINE__, FP_OFF(PS_FLIC_Header));  // Error: Incompatible type conversion
//     // dbg_prn("DEBUG: [%s, %d] (unsigned int)&PS_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, (unsigned int)&PS_FLIC_Header);
//     dbg_prn("DEBUG: [%s, %d] &PS_FLIC_Header: 0x%04X\n", __FILE__, __LINE__, &PS_FLIC_Header);
// #endif

    ST_MoveData((unsigned int)&PS_FLIC_Header, 0, 0, sa_FLIC_Header, sizeof(PS_FLIC_Header));

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Width                   = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Width);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Height                  = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Height);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Current_Frame           = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Current_Frame);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Frame_Count             = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Frame_Count);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Loop_Frame              = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Loop_Frame);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.EMM_Handle_Number       = 0x%02X)\n", __FILE__, __LINE__, PS_FLIC_Header.EMM_Handle_Number);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.EMM_Logical_Page_Number = 0x%02X)\n", __FILE__, __LINE__, PS_FLIC_Header.EMM_Logical_Page_Number);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.EMM_Logical_Page_Offset = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.EMM_Logical_Page_Offset);
//     dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Palette_Header_Offset   = 0x%04X)\n", __FILE__, __LINE__, PS_FLIC_Header.Palette_Header_Offset);
// #endif

    //memcpy(PS_FLIC_Header, sa_FLIC_Header, 16);

    Frame_Index = PS_FLIC_Header.Current_Frame;
    PS_FLIC_Header.Current_Frame += 1;

//     #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Current_Frame=%d\n", __FILE__, __LINE__, PS_FLIC_Header.Current_Frame);
//         dbg_prn("DEBUG: [%s, %d] Frame_Index=%d\n", __FILE__, __LINE__, Frame_Index);
//         dbg_prn("DEBUG: [%s, %d] PS_FLIC_Header.Current_Frame=%d\n", __FILE__, __LINE__, PS_FLIC_Header.Current_Frame);
//     #endif

    if ( PS_FLIC_Header.Current_Frame < PS_FLIC_Header.Frame_Count )
    {

//         #ifdef STU_DEBUG
//             dbg_prn("DEBUG: [%s, %d] (Next Frame < Frame Count) PS_FLIC_Header.Current_Frame=%d\n", __FILE__, __LINE__, PS_FLIC_Header.Current_Frame);
//         #endif

        farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Current_Frame);
    }
    else
    {
//         #ifdef STU_DEBUG
//             dbg_prn("DEBUG: [%s, %d] (Next Frame >= Frame Count) PS_FLIC_Header.Loop_Frame=%d\n", __FILE__, __LINE__, PS_FLIC_Header.Loop_Frame);
//         #endif

        farpokew(sa_FLIC_Header, 0x04, PS_FLIC_Header.Loop_Frame);
    }

    if ( PS_FLIC_Header.Palette_Header_Offset != 0 )
    {
        // HERE("FLIC_LoadPalette()");
        // FLIC_LoadPalette(sa_FLIC_Header, Frame_Index);  // s21p07
        // FLIC_LoadPalette_Redux(sa_FLIC_Header, Frame_Index);
        // FLIC_LoadPalette_ReRedux(sa_FLIC_Header, Frame_Index);
        // FLIC_LoadPalette_ReRedux(MK_FP(sa_FLIC_Header,0), Frame_Index);
        // FLIC_LoadPalette_ReRedux(fp_FLIC_Header, Frame_Index);
        FLIC_Load_Palette(MK_FP(sa_FLIC_Header,0), Frame_Index);
    }

    if ( PS_FLIC_Header.EMM_Handle_Number == 0 )
    {
        // HERE("( PS_FLIC_Header.EMM_Handle_Number == 0 )");

//         fptr_FlicHdr_SgmtAddr = MK_FP(sa_FLIC_Header, 0);
//         // dbg_prn("DEBUG: [%s, %d] fptr_FlicHdr_SgmtAddr: %Fp\n", __FILE__, __LINE__, fptr_FlicHdr_SgmtAddr);
//         alt_Shading = (unsigned char) fptr_FlicHdr_SgmtAddr[0x11];
//         // dbg_prn("DEBUG: [%s, %d] alt_Shading: %u\n", __FILE__, __LINE__, alt_Shading);
//         // 0x12 + (4 * Frame_Index)
// // #define lbxgfx_get_frameoffs0(_data_, _frame_) (GET_LE_32(&((_data_)[0x12 + 4 * (_frame_)])))
// // #define lbxgfx_get_frameoffs1(_data_, _frame_) (GET_LE_32(&((_data_)[0x16 + 4 * (_frame_)])))
// // #define lbxgfx_get_frameptr(_data_, _frame_) ((_data_) + lbxgfx_get_frameoffs0((_data_), (_frame_)))
//         // FLIC Frame Offset (unsigned long) fptr_FlicHdr_SgmtAddr[(0x12 + (4 * Frame_Index))]
//         // ...
//         // _BX = (unsigned int) c
//         // _BX = (_BX & 0x000F)
//         // _BX = _BX + 1
//         // IMG_Huge_Off = _BX
//         // IMG_Huge_Seg = sa_FLIC_Header + ( sa_FLIC_Header / 16 )


// TODO(JimBalcomb): switch this to the Src/Dst:SgmtOfst approach
asm {
    push ds
    push es
    mov ax, [sa_FLIC_Header]
    mov es, ax
    mov bx, FlicHdr_Shading     //; 11h 17d  1oom :: lbxgfx.h :: #define lbxgfx_get_format(_data_) ((_data_)[0x11])
    mov al, [es:bx]
    xor ah, ah
    mov [Shading], ax

    inc bx                              //; _BX = 12h 18d - offset in FLIC to FLIC Frame Offset Table
    mov ax, [Frame_Index]
    shl ax, 1
    shl ax, 1
    add bx, ax                          //; _BX  = _BX + (Frame_Index * 2^2) .. ; _BX  = 18 + (Frame_Index * 4)
    mov ax, [es:bx]     // loword
    mov dx, [es:bx+2]   // hiword
    mov bx, ax
    //db 81h,0E3h,0Fh,0 ; <BAD>and  bx, 0Fh
    and bx, 0Fh     // ?!? ~= xor bh, bh; clear the bits in the high byte of the word BX
    inc bx
    mov [FLIC_Frame_Ofst], bx

    // [DX:AX] is FLIC Frame Offset High-Word:Low-Word
    // The Shift Right instruction performs a right shift on the destinations operand, filling the lowest bit with 0. The lowest bit is moved into the Carry Flag.
    // The RCR (Rotate and Carry Right) instruction shifts each bit to the right, copies the Carry flag to the most significant bit and copies the least significant bit into the Carry flag.
    // CF <-----[][][][][][][][]<-|
    // []                         |
    //  |-------------------------|
    // SHR DX,1  [][][][][][][][]->CF
    // RCR AX,1  CF->[][][][][][][][]->CF
    // ... AX = [DX3][DX2][DX1][DX0][AX15][AX14][AX13][AX12][AX11][AX10][AX9][AX8][AX7][AX6][AX5][AX4]
    // ? Why bottom 4 bits of DX ? ... max would be F000h 61440d ... /16 = 3840d 0F00h ... /16 = 240d F0h ... /16 = 15d 0Fh
    // 4, because 20 bit address bus
    // https://stackoverflow.com/questions/2877417/what-is-meant-by-normalization-in-huge-pointers#:~:text=A%20normalized%20pointer%20is%20one,when%20an%20assignment%20is%20made.
    // linear = segment * 16 + offset;
    // ...
    // / A near pointer contains only the 16 bit offset of the object within the currently selected segment. ?

    shr dx, 1    //; / 4
    rcr ax, 1    // ? AX = (AX>>1) + CF ?
    shr dx, 1    //; / 4
    rcr ax, 1
    shr dx, 1    //; / 8
    rcr ax, 1
    shr dx, 1    //; / 16  ? 16, because segments ... 0000000000000001:0000000000000000 == 0000000000000000:1000000000000000 ?
    rcr ax, 1
    add ax, [sa_FLIC_Header]
    mov [FLIC_Frame_Sgmt], ax
    pop es
    pop ds
}

        if ( Shading != 0 ) {
            // HERE("( Shading != 0 )");
// #ifdef STU_DEBUG
//             dbg_prn("DEBUG: [%s, %d] CALL: FLIC_Draw_R(Left=%d, Top=%d, PS_FLIC_Header.Width=%d, FLIC_Frame_Ofst=0x%04X, FLIC_Frame_Sgmt=0x%04X)\n", __FILE__, __LINE__, Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
// #endif
            FLIC_Draw_R(Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        } else {
            // HERE("( Shading == 0 )");
// #ifdef STU_DEBUG
//             dbg_prn("DEBUG: [%s, %d] CALL: FLIC_Draw_A(Left=%d, Top=%d, PS_FLIC_Header.Width=%d, FLIC_Frame_Ofst=0x%04X, FLIC_Frame_Sgmt=0x%04X)\n", __FILE__, __LINE__, Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
// #endif
            FLIC_Draw_A(Left, Top, PS_FLIC_Header.Width, FLIC_Frame_Ofst, FLIC_Frame_Sgmt);
        }
    } else {
        // HERE("( PS_FLIC_Header.EMM_Handle_Number != 0 )");
// #ifdef STU_DEBUG
//         dbg_prn("DEBUG: [%s, %d] CALL: FLIC_Draw_EMM(Left=%d, Top=%d, sa_FLIC_Header=0x%04X, Frame_Index=%d)\n", __FILE__, __LINE__, Left, Top, sa_FLIC_Header, Frame_Index);
// #endif
        //FLIC_Draw_EMM_A(Left, Top, sa_FLIC_Header, Frame_Index);
        //FLIC_Draw_EMM_A2(Left, Top, sa_FLIC_Header, Frame_Index);
        //FLIC_Draw_EMM_A3(Left, Top, sa_FLIC_Header, Frame_Index);
        FLIC_Draw_EMM_C(Left, Top, sa_FLIC_Header, Frame_Index);
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: FLIC_Draw_XY(Left=%d, Top=%d, sa_FLIC_Header=0x%04X)\n", __FILE__, __LINE__, Left, Top, sa_FLIC_Header);
#endif
}

// s28p14
void FLIC_Set_CurrentFrame(unsigned int sa_FLIC_Header, int Frame_Index)
{
    SAMB_ptr fp_FLIC_Header;
    int Loop_Length;
    int Loop_Frame;
    int Frame_Count;
    Frame_Index = (Frame_Index & 0x7FFF);
    Frame_Count = farpeekw(sa_FLIC_Header, FlicHdr_FrameCount);  // FLIC_HDR.Frame_Count
    Loop_Frame = farpeekw(sa_FLIC_Header, FlicHdr_LoopFrame);  // FLIC_HDR.Loop_Frame
    Loop_Length = Frame_Count - Loop_Frame;  // e.g., 20 - 0 = 20
    if ( !(Frame_Index < Frame_Count) )
    {
        Frame_Index = Loop_Frame + ( (Frame_Index - Frame_Count) % Loop_Length );
    }
    farpokew(sa_FLIC_Header, FlicHdr_CurrentFrame, Frame_Index);  // FLIC_HDR.Current_Frame
    // fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    // FLIC_Set_Current_Frame(fp_FLIC_Header, Frame_Index);
}

// s28p15
void FLIC_Reset_CurrentFrame(SAMB_addr sa_FLIC_Header)
{
    SAMB_ptr fp_FLIC_Header;
    // farpokew(sa_FLIC_Header, 0x04, 0x00);
    fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    FLIC_Set_Current_Frame(fp_FLIC_Header, 0);
}

// s28p16
word FLIC_Get_CurrentFrame(SAMB_addr sa_FLIC_Header)
{
    SAMB_ptr fp_FLIC_Header;
    word Current_Frame;
    // Current_Frame = farpeekw(sa_FLIC_Header, FlicHdr_CurrentFrame);
    fp_FLIC_Header = MK_FP(sa_FLIC_Header,0);
    Current_Frame = FLIC_Get_Current_Frame(fp_FLIC_Header);
    return Current_Frame;
}
