#ifndef MOX_H
#define MOX_H

/*
    SimTex Game-Engine (STGE)
    MoX
    Mox - Common

*/

/* ~ STGE */
#include "MoX\MoX_Lib.H"



/*
    ¿ MoX Migration Plan ?
*/

#ifdef __cplusplus
extern "C" {
#endif

/*
    MoX_DBG.C/.H
*/
// WZD s24p01
void F11_Key__WIP(void);
// WZD s24p02
void Screen_Flic_Capture__STUB(void);
// WZD s24p03
// VGA_GetDACBlock();
// WZD s24p04
// VGA_ReadScreenLine();
// WZD s24p05
void Release_Version(void);
// WZD s24p06
int Check_Release_Version(void);
// WZD s24p07
// Screen_Picture_Capture();
// WZD s24p08
// Capture_Screen_Block();

/*
    FLIC_Draw.C/.H
*/
struct s_FLIC_HDR
{
    /* 00 */ int16_t width;
    /* 02 */ int16_t height;
    /* 04 */ int16_t current_frame;
    /* 06 */ int16_t frame_count;
    /* 08 */ int16_t loop_frame;
    /* 0A */ int8_t emm_handle_number;
    /* 0B */ int8_t emm_logical_page_number;
    /* 0C */ uint16_t emm_logical_page_offset;
    /* 0E */ int16_t palette_header_offset;
};
// // WZD s30p01
// // LBX_Image_Copy
// WZD s30p02
void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg);
// // WZD s30p03
// // LBX_IMG_VShiftRect
// // WZD s30p04
// // LBX_IMG_VertWarp
// // WZD s30p05
// // LBX_IMG_HorzWarp
// WZD s30p06
void Create_Blank_Picture(int16_t width, int16_t height, byte_ptr pict_seg, uint8_t color);
// // WZD s30p07
// // UU_LBX_IMG_ColorShift
// // WZD s30p08
// // UU_LBX_IMG_CReplaceRect
// // WZD s30p09
void Replace_Color_All(SAMB_ptr picture, uint8_t replacement_color);
// WZD s30p10
void Load_Palette_From_Animation(SAMB_ptr picture);
// WZD s30p11
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File);
// WZD s30p12
void Clipped_Draw(int16_t x, int16_t y, SAMB_ptr picture);
// WZD s30p13
void Draw_Picture_To_Bitmap(SAMB_ptr src_pict_seg, SAMB_ptr dst_pict_seg);
// WZD s30p14
void FLIC_Set_CurrentFrame(SAMB_ptr p_FLIC_Header, int16_t frame_index);
// WZD s30p15
void FLIC_Reset_CurrentFrame(SAMB_ptr p_FLIC_Header);
// WZD s30p16
int16_t FLIC_Get_CurrentFrame(SAMB_ptr p_FLIC_Header);
// WZD s30p17
int16_t FLIC_Get_FrameCount(SAMB_ptr p_FLIC_Header);
// WZD s30p18
int16_t FLIC_Get_Width(SAMB_ptr p_FLIC_Header);
// WZD s30p19
int16_t FLIC_Get_Height(SAMB_ptr p_FLIC_Header);
// // WZD s30p20
// // VGA_WndDrawRotateImg()
// // WZD s30p21
// // VGA_WndDrawImageRect()
// // WZD s30p22
// // UU_VGA_WndDrawTransform()
// // WZD s30p23
// // VGA_RotateRect()
// WZD s30p24
void Draw_Picture(int16_t x, int16_t y, byte_ptr pict_seg);
// WZD s30p25
void Draw_Picture_Windowed(int16_t x, int16_t y, byte_ptr pict_seg);
// WZD s30p26
void Clipped_Copy_Bitmap(int16_t x, int16_t y, byte_ptr dst_pict_seg, byte_ptr src_pict_seg);
// // WZD s30p27
// // LBX_IMG_Overlay()
// // WZD s30p28
// // LBX_IMG_StripColors()
// // WZD s30p29
// // UU_LBX_IMG_CropRect()
// WZD s30p30
void Clear_Bitmap_Region(int16_t x1, int16_t y1, int16_t x2, int16_t y2, byte_ptr bitm);
// // WZD s30p31
// // UU_LBX_IMG_FullGScale()
// // WZD s30p32
// // UU_LBX_IMG_ExtGScaleEC()
// // WZD s30p33
// // UU_LBX_IMG_ExtGrayScale()
// WZD s30p34
void Scale_Bitmap(SAMB_ptr bitmap, int16_t scale_x, int16_t scale_y);
// // WZD s30p35
void Open_File_Animation__STUB(char * file_name, int16_t entry_num);
// // WZD s30p36
// // VGA_FILEH_DrawFrame()
// // WZD s30p37
// // VGA_FILEH_Loader()
// // WZD s30p38
// // VGA_FILEH_GetFrame()
// // WZD s30p39
// // VGA_FILEH_SetFrame()
// // WZD s30p40
// int16_t Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header);
// WZD s30p41
void FLIC_Set_LoopFrame_1(SAMB_ptr p_FLIC_Header);
// WZD s30p42
void Outline_Bitmap_Pixels(SAMB_ptr pict_seg, uint8_t outline_color);
// WZD s30p43
void Outline_Bitmap_Pixels_No_Glass(SAMB_ptr pict_seg, uint8_t outline_color);
// WZD s30p44
void Bitmap_Aura_Pixels(SAMB_ptr pict_seg, uint8_t color, uint8_t * color_list);
// // WZD s30p45
// // UU_LBX_IMG_DrawRect()
// WZD s30p46
void Get_Bitmap_Actual_Size(SAMB_ptr bitmap_addr, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height);

// WZD s33p08
void Flip_Bitmap(SAMB_ptr pict_seg);
// WZD s33p09
void Replace_Color(SAMB_ptr pict_seg, uint8_t color_to_replace, uint8_t replacement_color);

/*
    Mouse.C/.H
*/
// // WZD s35p01
// void Set_Mouse_List(int16_t count, struct s_mouse_list * list);
// WZD s35p02
void Check_Mouse_Shape(int16_t x, int16_t y);
// // WZD s35p03
// int16_t Get_Pointer_Image_Number(void);
// WZD s35p04
int16_t Get_Pointer_Offset(void);
// // WZD s35p05
// int16_t Mouse_Button(void);
// // WZD s35p07
// int16_t Init_Mouse_Driver(void);
// // WZD s35p08
// // RP_Mouse_SetUsable
// // WZD s35p09
// // Reset_System_Mouse
// // WZD s35p10
// void Mouse_Movement_Handler(void);
// // WZD s35p11
// void Mouse_Button_Handler(void);
// // WZD s35p12
// /* PLATFORM */  void User_Mouse_Handler(int16_t buttons, int16_t mx, int16_t my);
// WZD s35p13
void Save_Mouse_State(void);
// WZD s35p14
void Restore_Mouse_State(void);
// // WZD s35p18
// int16_t Pointer_X(void);
// // WZD s35p19
// int16_t Pointer_Y(void);
// WZD s35p20
void Set_Pointer_Position(int16_t x, int16_t y);
// // WZD s35p21
// void Set_Mouse_Position(int16_t x, int16_t y);
// // WZD s35p22
// int16_t Mouse_Buffer_X(void);
// // WZD s35p23
// int16_t Mouse_Buffer_Y(void);
// // WZD s35p24
// int16_t Mouse_Buffer_Button(void);
// // WZD s35p25
// int16_t Mouse_Buffer(void);
// // WZD s35p26
// int16_t Mouse_Buffer2(void);
// // WZD s35p27
// // RP_MOUSE_SetSecClick()
// // WZD s35p28
// // UU_MOUSE_GenerateClick()
// // WZD s35p29
// void Check_Mouse_Buffer(int16_t x, int16_t y, int16_t buttons);
// WZD s35p30
// // WZD s35p31
// // WZD s35p32
void Save_Mouse_On_Page(int16_t x, int16_t y);
// void Save_Mouse_Off_Page(int16_t x, int16_t y);
// void Save_Mouse_On_Page_(int16_t x, int16_t y);
// // WZD s35p33
// void Copy_Mouse_Off_To_Mouse_Back(void);
// WZD s35p34
// // WZD s35p35
// // WZD s35p36
void Restore_Mouse_On_Page(void);
// void Restore_Mouse_Off_Page(void);
// WZD s35p37
// // WZD s35p38
// // WZD s35p39
// // WZD s35p40
void Draw_Mouse_On_Page(int16_t x, int16_t y);
// void Draw_Mouse_Off_Page(int16_t x, int16_t y);
// void Draw_Mouse_On_Page_(int16_t x, int16_t y);
// void Draw_Mouse_Off_Page_(int16_t x, int16_t y);

#ifdef __cplusplus
}
#endif





#ifdef __cplusplus
extern "C" {
#endif



/*
    Exit
*/
// WZD s05p02
// TODO  extern void (*Exit_With_Message)(char* string);

/*
    Allocate.C
*/
// WZD dseg:D49A
extern char near_buffer[];
// // WZD s08p01
// void Near_Allocate_Mark(void);
// // WZD s08p02
// void Near_Allocate_Undo(void);
// WZD s08p03
void Near_Allocate_Reset(void);  // used by Hire_Hero_Load()
// WZD s08p04
byte_ptr Near_Allocate_First(int16_t size);
// WZD s08p05
byte_ptr Near_Allocate_Next(int16_t size);
// WZD s08p06
void Near_Allocation_Error(int16_t size);
// WZD s08p07
SAMB_ptr Allocate_Space(uint16_t size);
// // WZD s08p08 / s08p09
// SAMB_ptr Allocate_Space_No_Header(uint16_t size);
// WZD s08p10
void Mark_Block(SAMB_ptr block);
// WZD s08p11
void Release_Block(SAMB_ptr block);
// WZD s08p12
SAMB_ptr Allocate_First_Block(SAMB_ptr SAMB_head, uint16_t size);
// WZD s08p13
void Reset_First_Block(SAMB_ptr block);  // used by Hier_Hero_Load()
// WZD s08p14
SAMB_ptr Allocate_Next_Block(SAMB_ptr SAMB_head, uint16_t size);
// WZD s08p15
uint16_t Get_Free_Blocks(SAMB_ptr SAMB_head);
/*
    MoO2
    Module: allocate
*/
SAMB_ptr Allocate_Dos_Space(int32_t size);
SAMB_ptr Allocate_Dos_Data_Space(int32_t size);

/*
    FLIC_Draw.C
*/



/*
    LBX_Load.C
*/
enum enum_LBX_ERROR
{
    le_not_found          =  1,
    le_corrupted          =  2,
    le_low_RAM            =  3,
    le_alloc_fail         =  4,
    le_reload_fail        =  5,
    le_bad_header         =  7,
    le_entries_exceeded   =  8,
    le_recsize_mismatch   =  9,
    le_records_exceeded   = 10,
    le_load_reg_first     = 11,
    le_low_EMS            = 12,
    le_cantload_reserved  = 13,
    le_low_ResEM          = 14,
    le_EM_swap_fail       = 15,
    le_anim_over_64k      = 16
};
// WZD s10p01
SAMB_ptr LBX_Load(char* lbx_name, int16_t entry_num);
// WZD s10p02
SAMB_ptr LBX_Reload(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// WZD s10p03
SAMB_ptr LBX_Reload_Next(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head);
// WZD s10p04
SAMB_ptr LBX_Load_Data(char* lbx_name, int16_t entry_num, int16_t start_rec, int16_t num_recs, int16_t record_size);
// WZD s10p12
void LBX_Load_Data_Static(char* lbx_name, int16_t entry_num, SAMB_ptr SAMB_head, uint16_t start_rec, uint16_t num_recs, uint16_t record_size);
// WZD s10p15
void Error_Handler(char * file_name, int16_t error_num, int16_t entry_num, int16_t pages);

/*
    WIZARDS.EXE  ovr067, ovr068, ovr150
    MainScr_Maps.C
*/
extern SAMB_ptr m_terrain_lbx_002;
extern uint16_t * m_terrain_lbx_001;
extern SAMB_ptr m_terrain_lbx_000;
// WZD o67p04
void Set_Draw_Active_Stack_Always(void);
// WZD o67p06
void Reset_Draw_Active_Stack(void);
// WZD o67p10
void Set_Entities_On_Map_Window(int16_t world_x, int16_t world_y, int16_t world_plane);
// WZD o67p13
void Set_Unit_Draw_Priority(void);
// WZD o67p14
void Reset_Stack_Draw_Priority(void);
// WZD o68p01
void Center_Map(int16_t * map_x, int16_t * map_y, int16_t world_grid_x, int16_t world_grid_y, int16_t world_plane);
// WZD o150p01
void Reset_Map_Draw(void);
// WZD o150p17
void Cartograph_Screen_Draw_Map_Terrain__NOWORKIE(int16_t cartograph_plane, SAMB_ptr cartograph_seg);



/*
    DNE
        Main Menu, Settings
    MGC
        {0,1,2,3,4} == {Continue,Load_Game,New_Game,Quit_To_DOS,Hall_Of_Fame}
    WZD
        {0,1,2,3,4,5,6,7,8,9,10,11/13,12,11/13,14,15} == {City,Load,Armies,Cities,Quit,MainGame,Magic,RoadBuilding,Production,Items,NextTurn,¿default?,Spellbook,¿default?,Advisor,Diplomacy}
*/
enum e_SCREENS
{
    scr_Main_Menu_Screen    = 0,

    scr_Continue            = 10,
    scr_Load_Screen         = 11,
    scr_New_Game_Screen     = 12,
    scr_Quit_To_DOS         = 13,
    scr_Hall_Of_Fame_Screen = 14,
    scr_Settings_Screen     = 15,

    /* WZD */
    scr_City_Screen         = 100,
    /* scr_Load_Screen = 101, */
    scr_Armies_Screen       = 102,
    scr_Cities_Screen       = 103,
    scr_Quit                = 104,
    scr_Main_Screen         = 105,
    scr_Magic_Screen        = 106,
    scr_Road_Build          = 107,
    scr_Production_Screen   = 108,  /* Production Pop-Up - ¿ from the City List Screen and City Screen ? */
    scr_Item_Screen         = 109,
    scr_NextTurn            = 110,  /* ¿ drake187: NextTurnProc ? ¿ MoO2: Do_Next_Turn_() ? */
    /* ¿default? */
    scr_Spellbook_Screen    = 112,
    /* ¿default? */
    scr_Advisor_Screen      = 114,   /* WZD 0x0E */
    scr_Diplomacy_Screen    = 115,
    // ¿!? Surveyor, Cartographer, etc.

    scr_Test_Screen         = 200,

    scr_PoC_Screen          = 240
};



#ifdef __cplusplus
}
#endif

#endif  /* MOX_H */
