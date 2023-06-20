
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM.hpp"

#include "Allocate.H"
#include "Fields.H"
#include "Input.H"
#include "Keyboard.H"
#include "Mouse.H"
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

/*
    WIZARDS.EXE
    seg035
    seg036

    ? seg0035 vs. seg036 ?
    Hi/Lo?
    C/Asm?
    
    Input vs. Keyboard vs. Mouse vs. Fields vs. ?
    ?!? redraw functions and Toggle_Pages() ?!?

*/


uint8_t g_Key_Pressed;          // Platform - Decl in MoM.hpp
uint16_t g_Last_Key_Pressed;    // Platform - Decl in MoM.hpp
uint16_t scan_code_char_code;   // Platform - Decl in MoM.hpp


// WZD dseg:824E
// MGC dseg:4D20
// DONT  mouse_installed = ST_FALSE;                     

// WZD dseg:8250
// ? click, hold drag ? e.g., magic staves ?
int16_t down_mouse_button = ST_UNDEFINED;

// WZD dseg:8272
// MGC dseg:4D44
int16_t _global_esc = ST_FALSE;


int16_t mouse_list_init_count = 1;
// WZD dseg:825C
struct s_mouse_list mouse_list_init[1] = {
    {0, 0, 0, 0, 319, 199}
};


// WZD dseg:E898
int16_t input_delay;



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
    WZD  seg036
    MGC  seg034
*/

// WZD s36p01
// MGC s34p01
int16_t Interpret_Mouse_Input(void)
{
    int16_t field_num;
    int16_t character;
    int16_t mx;
    int16_t my;
    int16_t pointer_offset;
    int16_t down_mouse_button;
    int16_t mouse_field;
    int16_t MD_ButtonStatus;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Interpret_Mouse_Input()\n", __FILE__, __LINE__);
#endif

    field_num = 0;
    down_mouse_button = ST_UNDEFINED;
    // MD_ButtonStatus = 0;
    mouse_field = 0;

    mx = Pointer_X();
    my = Pointer_Y();
    pointer_offset = Get_Pointer_Offset();


    if(Keyboard_Status() == ST_TRUE)  // MGC s33p16
    {
        DLOG("(Keyboard_Status() == ST_TRUE)");
        character = Interpret_Keyboard_Input(&field_num);  // MGC s34p22
    }
    else
    {
        DLOG("(Keyboard_Status() != ST_TRUE)");
        // Mouse_Movement_Handler();
        // WZD seg035
        if(MD_GetButtonStatus() == ST_FALSE)
        {
            // if(MD_Get_ClickRec1() == ST_FALSE)
            // {
            //     Mouse_Button_Handler();
            //     return 0;
            // }
            // else
            // {
            //     Mouse_Btn_Clicked = MD_GetClickedBtns();
            //     // if ...
            //     // Mouse_Btn_Clicked == 2
            //     // have_help == ST_FALSE
            //     // GUI_ContextBasedHelp() != ST_FALSE
            //     // _global_esc != ST_FALSE
            // }
            // TODO(JimBalcomb,20230619): Y/N/M do the whole ClickRec business here (there's a whole to it)
        }
        else
        {
            /*
                ProgramPath:
                    KBD_CheckBuffer == 0
                    MDI_Mv()
                    MD_INT_SetMvOnly()
                    MD_GetButtonStatus() != 0
            */
            MD_ButtonStatus = MD_GetButtonStatus();
        }

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

// WZD s36p25
int16_t Scan_Field(void)
{
    int16_t mx;
    int16_t my;
    int16_t itr;
    int16_t current_field;
    int16_t xmin;
    int16_t ymin;
    int16_t xmax;
    int16_t ymax;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Scan_Field()\n", __FILE__, __LINE__);
#endif

    mx = Pointer_X();
    my = Pointer_Y();

    current_field = 0;  // ? ST_NULL ?

    Check_Mouse_Shape(mx, my);
    cursor_offset = Get_Pointer_Offset();

    for(itr= 1; itr < fields_count; itr++)
    {
        xmin = p_fields[itr].x1;
        ymin = p_fields[itr].y1;
        xmax = p_fields[itr].x2;
        ymax = p_fields[itr].y2;

        if(
            (mx + cursor_offset >= xmin) &&
            (mx + cursor_offset <= xmax) &&
            (my + cursor_offset >= ymin) &&
            (my + cursor_offset <= ymax)
        )
        {
            current_field = itr;
            break;
        }
    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Scan_Field() { current_field = %d }\n", __FILE__, __LINE__, current_field);
#endif

    return current_field;
}

// WZD s36p26
int16_t Scan_Input(void)
{
    int16_t mx;
    int16_t my;
    int16_t itr;
    int16_t current_field;
    // MoO2 int16_t xmin;
    // MoO2 int16_t ymin;
    // MoO2 int16_t xpos;
    // MoO2 int16_t ypos;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Scan_Field()\n", __FILE__, __LINE__);
#endif

    mx = Pointer_X();
    my = Pointer_Y();

    current_field = 0;  // ? ST_NULL ?

    Check_Mouse_Shape(mx, my);
    cursor_offset = Get_Pointer_Offset();

    current_field = Scan_Field();

    /*
        Field Type: ? //drake178 ClickLink  DNE in MoO2
    */
    if(p_fields[current_field].type == ft_ClickLink)
    {
        *((int16_t *)p_fields[itr].Param2) = p_fields[itr].Param1;
        current_field = p_fields[itr].Param0;
    }

    /*
        Field Type: String List
    */
    /*
        Field Type: Grid
    */
    /*
        Another climb up Mt. Ifferest...

        p_fields[current_field].type == ft_StringList
        p_fields[itr].Selectable != ST_FALSE
        ...return 0
        p_fields[current_field].type == ft_Grid
        down_mouse_button == ST_UNDEFINED
        p_fields[down_mouse_button].type == ft_Grid
        ...return current_field

    */
    // TODO(JimBalcomb,20230620): make a home for notes like this - code oddities, program-flow for (major) components, gotchas, 
    /*
        What's up with the casting of the pointers/values for the field parameters?

        e.g.,
            the Grid Field for the Movement Map
                in the call to add Add_Grid_Field() it passes the address for _main_map_grid_x and _main_map_grid_y
                Add_Grid_Field() then sets the value for Param3 and Param4 as the address of _main_map_grid_x and _main_map_grid_y, respectively
                later, here in the call to Scan_Input() ...
                ? it updates the values stored in _main_map_grid_x and _main_map_grid_y ?

    */
    // if(p_fields[current_field].type == ft_StringList && p_fields[itr].Selectable != ST_FALSE)
    if(p_fields[current_field].type == ft_Grid)
    {
        *((int16_t *)p_fields[itr].Param3) = ( (Pointer_X() - p_fields[itr].x1) / p_fields[itr].Param1 );
        *((int16_t *)p_fields[itr].Param4) = ( (Pointer_Y() - p_fields[itr].y1) / p_fields[itr].Param2 );
    }

    return current_field;
}

// WZD s36p65
// IN_Init()
void Init_Mouse_Keyboard(int16_t input_type)
{

    p_fields = (struct s_Field *)Allocate_Space(357);  // 357 paragraphs = 367 * 16 = 5712 bytes  (? 150*38=5700 ? + 12 ?)

//     // The if else / switch in the DASM is borked
//     if(input_type == 0)
//     {
//         RP_MOUSE_SetUsable();
//         mouse_installed = ST_FALSE;
//         MOUSE_Emu_X = 158;
//         MOUSE_Emu_Y = 100;
//         MD_MoveCursor(158, 100);
//     }
//     if(input_type == 1)
//     {
//         Set_Mouse_List(1, mouse_list_init);
//         mouse_installed = Init_Mouse_Driver();
//         if(mouse_installed != ST_FALSE)
//         {
//             mouse_installed = ST_TRUE;
//         }
//         else
//         {
//             RP_MOUSE_SetUsable();
//             MOUSE_Emu_X = 158;
//             MOUSE_Emu_Y = 100;
//             MD_MoveCursor(158, 100);
//         }
// 
//     }
//     if(input_type == 2)
//     {
// 
//     }
//     else
//     {
// 
//     }
//     VGA_SaveCursorArea(158, 100);
//     input_delay = 0;
//     down_mouse_button = ST_UNDEFINED;
//     _global_esc = ST_FALSE;
//     Clear_Fields();

    Set_Mouse_List(1, mouse_list_init);
    Init_Mouse_Driver();
    // TODO  IN_CRL_Save_RSP(158, 100);
    input_delay = 0;
    // TODO  g_CTRL_Focused = ST_UNDEFINED;
    _global_esc = ST_FALSE;
    Clear_Fields();

    
// #ifdef DEBUG
//     dlvfprintf("DEBUG: [%s, %d] END: Init_Mouse_Keyboard()\n", __FILE__, __LINE__);
// #endif
}

// WZD s36p66
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

    // DONT if(mouse_installed == ST_FALSE) { field_index = CRP_GUI_KeyInputOnly(); }

    field_index = Interpret_Mouse_Input();  // WZD s36p01

    // TODO Set_Page_Off();  // MGC s26p02

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Get_Input() { field_index = %d }\n", __FILE__, __LINE__, field_index);
#endif

    return field_index;
}

// WZD s36p67
// MGC s34p67
void Set_Input_Delay(int16_t delay_count)
{

    input_delay = delay_count;

    // MOUSE_GetClick();
    // RP_MOUSE_GetSecClick();

}

// WZD s36p68
// MGC s34p68
int16_t Get_Input_Delay(void)
{
    return input_delay;
}



// WZD s36p85
// MGC s34p85
void Toggle_Pages(void)
{
    int16_t mouse_x;
    int16_t mouse_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Toggle_Pages()\n", __FILE__, __LINE__);
#endif

    // TODO  Save_Mouse_State()  AKA MD_CDraw_Disable();

    // MoO2
    // old_mouse_x = mouse_x;
    // old_mouse_y = mouse_y;

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();

    // NOTE(JimBalcomb,20221110): Presently, this feels like where what has been drawn is everything, so the draw-screen-page can be saved/exported/dumped.
    // STU_Export_DSP_To_BMP();


    // TODO  Draw_Visible_Fields()  AKA CTRL_DrawControls();  // MGC s34p71c  ST_CTRL.H

    Check_Mouse_Shape(mouse_x, mouse_y);  // AKA GUI_FindWindow();  // MGC s33p02  ST_GUI.H

    // TODO  Save_Mouse_Off()  AKA CRL_Save_DSP(mouse_x, mouse_y);
    // TODO  Draw_Mouse_Off()  AKA CRH_Draw_DSP(mouse_x, mouse_y);
    Save_Mouse(mouse_x, mouse_y);
    Draw_Mouse(mouse_x, mouse_y);


    // STU_Export_VBB_To_BMP32();


    // ? MoO2 Switch_Video_Pages() ?
    // TODO Page_Flip()  AKA VGA_PageFlip();  // MGC _s26p04a.c
    Page_Flip();
    
    // // Restore_Mouse_Off()  AKA CRL_Restore_DSP();
    Restore_Mouse();
    // // CRL_Copy_DSP2RSP();  // MGC _s33p33.asm
    // Copy_Mouse();


    // TODO  Restore_Mouse_State()  AKA MD_CDraw_Restore();


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Toggle_Pages()\n", __FILE__, __LINE__);
#endif
}