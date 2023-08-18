
# MoM - Color-Map

0 TRANSPARENT

224-232
8 colors
232-256
24 colors

214 + 6 banner colors
224 + 6 banner colors


## Remap Colors
Pictures only?
if animation_header.loop_frame != 0
{232,233,234,235,236,237,238,239,240,241,242,243,244,245}
16 blocks of 256 bytes
remap color block = color-map index - 232
remap color block = {232,233,234,235,236,237,238,239,240,241,242,243,244,245} - 232 = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15}


16 blocks of 256 colors
1st is hard-coded 0,0,0,50  (which seems odd, because that is the first block definition)

red, green, blue, percent

gsa_ShadingColors[1]:  3F 3F 3F 25
gsa_ShadingColors[1]:  63 63 63 25

lbxpal_build_colortable(1, 63, 63, 63, 25)




factor vs percentage
a factor of 150 means make the color 50% brighter
a factor of  25 means make the color 75% draker










Replace_Color_All()

Draw_Picture() |-> Draw_Picture_ASM() |-> Draw_Picture_Remap_ASM()
    [> 232 || >= 233]
    {233, ..., 256}

LBX_IMG_Grayscale
UNIT_Draw_UE_Outline




RLE checks for >= 224
    {225, ..., 256}
    32 colors
    ¿ 24 for remap ?





## ¿ Banner Color Replacement ?
City, Tower, Unit Background
-Node Aura?
-Unit Static Figure? uniform/shirt/tunic

Draw_Unit_StatFig
    Replace_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);

Draw_Map_Towers()
    Replace_Color(Map_Square_WorkArea, 224 + itr_color_remap, *(COL_Banners + (_players[tower_owner_idx].Banner * 5)));

Wizard Banner Color

City Sprite Banner Color

¿ (Magic) Weapon Icon ?


{214,215,216,217,218}

Draw_Unit_StatFig()
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 0);
    FLIC_Set_CurrentFrame(_unit_type_table[unit_type_idx].pict_seg, 1);
    Draw_Picture_To_Bitmap(_unit_type_table[unit_type_idx].pict_seg, UnitDraw_WorkArea);
    Replace_Color(UnitDraw_WorkArea, itr_color_remap + 214, COL_Banners[ ((banner_idx * 5) + itr_color_remap) ]);
