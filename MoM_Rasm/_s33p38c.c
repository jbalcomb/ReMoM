// _s33p38c.c CRH_Draw_DSP

#include "ST_CRSR.H"
#include "ST_GUI.H"


void CRH_Draw_DSP_C(int X_Pos, int Y_Pos)
{

    if (g_CRSR_Curr != 0)
    {
        CRL_Draw_DSP(X_Pos, Y_Pos, g_CRSR_Curr);  // _BX = X_Pos, _CX = Y_Pos, _DX = g_CRSR_Curr
    }

}
