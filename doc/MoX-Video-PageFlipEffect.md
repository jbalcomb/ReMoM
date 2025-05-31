PageFlipEffect
range (0-4)
1 is never used

STGE Default?
WZD_Startup_MainGame     PageFlipEffect = 0
Loaded_Game_Update       PageFlipEffect = 0
VGA_PageFlip_FX          PageFlipEffect = 0

Only 1 Usage
VGA_PageFlip_FX()

Only 3 reads
VGA_PageFlip_FX()
City_Screen()

Per Screen?


In Main_Screen, when you right-click on an Outpost
     PageFlipEffect = 4
     RP_GUI_GrowOutLeft = _main_map_grid_y * 20
     RP_GUI_GrowOutTop = (_main_map_grid_x * 18) + 20
     RP_GUI_GrowOutFrames = 8
     j_GAME_OutpostScreen()
In Main_Screen, when you right-click on a City
     PageFlipEffect = 4
     RP_GUI_GrowOutLeft = _main_map_grid_y * 20
     RP_GUI_GrowOutTop = (_main_map_grid_x * 18) + 20
     RP_GUI_GrowOutFrames = 8
     _current_screen =  scr_City_Screen
     done_flag = ST_TRUE

On Load_Screen, if(_current_screen == scr_Main_Screen)
     PageFlipEffect = 2
      VGA_Fade_Out()




sub_72690:loc_726EB                 mov     [PageFlipEffect], 0
IDK_DiplScrn_s6EA70:loc_6EACB       mov     [PageFlipEffect], 0
IDK_Spell_Casting_Screen:loc_5C508  mov     [PageFlipEffect], 0
sub_95E65+6BA                       mov     [PageFlipEffect], 0
G_CMB_ShowSummary+138               mov     [PageFlipEffect], 0
sub_B529D+40A                       mov     [PageFlipEffect], 0
USW_HireHero+93                     mov     [PageFlipEffect], 0
GAME_LearnSpellAnim+544             mov     [PageFlipEffect], 0
GAME_AwardItems+40                  mov     [PageFlipEffect], 0
EVNT_MercHireDialog+1AC             mov     [PageFlipEffect], 0
NameOutpost_Dialog_Popup+A          mov     [PageFlipEffect], 0
City_Screen:loc_47BDB               mov     [PageFlipEffect], 0
sub_BEB92+F2                        mov     [PageFlipEffect], 0
CMB_TacticalCombat+64               mov     [PageFlipEffect], 0
Outpost_Screen+D                    mov     [PageFlipEffect], 0
NameStartingCity_Dialog_Popup+A     mov     [PageFlipEffect], 0
G_CMB_SpellEffect+CB0               mov     [PageFlipEffect], 0
G_CMB_SpellEffect+B52               mov     [PageFlipEffect], 0

IDK_DiplScrn_s6EA70+1A5             mov     [PageFlipEffect], 2
IDK_Cartographer_Screen+1AB         mov     [PageFlipEffect], 2
[x] Load_Screen:loc_F2E17               mov     [PageFlipEffect], 2
GAME_LearnSpellAnim+67              mov     [PageFlipEffect], 2

WIZ_GlobalSpellAnim+18D             mov     [PageFlipEffect], 3
sub_95E65+9A                        mov     [PageFlipEffect], 3
sub_B529D+3F5                       mov     [PageFlipEffect], 3
USW_Display:loc_5EB66               mov     [PageFlipEffect], 3
SBK_Research_Dialog+141             mov     [PageFlipEffect], 3
Production_Screen:loc_4EA2A         mov     [PageFlipEffect], 3
sub_BEB92+72                        mov     [PageFlipEffect], 3
sub_BF2AF+2D                        mov     [PageFlipEffect], 3
sub_C0C28+95                        mov     [PageFlipEffect], 3
CMB_ShowInfoWindow+29A              mov     [PageFlipEffect], 3
CTY_Consecration:loc_B19F5          mov     [PageFlipEffect], 3
EVNT_MercHireDialog+193             mov     [PageFlipEffect], 3
G_CMB_CastSpell:loc_8D0F2           mov     [PageFlipEffect], 3
G_CMB_ShowSummary+12D               mov     [PageFlipEffect], 3
G_CMB_SpellEffect+B3D               mov     [PageFlipEffect], 3
G_CMB_SpellEffect+C9B               mov     [PageFlipEffect], 3
IDK_CityEncht_s4A3F0:loc_4A8ED      mov     [PageFlipEffect], 3
IDK_UnitListScreen_s6A2D0:loc_6A5A8 mov     [PageFlipEffect], 3
Items_Screen+181                    mov     [PageFlipEffect], 3
Items_Screen+56B                    mov     [PageFlipEffect], 3
Magic_Screen+4AB                    mov     [PageFlipEffect], 3
Mirror_Screen+121                   mov     [PageFlipEffect], 3
NameStartingCity_Dialog_Popup+182   mov     [PageFlipEffect], 3
Outpost_Screen+41D                  mov     [PageFlipEffect], 3
Advisor_Screen:loc_651A4            mov     [PageFlipEffect], 3

[x] Main_Screen:loc_50C43               mov     [PageFlipEffect], 4
[x] Main_Screen:loc_50CD4               mov     [PageFlipEffect], 4
