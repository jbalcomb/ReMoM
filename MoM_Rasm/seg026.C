// ~== MoO2  Module:video

#include "ST_TYPE.H"
#include "ST_DEF.H"

#include "ST_VGA.H"     /* VRAM_BASE; g_RSP_Idx, gsa_DSP_Addr; */

/*
    MoO2  Module:video

?
video_mask
    pointer (4 bytes)
    Address: 02:001B38BC
?
ds_number
    signed integer (4 bytes)
    Address: 02:001B38AC
es_number
    signed integer (2 bytes)
    Address: 02:001B3A3E
?

Cls
    Address: 01:00124B06
    Num params: 0
    Return type: void (1 bytes)
Cls_On
    Address: 01:00125554
    Num params: 0
    Return type: void (1 bytes)
        Locals:
            signed integer (4 bytes) i
            signed integer (4 bytes) pos

Set_Page_Off
    Address: 01:00124D41
    Num params: 0
    Return type: void (1 bytes)
Set_Page_Back
    Address: 01:00124D7A
    Num params: 0
    Return type: void (1 bytes)
Set_Page_On
    Address: 01:00124DB3
    Num params: 0
    Return type: void (1 bytes)

Copy_Off_To_Back
    Address: 01:00124DEC
    Num params: 0
    Return type: void (1 bytes)
Copy_Back_To_Off
    Address: 01:00124E36
    Num params: 0
    Return type: void (1 bytes)
Copy_Off_To_On
    Address: 01:00124E80
    Num params: 4
    Return type: void (1 bytes)
        signed integer (2 bytes) x1
        signed integer (2 bytes) y1
        signed integer (2 bytes) x2
        signed integer (2 bytes) y2
? function pointer - run-time dispatch depending on video mode ?
    pointer (4 bytes) Copy_Off_To_On_Page
    Address: 02:001B3888

Toggle_Pages
    Address: 01:00124ECB
    Num params: 0
    Return type: void (1 bytes) 
        Locals:
        signed integer (4 bytes) i
? function pointer - run-time dispatch depending on video mode ?
    pointer (4 bytes) Switch_Video_Pages
        Address: 02:001B3894


current_page_flag
    char (1 bytes)
    Address: 02:001B3A6B
Get_Current_Video_Page_Flag
    Address: 01:00124AA2
    Num params: 0
    Return type: char (1 bytes)
        ? why is this here in the MoO2 DASM ? void (1 bytes) 


video_memory
    pointer (4 bytes)
    Address: 02:001B3910
current_video_page
    pointer (4 bytes)
    Address: 02:001B3904
off_page_buffer
    pointer (4 bytes)
    Address: 02:001B390C
back_page_buffer
    pointer (4 bytes)
    Address: 02:001B38FC
draw_page_num
    signed integer (2 bytes)
    Address: 02:001B3A66

copy_to_on_page_flag
    signed integer (2 bytes)
    Address: 02:001B3A44

screen_pixel_height
    signed integer (4 bytes)
    Address: 02:0017C538
screen_pixel_width
    signed integer (4 bytes)
    Address: 02:0017C534
screen_pixel_size
    signed integer (4 bytes)
    Address: 02:0017C53C


total_video_memory
    signed integer (2 bytes)
    Address: 02:001B3A62
Get_Video_Total_Memory
    Address: 01:0012542A
    Num params: 0
    Return type: signed integer (4 bytes)



Set_Page_On
    mov current_page_flag, 0
    mov eax, __off_page_buffer
    mov current_video_page, eax
    mov copy_to_on_page_flag, 1

Set_Page_Off
    mov current_page_flag, 1
    mov eax, __off_page_buffer
    mov current_video_page, eax
    mov copy_to_on_page_flag, 0

Set_Page_Back
    mov current_page_flag, 2
    mov eax, __back_page_buffer
    mov current_video_page, eax
    mov copy_to_on_page_flag, 0

So, ...
    in MoO2
        on-screen and off-screen both use off_page_buffer
        but, on-screen sets 'copy to on-screen' flag
        ? meaning that off_page_buffer is effectively the front-buffer ?
        ...
        current_page_flag looks to be {0,1,2} == {on,off,back}
        ? how does current_page_flag relate to draw_page_num ?


Toggle_Pages()
    ...
    mov toggle_flag, 1
    ...
    mov eax, 1
    sub ax, draw_page_num
    mov draw_page_num
    ...
    call Set_Page_Off
    mov toggle_flag, 0

Cls()
    xor     ebx, ebx
    mov     edx, dword ptr screen_pixel_size
    mov     eax, __off_page_buffer
    call    Set_Memory_

So, ...
    off_page_buffer is ~default screen buffer

*/


// s26p01
/*
    sets the DSP to draw on-screen
*/
// ? MoO2  Set_Page_On() ?
void VGA_SetDirectDraw(void)
{
    // MoO2  draw_page_num = 1 - draw_page_num
    gsa_DSP_Addr = VRAM_BASE + ( (g_RSP_Idx) << 10 );
    p_DSP = (byte_ptr) MK_FP( (VRAM_BASE + ( (g_RSP_Idx) << 10 )), 0);
}

// s26p02
/*
    sets the DSP to draw off-screen
*/
// ? MoO2  Set_Page_Off() ?
void VGA_Set_DSP_Addr(void)
{
    // MoO2  draw_page_num = 1 - draw_page_num
    gsa_DSP_Addr = VRAM_BASE + ( (1 - g_RSP_Idx) << 10 );
    p_DSP = (byte_ptr) MK_FP( (VRAM_BASE + ( (1 - g_RSP_Idx) << 10 )), 0);
}

// s26p03
int VGA_IsDefaultDrawScreenPage(void)
{
    int is_default_DSP;

    if (gsa_DSP_Addr == 0xA000)
    {
        is_default_DSP = ST_TRUE;
    }
    else
    {
        is_default_DSP = ST_FALSE;
    }

    return is_default_DSP;
}

// s26p04
void VGA_PageFlip(void)
{

    g_RSP_Idx = (1 - g_RSP_Idx);  // NOTE: this is the only code that changes the RSP index

    outportb(0x03D4, 0x0C);  // CRTC_INDEX, CRTC_START_ADDRESS_HIGH
    outportb(0x3D5, (VRAM_BASE + ( g_RSP_Idx << 6 )));  // CRTC_INDEX, {0x0000, 0x4000}

    VGA_WaitSync();
    VGA_Set_DSP_Addr();  // _s26p02.asm
}


// s26p05
void VGA_Copy_RSP_DSP(void)
{

    if(CR_GetIndex() == 0)
    {
        VGA_Copy_RSP_DSP_NM();
    }
    else
    {
        VGA_Copy_RSP_DSP_YM();
    }
}
