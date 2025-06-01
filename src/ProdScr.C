/*
    Production Screen

    WIZARDS.EXE
        ovr056

*/

#include "MOX/MOX_DAT.H"  /* _screen_seg */

#include "ProdScr.H"
#include "MainScr.H"
#include "MOM_SCR.H"
#include "UnitStat.H"
#include "UnitView.H"
#include "WZD_o059.H"

#ifdef STU_DEBUG
#include "STU/STU_DBG.H"    /* DLOG() */
#endif



// WZD dseg:2D72                                                 ¿ BEGIN: ovr056 - Strings ?

// WZD dseg:2D72
char buildesc_lbx_file[] = "BUILDESC";

// WZD dseg:2D7B 1B 00                                           cnst_HOTKEY_Esc3 db 1Bh,0               ;
// WZD dseg:2D7D 54 68 65 72 65 20 61 72 65 20 61 6C 72 65 61 64+aThereAreAlreadyTooM db 'There are already too many units currently in the city.',0
// WZD dseg:2DB5 54 68 65 72 65 20 69 73 20 6E 6F 74 20 65 6E 6F+aThereIsNotEnoughPop db 'There is not enough population to support new construction.',0
// WZD dseg:2DF1 43 61 6E 63 65 6C 00                            cnst_Cancel db 'Cancel',0               ;
// WZD dseg:2DF8 4F 6B 00                                        cnst_Ok_BtnMsg db 'Ok',0                ;
// WZD dseg:2DFB 4F 00                                           cnst_HOTKEY_O_2 db 'O',0                ;
// WZD dseg:2DFD 53 70 65 61 72 6D 65 6E 00                      cnst_Spearmen_2 db 'Spearmen',0         ;
// WZD dseg:2E06 53 77 6F 72 64 73 6D 65 6E 00                   cnst_Swordsmen_2 db 'Swordsmen',0       ;

// WZD dseg:2E06                                                 ¿ END: ovr056 - Strings ?



// WZD dseg:BFF2                                                 ¿ BEGIN:  ovr056  Production Screen ?

char * building_description;
SAMB_ptr IDK_ProdScr_w42A94;
struct s_UV_List * prod_specials_list;
int16_t * product_indexes;
int16_t garrison_full;
int16_t G_CTY_ProdUnitCount;
int16_t G_CTY_ProducableCount2;
int16_t build_table_field_count;
SAMB_ptr PS_product_pict_seg;
int16_t UU_ProdScr_flag;
int16_t production_screen_ok_button;
int16_t production_screen_cancel_button;
// WZD dseg:C00E 00 00                                           _ulw_window_field dw 0                         ; DATA XREF: IDK_ULW_Add_Fields+20w ...
// WZD dseg:C010 00 00                                           _ulw_full_screen_ESC_field dw 0               ; DATA XREF: Unit_List_Window_Pup+272r ...
int16_t * product_fields;
int16_t bldg_list_y1;
int16_t bldg_list_x1;
int16_t list_item_height;
int16_t unit_list_y1;
int16_t unit_list_x1;
int16_t IDK_ProdScr_w42ABE;
int16_t IDK_ProdScr_w42AC0;
int16_t UU_height_28;
int16_t UU_width_90;
int16_t list_field_height;
int16_t list_field_width;
// current_item
int16_t current_item;
int16_t popup_base_y;
int16_t popup_base_x;

// WZD dseg:BFFC 00 00                                           G_CTY_ProdUnitCount dw 0                ; DATA XREF: Production_Screen+77o ...
// WZD dseg:BFFE 00 00                                           G_CTY_ProducableCount2 dw 0             ; DATA XREF: Production_Screen+7Bo ...
// WZD dseg:C000 00 00                                           build_table_field_count dw 0            ; DATA XREF: Production_Screen+7Fo ...
// WZD dseg:C002 00 00                                           PS_product_pict_seg dw 0                ; DATA XREF: Production_Screen_Allocate_First_Block+15w ...
// WZD dseg:C004 00 00                                           UU_ProdScr_flag dw 0                    ; DATA XREF: Production_Screen+1ACw ...
// WZD dseg:C006 00 00                                           production_screen_ok_button dw 0        ; DATA XREF: Production_Screen+3BBr ...
// WZD dseg:C008 00 00                                           production_screen_cancel_button dw 0    ; DATA XREF: Production_Screen+261r ...
// WZD dseg:C00A 00                                              db    0
// WZD dseg:C00B 00                                              db    0
// WZD dseg:C00C 00                                              db    0
// WZD dseg:C00D 00                                              db    0
// WZD dseg:C00E 00 00                                           _ulw_window_field dw 0                         ; DATA XREF: IDK_ULW_Add_Fields+20w ...
// WZD dseg:C010 00 00                                           _ulw_full_screen_ESC_field dw 0               ; DATA XREF: Unit_List_Window_Pup+272r ...
// WZD dseg:C012 00 00                                           product_fields dw 0
// WZD dseg:C014 00 00                                           bldg_list_y1 dw 0            ; DATA XREF: Production_Screen+66w ...
// WZD dseg:C016 00 00                                           bldg_list_x1 dw 0            ; DATA XREF: Production_Screen+5Dw ...
// WZD dseg:C018 00 00                                           list_item_height dw 0            ; DATA XREF: Production_Screen+2Aw ...
// WZD dseg:C01A 00 00                                           unit_list_y1 dw 0           ; DATA XREF: Production_Screen+57w ...
// WZD dseg:C01C 00 00                                           unit_list_x1 dw 0           ; DATA XREF: Production_Screen+4Ew ...
// WZD dseg:C01E 00 00                                           IDK_ProdScr_w42ABE dw 0                 ; DATA XREF: Production_Screen+42w ...
// WZD dseg:C020 00 00                                           IDK_ProdScr_w42AC0 dw 0                 ; DATA XREF: Production_Screen+3Cw ...
// WZD dseg:C022 00 00                                           UU_height_28 dw 0                ; DATA XREF: Production_Screen:loc_4E5F6w ...
// WZD dseg:C024 00 00                                           UU_width_90 dw 0                ; DATA XREF: Production_Screen+30w ...
// WZD dseg:C026 00 00                                           list_field_height dw 0              ; DATA XREF: Production_Screen+24w ...
// WZD dseg:C028 00 00                                           list_field_width dw 0              ; DATA XREF: Production_Screen+1Ew ...
// WZD dseg:C02A 00                                              db    0
// WZD dseg:C02B 00                                              db    0
// WZD dseg:C02C 00 00                                           current_item dw 0              ; DATA XREF: Production_Screen+DFw ...
// WZD dseg:C02E 00                                              db    0
// WZD dseg:C02F 00                                              db    0
// WZD dseg:C030 00                                              db    0
// WZD dseg:C031 00                                              db    0
// WZD dseg:C032 00                                              db    0
// WZD dseg:C033 00                                              db    0
// WZD dseg:C034 00                                              db    0
// WZD dseg:C035 00                                              db    0
// WZD dseg:C036 00                                              db    0
// WZD dseg:C037 00                                              db    0
// WZD dseg:C038 00                                              db    0
// WZD dseg:C039 00                                              db    0
// WZD dseg:C03A 00 00                                           popup_base_y dw 0           ; DATA XREF: Production_Screen+18w ...
// WZD dseg:C03C 00 00                                           popup_base_x dw 0           ; DATA XREF: Production_Screen+12w ...

// WZD dseg:C03C                                                 ¿ END:  ovr056  Production Screen ?



/*
    WIZARDS.EXE  ovr056
*/

// WZD o56p01
void Production_Screen(void)
{
    int16_t unit_array[9];
    int16_t unit_array_count;
    int16_t hotkey_idx_ESC;
    int16_t var_E;
    int16_t DispCnt;
    int16_t current_index;
    int16_t active_product_idx;
    int16_t screen_changed;
    int16_t input_field_idx;
    int16_t scanned_field;
    int16_t itr;  // _SI_
    int16_t leave_screen;  // _DI_

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Production_Screen()\n", __FILE__, __LINE__);
#endif

    Cache_Graphics_Things();

    Deactivate_Help_List();

    popup_base_x = 0;
    popup_base_y = 0;

    list_field_width = 74;
    list_field_height = 11;
    list_item_height = 14;

    UU_width_90 = 90;
    UU_height_28 = 28;

    IDK_ProdScr_w42AC0 = 102;
    IDK_ProdScr_w42ABE = 55;

    unit_list_x1 = popup_base_x + 240;
    unit_list_y1 = popup_base_y + 4;
    bldg_list_x1  = popup_base_x;
    bldg_list_y1  = popup_base_y + 4;

    Set_Production_Screen_Help();

    Production_Screen_Load();

    Calculate_Product_Array(_city_idx, &build_table_field_count, &G_CTY_ProducableCount2, &G_CTY_ProdUnitCount, &product_indexes[0]);

    Army_At_City(_city_idx, &unit_array_count, unit_array);

    garrison_full = ST_FALSE;

    if(unit_array_count == MAX_STACK)
    {
        garrison_full = ST_TRUE;
    }

    active_product_idx = _CITIES[_city_idx].construction;

    for(itr = 0; itr < build_table_field_count; itr++)
    {
        if(product_indexes[itr] == active_product_idx)
        {
            current_item = itr;
            break;
        }
    }

    current_index = product_indexes[current_item];  // MoO2  _active_prod

    uv_specials_list_count = 0;

    if(product_indexes[current_item] < 100)
    {
        LBX_Load_Data_Static(buildesc_lbx_file, 0, (SAMB_ptr)building_description, product_indexes[current_item], 1, 200);
    }
    else
    {
        active_product_idx = (product_indexes[current_item] - 100);
        Prod_Load_Battle_Unit(active_product_idx, global_battle_unit);
        Prod_Build_Specials_List(active_product_idx, prod_specials_list, &uv_specials_list_count);
    }

    uv_specials_list_index = 0;
    uv_multipage = ST_FALSE;

    if(uv_specials_list_count > 9)
    {
        uv_multipage = ST_TRUE;
    }

    Deactivate_Auto_Function();

    Assign_Auto_Function(Production_Screen_Draw, 1);

    Production_Screen_Allocate_First_Block();

    UU_ProdScr_flag = ST_FALSE;

    screen_changed = ST_FALSE;

    var_E = 1;

    Clear_Fields();

    Set_Page_Off();

    Copy_On_To_Off_Page();

    Load_Palette_From_Animation(unitview_small_background_seg);

    Apply_Palette();

    Clear_Palette_Changes(0, 255);

    Set_Palette_Changes(0, 255);

    Update_Remap_Color_Range(0, 1);

    Gradient_Fill(0, 0, 319, 199, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL);  // ...remap_block, Slope, Scale, Seed

    Copy_Off_To_Back();

    Set_Input_Delay(1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        Mark_Time();

        Clear_Fields();

        Production_Screen_Add_Fields();

        hotkey_idx_ESC = Add_Hot_Key('\x1B');

        input_field_idx = Get_Input();

        scanned_field = Scan_Input();


        /*
            Cancel Button  || ESC Hot-Key
        */
        if(
            (input_field_idx == production_screen_cancel_button)
            ||
            (input_field_idx == hotkey_idx_ESC)
        )
        {
            // Play_Left_Click();
            leave_screen = ST_TRUE;
            current_screen = scr_City_Screen;
        }


        /*
            BEGIN:  Left-Click Build Table Left/Right
        */
        for(itr = 0; itr < build_table_field_count; itr++)
        {
            if(product_fields[itr] == input_field_idx)
            {

                // Play_Left_Click();

                Deactivate_Help_List();

                if(itr >= G_CTY_ProducableCount2)
                {
                    // WZD dseg:2DB5 54 68 65 72 65 20 69 73 20 6E 6F 74 20 65 6E 6F+aThereIsNotEnoughPop db 'There is not enough population to support new construction.',0
                    strcpy(GUI_String_1, "There is not enough population to support new construction.");
                    Warn1(GUI_String_1);
                    Assign_Auto_Function(Production_Screen_Draw, 1);
                    screen_changed = ST_TRUE;
                }
                else if((garrison_full == ST_TRUE) && (itr < G_CTY_ProdUnitCount))
                {
                    // WZD dseg:2D7D 54 68 65 72 65 20 61 72 65 20 61 6C 72 65 61 64+aThereAreAlreadyTooM db 'There are already too many units currently in the city.',0
                    strcpy(GUI_String_1, "There are already too many units currently in the city.");
                    Warn1(GUI_String_1);
                    Assign_Auto_Function(Production_Screen_Draw, 1);
                    screen_changed = ST_TRUE;
                }
                else if(current_item == itr)
                {

                    /* Left-Click on what is already being produced - Close Pop-Up ~ ESC, Cancel, OK  ¿ ~== Double-Click ? */
                    _CITIES[_city_idx].construction = product_indexes[current_item];
    if(_CITIES[_city_idx].construction < 0)
    {
        STU_DEBUG_BREAK();
    }
    if(_CITIES[_city_idx].construction > 298)
    {
        STU_DEBUG_BREAK();
    }

                    leave_screen = ST_TRUE;
                    current_screen = scr_City_Screen;
                    Do_City_Calculations(_city_idx);
                }
                else
                {
                    current_item = itr;
                    current_index = product_indexes[current_item];
                    screen_changed = ST_TRUE;
                    Production_Screen_Allocate_First_Block();
                    if(current_index < 100)
                    {
                        LBX_Load_Data_Static(buildesc_lbx_file, 0, (SAMB_ptr)building_description, product_indexes[current_item], 1, 200);
                    }
                }
            }
            Set_Production_Screen_Help();
        }
        /*
            END:  Left-Click Build Table Left/Right
        */


        /*
            OK Button
        */
        if(input_field_idx == production_screen_ok_button)
        {
            // Play_Left_Click();
Check_Cities_Data();
            _CITIES[_city_idx].construction = product_indexes[current_item];
// Check_Cities_Data();
Capture_Cities_Data();
    if(_CITIES[_city_idx].construction < 0)
    {
        STU_DEBUG_BREAK();
    }
    if(_CITIES[_city_idx].construction > 298)
    {
        STU_DEBUG_BREAK();
    }

            leave_screen = ST_TRUE;
            current_screen = scr_City_Screen;
            Do_City_Calculations(_city_idx);
        }


        /*
            'Thing View' Up/Dn Buttons
        */
        UV_Handle_Arrow_Buttons(input_field_idx, &DispCnt); 


        if((leave_screen == ST_FALSE) && (screen_changed == ST_FALSE))
        {
            Copy_Back_To_Off();
            Production_Screen_Draw();
            PageFlip_FX();
            var_E = 0;
            Release_Time(2);
        }
        screen_changed = ST_FALSE;
    }

    Deactivate_Auto_Function();

    Deactivate_Help_List();

    Clear_Fields();
    
    Reset_Window();

    // BUGBUG  these should be one or the other, and not allow for neither
    if(production_screen_return_screen == 1)
    {
        current_screen = scr_Cities_Screen;
    }
    if(production_screen_return_screen == 2)
    {
        current_screen = scr_City_Screen;
    }

    PageFlipEffect = 3;

    Clear_Palette_Changes(0, 255);

    Set_Palette_Changes(0, 223);

    Update_Remap_Gray_Palette();

    Cache_Graphics_Overland();

}


// WZD o56p02
void Production_Screen_Load(void)
{
    GUI_String_1 = (char *)Near_Allocate_First(80);
    prod_specials_list = (struct s_UV_List *)Near_Allocate_Next(1520);
    IDK_ProdScr_w42A94 = Near_Allocate_Next(110);
    product_indexes = (int16_t *)Near_Allocate_Next(40);  // 20 2-Byte, Signed  bldg_idx/unit_type_idx
    product_fields = (int16_t *)Near_Allocate_Next(50);  // 20 2-Byte, Signed  field_idx
    building_description = (char *)Near_Allocate_Next(200);
}


// WZD o56p03
// MoO2  Module: MAINSCR  Allocate_First_Block_()
void Production_Screen_Allocate_First_Block(void)
{
    int16_t unit_type;

    PS_product_pict_seg = Allocate_First_Block(_screen_seg, 30);  // 30 PR  480 B  21x21 == 441

    if(product_indexes[current_item] >= 100)
    {
        unit_type = (product_indexes[current_item] - 100);
        Load_Unit_Figure(unit_type, ST_TRUE);
    }

}


// WZD o56p04
void Production_Screen_Add_Fields(void)
{
    int16_t unit_item_cnt;
    int16_t UU_var_0A;
    int16_t y2;
    int16_t x2;
    int16_t y1;
    int16_t x1;
    int16_t itr;  // _SI_
    int16_t bldg_item_cnt;  // _DI_

    UU_var_0A = 0;

    bldg_item_cnt = 0;
    unit_item_cnt = 0;

    for(itr = 0; itr < build_table_field_count; itr++)
    {
        if(product_indexes[itr] < 100)
        {
            x1 = bldg_list_x1;
            y1 = (bldg_list_y1 + (bldg_item_cnt * list_item_height));
            // _help_entries[bldg_item_cnt].help_idx = HLP_BUILDING_OPTIONS
            bldg_item_cnt++;
        }
        else
        {
            x1 = unit_list_x1;
            y1 = (unit_list_y1 + (unit_item_cnt * list_item_height));
            // _help_entries[(13 + unit_item_cnt)].help_idx = HLP_UNIT_OPTIONS
            unit_item_cnt++;
        }
        x2 = x1 + list_field_width - 1;
        y2 = y1 + list_field_height - 1;

        product_fields[itr] = Add_Hidden_Field(x1, y1, x2, y2, 0, ST_UNDEFINED);
    }

    UV_Clear_Specials_Fields();

    if(product_indexes[current_item] >= 100)
    {
        UV_Add_Specials_Fields((popup_base_x + 85), (popup_base_y + 108), prod_specials_list, uv_specials_list_count, uv_specials_list_index);

        // TODO  _help_entries[38].help_idx = -1;
        // TODO  _help_entries[39].help_idx = -1;
        // TODO  _help_entries[40].help_idx = -1;
        // TODO  _help_entries[41].help_idx = -1;
        for(itr = 0; itr < uv_specials_list_count; itr++)
        {
            // TODO  _help_entries[(38 + itr)].help_idx = [word ptr (_unit_type_table[prod_specials_list[itr]].Transport+3F0h)]
        }
    }

    Set_Font_Style(4, 4, 0, 0);
    Set_Alias_Color(203);

    production_screen_cancel_button = Add_Button_Field((popup_base_x + 99), (popup_base_y + 181), "Cancel", red_button_seg, 'C', ST_UNDEFINED);

    production_screen_ok_button = INVALID_FIELD;

    production_screen_ok_button = Add_Button_Field((popup_base_x + 172), (popup_base_y + 181), "OK", red_button_seg, 'O', ST_UNDEFINED);

}


// WZD o56p05
void Production_Screen_Draw(void)
{
    Set_Page_Off();
    Production_Screen_Draw_();
}


// WZD o56p06
void Production_Screen_Draw_(void)
{
    uint8_t colors[6];
    int16_t build_table_bldg_item_cnt;
    int16_t build_table_unit_item_cnt;
    int16_t unit_type;
    int16_t product_idx;
    int16_t var_A;
    int16_t var_8;
    int16_t UU_y2;
    int16_t UU_x2;
    int16_t ystart;
    int16_t itr;  // _SI_
    int16_t xstart;  // _DI_

    xstart = popup_base_x;
    ystart = popup_base_y;

    UU_ProdScr_flag = ST_FALSE;

    UU_x2 = xstart + UU_width_90;
    UU_y2 = ystart + UU_height_28;

    Set_Font_Style(2, 1, 0, 0);

    var_8 = xstart + IDK_ProdScr_w42AC0;
    var_A = ystart + IDK_ProdScr_w42ABE;

    product_idx = product_indexes[current_item];

    if(product_idx < 100)
    {
        // TODO  [_help_entries.help_idx+118h], HLP_BUILDING_DESCRIPTION
        // TODO  [_help_entries.help_idx+122h], HLP_ALLOWS
        // TODO  [_help_entries.help_idx+12Ch], HLP_MAINTENANCE
        // TODO  [_help_entries.help_idx+136h], -1
        // TODO  [_help_entries.help_idx+14Ah], -1
        // TODO  [_help_entries.help_idx+154h], -1
        // TODO  [_help_entries.help_idx+15Eh], -1
        // TODO  [_help_entries.help_idx+168h], -1
        // TODO  [_help_entries.help_idx+172h], -1
        // TODO  [_help_entries.help_idx+17Ch], -1
        // TODO  [_help_entries.help_idx+186h], -1
        // TODO  [_help_entries.help_idx+190h], -1
        // TODO  [_help_entries.help_idx+19Ah], -1
        // TODO  [_help_entries.help_idx+1A4h], -1
        UV_Setup_ProdScr(0);
    }
    else
    {
        // TODO  [_help_entries.help_idx+118h], -1
        // TODO  [_help_entries.help_idx+122h], -1
        // TODO  [_help_entries.help_idx+12Ch], -1
        // TODO  [_help_entries.help_idx+136h], HLP_UNIT_MOVES
        // TODO  [_help_entries.help_idx+14Ah], HLP_MELEE
        // TODO  [_help_entries.help_idx+154h], HLP_RANGE
        // TODO  [_help_entries.help_idx+15Eh], HLP_ARMOR
        // TODO  [_help_entries.help_idx+168h], HLP_RESISTANCE
        // TODO  [_help_entries.help_idx+172h], HLP_HITS
        // TODO  [_help_entries.help_idx+17Ch], -1
        // TODO  [_help_entries.help_idx+186h], -1
        // TODO  [_help_entries.help_idx+190h], -1
        // TODO  [_help_entries.help_idx+19Ah], -1
        // TODO  [_help_entries.help_idx+1A4h], HLP_UPKEEP

        unit_type = (product_idx - 100);
        Prod_Load_Battle_Unit(unit_type, global_battle_unit);
        Prod_Build_Specials_List(unit_type, prod_specials_list, &uv_specials_list_count);
        UV_Setup_ProdScr(uv_specials_list_count);
    }

    // ¿ draws at {74, 0} ?
    // overlap with build table left and right
    Thing_View_Draw__WIP((popup_base_x + list_field_width), popup_base_y, 0, product_idx, prod_specials_list, uv_specials_list_count, PS_product_pict_seg);

    build_table_unit_item_cnt = 0;
    build_table_bldg_item_cnt = 0;

    for(itr = 0; itr < build_table_field_count; itr++)
    {
        if(product_indexes[itr] < 100)
        {
            xstart = bldg_list_x1;
            ystart = (bldg_list_y1 + (build_table_bldg_item_cnt * list_item_height));
            FLIC_Draw(xstart, ystart, IMG_CTY_LeftBldTab);
            build_table_bldg_item_cnt++;
        }
        else
        {
            xstart = unit_list_x1;
            ystart = (unit_list_y1 + (build_table_unit_item_cnt * list_item_height));
            FLIC_Draw(xstart, ystart, IMG_CTY_RightBldTab);
            build_table_unit_item_cnt++;
        }

        if(itr < G_CTY_ProducableCount2)
        {
            colors[0] = PAL_TURQUOISE_DIM;
            colors[1] = ST_BLACK;
            Set_Outline_Color(PAL_TURQUOISE_LIGHT);
            if(itr == current_item)
            {
                colors[0] = PAL_TURQUOISE_LIGHT;
                colors[1] = PAL_TURQUOISE_BRIGHT;
                Set_Outline_Color(PAL_TURQUOISE_DARK);
            }
            if((garrison_full == ST_TRUE) && (itr < G_CTY_ProdUnitCount))
            {
                colors[0] = PAL_GREY_DARK;
                colors[1] = PAL_GREY_LIGHT;
            }
        }
        else
        {
            colors[0] = PAL_TURQUOISE_DIM;
            colors[1] = PAL_GREY_LIGHT;
            Set_Outline_Color(PAL_TURQUOISE_LIGHT);
        }

        // MoO2  mov     edx, offset _l_font_color_array
        // MoO2  mov     eax, 4
        // MoO2  call    Set_Font_Style_Shadow_Down
        Set_Font_Colors_15(2, &colors[0]);
        Set_Font_Style_Shadow_Down(2, 15, 0, 0);
        Set_Font_Spacing_Width(1);

        if(product_indexes[itr] < 100)
        {
            Print((xstart + 2), (ystart + 1), bldg_data_table[product_indexes[itr]].name);
        }
        else
        {
            unit_type = (product_indexes[itr] - 100);
            Print((xstart + 3), (ystart + 1), *_unit_type_table[unit_type].name);
        }
    }

}


// WZD o56p07
// drake178: BLD_GetAllowsList()
void Building_Allows_List__WIP(int16_t bldg_idx, int16_t * allows_list_count, int16_t allows_list[])
{
    int16_t Building_Index;
    int16_t reqd_terrtype;
    int16_t Cant_Build;
    int16_t city_race;
    int16_t itr_cantbuild;
    int16_t itr;  // _SI_

    *allows_list_count = 0;

    city_race = _CITIES[_city_idx].race;

    for(itr = 0; itr < NUM_BUILDINGS; itr++)
    {
        Cant_Build = ST_FALSE;
        if(
            (bldg_data_table[itr].reqd_bldg_1 == bldg_idx)
            &&
            (bldg_data_table[itr].reqd_bldg_2 == bldg_idx)
        )
        {
            for(itr_cantbuild = 0; itr_cantbuild < _race_type_table[city_race].cant_build_count; itr_cantbuild++)
            {
                if(_race_type_table[city_race].cant_build[itr_cantbuild] == itr)
                {
                    Cant_Build = ST_TRUE;
                    break;
                }
            }

            if(Cant_Build == ST_FALSE)
            {
                if(bldg_data_table[itr].reqd_bldg_1 > 100)
                {
                    reqd_terrtype = bldg_data_table[itr].reqd_terrain;
                    if(
                        (CTY_CheckTerrainReq__ALWAYS_TRUE(_city_idx, reqd_terrtype) == ST_TRUE) &&
                        (_CITIES[_city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_2] != bs_Built)  // BUGBUG: drake178: may add replaced buildings to the list
                    )
                    {
                        allows_list[*allows_list_count] = itr;
                        *allows_list_count += 1;
                    }
                }
                else
                {
                    Building_Index = BLD_AllowedByRace__WIP_TRUE(bldg_idx, itr);
                    if(Building_Index != ST_UNDEFINED)
                    {
                        allows_list[*allows_list_count] = Building_Index;
                        *allows_list_count += 1;
                    }
                }
            }
        }
    }

    for(itr = 35; itr < 198; itr++)
    {
        // TODO  add allows units
    }

}


// WZD o56p08
// drake178: CTY_GetProducables()
// MoO2  Module: COLBLDG  Calculate_Building_Array_() + Calculate_Military_Array_()
/*
fills the return array and counts
with the units and buildings/projects
that may be produced in the selected city based on its current state;
buildings are limited to the first 10(+2)
units are limited to 11, and will omit spearmen and swordsmen if there would be more
the total is limited to 20
called from Production_Screen()
    CTY_GetProducables(_city_idx, &build_table_field_count, &G_CTY_ProducableCount2, &G_CTY_ProdUnitCount, &product_indexes[0]);

seems like this might have been total, building, and unit count
called three other places, and all three ignore depr_count and unit_count
Production_Screen() uses Total1 and Total1 to iter over the product_indexes

*/
void Calculate_Product_Array(int16_t city_idx, int16_t * total_count, int16_t * depr_count, int16_t * unit_count, int16_t product_array[])
{
    int16_t bldg_max_adj_amt;
    int16_t UU_bldg_built_count;
    int16_t city_race;
    int16_t cant_build_flag;
    int16_t reqd_terrtype;
    int16_t itr_cant_build;
    int16_t itr;  // _SI_
    int16_t product_count;  // _DI_

    city_race = _CITIES[city_idx].race;

    product_count = 0;

Check_Cities_Data();
    _CITIES[city_idx].bldg_cnt = 0;
// Check_Cities_Data();
Capture_Cities_Data();

    UU_bldg_built_count = 0;

    for(itr = 1; itr < NUM_BUILDINGS; itr++)
    {

        if(_CITIES[city_idx].bldg_status[itr] != bs_NotBuilt)
        {

Check_Cities_Data();
            _CITIES[city_idx].bldg_cnt++;
// Check_Cities_Data();
Capture_Cities_Data();

        }

        if(_CITIES[city_idx].bldg_status[itr] == bs_Built)
        {

            UU_bldg_built_count++;

        }

    }

    for(itr = 35; ((itr < 198) && (product_count < 12)); itr++)
    {

        if(
            (_unit_type_table[itr].Race == city_race)
            ||
            (_unit_type_table[itr].Race == rt_Standard)
        )
        {

            if(
                (_unit_type_table[itr].reqd_bldg_1 != ST_UNDEFINED)
                &&
                (_unit_type_table[itr].reqd_bldg_2 != ST_UNDEFINED)
            )
            {

                if(
                    (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_1] != bs_NotBuilt)
                    &&
                    (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_1] != bs_Removed)
                    &&
                    (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_2] != bs_NotBuilt)
                    &&
                    (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_2] != bs_Removed)
                )
                {

                    product_array[product_count] = (itr + 100);

                    product_count++;

                }

            }

        }

    }

    if(product_count > 11)
    {

        for(itr = 0; itr < product_count; itr++)
        {

            product_array[itr] = 0;

        }

        product_count = 0;

        for(itr = 35; ((itr < 198) && (product_count < 12)); itr++)
        {

            if(
                (_unit_type_table[itr].Race == city_race)
                ||
                (_unit_type_table[itr].Race == rt_Standard)
            )
            {

                if(
                    (_unit_type_table[itr].reqd_bldg_1 != -1)
                    &&
                    (_unit_type_table[itr].reqd_bldg_2 != -1)
                )
                {

                    if
                    (
                        (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_1] != bs_NotBuilt)
                        &&
                        (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_1] != bs_Removed)
                        &&
                        (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_2] != bs_NotBuilt)
                        &&
                        (_CITIES[city_idx].bldg_status[_unit_type_table[itr].reqd_bldg_2] != bs_Removed)
                    )
                    {

                        if(
                            (strcmp("Spearmen", *_unit_type_table[itr].name) != 0)
                            &&
                            (strcmp("Swordsmen", *_unit_type_table[itr].name) != 0)
                        )
                        {

                            product_array[product_count] = (itr + 100);

                            product_count++;

                        }
                    }

                }

            }

        }

    }

    *depr_count = product_count;  // BUGBUG  drake178: will be overwritten before returning

    *unit_count = product_count;

    bldg_max_adj_amt = product_count;

    // ¿ ~== MoO2  Module: COLBLDG  Colony_Can_Build_Product_()
    for(itr = 1; ((itr < NUM_BUILDINGS) && (product_count < 20)); itr++)
    {

        cant_build_flag = ST_FALSE;

        // if not already built/replaced
        if(
            (_CITIES[city_idx].bldg_status[itr] == bs_NotBuilt)
            ||
            (_CITIES[city_idx].bldg_status[itr] == bs_Removed)
        )
        {

            for(itr_cant_build = 0; itr_cant_build < _race_type_table[city_race].cant_build_count; itr_cant_build++)
            {

                if(_race_type_table[city_race].cant_build[itr_cant_build] == itr)
                {

                    cant_build_flag = ST_TRUE;

                    break;

                }

            }
            
            if(cant_build_flag == ST_FALSE)
            {

                if(bldg_data_table[itr].reqd_bldg_1 > 100)
                {

                    reqd_terrtype = bldg_data_table[itr].reqd_terrain;

                    // TODO  CTY_CheckTerrainReq__ALWAYS_TRUE()
                    if(CTY_CheckTerrainReq__ALWAYS_TRUE(city_idx, reqd_terrtype) == ST_TRUE)
                    {

                        if(
                            (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_2] != bs_NotBuilt)
                            &&
                            (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_2] != bs_Removed)
                        )
                        {

                            product_array[product_count] = itr;

                            product_count++;

                        }

                    }

                }
                else
                {

                    if(
                        (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_1] != bs_NotBuilt)
                        &&
                        (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_1] != bs_Removed)
                        &&
                        (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_2] != bs_NotBuilt)
                        &&
                        (_CITIES[city_idx].bldg_status[bldg_data_table[itr].reqd_bldg_2] != bs_Removed)
                    )
                    {

                        product_array[product_count] = itr;

                        product_count++;

                    }

                }

            }

        }

    }

    *depr_count += 2;  // BUGBUG  drake178: will be overwritten before returning

    /*
        Total Max    20
        Unit Max     12
        Bldg Max     12
        Bldg Min Max  8
        Maxes: product_count 20 = {12,8} ... {8,12}
    */
    bldg_max_adj_amt = (product_count - bldg_max_adj_amt);  // ~== product count - unit count = building count

    if(bldg_max_adj_amt > 12)
    {

        bldg_max_adj_amt = (bldg_max_adj_amt - 12);  // (bldg_max_adj_amt - 12) = count of building over the max of 12

        product_count = (product_count - bldg_max_adj_amt);  // truncate product_count

    }

    *total_count = product_count;

    *depr_count = product_count;  // overrides the previous two assignments to depr_count

}


// WZD o56p09
// CTY_CheckTerrainReq()
int16_t CTY_CheckTerrainReq__ALWAYS_TRUE(int16_t city_idx, int16_t Terrain_Req_ID)
{
    return ST_TRUE;
}

// WZD o56p10
// drake178: CTY_GetProduceCost()
int16_t City_Production_Cost(int16_t production_idx, int16_t city_idx)
{
    int16_t tmp_production_idx;

    int16_t production_cost;  // _SI_

    production_cost = 0;

    tmp_production_idx = _CITIES[city_idx].construction;

Check_Cities_Data();
    _CITIES[city_idx].construction = production_idx;
// Check_Cities_Data();
Capture_Cities_Data();

if(_CITIES[city_idx].construction < 0)
{
    STU_DEBUG_BREAK();
}
if(_CITIES[city_idx].construction > 298)
{
    STU_DEBUG_BREAK();
}

Check_Cities_Data();
    production_cost = City_Current_Product_Cost(city_idx);
Check_Cities_Data();

    production_cost -= _CITIES[city_idx].Prod_Accu;

    if(production_cost < 0)
    {
        production_cost = 0;
    }

Check_Cities_Data();
    _CITIES[city_idx].construction = tmp_production_idx;
// Check_Cities_Data();
Capture_Cities_Data();
    if(_CITIES[city_idx].construction < -4)
    {
        STU_DEBUG_BREAK();
    }
    if(_CITIES[city_idx].construction > 298)
    {
        STU_DEBUG_BREAK();
    }

    return production_cost;
}


// WZD o56p11
// drake178: N/A
// MoO2  Module: COLCALC  Colony_N_Turns_To_Produce_()
int16_t City_N_Turns_To_Produce(int16_t production_cost, int16_t city_idx)
{

    int16_t turns_to_produce;  // _SI_

    turns_to_produce = 0;

    if(_CITIES[city_idx].production_units <= 0)
    {
        if(City_Production_Cost(_CITIES[city_idx].construction, city_idx) == 0)
        {
            turns_to_produce = 1;
        }
        else
        {
            turns_to_produce = 999;
        }
    }
    else
    {
        turns_to_produce = (production_cost / _CITIES[city_idx].production_units);
        
        // TODO  ROUNDUP()
        if((production_cost % _CITIES[city_idx].production_units) != 0)
        {
            turns_to_produce += 1;
        }

        if(turns_to_produce == 0)
        {
            turns_to_produce = 1;
        }
    }

    return turns_to_produce;
}


// WZD o56p12
// drake178: BLD_AllowedByRace()
int16_t BLD_AllowedByRace__WIP_TRUE(int16_t UU_Bld1, int16_t Building)
{
    int16_t Building_List;
    int16_t City_Screen_Race;
    int16_t Result;

    Result = Building;

    return Result;
}
