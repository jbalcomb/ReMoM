

Elsewhere, ...
MoO2
Module: MOX
    data (0 bytes) _combat_bitmap
    Address: 02:0018A824
    data (0 bytes) _draw_ship_bitmap
    Address: 02:0018A83C
    data (0 bytes) _ship_bitmap
    Address: 02:0018A840


// MoO2: ~ Clipped_Draw_Picture()
Draw_Picture_Windowed()
    |-> Draw_Picture_ASM()
...clipped...
...no RLE...

battle_unit_picts_seg[]
CMB_DrawEntities__WIP() |-> Draw_Picture_Windowed()
¿ remapped ?  ¿ of course, because wizard colors ?
55 PR, 880 B
1 per battle unit

size of allocation?
FIGUREX_OFFSET uses 56 PR
CMB_LoadResources__WIP() uses 55
BU_CreateImage__SEGRAX() uses 124
¿ figure_pointer_seg = Allocate_First_Block((EMS_PFBA + offset), 33) ?

FIGUREX_MAP
FIGUREX_OFFSET
FIGUREX_POINTER

## figure_set_idx
    which of the eight figures lbx entries to use
    e.g., ptr_figure_pointer_seg[figure_set_idx]
    passed to CMB_CreateEntity__WIP()
        CMB_CreateEntity__WIP(draw_x, draw_y, seg_or_idx, 13, 23, UU, 1, controller_idx, figure_set_idx, outline_magic_realm, BldAmt, UU, Blood_Frame);




## Combat_Figure_Load()

Where a figures loaded for the 'Main Screen'?
// WZD dseg:500A                                                 BEGIN:  ovr089 - Strings

// WZD dseg:500A 46 49 47 55 52 45 00                            cnst_FIGFile_Base db 'FIGURE',0
// WZD dseg:5011 53 00                                           cnst_FIGFile_S db 'S',0
// WZD dseg:5013 2E 4C 42 58 00                                  cnst_LBX_Ext db '.LBX',0


MoO2
Remap_Draw_Ship_Status_
    Draw_Picture_To_Bitmap(_draw_ship_bitmap, Get_Ship_Id_Picture_Seg_(_global_combat_data.ship_idx))
    Create_Blank_Bitmap(98, 98, _combat_bitmap)
    Add_Picture_To_Bitmap(16, 11, _draw_ship_bitmap, _combat_bitmap)
    Outline_Bitmap(_combat_bitmap)
    Replace_Color(_combat_bitmap, 1, 0);
    Replace_Color(_combat_bitmap, 2, 0);
    Draw(si, di, _combat_bitmap)
## Combat_Figure_Compose_USEFULL()
temp_GfxBuf_2400B = GfxBuf_2400B;
    GfxBuf_2400B = battle_unit_picts_seg[battle_units[battle_unit_idx].bufpi];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(outline_magic_realm);
    Combat_Figure_Effect__WIP(IMG_Effect);
    Combat_Figure_Active_Red_Outline(battle_unit_idx);

## battle_unit_picts_seg[]

CMB_LoadResources__WIP()
    battle_unit_picts_seg[itr] = Allocate_Next_Block(_screen_seg, 55)



BU_CreateImage__SEGRAX()
    IMG_CMB_FX_Figure = Allocate_Next_Block(World_Data, 124);  // 124 PR, 1984 B
    Create_Picture(47, 42, IMG_CMB_FX_Figure)
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B)
    Battle_Unit_Figure_Position(max_fig, itr_figures, &fig_x, &fig_y)
    Clipped_Copy_Bitmap(fig_x, fig_y - 4, IMG_CMB_FX_Figure, GfxBuf_2400B)







CMB_ComposeBackgrnd__WIP()  ==>  Combat_Screen_Compose_Background()
CMB_DrawMap__WIP  ==>  Combat_Screen_Draw_Map()


```c
    Reset_Cycle_Palette_Color();
    Load_Palette_From_Animation(combat_background_bottom);
    Apply_Palette();
    Cycle_Palette_Color__STUB(198, 0, 0, 0, 55, 0, 0, 8);
    Set_Palette_Changes(0, 243);
    Calculate_Remap_Colors();
```

## Copy_Off_To_Back() && Copy_Back_To_Off()
Combat_Screen_Compose_Background() && CMB_DrawMap__WIP() 
NOTE: combat information window and combat spellbook have their own Copy_Off_To_Back() && Copy_Back_To_Off()
...combat results uses whatever was the last/previous Copy_Off_To_Back()?



NOTE: Tactical_Combat_Draw() |-> CMB_DrawMap__WIP() calls Copy_Back_To_Off(), where 'Back' was drawn in CMB_ComposeBackgrnd__WIP()

CMB_ComposeBackgrnd__WIP()
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();
...
the last thing CMB_Terrain_Init__WIP() does is call CMB_ComposeBackgrnd__WIP()

CMB_ComposeBackgrnd__WIP()
    j_CMB_ComposeBackgrnd__WIP jmp     
        Tactical_Combat__WIP+B98             
        Tactical_Combat__WIP+D4D             
        Tactical_Combat__WIP+E59             
        Tactical_Combat__WIP+F02             
        Tactical_Combat__WIP+1127            
        Combat_Cast_Spell_With_Caster:@@Done 
        CMB_ComposeBookBG__WIP+3             
        Combat_Cast_Spell__WIP:loc_8D121     
        Combat_Cast_Spell__WIP+35D           
        Combat_Spellbook_Screen+7A6          
        TILE_DispelMagic+EA2                 
        CMB_RaiseDead+8                      
        CMB_AnimateDead+D                    
        TILE_BoltFromAbove+1ED               
        CMB_CallChaos:loc_B85C1              
        CMB_CallChaos+460                    
        CMB_SpellSlider__STUB+2FB            
        CMB_Terrain_Init__WIP+5CD            

CMB_Terrain_Init__WIP()
    j_CMB_Terrain_Init__WIP()
        NX_IDK_CombatInit_Tactical()
        Tactical_Combat__WIP()

Load_Combat_Terrain_Pictures()
loads set of 48 terrain pictures, 5 trees, 5 rocks
CMB_Terrain_Init__WIP()
    |-> Load_Combat_Terrain_Pictures(cts, wp);



## CMB_ComposeBackgrnd__WIP()
    Set_Page_Off()
    Reset_Window()
    EMM_TILEX_Init__HACK()
    ...
    Clipped_Draw(screen_x, screen_y, _combat_terrain_pict_segs[combat_terrain_type])
    ...
    FLIC_Draw(0, 164, combat_background_bottom);
    Copy_Off_To_Back();

_combat_terrain_pict_segs[48]  {0,...47}







any correlation with 'Main Screen'?
Draw_Maps()
    |-> Draw_Map_Window()
        |-> Draw_Map_Terrain()
        |-> Draw_Map_Minerals()
        |-> Draw_Map_Biota()
        |-> Draw_Map_Roads()
        |-> Draw_Map_Cities()
        |-> Draw_Map_Towers()
        |-> Draw_Map_Lairs()
        |-> Draw_Map_Units()
        |-> Draw_Map_Nodes()
        |-> Draw_Map_Unexplored_Area()
        |-> Cycle_Map_Animations()
            map_anim_cycle...terrain_anim_ctr, node_anim_ctr, road_anim_ctr





Tactical_Combat__WIP()



Tactical_Combat_Draw()
    CMB_CreateEntities__WIP()
    CMB_DrawMap__WIP()
        CMB_DrawEntities__WIP()




## CMB_DrawMap__WIP()
    Copy_Back_To_Off()
    Combat_Figure_Compose_USEFULL()
    Set_Page_Off()
    Set_Window()

    for(itr_y = 0; itr_y < 22; itr_y++)
        for(itr_x = 0; itr_x < 11; itr_x++)
            mud
            battlefield_terrain
            Central_Structure ... CS_SorceryNode, CS_ChaosNode, 
            Roads
            Central_Structure ... CS_Outpost
            Central_Structure ... CS_City || CS_Fortress
            Chasm
    for(itr_y = 0; itr_y < 22; itr_y++)
        for(itr_x = 0; itr_x < 11; itr_x++)
            active unit map square red outline
            scanned map square blue outline
    ...cycle animations...
    CMB_DrawEntities__WIP()
    Reset_Window()



## CMB_DrawEntities__WIP()
OON XREF: CMB_DrawMap__WIP()

combat_grid_entities[]
combat_grid_entity_count


for(itr_combat_entity_draw_order = 0; itr_combat_entity_draw_order < combat_grid_entity_count; itr_combat_entity_draw_order++)
    combat_grid_entity_idx = combat_grid_entities_draw_order[itr_combat_entity_draw_order];
    if(combat_grid_entities[combat_grid_entity_idx].entity_type == 1)  /* Battle Unit Figure */
        if(combat_grid_entities[combat_grid_entity_idx].Blood_Amt == 0)
            ptr_combat_grid_entity = &combat_grid_entities[combat_grid_entity_idx];
            pict_seg = battle_unit_picts_seg[combat_grid_entities[combat_grid_entity_idx].index];
            Draw_Picture_Windowed(
                (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
                (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
                pict_seg
            );




XREF:
    CMB_DrawMap__WIP()
    NX_j_CMB_DrawEntities__WIP()
