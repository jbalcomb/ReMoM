






## Road Building Effect

Road_Build_Screen()
    |-> RdBd_UNIT_MoveStack_STUB()

...
    |-> WIZ_NextIdleStack()
        |-> RdBd_UNIT_MoveStack_STUB()

WIZ_NextIdleStack()
    |-> RdBd_UNIT_MoveStack_STUB()

Road_Build_Screen()
    if(input_field_idx == roadbuild_ok_button_field)
        ...
        Active_Stack_Roadbuilders()
        Set_Army_Road_Building()
        RdBd_UNIT_MoveStack_STUB()
        WIZ_NextIdleStack()

RdBd_UNIT_MoveStack__WIP
XREF:
    WIZ_NextIdleStack()
    j_RdBd_UNIT_MoveStack_STUB()
        Road_Build_Screen()
        AI_UNIT_Move()

WIZ_NextIdleStack()
XREF:
    Move_Stack()
    j_WIZ_NextIdleStack()
        GAME_Overland_Init()
        Main_Screen()
        Main_Screen()
        Main_Screen()
        Main_Screen()
        Main_Screen()
        Main_Screen()
        Next_Turn_Proc()
        EarthGateTeleport__WIP()
        Road_Build_Screen()
        Unit_Statistics_Popup()
        GAME_NextHumanStack()

GAME_NextHumanStack()
    j_GAME_NextHumanStack()
        Loaded_Game_Update_WZD()
        Loaded_Game_Update()






## Path_Wrap()

XREF:
    Set_Active_Stack_Movement_Path()
    Road_Build_Path()
    Set_Army_Road_Building()
    Move_Units()
    UU_OVL_GetLinePath()
    sub_D601B()
    UU_SPATH_Segmented()

1oom

...
Clipped_Multi_Colored_Line()
uistarmap_common.c
uistarmap_enroute.c
uistarmap_orbit_own.c
uistarmap_reloc.c
uistarmap_trans.c
uistarmap_transport.c
...
uistarmap_common.c
ui_starmap_draw_starmap()
    ...colortbl_line_reloc
    ...fleet_enroute_t
    ...transport_t
...
util_math.c
util_math_get_route_len()
OON XREF:
    game_battle_item_move_find_route()
...
util_math.c
util_math_line_plot()
...
game_battle_set_route_from_tbl()
...
game_fleet.c
game_send_fleet_do()
    ...sets g->enroute[g->enroute_num]






## Road Build Path

## Road Build Path Line

Draw_Road_Build_Path_Line()
    |-> Clipped_Multi_Colored_Line()
        |-> Clipped_Line_Base()
            |-> Multi_Colored_Line()

Clipped_Multi_Colored_Line(x1, y1, x2, y2, &Pattern[0], P_Size, P_Off)
Clipped_Line_Base()

push    [bp+P_Off]                      ; P_Off
push    [bp+P_Size]                     ; P_Size
push    [bp+Pattern@]                   ; Pattern@
xor     ax, ax
push    ax                              ; color
mov     ax, 1
push    ax                              ; Patterned
push    [bp+y2]                         ; y2
push    [bp+x2]                         ; x2
push    [bp+y1]                         ; y1
push    [bp+x1]                         ; x1
nop
push    cs
call    near ptr Clipped_Line_Base




MoO2
    Do_Ship_Destination_Lines_()
    Draw_ETA_Destination_Line_()
    Draw_Fltscrn_Ship_Destination_Lines_()
    _ship_direction_line_green_colors
    _ship_direction_line_red_colors

1oom
    ctbl = ((r->owner == d->api) && (d->en.can_move != NO_MOVE) && (!d->en.in_frange)) ? colortbl_line_red : colortbl_line_green;
    ui_draw_line_limit_ctbl(x0 + 4, y0 + 1, x1 + 6, y1 + 6, ctbl, 5, ui_data.starmap.line_anim_phase, starmap_scale);





## _map_square_flags

// WZD dseg:9CB4
/*
8 bit bitfield

// 0x01
// 0x02
// 0x04
#define TF_Road         0x08
#define TF_Enc_Road     0x10
#define TF_Corruption   0x20
#define TF_Unk_40       0x40
#define TF_NoRoad80     0x80  // ~== -1 ST_UNDEFINED
#define TF_AnyRoad      0x18
*/
uint8_t * _map_square_flags;                // load in Load_SAVE_GAM()




##### Naming Things Is HArd

CITYSCAP_LBX
     5  BACKS       roads
    15  3DROADS     left mask
    16  3DROADS     right mask
HLPENTRY_LBX
    19  road Help
MAIN_LBX
    45    MAINROAD    road background
    46    CASTCNCL    road ok button
    48    C&RBORDR    road button border

MAPBACK_LBX
    45 ROADS    no road����������������
    46 ROADS    top��������������������
    47 ROADS    top right��������������
    48 ROADS    right������������������
    49 ROADS    bottom right�����������
    50 ROADS    bottom�����������������
    51 ROADS    bottom left������������
    52 ROADS    left�������������������
    53 ROADS    top left���������������
MAPBACK_LBX E_ROADS
