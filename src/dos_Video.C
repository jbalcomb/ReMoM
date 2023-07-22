
#include "MoX.H"



// WZD s28p04
// MGC s26p04
void Page_Flip(void)
{

    // MS-DOS & IBM-PC VGA
        // CRT Controller - Address
        // VGA_WaitSync();
    // MS Windows
        // pointer to buffer to render/present
    draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
    draw_page = video_page_buffer[draw_page_num];
    // HERE: what was drawn should now be displayed
    // Pump_Paints();
    
    Set_Page_Off();
}
