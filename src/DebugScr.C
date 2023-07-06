
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "DebugScr.H"

/*

Test Cases:
    Copy Screen To Picture To Screen
        Screen_Picture_Capture()
            |-> Set_Page_Off()()
            |-> Create_Picture()
            |-> Capture_Screen_Block()


FLIC_Draw
    // WZD s24p08
    void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, SAMB_ptr pict_seg)
    // WZD s24p09
    void Capture_Screen_Block(byte_ptr frame_data, int16_t x1, int16_t y1, int16_t x2, int16_t y2)

*/


char screen_title[] = "-- LOOPHOLES --";
char screen_caption[] = "YOU SHOULD NOT BE HERE";



void Debug_Screen(void);
void Draw_Debug_Screen(void);



void Debug_Screen(void)
{
    int16_t leave_screen_flag;
    int16_t screen_change_flag;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Debug_Screen()\n", __FILE__, __LINE__);
#endif

    leave_screen_flag = ST_FALSE;
    screen_change_flag = ST_FALSE;

    Draw_Debug_Screen();
    Toggle_Pages();

    while(leave_screen_flag == ST_FALSE)
    {

        input_field_num = Get_Input();



        if(leave_screen_flag == ST_FALSE && screen_change_flag == ST_FALSE)
        {
            Draw_Debug_Screen();
            Toggle_Pages();
        }

    }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Debug_Screen()\n", __FILE__, __LINE__);
#endif
}

void Draw_Debug_Screen(void)
{
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Draw_Debug_Screen()\n", __FILE__, __LINE__);
#endif



#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Draw_Debug_Screen()\n", __FILE__, __LINE__);
#endif
}
