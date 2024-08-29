
MoX / MoM
Cycling
Timer
Clock
Animation


Node Aura

Active Stack
Reduced Map Dot


win_CLK.C
    Get_System_Clock_Counter()

MainScr_Maps.C
Cycle_Map_Animations()
looks good at every fourth
what about blinking active stack?
what about unit enchantment outline?

MainScr.C
    Cycle_Unit_Enchantment_Animation()


MainScr_Maps.C
    List_Screen_Draw_Reduced_Map()
        reduced_map_mark_cycle


## Cycle_Unit_Enchantment_Animation()
    unit_enchantment_animation_flag += 1;
    if(unit_enchantment_animation_flag > 1)
        unit_enchantment_animation_flag = 0;
    if(unit_enchantment_animation_flag < 1)
        unit_enchantment_animation_count += 1;
        if(unit_enchantment_animation_count > 7)
            unit_enchantment_animation_count = 0;
...seems called by each screen draw routine as needed
call on every screen loop
which includes Release_Time()  ... Release_Time(1); for 'Main Screen'
    ...Screen_Draw;  PageFlip_FX();  Release_Time(1);
increments the counter on every call
    oddly not clever toggle type ~ counter = (1 - counter) or logical NOT


color_list = (uint8_t *)(enchantment_outline_colors + ((enchantment_magic_realm * 64) + (unit_enchantment_animation_count * 8)));




ovr063
dseg:305C 00 00                                           unit_weapon_type_animation_count dw 0   ; DATA XREF: Main_Screen_Draw_Unit_Window+90r ...
dseg:305E 00 00                                           unit_enchantment_animation_count dw 0   ; DATA XREF: Draw_Unit_Enchantment_Outline+309r ...
dseg:3060 00 00                                           unit_enchantment_animation_flag dw 0    ; DATA XREF: Cycle_Unit_Enchantment_Animation+3r ...
dseg:3062 2D 2C 2B 2A 29 2A 2B 2C 2C 2B 2A 29 2A 2B 2C 2D+enchantment_outline_colors


## List_Screen_Draw_Reduced_Map()
    reduced_map_mark_cycle = ((reduced_map_mark_cycle + 1) % 8);
    mark_flag  = ST_FALSE;
    // NOTE: same logic as cycler on 'Main Screen' for 'Active Stack'
    if(reduced_map_mark_cycle < 4)
    {
        mark_flag = ST_TRUE;
    }
    Reduced_Map_Coords(&reduced_map_x, &reduced_map_y, wx, wy, w, h);
    Draw_Reduced_Map(reduced_map_x, reduced_map_y, wp, _reduced_map_seg, w, h, wx, wy, mark_flag);



## Draw & Input

Mark_Time() ... Release_Time() ...

Input_Delay()



win_CLK.C
void Release_Time(int ticks)


// ~== INT 1A,0 - Read System Clock Timer
uint32_t Get_System_Clock_Counter(void)
{
    uint32_t dos_tick_count;
    DWORD win_tick_count;
#pragma warning(suppress : 28159)
    win_tick_count = GetTickCount();  // TODO  "Consider using GetTickCount64() ..."
    // dos_tick_count = win_tick_count / (uint32_t)54.92540;  // TODO(JimBalcomb,20231115): maybe identify castings that are purposeful and/or reasonable... macro? IMA_UINT32
    // dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 2.0);
    dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 1.5);
    return dos_tick_count;
}

