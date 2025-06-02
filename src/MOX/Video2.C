/*
    Video2
    ~== MoO2 Module: video
    SimTex progression to supporting multiple video modes and pixel formats
*/
#include "MOX_Lib.H"



uint8_t * video_page_buffer_2x[4];  // On, Off, Back
uint8_t * video_page_buffer_2x_XBGR[4];  // On, Off, Back

int screen_pixel_width = 320;
int screen_pixel_height = 200;
int screen_pixel_size = 64000;
int bytes_per_pixel = 1;
int screen_scale = 2;
// int video_mode = vm_Mode_Y;  // OG-MoM
// int video_mode = vm_Mode_Y_2x;
int video_mode = vm_Mode_Y_2x_XBGR;
int window_pixel_width = 640;
int window_pixel_height = 400;
int window_pixel_size = 256000;



/*
    MoO2
        Module: animate
            Draw_No_Glass()
            Draw()
            |->
                Module: animate
                    Draw_Screen_Background_On()
                    Draw_Screen_Background_Off()
                    Draw_No_Glassed_Animated_Sprite()
                    Draw_No_Glassed_Bitmap_Sprite()
                    Draw_Animated_Sprite_()
                    Draw_Bitmap_Sprite_()
                Module: clipped
                    Clipped_Draw_Animated_Sprite()
                    Clipped_Draw_Bitmap_Sprite()
                    Clipped_Draw_Glassed_Animated_Sprite()
                    Clipped_Draw_Glassed_Bitmap_Sprite()
                    Clipped_Draw_Stenciled_Animated_Sprite()
*/

// // ~== WZD s30p11  FLIC_Draw()
// void (*Draw)(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File);
// void (*Draw_Frame)(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data);
// 
// // ~== WZD s29p01  FLIC_Draw_Frame()
// void Draw_Frame_Data_Hi_Res(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data);
// 
// void Save_Mouse_Hi_Res(void);
// void Restore_Mouse_Hi_Res(void);
// void Draw_Mouse_Hi_Res(void);
// 
// void Full_Screen_Dump_Hi_Res(void);
// void Partial_Screen_Dump_Hi_Res(void);
// void Masked_Screen_Dump_Hi_Res(void);
// void Copy_Off_To_On_Hi_Res(void);
// void Set_Bank_Write_Vesa(void);
// void Set_Bank_Read_Vesa(void);
// void Switch_Video_Pages_Vesa(void);
// void Save_Mouse_Hi_Res(void);
// void Restore_Mouse_Hi_Res(void);
// void Draw_Mouse_Hi_Res(void);
// void Save_Video_State_Hi_Res(void);
// void Restore_Video_State_Hi_Res(void);
// void Get_Bank_Write_Vesa_(void);
// 
// void (*Full_Screen_Dump)(void);
// void (*Partial_Screen_Dump)(void);
// void (*Masked_Screen_Dump )(void);
// void (*Copy_Off_To_On_Page)(void);
// void (*Set_Bank_Write)(void);
// void (*Set_Bank_Read)(void);
// void (*Switch_Video_Pages)(void);
// void (*Save_Mouse)(void);
// void (*Restore_Mouse)(void);
// void (*Draw_Mouse)(void);
// void (*Save_Video_State)(void);
// void (*Restore_Video_State)(void);
// void (*Get_Bank_Write)(void);

void Init_Video_Drivers(int mode_num)
{
    // current_palette_address = &current_palette;
    video_mode = mode_num;

    switch (mode_num)
    {
        case vm_Lo_Res:
        {
            screen_pixel_width = 320;
            screen_pixel_height = 200;
        } break;
        case vm_Mode_X:
        {
            screen_pixel_width = 320;
            screen_pixel_height = 200;
        } break;
        case vm_Mode_X240:
        {
            screen_pixel_width = 320;
            screen_pixel_height = 240;
        } break;
        case vm_Mode_Y:
        {
            // TODO  DLOG("case vm_Mode_Y:");
            screen_pixel_width = 320;
            screen_pixel_height = 200;
            bytes_per_pixel = 1;
            window_pixel_width = 640;
            window_pixel_height = 400;
        } break;
        case vm_Hi_Res:
        {
            screen_pixel_width = 640;
            screen_pixel_height = 480;
        } break;
        case vm_Mode_Y_2x:
        {
            // TODO  DLOG("case vm_Mode_Y_2x:");
            screen_scale = 2;
            screen_pixel_width = 640;
            screen_pixel_height = 400;
            bytes_per_pixel = 1;
            window_pixel_width = 640;
            window_pixel_height = 400;
        } break;
        case vm_Mode_Y_2x_XBGR:
        {
            // TODO  DLOG("case vm_Mode_Y_2x_XBGR:");
            screen_scale = 2;
            screen_pixel_width = 640;
            screen_pixel_height = 400;
            bytes_per_pixel = 4;
            window_pixel_width = 640;
            window_pixel_height = 400;
        } break;
    }

    screen_pixel_size = screen_pixel_width * screen_pixel_height;
    window_pixel_size = window_pixel_width * window_pixel_height;
    Create_Screen_Buffers(screen_pixel_size);

    current_video_page = 0;
    current_video_page = video_page_buffer[1 - draw_page_num];

    Assign_Video_Function_Pointers(mode_num);

    // Set_Video_Mode();
    // Set_Page_Off();
    // Clear_Scan_List();
    // Clear_Stencil_List();
    // screen_refresh_flag = 0;
    // Reset_Window();
}

void Create_Screen_Buffers(int size)
{
    int64_t video_memory_size;
    int64_t video_memory_offset;

    // // // // MoO2      __off_page_buffer = Allocate_Space(size + 46);
    // // // // MoO2      __back_page_buffer = Allocate_Space(size + 46);
    // // // // MoO2      Set_Memory(__off_page_buffer, screen_pixel_size, 0);
    // // // // MoO2      Set_Memory(__back_page_buffer, screen_pixel_size, 0);
    // // // // video_page_buffer[0] = (uint8_t*)VirtualAlloc(NULL, (320 * 200 * 1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    // // // // video_page_buffer[1] = (uint8_t*)VirtualAlloc(NULL, (320 * 200 * 1), MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    // // // video_page_buffer[0] = (uint8_t*)Allocate_Space( ((screen_pixel_size + 1) / 16) );  // 320 * 200 * 1Bpp = 64000 / 16 = 4000 PR
    // // // video_page_buffer[1] = (uint8_t*)Allocate_Space( ((screen_pixel_size + 1) / 16) );
    // // // video_page_buffer[2] = (uint8_t*)Allocate_Space( ((screen_pixel_size + 1) / 16) );
    // // // video_page_buffer[3] = (uint8_t*)Allocate_Space( ((screen_pixel_size + 1) / 16) );
    // // video_page_buffer[0] = (uint8_t*)Allocate_Space( ((256 * 1024) / 16) );  // 256 * 1024  65536 * 4  262144 / 16 = 16384
    // video_page_buffer[0] = video_memory[(0 + (0 * 64 * 1024))];
    // video_page_buffer[1] = video_memory[(0 + (1 * 64 * 1024))];
    // video_page_buffer[2] = video_memory[(0 + (2 * 64 * 1024))];
    // video_page_buffer[3] = video_memory[(0 + (3 * 64 * 1024))];

    video_memory_size = (256 * 1024);

    video_memory = (uint8_t *)Allocate_Space(((256 * 1024) / 16));  // 256 * 1024  65536 * 4  262144 / 16 = 16384

    video_memory_offset = (0 + (0 * 64 * 1024));

    video_page_buffer[0] = &video_memory[video_memory_offset];

    video_memory_offset = (0 + (1 * 64 * 1024));

    video_page_buffer[1] = &video_memory[video_memory_offset];

    video_memory_offset = (0 + (2 * 64 * 1024));

    video_page_buffer[2] = &video_memory[video_memory_offset];

    video_memory_offset = (0 + (3 * 64 * 1024));

    video_page_buffer[3] = &video_memory[video_memory_offset];


    // // memset(video_page_buffer[0], 0, screen_pixel_size);
    // // memset(video_page_buffer[1], 0, screen_pixel_size);
    // // memset(video_page_buffer[2], 0, screen_pixel_size);
    // // memset(video_page_buffer[3], 0, screen_pixel_size);
    // memset(video_page_buffer[0], 0, (64 * 1024));
    // memset(video_page_buffer[1], 0, (64 * 1024));
    // memset(video_page_buffer[2], 0, (64 * 1024));
    // memset(video_page_buffer[3], 0, (64 * 1024));
    memset(video_memory, 0, (256 * 1024));

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[0]: %p\n", __FILE__, __LINE__, video_page_buffer[0]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[1]: %p\n", __FILE__, __LINE__, video_page_buffer[1]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[2]: %p\n", __FILE__, __LINE__, video_page_buffer[2]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[4]: %p\n", __FILE__, __LINE__, video_page_buffer[3]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[0] + (320*200*1): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer[0] + (320*200*1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[1] + (320*200*1): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer[1] + (320*200*1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[2] + (320*200*1): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer[2] + (320 * 200 * 1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer[3] + (320*200*1): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer[3] + (320 * 200 * 1)));
// #endif

    video_page_buffer_2x[0] = (uint8_t*)Allocate_Space( ((640 * 400 * 1) / 16) );  // 640 * 400 * 1Bpp = 256000  / 16 = 16000 PR
    video_page_buffer_2x[1] = (uint8_t*)Allocate_Space( ((640 * 400 * 1) / 16) );
    video_page_buffer_2x[2] = (uint8_t*)Allocate_Space( ((640 * 400 * 1) / 16) );
    video_page_buffer_2x[3] = (uint8_t*)Allocate_Space( ((640 * 400 * 1) / 16) );
    memset(video_page_buffer_2x[0], 0, screen_pixel_size * 2);
    memset(video_page_buffer_2x[1], 0, screen_pixel_size * 2);
    memset(video_page_buffer_2x[2], 0, screen_pixel_size * 2);
    memset(video_page_buffer_2x[3], 0, screen_pixel_size * 2);

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[0]: %p\n", __FILE__, __LINE__, video_page_buffer_2x[0]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[1]: %p\n", __FILE__, __LINE__, video_page_buffer_2x[1]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[2]: %p\n", __FILE__, __LINE__, video_page_buffer_2x[2]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[3]: %p\n", __FILE__, __LINE__, video_page_buffer_2x[3]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[0] + (640*400*1): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer_2x[0] + (640*400*1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[1] + (640*400*1): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer_2x[1] + (640*400*1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[2] + (640*400*1): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer_2x[2] + (640 * 400 * 1)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x[3] + (640*400*1): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer_2x[3] + (640 * 400 * 1)));
// #endif

    video_page_buffer_2x_XBGR[0] = (uint8_t*)Allocate_Space( ((640 * 400 * 4) / 16) );  // 640 * 400 * 4Bpp = 1000000  / 16 = 64000 PR
    video_page_buffer_2x_XBGR[1] = (uint8_t*)Allocate_Space( ((640 * 400 * 4) / 16) );
    video_page_buffer_2x_XBGR[2] = (uint8_t*)Allocate_Space( ((640 * 400 * 4) / 16) );
    video_page_buffer_2x_XBGR[3] = (uint8_t*)Allocate_Space( ((640 * 400 * 4) / 16) );
    memset(video_page_buffer_2x_XBGR[0], 0, (640 * 400 * 4));
    memset(video_page_buffer_2x_XBGR[1], 0, (640 * 400 * 4));
    memset(video_page_buffer_2x_XBGR[2], 0, (640 * 400 * 4));
    memset(video_page_buffer_2x_XBGR[3], 0, (640 * 400 * 4));

// #ifdef STU_DEBUG
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[0]: %p\n", __FILE__, __LINE__, video_page_buffer_2x_XBGR[0]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[1]: %p\n", __FILE__, __LINE__, video_page_buffer_2x_XBGR[1]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[2]: %p\n", __FILE__, __LINE__, video_page_buffer_2x_XBGR[2]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[3]: %p\n", __FILE__, __LINE__, video_page_buffer_2x_XBGR[3]);
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[0] + (640*400*4): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer_2x_XBGR[0] + (640*400*4)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[1] + (640*400*4): %p\n", __FILE__, __LINE__, (uint8_t *)(video_page_buffer_2x_XBGR[1] + (640*400*4)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[2] + (640*400*4): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer_2x_XBGR[2] + (640 * 400 * 4)));
//     dbg_prn("DEBUG: [%s, %d]: video_page_buffer_2x_XBGR[3] + (640*400*4): %p\n", __FILE__, __LINE__, (uint8_t*)(video_page_buffer_2x_XBGR[3] + (640 * 400 * 4)));
// #endif

}

void Assign_Video_Function_Pointers(int mode_num)
{

    switch (mode_num)
    {
        case vm_Lo_Res:
        {
        } break;
        case vm_Mode_X:
        {
        } break;
        case vm_Mode_X240:
        {
        } break;
        case vm_Mode_Y:
        {
            // Draw = FLIC_Draw_1x;
            // Draw_Frame = FLIC_Draw_Frame;

        } break;
        case vm_Hi_Res:
        {
            // Draw = FLIC_Draw_2x;
            // TODO  different paramters  Draw_Frame = FLIC_Draw_Frame_2x;

            // MoO2
            // Full_Screen_Dump = Full_Screen_Dump_Hi_Res;
            // Partial_Screen_Dump = Partial_Screen_Dump_Hi_Res;
            // Masked_Screen_Dump = Masked_Screen_Dump_Hi_Res;
            // Copy_Off_To_On_Page = Copy_Off_To_On_Hi_Res;
            // Set_Bank_Write = Set_Bank_Write_Vesa_;
            // Set_Bank_Read = Set_Bank_Read_Vesa_;
            // Switch_Video_Pages = Switch_Video_Pages_Vesa_;
            // Save_Mouse = Save_Mouse_Hi_Res_;
            // Restore_Mouse = Restore_Mouse_Hi_Res_;
            // Draw_Mouse = Draw_Mouse_Hi_Res;
            // Save_Video_State = Save_Video_State_Hi_Res;
            // Restore_Video_State = Restore_Video_State_Hi_Res;
            // Get_Bank_Write = Get_Bank_Write_Vesa_;
        } break;
        case vm_Mode_Y_2x:
        {
        } break;
        case vm_Mode_Y_2x_XBGR:
        {
        } break;
    }
}


void Save_Mouse_Hi_Res(void)
{

}
void Restore_Mouse_Hi_Res(void)
{

}
void Draw_Mouse_Hi_Res(void)
{

}



void Full_Screen_Dump_Hi_Res(void)
{

}
void Partial_Screen_Dump_Hi_Res(void)
{

}
void Masked_Screen_Dump_Hi_Res(void)
{

}
void Copy_Off_To_On_Hi_Res(void)
{

}
void Set_Bank_Write_Vesa(void)
{

}
void Set_Bank_Read_Vesa(void)
{

}
void Switch_Video_Pages_Vesa(void)
{

}
void Save_Video_State_Hi_Res(void)
{

}
void Restore_Video_State_Hi_Res(void)
{

}
void Get_Bank_Write_Vesa_(void)
{

}
