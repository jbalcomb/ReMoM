
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Allocate.H"
#include "Fields.H"
#include "Input.H"
#include "Keyboard.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif



uint8_t g_Key_Pressed;
uint16_t g_Last_Key_Pressed;




#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "Input.H"


/*
    MGC  seg033
*/

// TODO  // MGC s33p01
// TODO  Set_Mouse_List()

// TODO  // MGC s33p02
// TODO  Check_Mouse_Shape()

// TODO  // MGC s33p03

// TODO  // MGC s33p04
// TODO  // Get_Pointer_Offset()

// TODO  // MGC s33p07
// TODO  // MD_Init()

// TODO  // MGC s33p09
// TODO  // MD_Reset()

// TODO  // MGC s33p09
// TODO  // MD_Button_Hanlder()

// TODO  // MGC s33p11
// TODO  // MD_Draw_Disable()
// TODO  // MGC s33p12
// TODO  // MD_Draw_Restore()



/*
    MGC  seg034
*/

// MGC s34p01
int16_t Interpret_Mouse_Input(void)
{
    int16_t field_num;
    int16_t character;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Interpret_Mouse_Input()\n", __FILE__, __LINE__);
#endif

    if(Keyboard_Status() == ST_TRUE)  // MGC s33p16
    {
        DLOG("(Keyboard_Status() == ST_TRUE)");
        character = Interpret_Keyboard_Input(&field_num);  // MGC s34p22
    }
    else
    {
        DLOG("(Keyboard_Status() != ST_TRUE)");
    }

#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] character: %d\n", __FILE__, __LINE__, character);
#endif
#ifdef DEBUG
    dlvfprintf("DEBUG: [%s, %d] field_num: %d\n", __FILE__, __LINE__, field_num);
#endif

    // TODO  if(character != 0)
    // TODO  {
    // TODO      
    // TODO  }


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Interpret_Mouse_Input() { field_num = %d }\n", __FILE__, __LINE__, field_num);
#endif

    return field_num;
}

// MGC s34p65
void Init_Mouse_Keyboard(int16_t input_type)
{
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] BEGIN: Init_Mouse_Keyboard(Input_Type=%d)\n", __FILE__, __LINE__, Input_Type);
// #endif

    p_fields = (void *)Allocate_Space(357);  // 357 paragraphs = 367 * 16 = 5712 bytes  (? 150*38=5700 ? + 12 ?)

    // TODO  // GUI_SetWindows(1, g_GUI_VirtualScreen);  // _s33p01  ST_GUI.H
    // TODO  Set_Mouse_List(1, default_mouse);

    // TODO  g_MD_Hardware = MD_Init();  // _s33p07  int MOUSE_Init(void)  ST_GUI.H
    
    // TODO  IN_CRL_Save_RSP(158, 100);
    // TODO  input_delay = 0;
    // TODO  g_CTRL_Focused = ST_UNDEFINED;
    // TODO  _global_esc = ST_FALSE;
    Clear_Fields();

    
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: Init_Mouse_Keyboard()\n", __FILE__, __LINE__);
// #endif
}

// MGC s34p66
int16_t Get_Input(void)
{

    int16_t field_index;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Get_Input()\n", __FILE__, __LINE__);
#endif

    // TODO if(input_delay > 0)
    // TODO if(input_delay < 0)
    // TODO if(fields_count <= 1)

    field_index = Interpret_Mouse_Input();  // MGC s34p01

    // TODO Set_Page_Off();  // MGC s26p02

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Input() { field_index = %d }\n", __FILE__, __LINE__, field_index);
#endif

    return field_index;
}
