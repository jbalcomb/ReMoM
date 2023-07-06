
#include "MoX_DBG.H"

#include "MoX_TYPE.H"
#include "MoX_DEF.H"
#include "MoX_Data.H"

#include "MainScr.H"

#include "Fonts.H"
#include "Mouse.H"


uint8_t DBG_debug_flag = 0;

/*
    Initialized Data
*/
// uint16_t release_version = MoX_ST_FALSE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled

#ifdef STU_DEBUG
uint16_t release_version = ST_FALSE;
#else
uint16_t release_version = ST_TRUE;                  // dseg:434E    XREF: DBG_Quit; DBG_ScreenDump; DBG_Disable; DBG_IsDisabled
#endif


void Main_Screen_Draw_Debug_Information(void)
{
    int16_t screen_x;
    int16_t screen_y;
    int16_t field_x;
    int16_t field_y;
    int16_t grid_x;
    int16_t grid_y;
    int16_t world_x;
    int16_t world_y;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Draw_Debug_Information()\n", __FILE__, __LINE__);
#endif

    Set_Outline_Color(0);

    // Set_Font_Style1(0, 0, 0, 0);
    Set_Font_Style1(1, 0, 0, 0);

    Set_Alias_Color(8);

    // Print(10, 189, cstr_WarGames);
    // Print(268, 68, cstr_GP);
    // Print(306, 68, cstr_MP);
    // // Print(40, 100, cstr_1st5);
    // // Print(40, 180, cstr_ABC);

    //   M   a   p       X   ,   Y
    // ~ 6 + 5 + 5 + 4 + 6 + 3 + 6 = 35 + ? 10
    Print(2, 22, "Map X,Y");
    Print_Integer(45, 22, _map_x);
    Print_Integer(57, 22, _map_y);

    //   G   r   i   d       X   ,   Y
    // ~ 5 + 5 + 2 + 5 + 4 + 6 + 3 + 6 = 36 + ? 9
    Print(2, 22+8, "Grid X,Y");
    Print_Integer(45, 22+8, (int16_t)_main_map_grid_x);
    Print_Integer(57, 22+8, (int16_t)_main_map_grid_y);


    screen_x = Pointer_X();
    screen_y = Pointer_Y();
    // ~ translate screen coordinates to field coordinates
    field_x = screen_x -  0;  // always  0
    field_y = screen_y - 20;  // always 20
    // ~ translate field coordinates to grid coordinates  (from pixels to squares)
    grid_x = field_x / MOVEMENT_MAP_WIDTH;
    grid_y = field_y / MOVEMENT_MAP_HEIGHT;
    // ~ translate field coordinates to world coordinates  (from pixels to squares)
    world_x = field_x / WORLD_WIDTH;
    world_y = field_y / WORLD_HEIGHT;

    //   M   D       M   X   ,   M   Y
    // ~ 6 + 6 + 4 + 6 + 6 + 3 + 6 + 6 = 43 + ? 
    Print(2, 22+16, "MD MX,MY");
    Print_Integer(52, 22+16, grid_x);
    Print_Integer(64, 22+16, grid_y);

    //   M   D       W   X   ,   W   Y
    // ~ 6 + 6 + 4 + 6 + 6 + 3 + 6 + 6 = 43 + ? 
    Print(2, 22+24, "MD WX,WY");
    Print_Integer(52, 22+24, world_x);
    Print_Integer(64, 22+24, world_y);


    Set_Font_Style1(0, 0, 0, 0);
    Set_Outline_Color(0);

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: Main_Screen_Draw_Debug_Information()\n", __FILE__, __LINE__);
#endif
}



// WZD s24p05
void Release_Version(void)
{
    release_version = ST_TRUE;
}

// WZD s24p06
int Check_Release_Version(void)
{
    return release_version;
}
