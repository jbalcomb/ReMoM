
#include "CREDITS.H"

#include "MOX/Allocate.H"
#include "MOX/LBX_Load.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/SOUND.H"

#include "MGC_DATA.H"



/*
    MAGIC.EXE ovr060
*/

// MGC o60p1
// drake178: ¿ ?
// MoO2: ¿ ?
// 1oom: ¿ ?
/*
increases the credit roll timer and, if it is above
70, composes and overlays a corresponding credits
text image into the current draw frame
MGC_DEF.H
*/
/*

*/
void Draw_Credits(void)
{
    int16_t Role_String_Width = 0;
    int16_t itr = 0;
    uint8_t Credit_FadeIn_Colors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t Credit_FadeOut_Colors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t colors[4] = {0, 0, 0, 0};
    int16_t line_top = 0;  // _SI_

    GAME_CredsRollTimer++;

    if(GAME_CredsRollTimer > 30000)
    {
        GAME_CredsRollTimer = 70;
    }

    if(GAME_CredsRollTimer < 70)
    {
        return;
    }

    colors[0] = 68;

    for(itr = 0; itr < 10; itr++)
    {
        Credit_FadeOut_Colors[(itr)] = (224 + itr);
    }

    for(itr = 0; itr < 10; itr++)
    {
        Credit_FadeIn_Colors[(itr)] = (233 + itr);
    }

    Credit_FadeIn_Colors[0] = 232;

    Create_Picture(280, 122, gsa_GUI_Credits_Text_IMG);

    Set_Window(0, 40, 319, 137);

    line_top = GAME_CreditTextTop;

    for(itr = 0; itr < 10; itr++)
    {

        if(
            (line_top < 99)
            &&
            ((GAME_FirstShownCred + itr) < 48)
        )
        {

            colors[1] = 224;

            if(line_top < 10)
            {

                colors[1] = Credit_FadeOut_Colors[(10 - line_top)];
                
            }

            if(line_top > 88)
            {

                colors[1] = Credit_FadeIn_Colors[(10 - (line_top - 88))];

            }

            Set_Font_Colors_15(2, &colors[0]);

            Print_To_Bitmap(0, line_top, TBL_Credits_Texts[(GAME_FirstShownCred + itr)].Role, gsa_GUI_Credits_Text_IMG);

            Role_String_Width = Get_String_Width(TBL_Credits_Texts[(GAME_FirstShownCred + itr)].Role);

            if(Role_String_Width > 1)
            {

            }


            
        }

    }




}


// MGC o60p2
// drake178: ¿ ?
// MoO2: ¿ ?
// 1oom: ¿ ?
/*
allocates and prepares an image segment for composing
the rolling credits text (size 280x122) as the first
allocation in the sandbox, and resets the drawing
control values
*/
/*

*/
void Init_Credits(void)
{
    GAME_FirstShownCred = 0;
    GAME_CreditTextTop = 95;
    gsa_GUI_Credits_Text_IMG = Allocate_First_Block(_screen_seg, 3125);  // 3125 PR, 50000 B
    Create_Picture(280, 122, gsa_GUI_Credits_Text_IMG);
}
