// _o60p01c.c GAME_DrawCredits
// MGC_DEF.H

#include "MGC_DEF.H"


int GAME_DrawCredits(void)
{

    int Role_String_Width;
    int Loop_Var;
    unsigned char Credit_FadeIn_Colors[10];
    unsigned char Credit_FadeOut_Colors[10];
    unsigned char Color_Array[4];

    int tmp_SI;
    int tmp_DI;

    GAME_CredsRollTimer = GAME_CredsRollTimer + 1;

    if ( GAME_CredsRollTimer > 30000 )
    {
        GAME_CredsRollTimer = 70;
    }

    if ( GAME_CredsRollTimer < 70 )
    {
        return 0;
    }

    Color_Array[0] = 68;  // 0x44

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        Credit_FadeOut_Colors[Loop_Var] = 224 + Loop_Var;
    }

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        Credit_FadeOut_Colors[Loop_Var] = 233 - Loop_Var;
    }

    Credit_FadeIn_Colors[0] = 232;

    FLIC_Prepare(280, 122, gsa_GUI_Credits_Text_IMG);

    VGA_SetDrawWindow(0, 40, 319, 137);

    tmp_SI = GAME_CreditTextTop;

    for ( Loop_Var = 0; Loop_Var < 10; Loop_Var++ )
    {
        
        if ( tmp_SI < 99 )
        {

            if ( GAME_FirstShownCred + Loop_Var < 48 )
            {

                Color_Array[1] = 224;

                if ( tmp_SI < 10 )
                {
                    Color_Array[1] = Credit_FadeOut_Colors[10 - tmp_SI];
                }

                if ( tmp_SI > 88 )
                {
                    Color_Array[1] = Credit_FadeIn_Colors[10 - (tmp_SI + -88)];
                }

                //VGA_SetFontnColor(2, &Color_Array);

                //LBX_DrawTextLine(0, tmp_SI, TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Role, gsa_GUI_Credits_Text_IMG);

                Role_String_Width = VGA_GetStringWidth(TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Role);

                if ( Role_String_Width > 1 )
                {
                    for ( tmp_DI = Role_String_Width + 1; tmp_DI < 102; tmp_DI += 4 )
                    {
                        //LBX_DrawTextLine(tmp_DI, tmp_SI, cnst_Credits_DOT, gsa_GUI_Credits_Text_IMG);
                    }
                }

                //LBX_DrawTextLine(105, tmp_SI, TBL_Credits_Texts[GAME_FirstShownCred + Loop_Var].Name, gsa_GUI_Credits_Text_IMG);
                tmp_SI = tmp_SI + 10;

            }
        }

    }

    GAME_CreditTextTop = GAME_CreditTextTop - 1;

    if ( GAME_CreditTextTop < 1 )
    {
        GAME_FirstShownCred = GAME_FirstShownCred + 1;
        GAME_CreditTextTop = GAME_CreditTextTop + 10;

        if ( GAME_FirstShownCred > 48 )
        {
            GAME_CredsRollTimer = 0;
            GAME_FirstShownCred = 0;
            GAME_CreditTextTop = 95;
        }
    }

    //VGA_WndOverlayLBX(85, 35, gsa_GUI_Credits_Text_IMG);
    VGA_ResetDrawWindow();

}
