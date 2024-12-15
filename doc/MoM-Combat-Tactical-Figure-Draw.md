


20241126  (Post-Fix-CombatFigureBugMonster)
Combat_Figure_Active_Red_Outline()
    if(CMB_CursorBattleUnit == battle_unit_idx)
Tactical_Combat__WIP()
    CMB_CursorBattleUnit = ST_UNDEFINED;
CMB_PrepareTurn__WIP()
    CMB_CursorBattleUnit = ST_UNDEFINED;
Assign_Mouse_Image()
    CMB_CursorBattleUnit = ST_UNDEFINED;
    scanned_battle_unit_idx = CMB_TargetRows[cgy][cgx];
    CMB_CursorBattleUnit = scanned_battle_unit_idx;  // ; the combat unit display is based on this
...
CMB_TargetRows[cgy][cgx]
int8_t * CMB_TargetRows[22];  // 22 pointers to LBX_NearAlloc_Next()'s of 21 bytes each
allocated in CMB_SetNearAllocs__WIP()
...
Where populated?
...
input field  (Left-Click & Right-Click)
scanned field
...
¿ selected unit vs. highlighted unit ?
...
XREF:  (19)
    Tactical_Combat__WIP+9BB    mov     bx, [CMB_TargetRows@+bx]
    BU_CombatAction+10          mov     bx, [CMB_TargetRows@+bx]
    Assign_Combat_Grids+177 mov     bx, [CMB_TargetRows@+bx]
    Assign_Combat_Grids+1EA mov     bx, [CMB_TargetRows@+bx]
    Assign_Combat_Grids+248 mov     ax, [CMB_TargetRows@+bx]
    Assign_Combat_Grids+308 mov     bx, [CMB_TargetRows@+bx]
    Assign_Combat_Grids+338 mov     ax, [CMB_TargetRows@+bx]
    Assign_Mouse_Image+CC       mov     bx, [CMB_TargetRows@+bx]
    Assign_Mouse_Image+130      mov     bx, [CMB_TargetRows@+bx]
    Assign_Mouse_Image+207      mov     bx, [CMB_TargetRows@+bx]
    BU_MoveConfused+35          mov     bx, [CMB_TargetRows@+bx]
    CMB_SetNearAllocs__WIP+64   mov     [CMB_TargetRows@+bx], ax
    AITP_CombatSpell+FBF        mov     ax, [CMB_TargetRows@+bx]
    CMB_SetTargetCursor+69      mov     bx, [CMB_TargetRows@+bx]
    CMB_SetTargetCursor+D9      mov     bx, [CMB_TargetRows@+bx]
    CMB_TargetSpell+292         mov     ax, [CMB_TargetRows@+bx]
    BU_SummonDemon+139          mov     bx, [CMB_TargetRows@+bx]
    CMB_RaiseDead+1BC           mov     bx, [CMB_TargetRows@+bx]
    CMB_AnimateDead+1BC         mov     bx, [CMB_TargetRows@+bx]
...
Tactical_Combat__WIP()
    |-> Assign_Combat_Grids()










20241126
¿ Eight Go-Around ?
...working back from the draw call...
    CMB_DrawEntities__WIP()
        pict_seg = CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index];
        Draw_Picture_Windowed(
            (combat_grid_entities[combat_grid_entity_idx].draw_x - combat_grid_entities[combat_grid_entity_idx].draw_x_shift),
            (combat_grid_entities[combat_grid_entity_idx].draw_y - combat_grid_entities[combat_grid_entity_idx].draw_y_shift),
            pict_seg
        );
...capture and test...
    combat_grid_entity_idx
    combat_grid_entities[combat_grid_entity_idx].index
    CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index];
    pict_seg = CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index];
    combat_grid_entities[combat_grid_entity_idx].draw_x
    combat_grid_entities[combat_grid_entity_idx].draw_x_shift
    combat_grid_entities[combat_grid_entity_idx].draw_y
    combat_grid_entities[combat_grid_entity_idx].draw_y_shift
...trigger in CMB_DrawEntities__WIP()...
    combat_grid_entity_idx = combat_entity_draw_order_array[itr_combat_entity_draw_order];
    if(combat_grid_entity_idx == DBG_combat_grid_entity_idx)
    (DBG_combat_grid_entity_idx is set in CMB_SpawnFigure__WIP(), based on ...)
¿ unit vs. figure ?

Then, ...
    What should this be?     pict_seg = CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index];
    ...
    same address as was used in Combat_Figure_Compose_USEFULL()
        GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    ...
    in Combat_Figure_Compose_USEFULL()
        Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);
        ptr_figure_pointer_seg[figure_set_idx] should have the same address as was used at LBX_Entry_Reload()


Combat_Figure_Compose_USEFULL()
    if(battle_units[itr].unit_idx == DEBUG_UNIT_IDX)
    {
        DBG_pict_seg = (int64_t)GfxBuf_2400B;
    }


Combat_Figure_Load()
...capture address of figure picture...
...needs to be matched to what ends up being the figure_set_idx value, calculated in CMB_SpawnFigure__WIP()
    ...unit_type, battle_unit_figure_idx

Where to capture test data for unit_type?
Where to capture test data for figure_count?


Combat_Figure_Load()
    DBG_pict_seg__1 = ptr_figure_pointer_seg[itr]
CMB_DrawEntities__WIP()
    DBG_pict_seg__2 = ptr_figure_pointer_seg[itr]

Combat_Figure_Compose_USEFULL()
    DBG_bitm_seg__1 = (int64_t)CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
CMB_DrawEntities__WIP()
    DBG_bitm_seg__2 = (int64_t)CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index];
PASS
    assert(DBG_bitm_seg__2 == DBG_bitm_seg__1);



Combat_Figure_Load()
==> Combat_Figure_Picture_Load()
called for every battle_unit
8 calls to LBX_Reload_Next()
sets pointer in ptr_figure_pointer_seg[itr]
loads data in (EMM_PageFrame + offset)



DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 15879]: DBG_pict_seg__1: -1445232584
DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 13706]: DBG_pict_seg__2: -1445232584

DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 15876]: DBG_8_battle_unit_figure_idx__1: 0
DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 13720]: DBG_8_battle_unit_figure_idx__3: 0

DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 15877]: DBG_8_EMM_PageFrame__1: -1445237300
DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 13721]: DBG_8_EMM_PageFrame__3: -1445237300

DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 15878]: DBG_8_offset__1: 0
DEBUG: [C:\STU\devel\ReMoM\src\Combat.C, 13722]: DBG_8_offset__3: 0




CMB_BU_Figure_GFX
Where do the addresses come from?


uses ptr_figure_pointer_seg[figure_set_idx] to set the frame
could still just think it's like the EMM header-only business






REMINDME:   ¿ what is combat_grid_entities[].index ?




CMB_CreateEntity__WIP()
    sets values in combat_grid_entities[], based on combat_grid_entity_count
    so, ...
    could set DBG_combat_grid_entity_idx here
    or, in CMB_SpawnFigure__WIP(), which has all the values that get passed to CMB_CreateEntity__WIP()
CMB_SpawnFigure__WIP()
    ...SrcBld is hard-coded to 0
    ...draw_x_shift, draw_y_shift, and entity_type are hard-coded to 13, 23, 1
    ...calculated values...position_screen_x, position_screen_y, target_screen_x, target_screen_y, fig_x, fig_y, figure_set_idx, draw_x, draw_y, Blood_Frame, BldAmt




CMB_Units_Init__WIP()
    if(troops[itr] == DEBUG_UNIT_IDX)
    {
        DBG_battle_unit_idx = itr;
    }


CMB_CreateEntities__WIP()
    // if(battle_units[itr].unit_idx == DEBUG_UNIT_IDX)
    if(itr == DBG_battle_unit_idx)
    {
        DBG_battle_unit_figure_idx = battle_units[itr].battle_unit_figure_idx;
        DBG_position_cgc2 = battle_units[itr].position_cgc2;
        DBG_position_cgc1 = battle_units[itr].position_cgc1;
        DBG_target_cgc2 = battle_units[itr].target_cgc2;
        DBG_target_cgc1 = battle_units[itr].target_cgc1;
        DBG_MoveStage = battle_units[itr].MoveStage;
        DBG_itr_figures = itr_figures;
        DBG_unit_figure_maximum = unit_figure_maximum;
        DBG_controller_idx = battle_units[itr].controller_idx;
        DBG_outline_magic_realm = battle_units[itr].outline_magic_realm;
        DBG_Blood_Amount = battle_units[itr].Blood_Amount;
        DBG_Moving = battle_units[itr].Moving;
        DBG_Atk_FigLoss = battle_units[itr].Atk_FigLoss;
    }







For the love of god and all that is holy...
What in the flying fucks is going on with me?

Why is the wrong figure picture being drawn?
...draw battle unit figure picture...
AKA CMB_DrawEntities__WIP()
...operates on combat grid entities...
How do I know which combat grid entity I want to pay attention to?
    DBG_combat_grid_entity_idx
Where and how do I capture DBG_combat_grid_entity_idx?
...somewhere with something that uses combat_grid_entities[]...
CMB_CreateEntity__WIP()?
¿ unique identifiers ?
¿ draw_x, draw_y ?
...by way of CMB_SpawnFigure__WIP()...
¿ unique identifiers ?
¿ position_cgc2, position_cgc1 ?
...iterates over unit_figure_count...
CMB_SpawnFigure__WIP(int64_t seg_or_idx, int16_t position_cgc2, int16_t position_cgc1, int16_t target_cgc2, int16_t target_cgc1, int16_t MoveStage, int16_t current_figure, int16_t figure_count, int16_t controller_idx, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU, int16_t LostFigs, int16_t SrcBld)


CMB_SpawnFigure__WIP(
    battle_units[itr].battle_unit_figure_idx, battle_units[itr].position_cgc2, battle_units[itr].position_cgc1, battle_units[itr].target_cgc2, battle_units[itr].target_cgc1, 
    battle_units[itr].MoveStage, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, 
    battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0
)

CMB_SpawnFigure__WIP(
    seg_or_idx, position_cgc2, position_cgc1, target_cgc2, target_cgc1, 
    MoveStage, current_figure, figure_count, controller_idx, outline_magic_realm, 
    BldAmt, UU, LostFigs, SrcBld
)


Combat_Figure_Compose_USEFULL()
    iters over _combat_total_unit_count
        ...if(battle_units[itr].unit_idx == DEBUG_UNIT_IDX)
    ...
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);

How to match?
What to debug?

Does battle_units[itr].battle_unit_figure_idx match DBG battle unit idx / combat figure picture cache idx?

Does ptr_figure_pointer_seg[figure_set_idx] match DBG_figure_pict_seg[DBG_figure_set_idx]?

¿ CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx] matches ?



CMB_DrawEntities__WIP()
    iters over combat_grid_entity_count





Load
Compose
Draw
    CMB_DrawFullScreen__WIP()
        CMB_DrawMap__WIP()
            CMB_DrawEntities__WIP()


WTF USELESS_Combat_Figure_Load_Compose()
vs. Combat_Figure_Compose_USEFULL()

Combat_Figure_Load()
    ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMM_PageFrame + offset));
    |-> USELESS_Combat_Figure_Load_Compose()



ptr_figure_pointer_seg[]
vs.
DBG_ptr_figure_pointer_seg[]

DBG_ptr_figure_pointer_seg[] is set in Combat_Figure_Load()



`logical_page = ((battle_unit_figure_idx * 3) / 2);`
¿ 16 KB / 56 PR ?
16 * 1024 = 16384
56 * 16 = 896
16384 / 896 = 18.285714285714285714285714285714

e.g.,
    3 * 3 = 9 / 2 = 4.5
    ... * 1.5
    3 * 1.5 = 4.5

512 * 16 = 8192
16384 / 8192 = 2







battle_unit_figure_idx
offset
EMM_PageFrame

EMM_FIGUREX_Init__HACK()  AKA FIGUREX_MAP
Combat_Figure_Load()
Combat_Figure_Compose_USEFULL()
USELESS_Combat_Figure_Load_Compose()

¿ order in which these take place ?

Tactical_Combat__WIP()
    CMB_Units_Init__WIP()
        Combat_Figure_Load()
            FIGUREX_MAP  AKA EMM_FIGUREX_Init__HACK()

Tactical_Combat__WIP()
    CMB_DrawFullScreen__WIP()
        CMB_DrawMap__WIP()
            Combat_Figure_Compose_USEFULL()
                FIGUREX_MAP  AKA EMM_FIGUREX_Init__HACK()

Tactical_Combat__WIP()
    CMB_Units_Init__WIP()
        Combat_Figure_Load()
            USELESS_Combat_Figure_Load_Compose()

```
Tactical_Combat__WIP()
    CMB_Units_Init__WIP()
        Combat_Figure_Load()
            FIGUREX_MAP  AKA EMM_FIGUREX_Init__HACK()
            USELESS_Combat_Figure_Load_Compose()
                FIGUREX_MAP  AKA EMM_FIGUREX_Init__HACK()
    // ...
    // ...
    // ...
    CMB_DrawFullScreen__WIP()
        CMB_DrawMap__WIP()
            Combat_Figure_Compose_USEFULL()
                FIGUREX_MAP  AKA EMM_FIGUREX_Init__HACK()
```

EMM_FIGUREX_Init__HACK()
    DBG_logical_page__1
    DBG_offset__1
    DBG_EmmHndl_FIGUREX__1
    DBG_+EMM_PageFrame__1


























O---> Draw Battle Unit Fgiure

NOTE: screen and grid coordinates calculations are covered elsewhere



Steps / Stages:

Load, Compose, Draw
    ...
    LBX_Reload_Next()
    Draw_Picture_To_Bitmap()
    Draw_Picture_Windowed()

28 x 30
16 LBX files
8 directional figure pictures per unit type
18 unit maximum simultaneously in combat
uses EMM

18 pointers to SAMBs



EMM_FIGUREX_Init__HACK()
maps in EmmHndl_FIGUREX  (allocated in Allocate_Data_Space())
with an offset based on the index for the cached battle units figure pictures
alternates offset based on eve/odd cache index


## Debugging


Requirements:
    ~== Tactical_Combat__WIP() |-> CMB_Units_Init__WIP() |-> Combat_Figure_Load()

Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);



    Deactivate_Auto_Function();
    Clear_Fields();
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_BLACK);
    Toggle_Pages();
    PageFlipEffect = 0;
    CMB_BaseAllocs__WIP();
    CMB_LoadResources__WIP();
        Mark_Block(_screen_seg);
        for(itr = 0; itr < 18; itr++)
        {
            CMB_BU_Figure_GFX[itr] = LBX_Reload_Next(compix_lbx_file__ovr103, (55 + itr), _screen_seg);
        }
        Release_Block(_screen_seg);
    Defending_Unit_Count = CMB_Units_Init__WIP(troop_count, troops);


ptr_figure_pointer_seg is 8 pointers to loaded LBX entries


battle_units[_combat_total_unit_count].battle_unit_figure_idx = Combat_Figure_Load(_UNITS[troops[itr]].type, itr);

There are two rounds of calling Combat_Figure_Load().
    ...both pass unit_type
    ...first passes iter troop_count
    ...second passes _combat_total_unit_count
    ...these are equivalent, as the loop over troop_count increments _combat_total_unit_count
    (_combat_total_unit_count is used for iterations over battle_units)
The index passed in is returned unaltered and assigned to battle_units[_combat_total_unit_count].battle_unit_figure_idx.
So, ...
    battle_unit_idx?
    battle_unit_ctr?
    ...the index into battle_units[] is the exact same value in battle_units[].battle_unit_figure_idx

  // 512 PR, 8192 B; 512 / 56 = 9.14 ... ¿ 2 sets of 9 figure pictures ?


4 places for FIGUREX (EMM_PageFrame, EmmHndl_FIGUREX, logical_page, offset)
EMM_FIGUREX_Init__HACK()
Combat_Figure_Compose_USEFULL()  ...but, also, right beforehand, calls EMM_FIGUREX_Init__HACK()
Combat_Figure_Load()        ...but, also, right beforehand, calls EMM_FIGUREX_Init__HACK()
USELESS_Combat_Figure_Load_Compose       ...calls EMM_FIGUREX_Init__HACK() after offset calc, before using EMM_PageFrame
EMM_FIGUREX_Init__HACK() sets EMM_PageFrame, the other three use it for the call to Allocate_First_Block()


ptr_figure_pointer_seg[figure_set_idx]
...
    EMM_PageFrame = (EmmHndl_FIGUREX + (logical_page * SZ_EMM_LOGICAL_PAGE));
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
...
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);



figure_lbx_file__ovr163
Combat_Figure_Load()
    entry_num = ((unit_type % 15) * 8);
    for(itr = 0; itr < 8; itr++)
        ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMM_PageFrame + offset));

8 entries per unit_type - figure_set_idx / direction / facing

UNIT_TYPE_MAX  = 198



begin, ...
break in Combat_Figure_Load()
get pointer addresses for ptr_figure_pointer_seg[itr]
compare bytes for pictures ...versus DBG_figure_pict_seg[itr]
later, ...
Combat_Figure_Compose_USEFULL()
CMB_BU_Figure_GFX[]
¿ itr should be 0 ?
¿ battle_units[itr].battle_unit_figure_idx should be 0 ?
...because I'm attacking, attacker's units get assigned first, and the test only has 1 unit
¿ figure_set_idx should be 7 ?

¿ capture and check the pointer CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx] ?



what should be CMB_BU_Figure_GFX[combat_grid_entities[IDK_combat_entity_idx].index]?
combat_grid_entities[IDK_combat_entity_idx].index?
AKA pict_seg_or_idx

CMB_CreateEntity__WIP( seg_or_idx )
    combat_grid_entities[combat_grid_entity_count].seg_or_idx = seg_or_idx;

CMB_SpawnFigure__WIP( seg_or_idx )

CMB_CreateEntities__WIP()
    CMB_SpawnFigure__WIP( battle_units[itr].battle_unit_figure_idx )



Not sure if the values are wrong...
Not sure what the values should be...
Trying to reduce how many times I have to hit F-10/F-11...
Trying to reduce how many times I have to manually inspect the value of the variables...
Which call in the loop-de-loop do I want to break on?
e.g., ...
    CMB_DrawEntities__WIP()
        Which combat_grid_entity_idx?
            break on combat_grid_entity_idx?
            break on a value in struct s_COMBAT_ENTITY?
                draw_x,y? seg_or_idx? entity_type? draw_order_value?
            


iter over combat_grid_entity_count
index into combat_entity_draw_order_array[]
...contains combat_grid_entity_idx, of current combat_grid_entity, in order of draw_order_value

combat_grid_entity_idx
index into combat_grid_entities[]
get index for figure picture cache
...combat_grid_entities[combat_grid_entity_idx].index
index into CMB_BU_Figure_GFX[]
get pointer memory address for figure picture
...pict_seg = CMB_BU_Figure_GFX[combat_grid_entities[combat_grid_entity_idx].index]

*figure picture cache*
CMB_BU_Figure_GFX[]
...18 SAMBs, where picture/bitmap is drawn/composed/rendered
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);

index into battle_units[] is same index into CMB_BU_Figure_GFX[]


capture DBG_battle_unit_idx in CMB_Units_Init__WIP()



## CMB_BU_Figure_GFX


figure entries are loaded into EmmHndl_FIGUREX
figure pictures are *composed* into CMB_BU_Figure_GFX







EmmHndl_FIGUREX
EMM_PageFrame
EMM_FIGUREX_Init__HACK()
¿ same approach for terrain_pict_seg in Draw_Map_Terrain() MainScr_Maps.C ?












does USELESS_Combat_Figure_Load_Compose() actually accomplish anything?
...only called by Combat_Figure_Load()  ... |-> USELESS_Combat_Figure_Load_Compose(battle_unit_figure_idx, 0, 0, 0, 0);











Tactical_Combat__WIP()




Combat_Figure_Load()
    ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMM_PageFrame + offset));

Combat_Figure_Compose_USEFULL()
    battle_unit_figure_idx = battle_units[itr].battle_unit_figure_idx;
    EMM_FIGUREX_Init__HACK(battle_unit_figure_idx);
    ...
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);


CMB_DrawEntities__WIP()
    Draw_Picture_Windowed(
        (combat_grid_entities[IDK_combat_entity_idx].draw_x - combat_grid_entities[IDK_combat_entity_idx].draw_x_shift),
        (combat_grid_entities[IDK_combat_entity_idx].draw_y - combat_grid_entities[IDK_combat_entity_idx].draw_y_shift),
        CMB_BU_Figure_GFX[combat_grid_entities[IDK_combat_entity_idx].index]
    );




Combat_Figure_Compose_USEFULL()

Combat_Figure_Compose_USEFULL()
    GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);
    Combat_Figure_Banner_Color(player_idx);
    Combat_Unit_Enchantment_Outline_Draw(UE_Rlm);
    Combat_Figure_Effect__WIP(IMG_Effect);
    Combat_Figure_Active_Red_Outline(itr);


Tactical_Combat__WIP()
    // ...
    |-> CMB_ComposeBackgrnd__WIP()
    // ...
    CMB_DrawFullScreen__WIP()
        CMB_DrawMap__WIP()
            Combat_Figure_Compose_USEFULL()
            CMB_DrawEntities__WIP()
                ...
                Draw_Picture_Windowed(CMB_BU_Figure_GFX[])
                ...

        GfxBuf_2400B = CMB_BU_Figure_GFX[battle_units[itr].battle_unit_figure_idx];



CMB_SpawnFigure__WIP()
    sets draw type to 1

CMB_CreateEntity__WIP() is what sets draw_x and draw_x_shift, based on what is passed in from CMB_SpawnFigure__WIP()
CMB_SpawnFigure__WIP() uses Combat_Grid_Screen_Coordinates()
    battle_units[itr].position_cgc2, battle_units[itr].position_cgc1



    // void CMB_CreateEntity__WIP(int16_t draw_x, int16_t draw_y, int64_t seg_or_idx, int16_t draw_x_shift, int16_t draw_y_shift, int16_t Frame, int16_t DrawType, int16_t controller_idx, int16_t UU_10h, int16_t outline_magic_realm, int16_t BldAmt, int16_t UU_14h, int16_t BldFrm)
    CMB_CreateEntity__WIP(                 Left,           Top,         seg_or_idx,           13,           23,            UU,                1,         controller_idx, figure_set_idx,            outline_magic_realm,         BldAmt,             UU,    Blood_Frame);

combat_grid_entities[combat_grid_entity_count].outline_magic_realm = outline_magic_realm;
    <- CMB_CreateEntity__WIP(..., outline_magic_realm, ...)
        <- CMB_SpawnFigure__WIP(..., battle_units[itr].outline_magic_realm, ...)


CMB_SpawnFigure__WIP() hard-codes combat_grid_entities[].draw_x_shift,draw_y_shift to 13,23

