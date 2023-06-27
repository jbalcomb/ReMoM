
#include <string.h>

#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_Data.H"

#include "Fields.H"
#include "FLIC_Draw.H"
#include "Fonts.H"
#include "Graphics.H"
#include "Mouse.H"
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"    /* DLOG() */
#endif


/*
    WIZARDS.EXE
    Data Segment (DSEG)
    Initialized Data
    Uninitialized Data
    seg035
    seg036
    Begin: dseg:E872
    End:   dseg:E8B0
    Last:  dseg:E8AC
*/
/*
    Initialized Data
*/
// INPUT  _global_esc = ST_FALSE;
// HELP  int16_t help_list_active = ST_FALSE;
// HELP  int16_t help_list_count = 0;
// TODO  int16_t focused_field = ST_UNDEFINED;
// TODO  int16_t mouse_field = ST_FALSE;
// TODO  int16_t input_field_active = ST_FALSE;
// TODO  int16_t active_input_field_number = ST_UNDEFINED;



// WZD dseg:8254                                                 ¿ END: Mouse - Initialized Data ?
// ...
// ...
// ...
// WZD dseg:828A
int16_t DBG_ControlHighlight = ST_FALSE;
// WZD dseg:828C
int16_t DBG_HelpHighlight = ST_FALSE;
// ...
// ...
// ...
// WZD dseg:8296                                                 ¿ BEGIN: Help ?



// INPUT
extern int16_t down_mouse_button;

/*
    seg035
    seg036
    Uninitialized Data
    WZD dseg:E872
*/

// WZD dseg:E872
// _help_list dw 0
// WZD dseg:E874
// GUI_Processed_Btns dw 0
// WZD dseg:E876
// GUI_Processed_LastY dw 0
// WZD dseg:E878
// GUI_Processed_LastX dw 0
// WZD dseg:E87A
// GUI_EditString db 30 dup(0)
// WZD dseg:E898
int16_t input_delay;
// WZD dseg:E89A
// MOUSE_Emu_Y dw 0
// WZD dseg:E89C
// MOUSE_Emu_X dw 0
// WZD dseg:E89E
int16_t cursor_offset;
// WZD dseg:E8A0
// GUI_PrevControlCount dw 0
// WZD dseg:E8A2
int16_t fields_count;
// WZD dseg:E8A4
// GUI_Prev_Redraw_Fn dd 0
// WZD dseg:E8A8
// GUI_Redraw_Function dd 0
// WZD dseg:E8AC
struct s_Field * p_fields;  // "p_fields dd 0" ? Far Pointer?

// WZD dseg:E8AC                                                 ? END: fields, input, ... ?


/*
    WZD  seg036
    MGC  seg034
*/

// WZD s36p38
int16_t Add_Multi_Hot_Key_Field(char * argString)
{
    char varString[30];
    int16_t itr_string_length = 0;

    strcpy(varString, argString);

    while(varString[itr_string_length] != '\0' && itr_string_length < 30) itr_string_length++;

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_MultiHotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].Param0 = argString[0];  // TODO(JimBalcomb,20230612): fix Param0 - ? need char * ?
    p_fields[fields_count].Param1 = 0;
    p_fields[fields_count].Param2 = itr_string_length;
    p_fields[fields_count].hotkey = varString[0];  // TODO(JimBalcomb,20230612): fix hotkey - ? need char * ?

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count +=1;

    return (fields_count - 1);
}


// WZD s36p42
void Add_Button_Info(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;

    p_fields[fields_count].Param0 = string;

    p_fields[fields_count].x2 = FLIC_Get_Width(pict_seg) + p_fields[fields_count].x1 - 1;
    p_fields[fields_count].y2 = FLIC_Get_Height(pict_seg) + p_fields[fields_count].y1 - 1;

    p_fields[fields_count].Font_Index = Get_Current_Font_Index();
    p_fields[fields_count].ColorSet1 = Get_Current_Font_Color();

    p_fields[fields_count].help = help;

    // p_fields[fields_count].Param5 = pict_seg;
    // p_fields[fields_count].Param5 = (uint16_t *)pict_seg;
    p_fields[fields_count].pict_seg = pict_seg;

    p_fields[fields_count].Param3 = Get_Full_Store_Flag(pict_seg);

    // TODO(JimBalcomb,20230612): make hotkey make sense - looks like (char *)
    // mov     si, [bp+select_char]
    // mov     al, [si]
    // mov     [byte ptr es:bx+s_Field.hotkey], al

    p_fields[fields_count].hotkey = hotkey;

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

}


// WZD s36p43
int16_t Add_Button_Field(int16_t xmin, int16_t ymin, int16_t string, SAMB_ptr pict_seg, int16_t hotkey, int16_t help)
{

    Add_Button_Info(xmin, ymin, string, pict_seg, hotkey, help);

    p_fields[fields_count].type = ft_Button;

    fields_count +=1;

    return (fields_count - 1);
}

// WZD s36p47
int16_t Add_Hidden_Field(int16_t xmin, int16_t ymin, int16_t xmax, int16_t ymax, int16_t hotkey, int16_t help)
{

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;
    p_fields[fields_count].x2 = xmax;
    p_fields[fields_count].y2 = ymax;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = help;
    p_fields[fields_count].hotkey = hotkey;

    if ((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123) )
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

    return (fields_count - 1);
}

// WZD s34p49
int16_t Add_Hot_Key(int16_t select_char)
{

    p_fields[fields_count].x1 = 500;
    p_fields[fields_count].y1 = 500;
    p_fields[fields_count].x2 = 500;
    p_fields[fields_count].y2 = 500;
    p_fields[fields_count].type = ft_HotKey;
    p_fields[fields_count].help = ST_UNDEFINED;
    p_fields[fields_count].hotkey = select_char;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

    return (fields_count - 1);
}

// WZD s34p51
// DEDUCE: What happens with the xpos,ypos here? How does it get used? How does that work?
int16_t Add_Grid_Field(int16_t xmin, int16_t ymin, int16_t box_width, int16_t box_height, int16_t horizontal_count, int16_t vertical_count, int16_t *xpos, int16_t *ypos, int16_t help)
{

    p_fields[fields_count].x1 = xmin;
    p_fields[fields_count].y1 = ymin;

    p_fields[fields_count].x2 = (box_width * horizontal_count) + xmin - 1;
    p_fields[fields_count].y2 = (box_height * vertical_count) + ymin - 1;

    p_fields[fields_count].help = help;

    p_fields[fields_count].type = ft_Grid;

    p_fields[fields_count].Param1 = box_width;
    p_fields[fields_count].Param2 = box_height;

    p_fields[fields_count].hotkey = ST_NULL;

    // p_fields[fields_count].Param3 = *xpos;
    // p_fields[fields_count].Param4 = *ypos;
    // NOTE: We are storing the addresses of the pointers, to later be recast and dereferenced.
    p_fields[fields_count].Param3 = (uint16_t)xpos;
    p_fields[fields_count].Param4 = (uint16_t)ypos;

    if((p_fields[fields_count].hotkey > 96) && (p_fields[fields_count].hotkey < 123))
    {
        p_fields[fields_count].hotkey -= 32;
    }

    fields_count += 1;

    return (fields_count - 1);
}

// WZD s36p57
void Clear_Fields(void)
{

    down_mouse_button = ST_UNDEFINED;
    fields_count = 1;
    // TODO  focused_field = ST_UNDEFINED;
    // TODO  mouse_field = ST_FALSE;
    // TODO  input_field_active = ST_FALSE;
    // TODO  active_input_field_number = ST_UNDEFINED;

}

/*
Moo2
function (0 bytes) Draw_Visible_Fields
Address: 01:0011E718
Num params: 0
Return type: void (1 bytes) 
Locals:
signed integer (2 bytes) i
signed integer (2 bytes) j
signed integer (2 bytes) height
signed integer (2 bytes) space
signed integer (2 bytes) temp_value
signed integer (2 bytes) var_segment
signed integer (2 bytes) var_offset
signed integer (2 bytes) cursor_change
signed integer (2 bytes) old_window_x1
signed integer (2 bytes) old_window_x2
signed integer (2 bytes) old_window_y1
signed integer (2 bytes) old_window_y2
*/
// MoO2: Draw_Visible_Fields()

void Draw_Fields()
{
    int16_t old_windows_x1;
    int16_t old_windows_x2;
    int16_t old_windows_y1;
    int16_t old_windows_y2;
    int16_t itr_fields_count;
    int16_t itr_help_list_count;

    old_windows_x1 = screen_window_x1;
    old_windows_x2 = screen_window_x2;
    old_windows_y1 = screen_window_y1;
    old_windows_y2 = screen_window_y2;

    Reset_Window();

    Set_Page_Off();


    for(itr_fields_count = 1; itr_fields_count < fields_count; itr_fields_count++)
    {
        if( (down_mouse_button == itr_fields_count) && (p_fields[itr_fields_count].type != ft_Input) && (p_fields[itr_fields_count].type != ft_Grid) )
        {
            Draw_Field(itr_fields_count, 1);
        }
        else
        {

            // DEDUCE: ¿ does this not include field types 05, 07, 08, 09, and 12 ?
            switch(p_fields[itr_fields_count].type)
            {
                case ft_Button:                 /*  0  0x00 */  //drake178: TODO
                {

                } break;
                case ft_RadioButton:            /*  1  0x01 */  //drake178: ToggleButton
                {

                } break;
                case ft_LockedButton:           /*  2  0x02 */  //drake178: LockableButton
                {

                } break;
                case ft_MultiButton:            /*  3  0x03 */  //drake178: MStateButton
                {

                } break;
                case ft_Input:                  /*  4  0x04 */  //drake178: EditBox
                {

                } break;
                case ft_ImageLabel:             /*  5  0x05 */  //drake178: ImageLabel      DNE/NIU in MoO2
                {

                } break;
                case ft_Scroll:                 /*  6  0x06 */  //drake178: SlideBar
                {

                } break;
                case ft_HotKey:                 /*  7  0x07 */  //drake178: Label
                {

                } break;
                case ft_MultiHotKey:            /*  8  0x08 */  //drake178: Ctrl_AltString
                {

                } break;
                case ft_ClickLink:              /*  9  0x09 */  //drake178: ClickLink       DNE/NIU in MoO2
                {

                } break;
                case ft_StringList:             /* 10  0x0A */  //drake178: DialogLine
                {

                } break;
                case ft_ContinuousStringInput:  /* 11  0x0B */  //drake178: EditSelect
                {

                } break;
                case ft_Grid:                   /* 12  0x0C */  //drake178: TODO
                {
                    DLOG("switch(p_fields[itr_fields_count].type)  case ft_Grid");
                    
                } break;

            }

        }

        if( (DBG_ControlHighlight != ST_FALSE) && (p_fields[itr_fields_count].type != ft_MultiHotKey) )
        {
            /*
                Draw Rectangle
            */

            // color = 0x0E  14

            // p_fields[itr_fields_count].x1
            // p_fields[itr_fields_count].y1
            // p_fields[itr_fields_count].x2
            // p_fields[itr_fields_count].y2

            // VGA_WndDrawLine()
            // VGA_WndDrawLine()
            // VGA_WndDrawLine()
            // VGA_WndDrawLine()
        }

    }


// TODO      if( (DBG_HelpHighlight != ST_FALSE) && (help_list_count != 0) )
// TODO      {
// TODO          for(itr_help_list_count = 1; itr_help_list_count < help_list_count; itr_help_list_count++)
// TODO          {
// TODO              if(help_struct_pointer[itr_help_list_count]->help_idx != ST_UNDEFINED)
// TODO              {
// TODO              /*
// TODO                  Draw Rectangle
// TODO              */
// TODO              // color = 0xB4 180
// TODO              // p_fields[itr_fields_count].x1
// TODO              // p_fields[itr_fields_count].y1
// TODO              // p_fields[itr_fields_count].x2
// TODO              // p_fields[itr_fields_count].y2
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              // VGA_WndDrawLine()
// TODO              }
// TODO          }
// TODO      }


    Set_Window(old_windows_x1, old_windows_y1, old_windows_x2, old_windows_y2);

}

// WZD s36p71
/*
MoO2
function (0 bytes) Draw_Field
Address: 01:0011B05A
Num params: 2
Return type: void (1 bytes) 
signed integer (2 bytes) 
signed integer (2 bytes) 
Locals:
signed integer (2 bytes) field_num
signed integer (2 bytes) up_down_flag
signed integer (2 bytes) i
signed integer (2 bytes) height
signed integer (2 bytes) space
signed integer (2 bytes) var_segment
signed integer (2 bytes) var_offset
signed integer (2 bytes) temp_value
*/

// ¿ Is the code here for `case ft_Grid` the same as in Scan_Input() ?

// WZD s36p72
void Draw_Field(int16_t field_num, int16_t up_down_flag)
{
    int16_t screen_x;  // ~ Pointer_X()
    int16_t screen_y;  // ~ Pointer_Y()
    // ~ translate screen coordinates to field coordinates
    int16_t field_x;  // field_x = screen_x - p_fields[].x1
    int16_t field_y;  // field_y = screen_y - p_fields[].y1
    // ~ translate field coordinates to grid coordinates  (from pixels to squares)
    int16_t grid_x;  // grid_x = field_x % box_width
    int16_t grid_y;  // grid_y = field_y % box_height
                            

    switch(p_fields[field_num].type)
    {
        case ft_Button:                 /*  0  0x00 */  //drake178: TODO
        {

// Old-Code: .\MoM_Rasm\_s34p71c.c
// #ifdef STU_DEBUG
//                 dbg_prn("DEBUG: [%s, %d] case Ctrl_ClickButton\n", __FILE__, __LINE__);
// #endif
//                 FLIC_Reset_CurrentFrame(gfp_CTRL_Control_Table[tmp_SI].Param5);
//                 // FLIC_Draw_XY(gfp_CTRL_Control_Table[tmp_SI].Left, gfp_CTRL_Control_Table[tmp_SI].Top, gfp_CTRL_Control_Table[tmp_SI].Param5);
//                 FLIC_Draw(gfp_CTRL_Control_Table[tmp_SI].Left, gfp_CTRL_Control_Table[tmp_SI].Top, (SAMB_ptr)MK_FP(gfp_CTRL_Control_Table[tmp_SI].Param5,0));
//                 VGA_SetFont(gfp_CTRL_Control_Table[tmp_SI].Font_Index, gfp_CTRL_Control_Table[tmp_SI].ColorSet1, 0, 0);
//                 Half_Font_Height = ( (VGA_GetFontHeight() - 1) / 2);
//                 VGA_DrawCentered(
//                     ((gfp_CTRL_Control_Table[tmp_SI].Right - gfp_CTRL_Control_Table[tmp_SI].Left) + gfp_CTRL_Control_Table[tmp_SI].Left),
//                     (((gfp_CTRL_Control_Table[tmp_SI].Bottom - gfp_CTRL_Control_Table[tmp_SI].Top) / 2) + gfp_CTRL_Control_Table[tmp_SI].Top - Half_Font_Height),
//                     gfp_CTRL_Control_Table[tmp_SI].Param0
//                 );

        } break;
        case ft_RadioButton:            /*  1  0x01 */  //drake178: ToggleButton
        {

        } break;
        case ft_LockedButton:           /*  2  0x02 */  //drake178: LockableButton
        {

        } break;
        case ft_MultiButton:            /*  3  0x03 */  //drake178: MStateButton
        {

        } break;
        case ft_Input:                  /*  4  0x04 */  //drake178: EditBox
        {

        } break;
        case ft_ImageLabel:             /*  5  0x05 */  //drake178: ImageLabel      DNE/NIU in MoO2
        {

        } break;
        case ft_Scroll:                 /*  6  0x06 */  //drake178: SlideBar
        {

        } break;
        case ft_HotKey:                 /*  7  0x07 */  //drake178: Label
        {

        } break;
        case ft_MultiHotKey:            /*  8  0x08 */  //drake178: Ctrl_AltString
        {

        } break;
        case ft_ClickLink:              /*  9  0x09 */  //drake178: ClickLink       DNE/NIU in MoO2
        {

        } break;
        case ft_StringList:             /* 10  0x0A */  //drake178: DialogLine
        {

        } break;
        case ft_ContinuousStringInput:  /* 11  0x0B */  //drake178: EditSelect
        {

        } break;
        case ft_Grid:                   /* 12  0x0C */  //drake178: TODO
        {
            DLOG("switch(p_fields[field_num].type) case ft_Grid");
            if(up_down_flag == 1)  /* ¿ field up/down state: down ? */
            {
                DLOG("if(up_down_flag == 1)");

                /*
                    What it is, what is what, what is up?

                        Get the x,y coordinates relative to the field
                        ~ translate screen coordinates to field coordinates
                            field_x = Pointer_X() - p_fields[].x1
                            field_y = Pointer_Y() - p_fields[].y1

                        ~ translate field coordinates to grid coordinates
                            grid_x = field_x % box_width
                            grid_y = field_y % box_height
                        

                    e.g.,
                        Movement Map Grid Field
                                                                 (xmin, ymin, box_width, box_height, horizontal_count, vertical_count, *xpos, *ypos, help)
                            _main_map_grid_field = Add_Grid_Field(0, 20, 20, 18, 12, 10, &_main_map_grid_y, &_main_map_grid_x, ST_UNDEFINED);
                                p_fields[].x1 =  0
                                p_fields[].y1 = 20
                                p_fields[].x2 = 239 = (240 - 1) = (240 +  0 - 1) = ((20 * 12) +  0 - 1) = (box_width  * horizontal_count) + xmin - 1;
                                p_fields[].y2 = 199 = (200 - 1) = (180 + 20 - 1) = ((18 * 10) + 20 - 1) = (box_height * vertical_count  ) + ymin - 1;
                                p_fields[].Param1 = 20  box_width
                                p_fields[].Param2 = 18  box_height
                                p_fields[].Param3 = memory address of _main_map_grid_x
                                p_fields[].Param4 = memory address of _main_map_grid_y
                */

                screen_x = Pointer_X();
                screen_y = Pointer_Y();
                // ~ translate screen coordinates to field coordinates
                field_x = screen_x - p_fields[field_num].x1;
                field_y = screen_y - p_fields[field_num].y1;
                // ~ translate field coordinates to grid coordinates  (from pixels to squares)
                grid_x = field_x % p_fields[field_num].Param1;  // box_width
                grid_y = field_y % p_fields[field_num].Param2;  // box_height

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: field_num: %d\n", __FILE__, __LINE__, field_num);
    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].type: %d\n", __FILE__, __LINE__, p_fields[field_num].type);

    dbg_prn("DEBUG: [%s, %d]: screen_x: %d\n", __FILE__, __LINE__, screen_x);
    dbg_prn("DEBUG: [%s, %d]: screen_y: %d\n", __FILE__, __LINE__, screen_y);
    
    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].x1: %d\n", __FILE__, __LINE__, p_fields[field_num].x1);
    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].y1: %d\n", __FILE__, __LINE__, p_fields[field_num].y1);
    
    dbg_prn("DEBUG: [%s, %d]: field_x: %d\n", __FILE__, __LINE__, field_x);
    dbg_prn("DEBUG: [%s, %d]: field_y: %d\n", __FILE__, __LINE__, field_y);

    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param1: %d\n", __FILE__, __LINE__, p_fields[field_num].Param1);
    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param2: %d\n", __FILE__, __LINE__, p_fields[field_num].Param2);

    dbg_prn("DEBUG: [%s, %d]: grid_x: %d\n", __FILE__, __LINE__, grid_x);
    dbg_prn("DEBUG: [%s, %d]: grid_x: %d\n", __FILE__, __LINE__, grid_x);

    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param3: %d\n", __FILE__, __LINE__, p_fields[field_num].Param3);
    dbg_prn("DEBUG: [%s, %d]: p_fields[field_num].Param4: %d\n", __FILE__, __LINE__, p_fields[field_num].Param4);

    // ~ get the value in Param3, as an unsigned 2-byte value, treat it as a pointer to a signed 2-byte value
    dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ) );
    dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ) );
#endif

            // ¿ same code as in Add_Grid_Field() ?
            // NO!! Here, assigning value, there assigning memory address

            // ~ access Param3/4 as an unsigned 2-byte value, treat that value as a pointer to a signed 2-byte value, dereference that pointer to assign a signed 2-byte value
            *( (int16_t *) ( (uint16_t) p_fields[field_num].Param3 ) ) = grid_x;
            *( (int16_t *) ( (uint16_t) p_fields[field_num].Param4 ) ) = grid_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param3) ) );
    dbg_prn("DEBUG: [%s, %d]: *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ): %d\n", __FILE__, __LINE__, *( (int16_t *) ((uint16_t)p_fields[field_num].Param4) ) );
#endif

            }
        } break;

    }
}
