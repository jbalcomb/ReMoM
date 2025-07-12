
#include "MOM.h"



// WZD s28p04
// MOX/Video.C  void Page_Flip(void)
// MOX/Video.C  {
// MOX/Video.C  
// MOX/Video.C      // MS-DOS & IBM-PC VGA
// MOX/Video.C          // CRT Controller - Address
// MOX/Video.C          // VGA_WaitSync();
// MOX/Video.C      // MS Windows
// MOX/Video.C          // pointer to buffer to render/present
// MOX/Video.C      draw_page_num = (1 - draw_page_num);  // NOTE: this is the only code that changes 'draw_page_num'
// MOX/Video.C      draw_page = video_page_buffer[draw_page_num];
// MOX/Video.C      // current_video_page = video_page_buffer[1 - draw_page_num];
// MOX/Video.C      // HERE: what was drawn should now be displayed
// MOX/Video.C      Pump_Paints();
// MOX/Video.C      
// MOX/Video.C      Set_Page_Off();
// MOX/Video.C  }
