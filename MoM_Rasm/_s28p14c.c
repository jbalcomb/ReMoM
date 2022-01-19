// _s28p1c.c FLIC_SetFrame

#include "STU_DBG.H"


 void FLIC_SetFrame(unsigned int FlicHdr_SgmtAddr, int Frame_Index)
 {
    int Loop_Length;
    int Loop_Frame;
    int Frame_Count;

    unsigned int tmp_DI;
    int tmp_SI;

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_SetFrame(FlicHdr_SgmtAddr=0x%04X, Frame_index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
#endif

    tmp_DI = FlicHdr_SgmtAddr;

    tmp_SI = Frame_Index;
    tmp_SI = (tmp_SI & 0x7FFF);

    Frame_Count = farpeekw(tmp_DI, 0x06);  // FLIC_HDR.Frame_Count
    Loop_Frame = farpeekw(tmp_DI, 0x08);  // FLIC_HDR.Loop_Frame

    Loop_Length = Frame_Count - Loop_Frame;

    // DBGLOG_S16("DEBUG: [%s, %d]: Frame_Count = %u\n", __FILE__, __LINE__, Frame_Count);
    // DBGLOG_S16("DEBUG: [%s, %d]: Loop_Frame = %u\n", __FILE__, __LINE__, Loop_Frame);
    // DBGLOG_S16("DEBUG: [%s, %d]: Loop_Length = %u\n", __FILE__, __LINE__, Loop_Length);    
    
    if ( !(tmp_SI < Frame_Count) )
    {
        HERE("if ( !(tmp_SI < Frame_Count) )");
        // mov  ax, si
        // sub  ax, [bp+Frame_Count]
        // xor  dx, dx
        // div  [bp+Loop_Length]
        // mov  ax, [bp+Loop_Frame]
        // add  ax, dx
        // mov  si, ax
        // // Computes quotient
        // quotient = dividend / divisor;
        // // Computes remainder
        // remainder = dividend % divisor;
        //...
        // (tmp_SI - Frame_Count)
        // DX:AX = ( (tmp_SI - Frame_Count) / Loop_Length )
        // SI = AX = Loop_Frame + ( (tmp_SI - Frame_Count) % Loop_Length )
        // ?
        tmp_SI = Loop_Frame + ( (tmp_SI - Frame_Count) % Loop_Length );
    }

    // DBGLOG_S16("DEBUG: [%s, %d]: FLIC_HDR.Current_Frame = %u\n", __FILE__, __LINE__, tmp_SI);
    farpokew(tmp_DI, 0x04, tmp_SI);  // FLIC_HDR.Current_Frame

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] END: FLIC_SetFrame(FlicHdr_SgmtAddr=0x%04X, Frame_index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
#endif
}
