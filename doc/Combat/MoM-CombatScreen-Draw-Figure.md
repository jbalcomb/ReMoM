



















## Battle Unit Enchantment Outline


CMB_Units_Init__WIP()
    battle_units[itr].outline_magic_realm = 0;

CMB_CreateEntities__WIP()
    CMB_SpawnFigure__WIP(battle_units[itr].bufpi, battle_units[itr].position_cgc2, battle_units[itr].position_cgc1, battle_units[itr].target_cgc2, battle_units[itr].target_cgc1, battle_units[itr].move_anim_ctr, itr_figures, unit_figure_maximum, battle_units[itr].controller_idx, battle_units[itr].outline_magic_realm, battle_units[itr].Blood_Amount, battle_units[itr].Moving, battle_units[itr].Atk_FigLoss, 0);

CMB_Predraw_Figures__WIP()
    UE_Rlm = battle_units[itr].outline_magic_realm;
    Combat_Unit_Enchantment_Outline_Draw(UE_Rlm);

Combat_Unit_Enchantment_Outline_Set()
    sets battle_units[itr].outline_magic_realm
Combat_Unit_Enchantment_Outline_Draw()
    uses battle_units[itr].outline_magic_realm to draw figure outline





¿ allocate, load, draw ?




figures are 28 x 30
28 * 30 = 840
battle_unit_picts_seg[] is 55 PR, 880 B

840 * 8
6720
* 197
1323840
840 * 8 * 197 = 1,323,840 B

16 KB * 3 = 49152 48KB

...something like having a 49,152 B cache to manage 1,323,840 B of unit figure assets...




CMB_Predraw_Figures__WIP()
    SAMB_ptr * ptr_figure_pointer_seg = 0;
    SAMB_ptr figure_pointer_seg = 0;
    EMM_FIGUREX_Init__HACK(Fig_Index);
    offset += (Fig_Index * 56);
    // figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 2);
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    // TODO  ptr_figure_pointer_seg = SA_MK_FP0(figure_pointer_seg);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
    FLIC_Set_CurrentFrame((SAMB_ptr)&ptr_figure_pointer_seg[figure_set_idx], Frame_Index);
    GfxBuf_2400B = battle_unit_picts_seg[battle_units[itr].bufpi];
    Draw_Picture_To_Bitmap((SAMB_ptr)&ptr_figure_pointer_seg[figure_set_idx], GfxBuf_2400B);

void Combat_Figure_Compose(int16_t figure_index, int16_t Direction, int16_t player_idx, int16_t enchantment_magic_realm, int16_t Frame)
    SAMB_ptr * ptr_figure_pointer_seg = 0;
    SAMB_ptr figure_pointer_seg = 0;
    EMM_FIGUREX_Init__HACK(figure_index);
    offset += (figure_index * 56);
    EMM_FIGUREX_Init__HACK(figure_index);
    // figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 2);
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);
    // TODO  ptr_figure_pointer_seg = SA_MK_FP0(figure_pointer_seg);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
    FLIC_Set_CurrentFrame(ptr_figure_pointer_seg[Direction], Frame);
    Draw_Picture_To_Bitmap(ptr_figure_pointer_seg[Direction], GfxBuf_2400B);

int16_t Combat_Figure_Load(int16_t unit_type, int16_t figure_index)
    SAMB_ptr * ptr_figure_pointer_seg = 0;
    SAMB_ptr figure_pointer_seg = 0;
    EMM_FIGUREX_Init__HACK(figure_index);
    offset += (figure_index * 56);
    // figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 2);  // ¿ 2 as in 16-byte header and 8 2-byte pointers ?
    figure_pointer_seg = Allocate_First_Block((EMM_PageFrame + offset), 33);  // 16-byte header + 8 64-byte pointers = (16 + 512) = 528 / 16 = 33
    // TODO  ptr_figure_pointer_seg = SA_MK_FP0(figure_pointer_seg);
    ptr_figure_pointer_seg = (SAMB_ptr *)figure_pointer_seg;
    ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (entry_num + itr), (EMM_PageFrame + offset));

void EMM_FIGUREX_Init__HACK(int16_t bufpi)
    logical_page = ((bufpi * 3) / 2);
    
    if((bufpi & 0x1) == 0)
    {
        offset = 0;
    }
    else
    {
        offset = 512;
    }

    // offset += (bufpi * 56);
    offset += (bufpi * (56 * SZ_PARAGRAPH_B));




CMB_DrawMap__WIP()
    |-> CMB_Predraw_Figures__WIP()

Combat_Figure_Load()
    |-> Combat_Figure_Compose()


LBX entry is definitely, only loaded in Combat_Figure_Load()
It loads into EMM_PageFrame, which is `EmmHndl_FIGUREX = Allocate_Space(28672);`, in Allocate_Data_Space()
LBX_Reload_Next() uses allocation_type = sa_Next, ...
...which does `SAMB_data = SAMB_head + 12 + (SA_GET_USED(SAMB_head) * SZ_PARAGRAPH_B);`

Why did I do `+ 12`?
SAMB_head already points passed the malloc header?

...sets the new used value in the block header...shouldn't it (also) be in the sub-allocation?


FIGURES8.LBX
entry_num 56
entry_length 503
num_blocks 32
num_blocks_used 67

SAMB_head
0x00000182C51CF00C
SAMB_data
0x00000182c51cf248

Why is 'used' 35 on the first run through?
...because, we call Allocate_First_Block() with 33, for the 8 8-byte pointers, which I miscalculated as 64-byte pointers


28 03 cd b7 16 02 00 00 
68 05 cd b7 16 02 00 00
b8 07 cd b7 16 02 00 00 
38 0a cd b7 ce cd 00 00

0x0000cdceb7cd0a38


00000216b7cd0328
00000216b7cd0568
00000216b7cd07b8

0568
07b8
0x07b8 - 0x0568 = 0x250  592d
0x0568 - 0x0328 = 0x240  576d




Combat_Figure_Compose()

offset += (figure_index * 56);
35 * 56 = 1960
+ 512 = 2472


25456 bytes allocated?

Allocate_Data_Space()
    EmmHndl_FIGUREX = Allocate_Space(28672);
Where did I get that number from?
16 * 1024 = 16384 * 28 EMM pages = 458752 / SZ_PARAGRAPH = 28672



cd cd 
cd cd 
fa 12 
cf 4e 
37 06 
01 00 
cd cd 
cd cd




44 bytes short?



Combat_Figure_Load()

ptr_figure_pointer_seg[itr] = LBX_Reload_Next(file_name, (Fig_First_IMG_Entry + itr), (EMM_PageFrame + offset));



CMB_Units_Init__WIP()
    battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[troops[itr]].type, itr);
    ...
    battle_units[_combat_total_unit_count].bufpi = Combat_Figure_Load(_UNITS[itr].type, _combat_total_unit_count);



char figure_lbx_file__ovr163[] = "FIGURE";


120 records each, excepting 24 in the last one
8 entries per figure
...(unit_type * 8) + facing/direction...

ut_Dwarf      =   0   FIGURES1.LBX,   0     BRAX
...
ut_Sky_Drake  = 196   FIGURE16.LBX,  16    SKYDRAKE
¿ ut_Nagas  = 197 ?
ummm... WTF?
ut_Sky_Drake  = 196   FIGURE14.LBX,   8    SKYDRAKE
ut_Nagas      = 197   FIGURE14.LBX,  16    NAGA

197 * 8 = 1576
15 * 120 = 1800
1800 + 24 = 1842
1842 / 8 = 228

196 / 15 = 13.066666666666666666666666666667
13 + 1 = 14


Load_Unit_Figure()  UnitView.C
    SDL_itoa(((figure_num / 15) + 1), buffer, 10);
    ...
    entry_num = (((figure_num % 15) * 8) + 2);
    unit_figure_seg = LBX_Reload_Next(file_name, entry_num, _screen_seg);





FIGURES{1,...,9}
FIGURE{10,...,16}

FIGURES1.LBX
FIGURES2.LBX
FIGURES3.LBX
FIGURES4.LBX
FIGURES5.LBX
FIGURES6.LBX
FIGURES7.LBX
FIGURES8.LBX
FIGURES9.LBX
FIGURE10.LBX
FIGURE11.LBX
FIGURE12.LBX
FIGURE13.LBX
FIGURE14.LBX
FIGURE15.LBX
FIGURE16.LBX
