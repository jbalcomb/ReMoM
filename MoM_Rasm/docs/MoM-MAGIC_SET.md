


_main+5D       call j_GAME_LoadSettings
SCREEN_Menu+A3 call j_GAME_LoadSettings



Up   r _main+393                     cmp  [magic_set.BG_Music], 1                
Up   o _main+25A                     mov  ax, offset magic_set                   
Up   t _main+232                     add  ax, offset magic_set.Save_Names-14h    
Up   t _main+220                     mov  magic_set.Using_Saves[bx-2], 0         
Up   t _main+1BC                     cmp  magic_set.Using_Saves[bx-2], 0         
Down w Set_Default_Game_Settings_+A  mov  [magic_set.BG_Music], 1                
Down w Set_Default_Game_Settings_+58 mov  [magic_set.Summoning_Events], 1        
Down w Set_Default_Game_Settings_+52 mov  [magic_set.Ovl_Spell_Events], 1        
Down w Set_Default_Game_Settings_+4C mov  [magic_set.City_Spell_Events], 1       
Down w Set_Default_Game_Settings_+4  mov  [magic_set.SoundFX], 1                 
Down w Set_Default_Game_Settings_+34 mov  [magic_set.Raze_City], 1               
Down w Set_Default_Game_Settings_+2E mov  [magic_set.EoT_Summary], 1             
Down w Set_Default_Game_Settings_+10 mov  [magic_set.Event_Music], 1             
Up   o SCREEN_Settings:loc_3D0CC     mov  [GUI_Settings_Offset], offset magic_set
Up   o SCREEN_Settings+F7            mov  ax, offset magic_set                   
Up   o SCREEN_Settings+2D5           mov  ax, offset magic_set                   
Down o SCREEN_NewGame_0+34F          mov  ax, offset magic_set                   
Down o SCREEN_NewGame_0+162          mov  ax, offset magic_set                   
Down r SCREEN_IntroAnim:loc_55828    cmp  [magic_set.BG_Music], 1                
Down r SCREEN_IntroAnim+320          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+306          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+2EC          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+2D2          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+28D          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+232          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+1D6          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+1BD          cmp  [magic_set.SoundFX], 1                 
Down r SCREEN_IntroAnim+161          cmp  [magic_set.SoundFX], 1                 
Up   o SAVEGAM_Save+3DD              mov  ax, offset magic_set                   
Down r Load_Game_Settings_:loc_3D893 cmp  [magic_set.Raze_City], 0               
Down r Load_Game_Settings_:loc_3D886 cmp  [magic_set.Raze_City], 1               
Down w Load_Game_Settings_+B3        mov  [magic_set.Raze_City], 1               
Down w Load_Game_Settings_+A6        mov  [magic_set.Raze_City], 1               
     o Load_Game_Settings_+72        mov  ax, offset magic_set                   
Up   o Load_Game_Settings_+4D        mov  ax, offset magic_set                   
Down o Load_Game_Settings_+10E       mov  ax, offset magic_set                   





MGC seg049
o49p01  Settings Screen
o49p02  Settings Screen Draw
o49p03  Load_MAGIC_SET()        MoO2 Load_Game_Settings()
o49p04  Default_MAGIC_SET()     MoO2 Set_Default_Game_Settings()
o49p05  Load Settings Screen Help

  mov  [Settings.SoundFX], 1
   mov  [Settings.BG_Music], 1
   mov  [Settings.Event_Music], 1
   mov  [Settings.Enemy_Moves], 1
   mov  [Settings.Enemy_Spells], 1
   mov  [Settings.Random_Events], 1
   mov  [Settings.EoT_Wait], 1
   mov  [Settings.EoT_Summary], 1
   mov  [Settings.Raze_City], 1
   mov  [Settings.Spell_Book_Order], 1
   mov  [Settings.Strategic_Combat], 0
   mov  [Settings.Spell_Anims], 1
   mov  [Settings.City_Spell_Events], 1
   mov  [Settings.Ovl_Spell_Events], 1
   mov  [Settings.Summoning_Events], 1
   mov  [Settings.Auto_Unit_Info], 1
   mov  [Settings.Show_Node_Owners], 1
   mov  [Settings.Expanding_Help], 1
   mov  [Settings.Movement_Anims], 1
   mov  [Settings.Difficulty], 0
   mov  [Settings.Opponents], 4
   mov  [Settings.MagicPower], 1
   mov  [Settings.LandSize], 1
   mov  [Settings.Input_Type], 1
   mov  [Settings.Snd_Channels], 2

8
[Settings.Using_Saves+bx],
10
[Settings.HallofFame_Scores+bx], 0
[Settings.HallofFame_Races+bx], 0
Settings.HallofFame_Names
