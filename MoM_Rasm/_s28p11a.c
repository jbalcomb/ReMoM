// _s28p11a.c VGA_DrawLBXImage
// ST_FLIC.H

//#pragma inline

#include "ST_FLIC.H"

#include "STU_DBG.H"


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
            FLIC_ResetFrame(gsa_MAINSCRN_0_AnimatedLogo);
                farpokew(IMG_Seg, FLIC_HDR.Current_Frame, 0);

        VGA_DrawLBXImage(0, 0, gsa_MAINSCRN_0_AnimatedLogo)


*/

void VGA_DrawLBXImage(int Left, int Top, unsigned int FlicHdr_SgmtAddr)
{
    static struct s_FLIC_HDR FLIC_Header;
    unsigned int IMG_Huge_Seg;
    unsigned int IMG_Huge_Off;
    int Frame_Index;
    int Shading;

    int tmp_SI;
    int tmp_DI;
    unsigned char _FAR *fptr_FlicHdr_SgmtAddr;
    unsigned char alt_Shading;
    //struct s_FLIC_HDR _FAR *fptr_FlicHeader;
    struct s_FLIC_HDR _FAR *fptr_FLIC_Header;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: VGA_DrawLBXImage(Left=%d, Top=%d, FlicHdr_SgmtAddr=0x%04X)\n", __FILE__, __LINE__, Left, Top, FlicHdr_SgmtAddr);
#endif

    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x00]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x00));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x02]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x02));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x04]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x04));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x06]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x06));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x08]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x08));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x0A]: 0x%02X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x0A));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x0B]: 0x%02X\n", __FILE__, __LINE__, farpeekb(FlicHdr_SgmtAddr, 0x0B));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x0C]: 0x%04X\n", __FILE__, __LINE__, farpeekb(FlicHdr_SgmtAddr, 0x0C));
    // dlvfprintf("DEBUG: [%s, %d] FLIC_HDR[0x0E]: 0x%04X\n", __FILE__, __LINE__, farpeekw(FlicHdr_SgmtAddr, 0x0E));

    tmp_SI = Left;
    tmp_DI = Top;

    // // J:\STU\DBWD\developc\_MPL13\SAMPCODE\C\OTHER\MEMORY\MOVEMEM.C
    // // int ST_MoveData(unsigned int destoff, unsigned int destseg, unsigned int srcoff, unsigned int srcseg, unsigned int nbytes)
    // // // // // ST_MoveData((unsigned int)&varFlicHeader, 0, 0, FlicHdr_SgmtAddr, 16);
    // // // //ST_MoveData((unsigned int)&FLIC_Header, 0, 0, FlicHdr_SgmtAddr, sizeof(FLIC_Header));
    // // // fptr_FlicHeader = (struct s_FLIC_HDR _FAR *)MK_FP(FlicHdr_SgmtAddr, 0);
    // // // ST_MoveData(FP_OFF(fptr_FlicHeader), FP_SEG(fptr_FlicHeader), 0, FlicHdr_SgmtAddr, sizeof(FLIC_Header));
    // // // // ? ST_MoveData(MK_FP(FP_SEG(FLIC_Header),FP_OFF(FLIC_Header)), MK_FP(FlicHdr_SgmtAddr,0), sizeof(FLIC_Header));
    // // *fptr_FLIC_Header = FLIC_Header;
    // // ST_MoveData(FP_OFF(fptr_FLIC_Header), FP_SEG(fptr_FLIC_Header), 0, FlicHdr_SgmtAddr, sizeof(FLIC_Header));
    // dlvfprintf("DEBUG: [%s, %d] &FLIC_Header: 0x%04X\n", __FILE__, __LINE__, &FLIC_Header);
    // dlvfprintf("DEBUG: [%s, %d] (unsigned int)&FLIC_Header: 0x%04X\n", __FILE__, __LINE__, (unsigned int)&FLIC_Header);
    // // *fptr_FLIC_Header = FLIC_Header;
    // // dlvfprintf("DEBUG: [%s, %d] fptr_FLIC_Header: %Fp\n", __FILE__, __LINE__, fptr_FLIC_Header);
    // ST_MoveData((unsigned int)&FLIC_Header, 0, 0, FlicHdr_SgmtAddr, sizeof(FLIC_Header));
    // // //void movedata(unsigned srcseg, unsigned srcoff, unsigned dstseg, unsigned dstoff, size_t n);
    // // movedata(FlicHdr_SgmtAddr, 0, unsigned dstseg, unsigned dstoff, sizeof(FLIC_Header));
    ST_MoveData((unsigned int)&FLIC_Header, 0, 0, FlicHdr_SgmtAddr, sizeof(FLIC_Header));

    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Width                   = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Width);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Height                  = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Height);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Current_Frame           = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Current_Frame);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Frame_Count             = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Frame_Count);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Loop_Frame              = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Loop_Frame);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.EMM_Handle_Number       = 0x%02X)\n", __FILE__, __LINE__, FLIC_Header.EMM_Handle_Number);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.EMM_Logical_Page_Number = 0x%02X)\n", __FILE__, __LINE__, FLIC_Header.EMM_Logical_Page_Number);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.EMM_Logical_Page_Offset = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.EMM_Logical_Page_Offset);
    // dlvfprintf("DEBUG: [%s, %d] FLIC_Header.Palette_Data_Offset     = 0x%04X)\n", __FILE__, __LINE__, FLIC_Header.Palette_Data_Offset);

    //memcpy(FLIC_Header, FlicHdr_SgmtAddr, 16);

    Frame_Index = FLIC_Header.Current_Frame;
    FLIC_Header.Current_Frame += 1;

    if ( FLIC_Header.Current_Frame < FLIC_Header.Frame_Count )
    {
        farpokew(FlicHdr_SgmtAddr, 0x04, FLIC_Header.Current_Frame);
    }
    else
    {
        farpokew(FlicHdr_SgmtAddr, 0x04, FLIC_Header.Loop_Frame);
    }

    if ( FLIC_Header.Palette_Data_Offset != 0 )
    {
        HERE("FLIC_LoadPalette()");
        FLIC_LoadPalette(FlicHdr_SgmtAddr, Frame_Index);  // _s21p07c.c
    }

    if ( FLIC_Header.EMM_Handle_Number == 0 )
    {
        HERE("( FLIC_Header.EMM_Handle_Number == 0 )");

        fptr_FlicHdr_SgmtAddr = MK_FP(FlicHdr_SgmtAddr, 0);
        // dlvfprintf("DEBUG: [%s, %d] fptr_FlicHdr_SgmtAddr: %Fp\n", __FILE__, __LINE__, fptr_FlicHdr_SgmtAddr);
        alt_Shading = (unsigned char) fptr_FlicHdr_SgmtAddr[0x11];
        // dlvfprintf("DEBUG: [%s, %d] alt_Shading: %u\n", __FILE__, __LINE__, alt_Shading);
        // 0x12 + (4 * Frame_Index)
// #define lbxgfx_get_frameoffs0(_data_, _frame_) (GET_LE_32(&((_data_)[0x12 + 4 * (_frame_)])))
// #define lbxgfx_get_frameoffs1(_data_, _frame_) (GET_LE_32(&((_data_)[0x16 + 4 * (_frame_)])))
// #define lbxgfx_get_frameptr(_data_, _frame_) ((_data_) + lbxgfx_get_frameoffs0((_data_), (_frame_)))
        // FLIC Frame Offset (unsigned long) fptr_FlicHdr_SgmtAddr[(0x12 + (4 * Frame_Index))]
        // ...
        // _BX = (unsigned int) c
        // _BX = (_BX & 0x000F)
        // _BX = _BX + 1
        // IMG_Huge_Off = _BX
        // IMG_Huge_Seg = FlicHdr_SgmtAddr + ( FlicHdr_SgmtAddr / 16 )

asm {
    push ds
    push es
    mov ax, [FlicHdr_SgmtAddr]
    mov es, ax
    mov bx, 11h
    mov al, [es:bx]
    xor ah, ah
    mov [Shading], ax
    inc bx                              //; _BX = 12h 18d
    mov ax, [Frame_Index]
    shl ax, 1
    shl ax, 1
    add bx, ax                          //; _BX  = _BX + (Frame_Index * 2^2) .. ; _BX  = 18 + (Frame_Index * 4)
    mov ax, [es:bx]
    mov dx, [es:bx+2]
    mov bx, ax
    //db 81h,0E3h,0Fh,0 ; <BAD>and  bx, 0Fh
    and bx, 0Fh
    inc bx
    mov [IMG_Huge_Off], bx
    shr dx, 1    //; / 2
    rcr ax, 1
    shr dx, 1    //; / 4
    rcr ax, 1
    shr dx, 1    //; / 8
    rcr ax, 1
    shr dx, 1    //; / 16
    rcr ax, 1
    add ax, [FlicHdr_SgmtAddr]
    mov [IMG_Huge_Seg], ax
    pop es
    pop ds
}

        if ( Shading != 0 )
        {
            HERE("( Shading != 0 )");

            HERE("VGA_DrawBitmap_R()");
            //VGA_DrawBitmap_R(tmp_SI, tmp_DI, FLIC_Header.Width, IMG_Huge_Off, IMG_Huge_Seg);
        }
        else
        {
            HERE("( Shading == 0 )");

            HERE("VGA_DrawBitmap()");
            VGA_DrawBitmap(tmp_SI, tmp_DI, FLIC_Header.Width, IMG_Huge_Off, IMG_Huge_Seg);
        }
    }
    else
    {
        HERE("( FLIC_Header.EMM_Handle_Number != 0 )");
        HERE("VGA_DrawEMSBitmap()");
        //VGA_DrawEMSBitmap(tmp_SI, tmp_DI, FlicHdr_SgmtAddr, Frame_Index);
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: VGA_DrawLBXImage()\n", __FILE__, __LINE__);
#endif
}
