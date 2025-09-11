/*

    WIZARDS.EXE
    ovr149
    ~== TEXTBOX
    Confirmation, Warn, Selection, Notify, Help

*/
#include "Allocate.h"
#include "FLIC_Draw.h"
#include "Fields.h"
#include "Fonts.h"
#include "GENDRAW.h"
#include "Graphics.h"
#include "LBX_Load.h"
#include "MOX_DAT.h"
#include "MOX_BASE.h"
#include "MOX_TYPE.h"

#include "../../MoX/src/Help.h"
#include "Video.h"
#include "paragrph.h"

#include <string.h>     /* memcpy() memset(), strcat(), strcpy(), stricmp() */



// WZD dseg:7118                                                 BEGIN:  ovr159 - Initialized Data

// WZD dseg:7118
uint8_t COL_HLP_Titles[] = { 185, 47, 47, 47, 47 };

// WZD dseg:711D
uint8_t COL_HLP_Text[] = { 184, 55, 55, 55, 55 }; 

// WZD dseg:7122
char HELP_lbx_file__ovr159[] = "HELP";

// WZD dseg:7127
char help_lbx_file__ovr159[] = "help";

// WZD dseg:7127                                                 END:  ovr159 - Initialized Data




/*
    WIZARDS.EXE  ovr159
*/

// WZD o159p01
// drake178: GUI_Help_RedrawFn()
/*
displays the indicated context-based help entry

(dumps the first 64k of the sandbox into VGAFILEH
pages 1-5, and the first 4000 bytes of the LBX near
buffer into a sandbox allocation)
*/
/*
MoO2
Module: TEXTBOX
    code (0 bytes) Draw_Help_Entry_
    Address: 01:000A59A1
        Locals:

*/
/*


*/
void Draw_Help_Entry__WIP(int16_t help_entry_idx)
{
    SAMB_ptr help_pict_segs[10] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    SAMB_ptr help_pict_seg = 0;
    int16_t Image_Start_Top = 0;
    int16_t old_fields_count = 0;
    int16_t pict_width = 0;
    int16_t pict_height = 0;
    int16_t Current_Entry = 0;
    SAMB_ptr near_buffer_save = 0;
    int16_t Title_Font_Height = 0;
    int16_t pict_y1 = 0;
    int16_t pict_x1 = 0;
    int16_t max_height = 0;
    int16_t max_width = 0;
    int16_t Include_Entry = 0;
    int16_t Title_Height = 0;
    int16_t Title_Top = 0;
    int16_t Title_Indent = 0;
    int16_t Entry_Load_Count = 0;
    int16_t Max_Y = 0;
    int16_t Scroll_Display_Top = 0;
    int16_t Scroll_Display_Left = 0;
    int16_t Bottom_Scroll_Height = 0;
    int16_t Top_Scroll_Height = 0;
    int16_t Entry_Loop_Var = 0;
    SAMB_ptr EMS_PFBA = 0;
    int16_t Total_Text_Height = 0;
    SAMB_ptr Scroll_Bottom_IMG = 0;
    SAMB_ptr Big_Scroll_IMG = 0;
    int16_t itr = 0;  // _DI_
    int16_t IDK = 0;  // _SI_

    Copy_On_To_Off_Page();

    Set_Mouse_List(1, mouse_list_default);

    /*
        BEGIN:  Text Box Startup
    */
    
    SAVE_SCREEN_SEG

    old_fields_count = fields_count;

    fields_count = 0;

    near_buffer_save = Allocate_First_Block(_screen_seg, 252);  // 252 * 16 = 4032 bytes ... 4000 / 16 + 1 + 1

    memcpy(&near_buffer_save[0], &near_buffer[0], 4000);

    help_pict_seg = Allocate_Next_Block(_screen_seg, 250);  // 250 * 16 = 4000 bytes

    Near_Allocate_Mark();

    help_data_near_buffer = (struct s_HELP_DATA *)Near_Allocate_First(sizeof(struct s_HELP_DATA));

    /*
        END:  Text Box Startup
    */


// ; append the required images into the sandbox, count
// ; them, and check the width and height needed to fit
// ; the largest ones

    Entry_Load_Count = 0;

    Include_Entry = 1;

    max_width = 0;

    max_height = 0;

    itr = help_entry_idx;

    while(Include_Entry != 0)
    {

        LBX_Load_Data_Static(HELP_lbx_file__ovr159, 2, (SAMB_ptr)help_data_near_buffer, itr, 1, sizeof(struct s_HELP_DATA));
        
        Include_Entry = help_data_near_buffer->Include_Entry;

        if(Include_Entry == ST_UNDEFINED)
        {

            itr++;

        }
        else
        {

            itr = Include_Entry;

        }

        if(help_data_near_buffer->lbx_name [0] == ST_NULL)
        {

            help_pict_segs[Entry_Load_Count] = ST_NULL;

        }
        else
        {

            help_pict_segs[Entry_Load_Count] = LBX_Reload_Next(help_data_near_buffer->lbx_name, help_data_near_buffer->lbx_entry, _screen_seg);

            Reset_Animation_Frame(help_pict_segs[Entry_Load_Count]);
            
            Draw_Picture_To_Bitmap(help_pict_segs[Entry_Load_Count], help_pict_seg);

            Get_Bitmap_Actual_Size(help_pict_seg, &pict_x1, &pict_y1, &pict_width, &pict_height);

            if(pict_width > max_width)
            {

                max_width = pict_width;

            }

            if(pict_height > max_height)
            {

                max_height = pict_height;

            }

        }

        Entry_Load_Count++;
    }


    if(max_width != 0)
    {
        max_width += 6;
    }

    Title_Font_Height = 9;

    if(max_height > Title_Font_Height)
    {

        Title_Top = ((max_height - Title_Font_Height) / 2);

        pict_y1 = 0;

        Title_Height = (max_height + 2);

    }
    else
    {

        pict_y1 = ((Title_Font_Height - max_height) / 2);

        Title_Top = 0;

        Title_Height = (Title_Font_Height + 2);

    }

    Title_Indent = max_width;


    Big_Scroll_IMG = LBX_Reload_Next(help_lbx_file__ovr159, 0, _screen_seg);

    Scroll_Bottom_IMG = LBX_Reload_Next(help_lbx_file__ovr159, 1, _screen_seg);


    IDK = 2;


    itr = help_entry_idx;


    for(Entry_Loop_Var = 0; Entry_Loop_Var < Entry_Load_Count; Entry_Loop_Var++)
    {

        Set_Font_Colors_15(1, &COL_HLP_Text[0]);

        LBX_Load_Data_Static(help_lbx_file__ovr159, 2, (SAMB_ptr)&help_data_near_buffer[0], itr, 1, sizeof(struct s_HELP_DATA));
        
        if(help_data_near_buffer->help_text[0] == '\0')
        {

            IDK += (Title_Height + 4);
            
        }
        else
        {

            IDK += (Title_Height + Get_Paragraph_Max_Height(180, help_data_near_buffer->help_text) + 6);
            
        }

        Include_Entry = help_data_near_buffer->Include_Entry;

        if(Include_Entry == ST_UNDEFINED)
        {
            itr++;
        }
        else
        {
            itr = Include_Entry;
        }

    }

    Total_Text_Height = (IDK + 5);

    Top_Scroll_Height = 24;

    Bottom_Scroll_Height = 24;

    Scroll_Display_Left = 50;

    Scroll_Display_Top = (100 - ((Total_Text_Height + Top_Scroll_Height + Bottom_Scroll_Height) / 2));

    Max_Y = (Scroll_Display_Top + Top_Scroll_Height + Total_Text_Height - 1);

    Set_Page_Off();

    Set_Window(0, 0, 319, Max_Y);

    Clipped_Draw(Scroll_Display_Left, Scroll_Display_Top, Big_Scroll_IMG);

    Reset_Window();

    Clipped_Draw(Scroll_Display_Left, (Scroll_Display_Top + Top_Scroll_Height + Total_Text_Height), Scroll_Bottom_IMG);

    Reset_Window();

    IDK = (Scroll_Display_Top + Top_Scroll_Height + 2);

    itr = help_entry_idx;

    for(Entry_Loop_Var = 0; Entry_Loop_Var < Entry_Load_Count; Entry_Loop_Var++)
    {

        Current_Entry = itr;

        LBX_Load_Data_Static(help_lbx_file__ovr159, 2, (SAMB_ptr)&help_data_near_buffer[0], itr, 1, sizeof(struct s_HELP_DATA));

        Include_Entry = help_data_near_buffer->Include_Entry;

        if(Include_Entry == ST_UNDEFINED)
        {
            itr++;
        }
        else
        {
            itr = Include_Entry;
        }

        // ; BUG: the images are already loaded
        if(help_pict_segs[Entry_Loop_Var] != ST_NULL)
        {

            help_pict_segs[Entry_Loop_Var] = LBX_Reload_Next(help_data_near_buffer->lbx_name, help_data_near_buffer->lbx_entry, _screen_seg);

            Reset_Animation_Frame(help_pict_segs[Entry_Loop_Var]);
            
            Draw_Picture_To_Bitmap(help_pict_segs[Entry_Loop_Var], help_pict_seg);
            
            Get_Bitmap_Actual_Size(help_pict_seg, &pict_x1, &Image_Start_Top, &pict_width, &pict_height);
            
            if(Current_Entry < HLP_CITY_NAME)
            {

                FLIC_Set_LoopFrame_1(help_pict_seg);
                
            }
            
            Draw_Picture((((max_width - pict_width - 6) / 2) + Scroll_Display_Left + 20 - pict_x1), (IDK + pict_y1 - Image_Start_Top), help_pict_seg);
            
        }

        Set_Font_Colors_15(4, &COL_HLP_Titles[0]);

        Print((Title_Indent + Scroll_Display_Left + 18), (IDK + Title_Top), help_data_near_buffer->title);

        Set_Font_Colors_15(1, &COL_HLP_Text[0]);

        IDK += Title_Height;

        Print_Paragraph((Scroll_Display_Left + 20), IDK, 180, help_data_near_buffer->help_text, 0);

        if(help_data_near_buffer->help_text[0] == '\0')
        {
            IDK += 4;
        }
        else
        {
            IDK += Get_Paragraph_Max_Height(180, help_data_near_buffer->help_text);
        }

    }

    IDK -= 5;

    Toggle_Pages();

    Copy_On_To_Off_Page();

    Save_Auto_Function();

    Assign_Auto_Function(Draw_Help_Entry_Draw, 1);
    
    Wait_For_Input();
    
    /*
        BEGIN:  Text Box Cleanup
    */
    
    Restore_Auto_Function();
    
    memcpy(&near_buffer[0], near_buffer_save, 4000);

    Near_Allocate_Undo();

    RESTORE_SCREEN_SEG

    fields_count = old_fields_count;

    /*
        END:  Text Box Cleanup
    */

}


// WZD o159p02
// drake178: GUI_Help_RedrawFn()
/*
an empty redraw function, as the only action that
can be taken is to move the mouse, clicking or
pressing a button will end the help display
*/
void Draw_Help_Entry_Draw(void)
{
    // NOTE: Empty, on purpose!! Also, IDGI.
}


// WZD o159p03
// drake178: EMM_Map_VGAFILEH1_5()
/*
    IDK. IDGI.
~ hard-coded to VGAFILEH, pages 1-4
What happens after this?
elsewhere, *only* used by 'file animation? page 0 is header, pages 1-4 are data?

XREF:
    Draw_Help_Entry__WIP()
    Draw_Help_Entry__WIP()
    j_EMM_Map4Pages_VGAFILEH1234()
        Save_ScreenSeg()
        Restore_ScreenSeg()

*/
SAMB_ptr EMM_Map4Pages_VGAFILEH1234(void)
{
    // TODO  EMM_Map4Pages(1, _VGAFILEH_seg);
    EMS_PFBA = (_VGAFILEH_seg + (1 * SZ_EMM_LOGICAL_PAGE));

    return EMS_PFBA;
}
