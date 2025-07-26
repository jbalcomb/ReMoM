
!!!! COMBAT ENTITIES IS PER-FIGURE !!!!!
(8 * (2 * 9)) = 144)
256 - 144 = 112 other entities



NOTE: this are rebuilt for every screen draw call



CMB_CreateEntity__WIP()
sets
    draw_x
    draw_y
    seg_or_idx
    frame_num
    draw_x_shift
    draw_y_shift
    entity_type
    owner_idx
    draw_order_value



MainScr_Maps.C
Draw_Map_Units()
    unit_idx = entities_on_movement_map[entities_index];
        if((unit_idx != ST_UNDEFINED) && (unit_idx < MAX_UNIT_COUNT))
            Draw_Unit_Picture(itr_screen_x, itr_screen_y, unit_idx, 2);
                Draw_Unit_StatFig(x, y, unit_idx, flag);
                    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
                    Draw_Picture(x+1, y+1, UnitDraw_WorkArea);
So, ...
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
    ~==
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);



Tactical_Combat_Draw()
    CMB_CreateEntities__WIP()
    CMB_DrawMap__WIP()
        Combat_Figure_Compose_USEFULL();
        CMB_DrawEntities__WIP()
            Draw_Picture_Windowed()
                battle_unit_picts_seg[]

Tactical_Combat__WIP()
CMB_LoadResources__WIP()
    battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55)



Tactical_Combat_Draw()
    CMB_CreateEntities__WIP();
        CMB_EntitiesReset();
        ...
        CMB_SpawnFigure__WIP(
            battle_units[itr].bufpi,
            battle_units[itr].cgx,
            battle_units[itr].cgy,
            battle_units[itr].target_cgx,
            battle_units[itr].target_cgy,
            battle_units[itr].move_anim_ctr,
            itr_figures,
            unit_figure_maximum,
            battle_units[itr].controller_idx,
            battle_units[itr].outline_magic_realm,
            battle_units[itr].Blood_Amount,
            battle_units[itr].Moving,
            battle_units[itr].Atk_FigLoss,
            0
        );
            CMB_CreateEntity__WIP(draw_x, draw_y, seg_or_idx, 13, 23, UU, 1, controller_idx, figure_set_idx, outline_magic_realm, BldAmt, UU, Blood_Frame);
                Set_Entity_Draw_Order();
...
What's with the 13 and 23?

CMB_SpawnFigure__WIP() uses itr_figures to calc fix_x,fig_y and then uses those in the calc for draw_x,draw_y
    combat_grid_entities[combat_grid_entity_count].draw_x = draw_x;
    combat_grid_entities[combat_grid_entity_count].draw_y = draw_y;




## Set_Entity_Draw_Order()
OON XREF:  CMB_CreateEntity__WIP()
sets combat_grid_entities_draw_order[]




CMB_UpdateTrueSight()
    CMB_ATKR_TrueSight = ST_FALSE/ST_TRUE
    CMB_DEFR_TrueSight = ST_FALSE/ST_TRUE
Up   r BU_IsVisible+97                   cmp     [CMB_ATKR_TrueSight], e_ST_TRUE 
     r BU_GetRangedCursor__WIP:loc_800C3 cmp     [CMB_ATKR_TrueSight], e_ST_TRUE 
Down w CMB_UpdateTrueSight+3             mov     [CMB_ATKR_TrueSight], e_ST_FALSE
Down w CMB_UpdateTrueSight+54            mov     [CMB_ATKR_TrueSight], e_ST_TRUE 
Down r BU_SetVisibility__WIP+9E          cmp     [CMB_ATKR_TrueSight], e_ST_TRUE 
Down r Auto_Move_Unit+3C4                cmp     [CMB_ATKR_TrueSight], e_ST_TRUE 
Up   r BU_IsVisible+AA          cmp     [CMB_DEFR_TrueSight], e_ST_TRUE 
     w CMB_UpdateTrueSight+9    mov     [CMB_DEFR_TrueSight], e_ST_FALSE
Down w CMB_UpdateTrueSight+89   mov     [CMB_DEFR_TrueSight], e_ST_TRUE 
Down r BU_SetVisibility__WIP+C6 cmp     [CMB_DEFR_TrueSight], e_ST_TRUE 
Down r Auto_Move_Unit+3E3       cmp     [CMB_DEFR_TrueSight], e_ST_TRUE 





Combat.C
// WZD dseg:D15A
struct s_COMBAT_ENTITY * combat_grid_entities;
// WZD dseg:D15E
int16_t combat_grid_entity_count;

// WZD o99p06
void CMB_CreateEntities__WIP(void)

// WZD ovr153p14
void CMB_EntitiesReset(void)





## CMB_DrawEntities__WIP()
OON XREF: CMB_DrawMap__WIP()

combat_grid_entities[]
combat_grid_entity_count






## CMB_EntitiesReset()
combat_grid_entity_count = 0;
for(itr = 0; itr < 256; itr++)
    combat_grid_entities_draw_order[itr] = itr;


// WZD ovr153p15
void USELESS_Combat_Figure_Load_Compose(int16_t bufpi, int16_t figure_set_idx, int16_t player_idx, int16_t enchantment_magic_realm, int16_t frame_num)
    FLIC_Set_CurrentFrame(ptr_figure_pointer_seg[figure_set_idx], frame_num);
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);

¿ ~ UnitView.C Draw_Unit_Figure() ?
...
    unit_figure_count = _unit_type_table[unit_type].Figures;
    for(itr_figure_count = 0; itr_figure_count < unit_figure_count; itr_figure_count++)
        FLIC_Set_CurrentFrame(unit_figure_seg, 2);
        Draw_Picture_To_Bitmap(unit_figure_seg, UnitDraw_WorkArea);
        ...
        Unit_Figure_Position(unit_figure_count, itr_figure_count, &fig_y, &fig_x);
        Draw_Picture((x_start + fig_y), (y_start + fig_x), UnitDraw_WorkArea);



## combat_grid_entity_count

XREF:
    CMB_DrawEntities__WIP:loc_DD3A3 cmp     _DI_itr_combat_grid_entities, [combat_grid_entity_count]
    CMB_DrawEntities__WIP:loc_DD4EF cmp     _DI_itr_combat_grid_entities, [combat_grid_entity_count]
    CMB_EntitiesReset+3             mov     [combat_grid_entity_count], 0                           
    CMB_CreateEntity__WIP+B         cmp     [combat_grid_entity_count], 256                         
    CMB_CreateEntity__WIP:loc_DF255 mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+2E        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+43        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+58        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+6D        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+82        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+97        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+AC        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+C1        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+D6        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+EB        mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+100       mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+115       mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+199       mov     ax, [combat_grid_entity_count]                          
    CMB_CreateEntity__WIP+1B4       inc     [combat_grid_entity_count]                              
    Set_Entity_Draw_Order+8         cmp     [combat_grid_entity_count], 0                           
    Set_Entity_Draw_Order:loc_DF409 mov     ax, [combat_grid_entity_count]                          
    Set_Entity_Draw_Order:loc_DF42A cmp     _SI_itr_up, [combat_grid_entity_count]                  
    Set_Entity_Draw_Order:loc_DF455 cmp     _SI_itr_up, [combat_grid_entity_count]                  
    Set_Entity_Draw_Order+69        mov     _DI_itr_dn, [combat_grid_entity_count]                  
    Set_Entity_Draw_Order+7E        mov     al, [byte ptr combat_grid_entity_count]                 




## Battle_Unit_Figure_Position()

// WZD ovr153p17
// drake178: CMB_GetFigDrawPos()
void Battle_Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y)
// WZD o89p06
void Unit_Figure_Position(int16_t figure_count, int16_t current_figure, int16_t * figure_x, int16_t * figure_y)

XREF:
    j_Battle_Unit_Figure_Position()
        BU_CreateImage__SEGRAX+2C9 call    j_Battle_Unit_Figure_Position
        CMB_SetProjectiles+19      call    j_Battle_Unit_Figure_Position
        CMB_SetProjectiles+33      call    j_Battle_Unit_Figure_Position



loc_84B39:
lea     ax, [bp+fig_y]
push    ax                              ; Top@
lea     ax, [bp+fig_x]
push    ax                              ; Left@
push    [bp+itr_figures]                ; Cur_Fig
push    [bp+max_figs]                   ; Max_Figs
call    j_Battle_Unit_Figure_Position
add     sp, 8


push    [GfxBuf_2400B]                  ; src_pict_seg
push    [IMG_CMB_FX_Figure@]            ; dst_pict_seg
mov     ax, [bp+fig_y]
add     ax, -4
push    ax                              ; y
push    [bp+fig_x]                      ; x
call    Clipped_Copy_Bitmap
add     sp, 8

// WZD s103p07
// drake178: BU_CreateImage()
/*
; allocates a 2k buffer in World_Data@, into which it
; composes an image of the battle unit, saving the
; pointer to IMG_CMB_FX_Figure@ - this is drawn
; separately from the combat entity sprites
*/
/*

~ USELESS_Combat_Figure_Load_Compose()

*/
void BU_CreateImage__SEGRAX(int battle_unit_idx)

    for(itr_figures = 0; itr_figures < cur_fig; itr_figures++)
        Battle_Unit_Figure_Position(max_fig, itr_figures, &fig_x, &fig_y);
        Clipped_Copy_Bitmap(fig_x, fig_y - 4, IMG_CMB_FX_Figure, GfxBuf_2400B);
