
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM.H"

#include "Allocate.H"
#include "Fields.H"
#include "Input.H"
#include "Keyboard.H"
#include "Mouse.H"
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
int16_t DBG_In_MouseButtonLeft = ST_FALSE;
int16_t DBG_In_MouseButtonRight = ST_FALSE;
int16_t DBG_movement_map_grid_field_idx = 0;
int16_t DBG_reduced_map_grid_field_idx = 0;

// char field_names[127][30];
char * field_names[FIELD_COUNT_MAX] = {
    /* 000 */   "NOT IN USE",
    /* 001 */   "NOT IN USE",
    /* 002 */   "NOT IN USE",
    /* 003 */   "NOT IN USE",
    /* 004 */   "NOT IN USE",
    /* 005 */   "NOT IN USE",
    /* 006 */   "NOT IN USE",
    /* 007 */   "NOT IN USE",
    /* 008 */   "NOT IN USE",
    /* 009 */   "NOT IN USE",
    /* 010 */   "NOT IN USE",
    /* 011 */   "NOT IN USE",
    /* 012 */   "NOT IN USE",
    /* 013 */   "NOT IN USE",
    /* 014 */   "NOT IN USE",
    /* 015 */   "NOT IN USE",
    /* 016 */   "NOT IN USE",
    /* 017 */   "NOT IN USE",
    /* 018 */   "NOT IN USE",
    /* 019 */   "NOT IN USE",
    /* 020 */   "NOT IN USE",
    /* 021 */   "NOT IN USE",
    /* 022 */   "NOT IN USE",
    /* 023 */   "NOT IN USE",
    /* 024 */   "NOT IN USE",
    /* 025 */   "NOT IN USE",
    /* 026 */   "NOT IN USE",
    /* 027 */   "NOT IN USE",
    /* 028 */   "NOT IN USE",
    /* 029 */   "NOT IN USE",
    /* 030 */   "NOT IN USE",
    /* 031 */   "NOT IN USE",
    /* 032 */   "NOT IN USE",
    /* 033 */   "NOT IN USE",
    /* 034 */   "NOT IN USE",
    /* 035 */   "NOT IN USE",
    /* 036 */   "NOT IN USE",
    /* 037 */   "NOT IN USE",
    /* 038 */   "NOT IN USE",
    /* 039 */   "NOT IN USE",
    /* 040 */   "NOT IN USE",
    /* 041 */   "NOT IN USE",
    /* 042 */   "NOT IN USE",
    /* 043 */   "NOT IN USE",
    /* 044 */   "NOT IN USE",
    /* 045 */   "NOT IN USE",
    /* 046 */   "NOT IN USE",
    /* 047 */   "NOT IN USE",
    /* 048 */   "NOT IN USE",
    /* 049 */   "NOT IN USE",
    /* 050 */   "NOT IN USE",
    /* 051 */   "NOT IN USE",
    /* 052 */   "NOT IN USE",
    /* 053 */   "NOT IN USE",
    /* 054 */   "NOT IN USE",
    /* 055 */   "NOT IN USE",
    /* 056 */   "NOT IN USE",
    /* 057 */   "NOT IN USE",
    /* 058 */   "NOT IN USE",
    /* 059 */   "NOT IN USE",
    /* 060 */   "NOT IN USE",
    /* 061 */   "NOT IN USE",
    /* 062 */   "NOT IN USE",
    /* 063 */   "NOT IN USE",
    /* 064 */   "NOT IN USE",
    /* 065 */   "NOT IN USE",
    /* 066 */   "NOT IN USE",
    /* 067 */   "NOT IN USE",
    /* 068 */   "NOT IN USE",
    /* 069 */   "NOT IN USE",
    /* 070 */   "NOT IN USE",
    /* 071 */   "NOT IN USE",
    /* 072 */   "NOT IN USE",
    /* 073 */   "NOT IN USE",
    /* 074 */   "NOT IN USE",
    /* 075 */   "NOT IN USE",
    /* 076 */   "NOT IN USE",
    /* 077 */   "NOT IN USE",
    /* 078 */   "NOT IN USE",
    /* 079 */   "NOT IN USE",
    /* 080 */   "NOT IN USE",
    /* 081 */   "NOT IN USE",
    /* 082 */   "NOT IN USE",
    /* 083 */   "NOT IN USE",
    /* 084 */   "NOT IN USE",
    /* 085 */   "NOT IN USE",
    /* 086 */   "NOT IN USE",
    /* 087 */   "NOT IN USE",
    /* 088 */   "NOT IN USE",
    /* 089 */   "NOT IN USE",
    /* 090 */   "NOT IN USE",
    /* 091 */   "NOT IN USE",
    /* 092 */   "NOT IN USE",
    /* 093 */   "NOT IN USE",
    /* 094 */   "NOT IN USE",
    /* 095 */   "NOT IN USE",
    /* 096 */   "NOT IN USE",
    /* 097 */   "NOT IN USE",
    /* 098 */   "NOT IN USE",
    /* 099 */   "NOT IN USE",
    /* 100 */   "NOT IN USE",
    /* 101 */   "NOT IN USE",
    /* 102 */   "NOT IN USE",
    /* 103 */   "NOT IN USE",
    /* 104 */   "NOT IN USE",
    /* 105 */   "NOT IN USE",
    /* 106 */   "NOT IN USE",
    /* 107 */   "NOT IN USE",
    /* 108 */   "NOT IN USE",
    /* 109 */   "NOT IN USE",
    /* 110 */   "NOT IN USE",
    /* 111 */   "NOT IN USE",
    /* 112 */   "NOT IN USE",
    /* 113 */   "NOT IN USE",
    /* 114 */   "NOT IN USE",
    /* 115 */   "NOT IN USE",
    /* 116 */   "NOT IN USE",
    /* 117 */   "NOT IN USE",
    /* 118 */   "NOT IN USE",
    /* 119 */   "NOT IN USE",
    /* 120 */   "NOT IN USE",
    /* 121 */   "NOT IN USE",
    /* 122 */   "NOT IN USE",
    /* 123 */   "NOT IN USE",
    /* 124 */   "NOT IN USE",
    /* 125 */   "NOT IN USE",
    /* 126 */   "NOT IN USE",
    /* 127 */   "NOT IN USE"
};

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


uint8_t g_Key_Pressed = ST_FALSE;   // Platform - Decl in MoM.hpp
uint16_t g_Last_Key_Pressed = 0;    // Platform - Decl in MoM.hpp
uint16_t scan_code_char_code = 0;   // Platform - Decl in MoM.hpp


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
    int16_t character = 0;
    int16_t mx;
    int16_t my;
    int16_t pointer_offset;
    int16_t down_mouse_button;
    int16_t mouse_field;
    int16_t MD_ButtonStatus;
    
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Interpret_Mouse_Input()\n", __FILE__, __LINE__);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] character: %d\n", __FILE__, __LINE__, character);
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

        /*
            Global Debug Keys
        */

        if(p_fields[field_num].type == ft_MultiHotKey)
        {

        }
        else if(field_num == 0  || p_fields[field_num].hotkey != character)
        {

        }
    }
    else
    {
        DLOG("(Keyboard_Status() != ST_TRUE)");

        // Mouse_Movement_Handler();

        
        if(MD_GetButtonStatus() != ST_FALSE)
        {
            DLOG("(MD_GetButtonStatus() != ST_FALSE)");
            /*
                ProgramPath:
                    KBD_CheckBuffer == 0
                    MDI_Mv()
                    MD_INT_SetMvOnly()
                    MD_GetButtonStatus() != 0
            */
            MD_ButtonStatus = MD_GetButtonStatus();
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: MD_ButtonStatus: %d\n", __FILE__, __LINE__, MD_ButtonStatus);
#endif

//              /*
//                  ? Right Click for Help ?
//  
//                  Check_Help_List() draws the help, if it can, then wait for a key press or button click...
//                  so, nothing needs to happen after this?
//              */
//              if(MD_ButtonStatus == MOUSE_BUTTON_RIGHT && help_list_active != ST_FALSE && Check_Help_List() == 0)
//              {
//  
//  
//              }



            // HACK: 
            if(MD_ButtonStatus == MOUSE_BUTTON_LEFT)
            {
#ifdef STU_DEBUG
                DLOG("if(MD_ButtonStatus == MOUSE_BUTTON_LEFT)");
                DBG_In_MouseButtonLeft = ST_TRUE;
#endif
                mouse_x = Pointer_X();
                mouse_y = Pointer_Y();
                cursor_offset = Get_Pointer_Offset();
                field_num = 0;
                character = 0;
                field_num = Scan_Field();
                if(field_num != 0)
                {
                    field_num = field_num;
                }
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d]: Left-Click field_num: %d\n", __FILE__, __LINE__, field_num);
                DBG_In_MouseButtonLeft = ST_FALSE;
#endif
                
            }


            // HACK: 
            if(MD_ButtonStatus == MOUSE_BUTTON_RIGHT)
            {
#ifdef STU_DEBUG
                DLOG("if(MD_ButtonStatus == MOUSE_BUTTON_RIGHT)");
                DBG_In_MouseButtonRight = ST_TRUE;
#endif
                mouse_x = Pointer_X();
                mouse_y = Pointer_Y();
                cursor_offset = Get_Pointer_Offset();
                field_num = 0;
                character = 0;
                field_num = Scan_Field();
                if(field_num != 0)
                {
                    field_num = (field_num * -1);
                }
#ifdef STU_DEBUG
                dbg_prn("DEBUG: [%s, %d]: Right-Click field_num: %d\n", __FILE__, __LINE__, field_num);
                DBG_In_MouseButtonRight = ST_FALSE;
#endif
            }



            // if(MD_GetButtonStatus() != 0)
            if(ST_FALSE)
            {
                // HERE: ¿ != 2 && != 0 ?
                mouse_x = Pointer_X();
                mouse_y = Pointer_Y();
                cursor_offset = Get_Pointer_Offset();
                field_num = 0;
                // Unused_Local = ST_UNDEFINED
                character = 0;
                field_num = Scan_Field();
                if(field_num != 0)
                {
                    if( (down_mouse_button == ST_FALSE) && (p_fields[field_num].type != ft_Input) && (p_fields[field_num].type != ft_ContinuousStringInput) )
                    {
                        // if ...
                        // (down_mouse_button != ST_UNDEFINED) &&||
                        // (p_fields[field_num].type != ft_Grid) &&||
                        // (p_fields[down_mouse_button].type != ft_Grid) &&||
                        // (p_fields[down_mouse_button].type != ft_Slidebar)
                        // GUI_CallRedrawFn()
                        // else Push_Field_Down(field_num, mouse_x, mouse_y)



                    }


                    // if...
                    // p_fields[field_num].type != ft_ContinuousStringInput  &&||
                    // input_field_active !=/== ST_FALSE
                    // ... Param0 ... edit string ... edit cursor ... animation ... GUI_Active_EditSlct = field_num ...


                    // GUI_MouseFocusCtrl = field_num;
                    // if(GUI_ClickActivate !=/== ST_FALSE)


                    // ft_Slider


                    // MD_GetClickRec1()
                    // MD_Get_ClickRec2()
                    // GUI_Processed_LastX = mouse_x;
                    // GUI_Processed_LastY = mouse_y;
                    // GUI_Processed_Btns = MD_ButtonStatus;

                    // switch(p_fields[field_num].type)


                }


            }


        }
        // else if(MD_GetClickRec1() != ST_FALSE)
        else if(ST_FALSE)
        {
            DLOG("(MD_GetButtonStatus() == ST_FALSE)");
            DLOG("(MD_GetClickRec1() != ST_FALSE)");
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
            DLOG("(Keyboard_Status() != ST_TRUE)");
            DLOG("(MD_GetButtonStatus() == ST_FALSE)");
            DLOG("(MD_GetClickRec1() == ST_FALSE)");
            // Mouse_Button_Handler();
            field_num = 0;
        }
        

    }

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] character: %d\n", __FILE__, __LINE__, character);
#endif
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] field_num: %d\n", __FILE__, __LINE__, field_num);
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

#ifdef STU_DEBUG
        dbg_prn("DEBUG: [%s, %d]: fields_names[%d]: %s\n", __FILE__, __LINE__, itr, field_names[itr]);
#endif

#ifdef STU_DEBUG
        if(DBG_In_MouseButtonLeft == ST_TRUE)
        {
            DLOG("if(DBG_In_MouseButtonLeft == ST_TRUE)");
            dbg_prn("DEBUG: [%s, %d]: xmin xmax: %d > %d < %d\n", __FILE__, __LINE__, xmin, (mx + cursor_offset), xmax);
            dbg_prn("DEBUG: [%s, %d]: xmin xmax: %d > %d < %d\n", __FILE__, __LINE__, ymin, (my + cursor_offset), ymax);
        }
        if(DBG_In_MouseButtonLeft == ST_TRUE && itr == DBG_movement_map_grid_field_idx)
        {
            DLOG("if(DBG_In_MouseButtonLeft == ST_TRUE && itr == DBG_movement_map_grid_field_idx)");
            // dbg_prn("DEBUG: [%s, %d]: (xmin > mx + cursor_offset): (%d > %d): %d\n"), __FILE__, __LINE__, xmin, (mx + cursor_offset), (xmin > (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (xmax < mx + cursor_offset): (%d < %d): %d\n"), __FILE__, __LINE__, xmax, (mx + cursor_offset), (xmax < (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (ymin > mx + cursor_offset): (%d > %d): %d\n"), __FILE__, __LINE__, ymin, (mx + cursor_offset), (ymin > (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (ymax > mx + cursor_offset): (%d < %d): %d\n"), __FILE__, __LINE__, ymax, (mx + cursor_offset), (ymax < (mx + cursor_offset));
        }
        if(DBG_In_MouseButtonRight == ST_TRUE)
        {
            DLOG("if(DBG_In_MouseButtonRight == ST_TRUE)");
            dbg_prn("DEBUG: [%s, %d]: xmin xmax: %d > %d < %d\n", __FILE__, __LINE__, xmin, (mx + cursor_offset), xmax);
            dbg_prn("DEBUG: [%s, %d]: xmin xmax: %d > %d < %d\n", __FILE__, __LINE__, ymin, (my + cursor_offset), ymax);
        }
        if(DBG_In_MouseButtonRight == ST_TRUE && itr == DBG_movement_map_grid_field_idx)
        {
            DLOG("if(DBG_In_MouseButtonRight == ST_TRUE && itr == DBG_movement_map_grid_field_idx)");
            // dbg_prn("DEBUG: [%s, %d]: (xmin > mx + cursor_offset): (%d > %d): %d\n"), __FILE__, __LINE__, xmin, (mx + cursor_offset), (xmin > (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (xmax < mx + cursor_offset): (%d < %d): %d\n"), __FILE__, __LINE__, xmax, (mx + cursor_offset), (xmax < (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (ymin > mx + cursor_offset): (%d > %d): %d\n"), __FILE__, __LINE__, ymin, (mx + cursor_offset), (ymin > (mx + cursor_offset));
            // dbg_prn("DEBUG: [%s, %d]: (ymax > mx + cursor_offset): (%d < %d): %d\n"), __FILE__, __LINE__, ymax, (mx + cursor_offset), (ymax < (mx + cursor_offset));
            dbg_prn("DEBUG: [%s, %d]: (mx + cursor_offset >= xmin): %d\n", __FILE__, __LINE__, (mx + cursor_offset >= xmin));
            dbg_prn("DEBUG: [%s, %d]: (mx + cursor_offset <= xmax): %d\n", __FILE__, __LINE__, (mx + cursor_offset <= xmax));
            dbg_prn("DEBUG: [%s, %d]: (my + cursor_offset >= ymin): %d\n", __FILE__, __LINE__, (my + cursor_offset >= ymin));
            dbg_prn("DEBUG: [%s, %d]: (my + cursor_offset <= ymax): %d\n", __FILE__, __LINE__, (my + cursor_offset <= ymax));
        }
#endif

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
    dbg_prn("DEBUG: [%s, %d]: current_field: %d\n", __FILE__, __LINE__, current_field);
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
//     int16_t new_Param3;
//     int16_t new_Param4;

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
        DLOG("if(p_fields[current_field].type == ft_ClickLink)");
        *((int16_t *)p_fields[current_field].Param2) = p_fields[current_field].Param1;
        current_field = p_fields[current_field].Param0;
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

    /*
        Field Type: Grid Field

        ...target code-block, by way of...
            p_fields[current_field].type == ft_Grid && down_mouse_button == ST_UNDEFINED
            p_fields[current_field].type == ft_Grid && p_fields[down_mouse_button].type != ft_Grid
    */
    // &_main_map_grid_y, &_main_map_grid_x
    // Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help)
    //     p_fields[fields_count].Param3 = (int16_t)xpos;
    //     p_fields[fields_count].Param4 = (int16_t)ypos;

//    new_Param3 = ( (Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1 );
//    new_Param4 = ( (Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2 );
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].type: %d\n", __FILE__, __LINE__, p_fields[current_field].type);
//     dbg_prn("DEBUG: [%s, %d]: Pointer_X(): %d\n", __FILE__, __LINE__, Pointer_X());
//     dbg_prn("DEBUG: [%s, %d]: Pointer_Y(): %d\n", __FILE__, __LINE__, Pointer_Y());
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].x1: %d\n", __FILE__, __LINE__, p_fields[current_field].x1);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].y1: %d\n", __FILE__, __LINE__, p_fields[current_field].y1);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param1: %d\n", __FILE__, __LINE__, p_fields[current_field].Param1);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param2: %d\n", __FILE__, __LINE__, p_fields[current_field].Param2);
//     dbg_prn("DEBUG: [%s, %d]: (Pointer_X() - p_fields[current_field].x1): %d\n", __FILE__, __LINE__, (Pointer_X() - p_fields[current_field].x1));
//     dbg_prn("DEBUG: [%s, %d]: (Pointer_Y() - p_fields[current_field].y1): %d\n", __FILE__, __LINE__, (Pointer_Y() - p_fields[current_field].y1));
//     dbg_prn("DEBUG: [%s, %d]: ( (Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1 ): %d\n", __FILE__, __LINE__, ( (Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1 ));
//     dbg_prn("DEBUG: [%s, %d]: ( (Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2 ): %d\n", __FILE__, __LINE__, ( (Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2 ));
//     dbg_prn("DEBUG: [%s, %d]: new_Param3: %d\n", __FILE__, __LINE__, new_Param3);
//     dbg_prn("DEBUG: [%s, %d]: new_Param4: %d\n", __FILE__, __LINE__, new_Param4);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param3: %d\n", __FILE__, __LINE__, p_fields[current_field].Param3);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param4: %d\n", __FILE__, __LINE__, p_fields[current_field].Param4);
//     dbg_prn("DEBUG: [%s, %d]: *((int16_t *)(p_fields[current_field].Param3)): %d\n", __FILE__, __LINE__, *((int16_t *)(p_fields[current_field].Param3)));
//     dbg_prn("DEBUG: [%s, %d]: *((int16_t *)(p_fields[current_field].Param4)): %d\n", __FILE__, __LINE__, *((int16_t *)(p_fields[current_field].Param4)));
// #endif
//     if(p_fields[current_field].type == ft_Grid)
//     {
//         DLOG("if(p_fields[current_field].type == ft_Grid)");
//         *((int16_t *)p_fields[current_field].Param3) = ( (Pointer_X() - p_fields[current_field].x1) / p_fields[current_field].Param1 );
//         *((int16_t *)p_fields[current_field].Param4) = ( (Pointer_Y() - p_fields[current_field].y1) / p_fields[current_field].Param2 );
//     }
// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param3: %d\n", __FILE__, __LINE__, p_fields[current_field].Param3);
//     dbg_prn("DEBUG: [%s, %d]: p_fields[current_field].Param4: %d\n", __FILE__, __LINE__, p_fields[current_field].Param4);
// #endif

    return current_field;
}

// WZD s36p65
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
    // DONT  mouse_installed = ST_TRUE;

    Save_Mouse_On_Page_(158, 100);
    input_delay = 0;
    down_mouse_button = ST_UNDEFINED;
    _global_esc = ST_FALSE;
    Clear_Fields();

}

// WZD s36p66
// MGC s34p66
/*
    returns field_idx/num
*/
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
void Toggle_Pages(void)
{
    int16_t mouse_x;
    int16_t mouse_y;

    Save_Mouse_State();

    mouse_x = Pointer_X();
    mouse_y = Pointer_Y();

    Draw_Fields();

    Check_Mouse_Shape(mouse_x, mouse_y);

    Save_Mouse_Off_Page(mouse_x, mouse_y);  // [1 - draw_page_num] -> mouse_off_page_buffer
    Draw_Mouse_Off_Page(mouse_x, mouse_y);  // mouse_palette -> [1 - draw_page_num]

    Page_Flip();  // draw_page_num = 1 - draw_page_num

    // Restore_Mouse_Off_Page();  // mouse_background_buffer -> [1 - draw_page_num]
    // Copy_Mouse_Off_To_Mouse_Back();  // mouse_off_page_buffer -> mouse_background_buffer

    Restore_Mouse_State();

}