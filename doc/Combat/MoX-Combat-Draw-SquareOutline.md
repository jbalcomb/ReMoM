



"...the currently active unit (a red outline surrounds the active unit’s square on the combat grid)."
active unit
"Note that as you run the mouse cursor over the map squares of the combat grid, the squares appear outlined in blue."



CMB_ActiveUnitFrame  ==>  frame_active_flag
CMB_TargetFrame ==>  frame_scanned_flag



loaded in Reload_Combat_Graphics_Cache()

drawn in CMB_DrawMap__WIP()

frame_anim_cycle is cycled at the end of Tactical_Combat_Draw()



Tactical_Combat__WIP()
    |-> Tactical_Combat_Draw()
        |-> CMB_DrawMap__WIP()



// WZD dseg:7080
int16_t frame_anim_cycle = 0;

// WZD dseg:9278
int16_t frame_active_cgy;
// WZD dseg:927A
int16_t frame_active_cgx;
// WZD dseg:927C
int16_t frame_active_flag;
// WZD dseg:927E
int16_t frame_scanned_cgy;
// WZD dseg:9280
int16_t frame_scanned_cgx;
// WZD dseg:9282
int16_t frame_scanned_flag;

// WZD dseg:D188
SAMB_ptr frame_active_seg;
// WZD dseg:D18A
SAMB_ptr frame_scanned_seg;



Reload_Combat_Graphics_Cache()
    frame_scanned_seg = LBX_Reload_Next(cmbtcity_lbx_file__ovr052, 67, GFX_Swap_Seg);
    frame_active_seg = LBX_Reload_Next(cmbtcity_lbx_file__ovr052, 68, GFX_Swap_Seg);

CMB_DrawMap__WIP()
    FLIC_Set_CurrentFrame(frame_scanned_seg, frame_anim_cycle);
    Clipped_Draw(screen_x, screen_y, frame_scanned_seg);
    FLIC_Set_CurrentFrame(frame_active_seg, frame_anim_cycle);
    Clipped_Draw(screen_x, screen_y, frame_active_seg);



## frame_active_flag

XREF:
    Tactical_Combat__WIP+246     mov     [frame_active_flag], 0         
    Tactical_Combat__WIP+29E     mov     [frame_active_flag], 0         
    Tactical_Combat__WIP+5D5     mov     [frame_active_flag], 0         
    CMB_ProgressTurnFlow__WIP+C  mov     [frame_active_flag], 0         
    CMB_ProgressTurnFlow__WIP+34 mov     [frame_active_flag], 0         
    Assign_Mouse_Image+E         mov     [frame_active_flag], 0         
    Assign_Mouse_Image+1B        mov     [frame_active_flag], 1         
    Assign_Mouse_Image:loc_7FC8F mov     [frame_active_flag], 0         
    Assign_Mouse_Image+324       mov     [frame_active_flag], e_ST_FALSE
    CMB_SetVortexCursor+2E       mov     [frame_active_flag], 1         
    CMB_VortexMovement+32        mov     [frame_active_flag], 0         
    CMB_DrawMap__WIP:loc_DCF93   cmp     [frame_active_flag], 1         


## frame_scanned_flag

set to ST_TRUE in the three different mouse image functions

Assign_Mouse_Image()
    if(_active_battle_unit > ST_UNDEFINED)

XREF:
    Tactical_Combat__WIP+24C                  mov     [frame_scanned_flag], 0         
    Tactical_Combat__WIP+2A4                  mov     [frame_scanned_flag], 0         
    Tactical_Combat__WIP+5DB                  mov     [frame_scanned_flag], 0         
    CMB_ProgressTurnFlow__WIP+12              mov     [frame_scanned_flag], 0         
    CMB_ProgressTurnFlow__WIP+3A              mov     [frame_scanned_flag], 0         
    Assign_Mouse_Image+98                     mov     [frame_scanned_flag], 0         
    Assign_Mouse_Image:loc_7FD0D              mov     [frame_scanned_flag], e_ST_TRUE 
    Assign_Mouse_Image:loc_7FD28              mov     [frame_scanned_flag], 1         
    Assign_Mouse_Image:loc_7FDFF              mov     [frame_scanned_flag], 1         
    Assign_Mouse_Image+32A                    mov     [frame_scanned_flag], e_ST_FALSE
    Combat_Set_Mouse_List_Image_Num+31        mov     [frame_scanned_flag], 0         
    Combat_Set_Mouse_List_Image_Num:loc_8F5C6 mov     [frame_scanned_flag], 1         
    CMB_SetVortexCursor:loc_B7E42             mov     [frame_scanned_flag], 0         
    CMB_SetVortexCursor+11A                   mov     [frame_scanned_flag], 1         
    CMB_DrawMap__WIP+5AE                      cmp     [frame_scanned_flag], 1         
