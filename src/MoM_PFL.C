
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoM_PFL.H"

#include "MoM_main.H"

#include "Mouse.H"  /* mouse_x, mouse_y */
#include "Video.H"

#ifdef STU_DEBUG
#include "STU_DBG.H"
#endif

#include <stdio.h>
#include <stdbool.h>

#include <windows.h>



uint8_t g_State_Run;

int Pump_Events_Cnt = 0;
int Pump_Paints_Cnt = 0;



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




void Pump_Events(void)
{
    Pump_Events_Cnt++;
    // InvalidateRect(g_Window, NULL, TRUE);
    // InvalidateRect(g_Window, NULL, FALSE);
    MSG Message;
    while (PeekMessageA(&Message, 0, 0, 0, PM_REMOVE))
    {
        if (Message.message == WM_QUIT)
        {
            // quit_flag = ST_TRUE;
            g_State_Run = false;
        }
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
}

void Pump_Paints(void)
{
    Pump_Paints_Cnt++;
    InvalidateRect(g_Window, NULL, FALSE);
    MSG Message;
    while (PeekMessageA(&Message, 0, WM_PAINT, WM_PAINT, PM_REMOVE))
    {
        TranslateMessage(&Message);
        DispatchMessageA(&Message);
    }
}


// ¿ ~== Mouse_Int_Handler() ?
void Update_Mouse_Position(int16_t platform_mouse_x, int16_t platform_mouse_y)
{
    if (lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
        // mouse_x = platform_mouse_x / display_scale;
        // mouse_y = platform_mouse_y / display_scale;
    }
    if (MOUSE_INT_Process == ST_FALSE)
    {
        MOUSE_INT_Process = ST_TRUE;
        // TODO  MOUSE_SaveClick();
        if (mouse_enabled != ST_FALSE)
        {
            mouse_enabled = ST_FALSE;
            if (current_mouse_list_count >= 2)
            {
                Check_Mouse_Shape(mouse_x, mouse_y);
            }
            Restore_Mouse_On_Page();                // mouse_background_buffer -> video_page_buffer[draw_page_num]
            Save_Mouse_On_Page(mouse_x, mouse_y);   // video_page_buffer[draw_page_num] -> mouse_background_buffer
            Draw_Mouse_On_Page(mouse_x, mouse_y);   // mouse_palette -> video_page_buffer[draw_page_num]
            mouse_enabled = ST_TRUE;
        }
        MOUSE_INT_Process = ST_FALSE;
    }
}

void Update_Mouse_Button_Status(int16_t platform_mouse_x, int16_t platform_mouse_y, int16_t mouse_button_status)
{
    if(lock_mouse_button_status_flag != ST_TRUE)
    {
        mouse_x = platform_mouse_x / 2;
        mouse_y = platform_mouse_y / 2;
        // mouse_x = platform_mouse_x / display_scale;
        // mouse_y = platform_mouse_y / display_scale;
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
