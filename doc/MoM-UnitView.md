



Thing_View_Draw__WIP()
    USW_DrawListPage()

last called function

So, how have I been drawing specials list stuuf, without this function?











~== 'Called From Flag'
AKA 'CalledFromFlag' ... uvt_Prod, uvt_Stat, uvt_Combat
0 means 'Production Screen'
1 means ¿ Stat OR Hire ?
2 means 'Combat Screen'

View_Type
0 Buidling
1 ¿ Unit ? even if 'Production Screen'
2 Unit ¿ regardless of 'Combat Screen' ?
    tested 3 times
    the ONLY test for 0 branches off the Building code, and it assumes 'Production Screen'
    the ONLY test for 2 is included with a test for 1
    the ONLY test for 1 is after a test for 'Production Screen', for the discounted cost
for Upkeep vs. Damage, uses CalledFromFlag
for unit_type_idx vs. unit_idx, uses CalledFromFlag


thing_idx == {product_idx:{bldg_idx, unit_type_idx}, unit_idx, unit_idx}
0: product_idx:{bldg_idx, unit_type_idx}
1: unit_idx
2: unit_idx

IIF {1,2}, is hero unit?



State of 'Thing View'
Prod
Stat
¿Hire?
Cmbt


Prod && Bldg
Prod && Norm
Stat
Cmbt

CalledFromFlag && View_Type && UV_IsHeroUnit
{0,0,0}  Prod-Bldg
{0,1,0}  Prod-Norm
{1,1,0}  Stat-Norm
{2,2,0}  Cmbt-Norm
{1,1,1}  Stat-Hero
{2,2,1}  Cmbt-Hero


In-Between:
CalledFromFlag == 0 ALWAYS means Prod-View
View_Type == 0 ALWAYS means Prod-View && Bldg
View_Type == 1 ALWAYS means 'Unit'
    BUT
        could still be Prod-View or Stat-View
        could still be Norm-Unit or Hero-Unit
    ALSO
        NEVER means Cmbt-View

if Prod, skips hero unit check
UV_IsHeroUnit
    Stat || Cmbt

CalledFromFlag gets passed to Draw_Unit_Figure()
if != 0, uses CombatData, rather than unit_type_table
and assumes human_player_idx
and assumes unit_type is passed



bldg_pict_y1 0 OR -4
     0
    -4 if Prod-Norm

Hero Portrait & Name
OR
Unit Figure & Name

"Upkeep" OR "Damage"
    "Upkeep" if CalledFromFlag == 0 || 1
    else "Damage"

Icon_Row_Length 15 OR 10
    15
    10 if Prod-Norm

bldg_pict_y1 0 OR -7
     0
    -7 if Prod-Norm

in the call to UNIT_DrawUpkeep()
for Prod-Norm, uses -(unit_type_idx)
else uses unit_idx












Eh.
Unit View
YaNayMay Unit, Building, etc.
~ Thing / "Project", "Structure"


if(USW_TransparentBase)
    Fill(xstart, ystart, xstart + 281, ystart + 183, ST_TRANSPARENT)
So, ...
    does the fill if the flag is TRUE
    So, ...
        flag is wha-wha-what?
            F/T wipe out the background of the *view* space
    But, ...
        hard-coded {xadd,yadd}?
    So, ...
        use-case?


~ view type flag
    unitview_small_background_seg
    OR
    unitview_large_background_seg
        AND
        unitview_button_background_seg
...
    view type flag == 0
        unitview_small_background_seg
    view type flag == 1
        unitview_large_background_seg
        unitview_button_background_seg
    view type flag == 2
        unitview_large_background_seg

Unit_Statistics_Popup_Do_Draw()
    calls Thing_View_Draw__WIP()
    with UV_view_type
    OON XREF
        Unit_Statistics_Popup()
            called with CalledFromFlag
                XREF:
                    j_USW_Display        jmp     Unit_Statistics_Popup           ; displays the unit statistics window for the specified
                    USW_CombatDisplay+8C call    near ptr Unit_Statistics_Popup  ; displays the unit statistics window for the specified
                    USW_LoadAndShow+A1   call    near ptr Unit_Statistics_Popup  ; displays the unit statistics window for the specified

Then, ...
    view type = view type flag
    but, ...
        view type flag == 0
            thing_idx = unit_idx < 100 || >= 100
                assuming thing_idx is unit or bldg
                if bldg, sets View_Type = 0
                if unit, sets View_Type = 1
                and, ...
                    if CalledFromFlag != 0
                        keeps View_Type = CalledFromFlag
    So, ...
        CalledFromFlag == 0 AND unit_idx = thing_idx < 100
            View_Type = 0
        CalledFromFlag == 0 AND unit_idx = thing_idx >= 100
            View_Type = 1
            unit_type = (unit_idx - 100) = (thing_idx - 100)
        CalledFromFlag != 0
            unit_type = _UNITS[thing_idx].type = _UNITS[unit_idx].type
            AND, keeps View_Type == {1, 2}
        So, ...
            View_Type == 0 AND assumes Building and bldg_idx
            View_Type == 1 and unit_type is thing_idx - 100
            View_Type ~ 1 and unit_type is _UNITS[thing_idx].type
            View_Type ~ 2 and unit_type is _UNITS[thing_idx].type
        

### ~ View Type Flag 0
    unitview_small_background_seg
### ~ View Type Flag 0 && thing_idx < 100
    View_Type = 0

Draw_Building_Picture_To_Bitmap()

### ~ View Type Flag 0 && think_idx >= 100
    unitview_small_background_seg
    unit_type_idx = thing_idx
    View_Type = 1


### ~ View Type Flag 1
    unitview_large_background_seg
    unitview_button_background_seg
    View_Type = 1
    if(_UNITS[thing_idx].Hero_Slot != -1) { UV_IsHeroUnit = ST_TRUE; }

### ~ View Type Flag 2
    unitview_large_background_seg
    View_Type = 2
    if(_UNITS[thing_idx].Hero_Slot != -1) { UV_IsHeroUnit = ST_TRUE; }



### ~ View_Type 0
    IIF (CalledFromFlag == 0) && (thing_idx < 100>)









thing_idx
    unit or bldg?
    ¿ ?
        Draw_Building_Picture_To_Bitmap()
            ... TradeGoods, Housing, CityWall, or regular-like *IDK_buildings_35*
            YNM animated - itr frames
            Draw_Picture_To_Bitmap()
        ¿ IIF view type == 0 ?




Unit View



x_start, y_start
x1, y1, x2, y2



Background

Unit Picture Base

Unit Picture
    Unit Figures
    Hero Portrait




Unit View - View Type
0:
1:
2:  ¿ Combat Only ?

Unit View Background
0:  unitview_small_background_seg
1:  unitview_large_background_seg & unitview_button_background_seg
2:  unitview_large_background_seg



Unit_Statistics_Popup_Draw__WIP()
    if(UV_view_type == 0)
        FLIC_Draw(UV_x_start, UV_y_start, unitview_small_background_seg);
    else
        FLIC_Draw(UV_x_start, UV_y_start, unitview_large_background_seg);
            if(UV_view_type != 2)
                FLIC_Draw((UV_x_start + 213), (UV_y_start + 133), unitview_button_background_seg);



// WZD dseg:97B8
// drake178: IMG_USW_Background@
// FULLUNIT large unit backgrn
unitview_large_background_seg = LBX_Reload_Next("UNITVIEW", 1, GFX_Swap_Seg);

// WZD dseg:97BA
// drake178: IMG_USW_UnitHire_BG@
// UNITBACK small unit backgrn
unitview_small_background_seg = LBX_Reload_Next("UNITVIEW", 0, GFX_Swap_Seg);

// WZD dseg:9540
// drake178: IMG_USW_SideBtns_BG
// BLDBUTBK button background
unitview_button_background_seg = LBX_Reload_Next("UNITVIEW", 2, GFX_Swap_Seg);





##### Naming Things Is Hard

"special abilities"
"unit enchantments"
"enchantments"
"abilities"
"attribute modifier"
"modifier"
"special items"
"statistics"
"bottom half"
"icons"


HLPENTRY.LBX
build Help
view Help
level Help
enemy unit Help
combat view Help
hiring view Help



Page 77  (PDF Page 82)

Special Abilities and Enchantments

Any special abilities and unit enchantments appear as icons in the bottom half of the unit statistics window.  
Clicking on the enchantments removes them (if they were cast by your wizard).  
Right-clicking on the enchantments or abilities describes their effects.  
Attribute modifiers (both positive and negative)  
  from enchantments and special abilities (see Special Unit Abilities)  
  or from carrying special items  
  are always incorporated in the statistics  
  shown on the unit statistics window  
  (i.e., the number of swords and shields is always current).  
The only exception is if the attribute modifier is conditional in some way  
  (for example, ranged defense provided by shields (see Artifacts) only applies to defenses against ranged attacks);  
  in this case, the modifier is not reflected in the number of swords or shields in the unit statistics window.  
