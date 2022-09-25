// _s28p1c.c FLIC_SetFrame
// ST_FLIC.H

#include "ST_FLIC.H"

// #ifdef STU_DEBUG
// #include "STU_DBG.H"
// #endif


 void FLIC_SetFrame(unsigned int FlicHdr_SgmtAddr, int Frame_Index)
 {
    int Loop_Length;
    int Loop_Frame;
    int Frame_Count;

    unsigned int tmp_DI;
    int tmp_SI;

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: FLIC_SetFrame(FlicHdr_SgmtAddr=0x%04X, Frame_index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
// #endif

    tmp_DI = FlicHdr_SgmtAddr;

    tmp_SI = Frame_Index;
    tmp_SI = (tmp_SI & 0x7FFF);

    Frame_Count = farpeekw(tmp_DI, FlicHdr_FrameCount);  // FLIC_HDR.Frame_Count
    Loop_Frame = farpeekw(tmp_DI, FlicHdr_LoopFrame);  // FLIC_HDR.Loop_Frame

    Loop_Length = Frame_Count - Loop_Frame;  // e.g., 20 - 0 = 20
    
    if ( !(tmp_SI < Frame_Count) )
    {
        // HERE("if ( !(tmp_SI < Frame_Count) )");
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

    farpokew(tmp_DI, FlicHdr_CurrentFrame, tmp_SI);  // FLIC_HDR.Current_Frame

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] Frame_index=%d, tmp_SI=%d, Frame_Count=%d, Loop_Frame=%d, Loop_Length=%d)\n", __FILE__, __LINE__, Frame_Index, tmp_SI, Frame_Count, Loop_Frame, Loop_Length);
// #endif

// #ifdef STU_DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: FLIC_SetFrame(FlicHdr_SgmtAddr=0x%04X, Frame_index=%d)\n", __FILE__, __LINE__, FlicHdr_SgmtAddr, Frame_Index);
// #endif
}
