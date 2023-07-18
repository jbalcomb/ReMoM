/*
    Video2
    ~== MoO2 Module: video
    SimTex progression to supporting multiple video modes and pixel formats
*/
#ifndef VIDEO2_H
#define VIDEO2_H

#include "MoX_TYPE.H"

#ifdef __cplusplus
extern "C" {
#endif



enum e_Video_Mode
{
    vm_Lo_Res,
    vm_Mode_X,
    vm_Mode_X240,
    vm_Mode_Y,
    vm_Hi_Res,
    vm_Hi_Res_Hi_Col
};



extern uint8_t * video_page_buffer_XBGR[];

extern int screen_pixel_width;
extern int screen_pixel_height;
extern int screen_pixel_size;
extern int bytes_per_pixel;
extern int screen_scale;
extern int video_mode;
extern int window_pixel_width;
extern int window_pixel_height;
extern int window_pixel_size;



void Init_Video_Drivers(int mode_num);

void Assign_Video_Function_Pointers(int mode_num);

void Create_Screen_Buffers(int size);



// ~== WZD s30p11  FLIC_Draw()
// void (*Draw)(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File);
// ~== WZD s29p01  FLIC_Draw_Frame()
// void (*Draw_Frame)(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data);



//void (*Full_Screen_Dump)(void);
//void (*Partial_Screen_Dump)(void);
//void (*Masked_Screen_Dump )(void);
//void (*Copy_Off_To_On_Page)(void);
//void (*Set_Bank_Write)(void);
//void (*Set_Bank_Read)(void);
//void (*Switch_Video_Pages)(void);
//void (*Save_Mouse)(void);
//void (*Restore_Mouse)(void);
//void (*Draw_Mouse)(void);
//void (*Save_Video_State)(void);
//void (*Restore_Video_State)(void);
//void (*Get_Bank_Write)(void);


#ifdef __cplusplus
}
#endif

#endif /* VIDEO2_H */
