
MoM  
Unit View  
Effect List  
"Unit Statistics Window"  

Special Unit Abilities  
special abilities  
specials  
...  
attribute modifiers (enchantments, abilities, items)  
...  
Item Powers  
Item Effects  
Item Effect List  
Effects List  





What's with the indexing offset into special_seg?
e.g.,
    IDK_Hero_Populate_USW_List_Structure()
mov     bx, _SI_specials_index
shl     bx, 1
mov     ax, [special_seg+0FAh]
mov     [bx+_DI_specials_list+s_UV_List.picts], ax
FAh 250d sizeof() is 2 so 125

specials_list->picts[specials_index] = special_seg[125]

icons for level (just hero level?)
hard-coded to specials_list[0]
specials_list->picts[0] = special_seg[(102 + level)]

specials_list->helps[0] = (HLP_HERO + level)  533

"UNICON..." {1,2,3,4,5,6,7}




USW_HeroAbilities <=> _HEROES[][].abilities


Data Segment - ovr078
USW_HeroAbilities
USW_Abilities
USW_Attributes_1
USW_Attributes_2
USW_ATK_Flags
USW_ChaosChannels
USW_SR_Attacks
USW_UnitEnchants
USW_Combat_FX
USW_Item_UEs
USW_UnitLevels@
USW_HeroLevels@
ITEM_ViewTexts
...then, all the strings and Le Fin
¿ no way to know how this was coded ?
The string optimizer definitely rearranged everything.


Structure?

All go in to UV_List?
So, must have name, flags, pict, help?

40 specials
30 character description (name + info)
¿ "flags" ? ¿ ever not -1 ? Yes. -99 for Items. Sets flag for 'USW Unit Enchantments'.
pointer to header-only special_seg[] (index per specials group is offset)
help index



00000000 struc s_UV_List ; (sizeof=0x5F0)
00000000 names db 1200 dup(?)
000004B0 flags dd 40 dup(?)
00000550 picts dw 40 dup(?)
000005A0 helps dw 40 dup(?)                      ; enum HELP_TXT
000005F0 ends s_UV_List

00000000 struc USW_UnitEnchant ; (sizeof=0xA)
00000000 Name dw ?                               ; offset (00036AA0)
00000002 Flags dd ?
00000006 USW_IMG_Index db ?
00000007 Unused_7h db ?
00000008 HLP_Index dw ?                          ; enum HELP_TXT
0000000A ends USW_UnitEnchant

00000000 struc USW_HeroAbl ; (sizeof=0x8)
00000000 field_0 dd ?
00000004 USW_IMG_Index dw ?
00000006 HLP_Index dw ?                          ; enum HELP_TXT
00000008 ends USW_HeroAbl

00000000 struc USW_Ability ; (sizeof=0x8, standard type)
00000000 Name dw ?                               ; offset (00036AA0)
00000002 Abl_Flag dw ?                           ; enum ABL_FLAGS
00000004 USW_IMG_Index db ?
00000005 Unused_5h db ?
00000006 HLP_Index dw ?                          ; enum HELP_TXT
00000008 ends USW_Ability

00000000 struc USW_ItemPower ; (sizeof=0xA)
00000000 Name dw ?                               ; offset (00036AA0)
00000002 field_2 dd ?
00000006 IMG_Index dw ?                          ; base 10
00000008 HLP_Index dw ?                          ; enum HELP_TXT
0000000A ends USW_ItemPower




### USW_Build_Effect_List()

Level
`if Hero Unit`
    Items
    Item Enchantments
    Hero Special Abilities
    Caster



### USW_Item_UEs
XREF:
    USW_Build_Effect_List()



### USW_HeroAbilities
XREF:
    USW_Build_Effect_List()
    IDK_Hero_Populate_USW_List_Structure()


#### SPECIAL LBX

SPECIAL.LBX  
111 entries  
UNICON1 {0, ..., }  
UNICON2 { }  
UNICON3 { }  
UNICON4 { }  
UNICON5 { }  
MEDALS  

SPECIAL2.LBX  
44 entries  
UNICON6 { }  
UNICON7 { }  
SMLBOOK, BCORNERS, BOOKSYMB, SMALTEXT  

special_seg Indices
USW_HeroAbilities  special_seg[0]  





##### Naming Things Is Hard

special abilities
Special Unit Abilities
a unit statistics window
cost or upkeep cost (magic power or gold)
attack strengths, defense, resistance to magic, movement capabilities, experience level  
Attribute modifiers from enchantments and special abilities or from carrying special items
... enchantments ... abilities ... items ...


Page 47  (PDF Page 52)  

Unit Window  
...  
Right-clicking on a unit displays the unit’s statistics (see Basic Unit Information).  
...  

Page 57  (PDF Page 62)  

GARRISON WINDOW  
...  
Right-clicking on a unit in this window calls up the unit statistics window (see Basic Unit Information).  
...  

Page 73  (PDF Page 78)  

BASIC UNIT INFORMATION  

Information about units can be obtained by right-clicking on their
pictures in most of the game screens.  
This calls up a unit statistics window.  
This window shows the size of the unit
(i.e., the number of figures it has),  
its cost (in magic power or gold) or its upkeep cost (depending on the screen),  
the unit’s attack strengths, defense, resistance to magic, movement capabilities, the unit’s experience level  
and special abilities (see Special Unit Abilities), if any.  
Units may also be dismissed from this unit statistics window by clicking on the dismiss button.  


Page 77 (PDF Page 82)  

Special Abilities and Enchantments  
Any special abilities and unit enchantments appear as icons in the
bottom half of the unit statistics window.  
Clicking on the enchantments removes them (if they were cast by your wizard).  
Right-clicking on the enchantments or abilities describes their effects.  
Attribute modifiers (both positive and negative) from enchantments and special abilities (see Special Unit Abilities) or from carrying special items are always incorporated in the statistics shown on the unit statistics window (i.e., the number of swords and shields is always current).  
The only exception is if the attribute modifier is conditional in some way (for example, ranged defense provided by shields (see Artifacts) only applies to defenses against ranged attacks);  
in this case, the modifier is not reflected in the number of swords or shields in the unit statistics window.


Page 80  (PDF Page 85)

Special Unit Abilities

Creature Summoning. . . . . . . . 
Allows unit to summon a creature (specified by the special ability of the unit) instead of attacking during a combat turn.  
Summoned units are randomly placed on the combat screen and do not last beyond the duration of the battle itself.  

Quiver . . . . . . . . . . .
The total number of ranged attacks remaining to
the unit for the current battle. All ammunition is
replaced after each battle. The quiver shows the
number of shots (arrows) left for that particular
battle.
