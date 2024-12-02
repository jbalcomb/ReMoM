


Active Unit Window

CMB_DrawFullScreen__WIP()
    |-> Draw_Active_Unit_Window()
        |-> Draw_Active_Unit_Stats_And_Icons()
        |-> Draw_Active_Unit_Damage_Bar()







MainScr.C
Main_Screen_Draw_Unit_Window()
    |-> Unit_Window_Draw_Unit_Attributes()

Unit_Window_Draw_Unit_Attributes()
weapon_type_icon_seg
if((current_hits != max_hits) && (current_hits > 0) )
bar_length = (((current_hits * 10) / max_hits) - 1);
Line(x + 5, y + 19, x + bar_length + 5, y + 19, Color);

Main_Screen_Draw_Movement_Bar()
    |-> Draw_Movement_Mode_Icons()


MoX_DEF.H
// Unit_Window_Draw_Unit_Attributes()
#define DAMAGE_BAR_GREEN        0xD9  // 217
#define DAMAGE_BAR_YELLOW       0xD3  // 211
#define DAMAGE_BAR_RED          0xCA  // 202








##### Naming Things is Hard



Page 92  (PDF Page 97)

The active unit window shows information about the currently active unit  
  (a red outline surrounds the active unit’s square on the combat grid).  
Next to the unit’s picture are tiny symbols with numbers.  
These symbols represent  
  melee attack strength (sword symbol),  
  ranged attack strength (with symbols of a bow, fireball or rock for missile weapons, magic ranged weapons and rocks, respectively, depending on the active unit’s ranged weapon type)  
  and movement points left (with wings or boots depending on whether the unit flies or not).  

In addition, a damage bar runs along the bottom edge of the active unit’s picture.  
When this bar is green, the unit has two-thirds or more of its total, full strength (the sum of all the figures’ hit points in the unit) hit points left.  
When this bar is yellow, the unit has between one-third and two-thirds of its hit points left.  
Finally, if this bar is red, the unit is severely injured and has less than one-third of its hits points left.
