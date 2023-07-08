
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM.H"

#include "MoM_main.H"

#include "Mouse.H"  /* mouse_x, mouse_y */
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>
#include <stdbool.h>

#include <windows.h>
// #include <debugapi.h>  /* OutputDebugStringA() */

/*

screen_pixel_width1   =      0
screen_pixel_width2   =    640
screen_pixel_height1  =      0
screen_pixel_height2  =    480
screen_pixel_size     = 307200 ; (640 * 480)

draw_page_num

Set_Page_Off()
    current_page_flag = 1
    current_video_page = __off_page_buffer
    copy_to_on_page_flag = 0

Set_Page_Back()
    current_page_flag = 2
    current_video_page = __back_page_buffer
    copy_to_on_page_flag = 0

Set_Page_On()
    current_page_flag = 0
    current_video_page = __off_page_buffer
    copy_to_on_page_flag = 1

*/


uint16_t screen_pixel_width   =  320;
uint16_t screen_pixel_height  =  200;
// error C2099: initializer is not a constant
// uint16_t screen_pixel_size    =  (screen_pixel_width * screen_pixel_height);
uint16_t screen_pixel_size    =  64000;


struct game_offscreen_buffer Buffer = {0};  // clear/set to zero!


uint8_t g_State_Run;


// char game_files[] = {
//     "SAVETEST.GAM";
//     "SAVE1.GAM";
//     "SAVE9.GAM"
// };

#define GAME_FILE_COUNT 14
char * game_file_names[GAME_FILE_COUNT] = {
    "CONFIG.MOM",
    "FONTS.LBX",
    "HLPENTRY.LBX",
    "MAGIC.SET",
    "MAIN.LBX",
    "MAINSCRN.LBX",
    "MAPBACK.LBX",
    "SAVE1.GAM",
    "SAVE9.GAM",
    "SAVETEST.GAM",
    "TERRAIN.LBX",
    "UNITS1.LBX",
    "UNITS2.LBX",
    "VORTEX.LBX"
};




void Render_VBB(struct game_offscreen_buffer * Buffer)
{

    uint32_t * Pixel = (uint32_t*)Buffer->Memory;
    int itr;
    unsigned char vbb_byte;
    unsigned int * p_XBGR;
    unsigned int long_XBGR;
    p_XBGR = (uint32_t *)g_Palette_XBGR;
    for(itr = 0; itr < 64000; itr++)
    {
        // vbb_byte = *(g_Video_Back_Buffer + itr);
        vbb_byte = *(current_video_page + itr);
        long_XBGR = *(p_XBGR + vbb_byte);
        *Pixel++ = long_XBGR;
    }

}

void GameUpdateAndRender(struct game_offscreen_buffer * Buffer)
{
    Render_VBB(Buffer);
}

void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y)
{
    if(lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
    }
}

void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status)
{
    if(lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
        platform_mouse_button_status = mouse_button_status;
    }
}


char Debug_Message_Buffer[4096];

void Get_Current_Working_Direct(void)
{

    TCHAR tszBuffer[MAX_PATH];
    DWORD dwRet;
    dwRet = GetCurrentDirectory(MAX_PATH, tszBuffer);
    // if(dwRet == 0) { OutputDebugStringA("FAILURE: GetCurrentDirectory(): function failed\n"); }
    // if(dwRet > MAX_PATH) { OutputDebugStringA("FAILURE: GetCurrentDirectory(): buffer too short\n"); }

    char Current_Working_Directory[MAX_PATH];
    sprintf(Current_Working_Directory, "%s\n", tszBuffer);
    // // OutputDebugStringA(Current_Working_Directory);
    // // OutputDebugStringA(tszBuffer);  // TCHAR * vs. LPCSTR
    // OutputDebugString(tszBuffer);
    // // "J:\STU\devel\STU-MoM_Rasm\data"
#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: Current Working Directory: %s\n", __FILE__, __LINE__, Current_Working_Directory);
#endif

}

// https://www.learnc.net/c-tutorial/c-file-exists/
// return true if the file specified by the filename exists
bool file_exists(const char* filename)
{
    sprintf(Debug_Message_Buffer, "filename: %s\n", filename);
    OutputDebugStringA(Debug_Message_Buffer);

    FILE* fp = fopen(filename, "r");
    bool is_exist = false;
    if (fp != NULL)
    {
        is_exist = true;
        fclose(fp); // close the file
    }
    return is_exist;
}

void Check_Game_Files(void)
{
    int itr_game_file_count;
    char game_file_name[30];

    char Current_Working_Directory[4096];
    GetCurrentDirectoryA(MAX_PATH, Current_Working_Directory);

    for (itr_game_file_count = 0; itr_game_file_count < GAME_FILE_COUNT; itr_game_file_count++)
    {
        // strcpy(game_file_name, game_file_names[itr_game_file_count]);
        if(!file_exists(game_file_names[itr_game_file_count]))
        {
            printf("File %s doesn't exist.", game_file_names[itr_game_file_count]);
            OutputDebugStringA("File XXXXXXXX.XXX doesn't exist.\n");
            char Error_Message_Buffer[4096];
            sprintf(Error_Message_Buffer, "Hey, Buddy. Hey!! The file - %s - doesn't exist in %s.\n", game_file_names[itr_game_file_count], Current_Working_Directory);
            OutputDebugStringA(Error_Message_Buffer);

#ifdef STU_DEBUG
            dbg_prn("DEBUG: [%s, %d]: File %s doesn't exist.\n", __FILE__, __LINE__, game_file_names[itr_game_file_count]);
#endif

            exit(1);  // ~EXIT_FAILURE
        }
    }
}
