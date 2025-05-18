







something with cycling the mouse image

    crsr_CastAnim1   =  9,
    crsr_CastAnim2   = 10,
    crsr_CastAnim3   = 11,
    crsr_CastAnim4   = 12,
    crsr_CastAnim5   = 13






// WZD dseg:9518
SAMB_ptr survey_background;
// MAIN.LBX,57  MAINSRVY    survey backgrnd
// WZD dseg:951A
SAMB_ptr road_button_border;
// MAIN.LBX,48  C&RBORDR    road button border
// WZD dseg:951C
SAMB_ptr road_background;
// MAIN.LBX,45  MAINROAD    road background
// WZD dseg:951E
SAMB_ptr road_ok_button;
// MAIN.LBX,46  CASTCNCL    road ok button
// WZD dseg:9522
SAMB_ptr deselect_button_blockout;
// MAIN.LBX,44    DESELECT    button blockout
// WZD dseg:9524
SAMB_ptr cast_button_border;
// MAIN.LBX,47  C&RBORDR    cast button border
// WZD dseg:9526
/*
same cancel button used by surveyor, road build, and overland spell cast
*/
SAMB_ptr cast_cancel_button;
// MAIN.LBX,41  CASTCNCL    cast cancel button
// WZD dseg:9528
SAMB_ptr cast_background;
// MAIN.LBX,40  MAINCAST    cast background



(Overland) Spell Casting Screen
WZD ovr070
...OverCast...OVL...
¿OSC?



'spell target type'
{ 0, 1, 2, 3, 4, 5, 6, 7 }
Usages:
    Build_Select_Target_String()
        0: Friendly Unit
        1: Friendly Group
        2: Enemy Unit
        3: Enemy Group
        4: Map Square
        5: Friendly City
        6: Enemy City
        7: Magic Node

¿ IDK_NearAlloc_100 is only used for the target select prompt string ?





## Spell_Casting_Screen__WIP()

...uses j_Spell_Casting_Screen_Draw()

~== Main_Screen()
Surveyor_Screen()?



XREF:
    j_Spell_Casting_Screen__WIP()
        Cast_CallOfTheVoid+40
        Cast_ChaosChannels+6B
        Cast_WarpNode+47
        Cast_Stasis+3D
        Cast_NaturesCures+2B
        Cast_Move_Fortress+3B
        Cast_Earthquake+2B
        Cast_BlackWind+2B
        sAFA06_Anim_EarthLore+3E
        Cast_ChangeTerain+4A
        Cast_RaiseVolcano+45
        Cast_Transmute+49
        Cast_Corruption+78
        Cast_WordOfRecall+39
        Cast_EnchantRoad+62
        Cast_SpellWard+62
        Cast_PlaneShift+8F
        Cast_Floating_Island+39
        Cast_Lycantrophy+47
        Cast_WallOfStone+49
        Cast_SpellOfReturn+3A
        Cast_Spell_Overland__WIP+49B
        Cast_Spell_Overland__WIP+6A3
        Cast_Spell_Overland__WIP+DCC
        Cast_Spell_Overland__WIP+11E5
        Cast_Spell_Overland__WIP+19BA


// WZD o70p01
int16_t IDK_Spell_Casting_Screen__WIP(int16_t something, int16_t * wx, int16_t * wy, int16_t * wp, int16_t * var_8, int16_t * var_A, char * spell_name)









##### Naming Things Is Hard

Page 101  (PDF Page 106)  

SPELL CASTING  

To cast spells either overland or during combat, you must first get to your spellbooks.  
For overland spells, click on the main movement screen’s spells button; during combat, click on the spell button.  

Choose a spell to cast by left-clicking on the spell name.
Right-clicking on a spell calls up its description.  

Outside of combat,  
  spells that are currently being cast appear in glowing letters with the number of symbols below them equivalent to the number of turns left before casting is complete.  


Page 102  (PDF Page 107)  

Casting Overland  

Your spellbook only shows you the spells it is possible to cast overland (i.e., not in combat) when you click on the spells button of the main movement screen.  
Click on spell page corners to turn the spellbook pages, and select your choice for casting by clicking on the spell’s name.  
You may, however, only cast one overland spell at a time.  
When casting overland spells, you may take as many turns as you need to complete the spell.  
The total amount of mana you can put into a spell every turn depends on your spell casting skill (magic summary screen) and the amount of mana in your mana reserves.  
Once you have spent an amount of magic power equal to its casting cost, the spell is cast.  
Spell casting heroes may help you cast overland spells.  
At the end of every turn in which such a hero remains in your enchanted fortress city, half of his or her spell casting skill is added to that of your wizard.  
This allows your wizard to cast overland spells at a faster rate, because more mana can be contributed to the casting per turn.  
The mana itself, however, still comes from your wizard’s sources (the hero’s mana is only accessible during combat; see Casting During Combat).  


Page 103  (PDF Page 108)  

When casting enchantments, click on the appropriate target.  
This may be a city, map square or creature.  
Creatures as targets for overland (continuous) enchantments can be chosen by first clicking on the square they are occupying  
  and then clicking on the specific target unit from the list of those available.  




Earth Lore:  
Nature Instant. Casting Cost: 20 mana. Common.  
Fully reveals all of the terrain, terrain specials, cities and armies on all map squares in the targeted "map window."  
(Hint: get as much "black" on the screen as possible before left-clicking to accept this spell.)  
This spell basically does your exploring for you, making huge expanses of the world permanently visible.  
