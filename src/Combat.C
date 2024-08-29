/*
    Combat

    WIZARDS.EXE
        ovr090
        ovr096
        ovr103
        ovr105
        ovr110
        ovr112
        ovr122
        ovr123
        ovr124

*/

#include "MoX.H"



// WZD dseg:5660                                                 ¿ BEGIN: ovr096 - Strings ?

// WZD dseg:5660
char cnst_TreatyAtk_Msg1[] = "You have a treaty with ";
// WZD dseg:5678
char cnst_TreatyAtk_Msg2[] = ".  Do you still wish to attack?";

// WZD dseg:5678                                                 ¿ END: ovr096 - Strings ?





// WZD dseg:5EA6                                                 ¿ BEGIN:  ovr123 - Strings ?

// WZD dseg:5EA6
char cnst_RazeCity_Msg[] = "Do you wish to completely destroy this city?";

// WZD dseg:5ED3
char cnst_Zombie_Msg_1[] = " units have";
// WZD dseg:5EDF
char cnst_Zombie_Msg_2[] = " unit has";
// WZD dseg:5EE9
char cnst_Zombie_Msg_3[] = " been transformed into zombies.";

// WZD dseg:5F07
char cnst_Dot9[] = ".";  // string optimizer

// WZD dseg:5F09
char cnst_Undead_Msg_1[] = " units rise from the dead to serve ";
// WZD dseg:5F2D
char cnst_Undead_Msg_2[] = " unit rises from the dead to serve ";
// WZD dseg:5F51
char cnst_Undead_Msg_3[] = "you.";
// WZD dseg:5F56
char cnst_Undead_Msg_4[] = " their creator.";
// WZD dseg:5F56
char cnst_Undead_Msg_5[] = " its creator.";

// WZD dseg:5F74
char scroll_lbx_file__ovr123[] = "scroll";

// WZD dseg:5F7B
char help_lbx_file__ovr123[] = "help";

// WZD dseg:5F80
char cnst_HOTKEY_R = 'r';

// WZD dseg:5F82
char cnst_HOTKEY_R_2 = 'R';

// WZD dseg:5F84
char cnst_HOTKEY_Esc1A = '\x1B';

// WZD dseg:5F86
char message_lbx_file__ovr123[] = "message";

// WZD dseg:5F8E
char cnst_CityLost_Msg[] = " has been conquered";

// WZD dseg:5FA2
char cnst_NewRuins_Msg[] = " has been reduced to ruins";

// WZD dseg:5FBD
char cnst_CityRaided_Msg[] = " has fallen to raiders";

// WZD dseg:5FD4
char cnst_ScrlFame_Msg_1[] = "You have ";

// WZD dseg:5FDE
char cnst_ScrlFame_Msg_2[] = "lost ";

// WZD dseg:5FE4
char cnst_ScrlFame_Msg_3[] = "gained ";

// WZD dseg:5FEC
char cnst_ScrlFame_Msg_4[] = " fame.";

// WZD dseg:5FF3
char cnst_ScrlGold_Msg[] = " gold pieces were looted.";

// WZD dseg:600D
char cnst_ScrlRaze_Msg[] = "The city has been completly destroyed.";

// WZD dseg:6034
char cnst_ScrlPop_Msg[] = " thousand inhabitants killed.";

// WZD dseg:6052
char cnst_ScrlBldng_Msg[] = "Destroyed Buildings";

// WZD dseg:6052                                                 ¿ END:  ovr123 - Strings ?

// WZD dseg:6066                                                 ¿ BEGIN:  ovr124 ?

// WZD dseg:6066 73 6F 75 6E 64 66 78                            cnst_SOUNDFX_File5 db 'soundfx'         ; DATA XREF: BU_LoadMoveSound+47o ...
// WZD dseg:606D 00                                              cnst_ZeroString_6 db 0                  ; DATA XREF: Raze_City_Prompt+DEo ...
// WZD dseg:606E 6E 65 77 73 6F 75 6E 64 00                      cnst_NEWSOUND_File db 'newsound',0      ; DATA XREF: BU_LoadMoveSound+63o
// WZD dseg:6077 63 6D 62 74 73 6E 64 00                         cnst_CMBTSND_File db 'cmbtsnd',0        ; DATA XREF: BU_LoadMeleeSound+4Fo
// WZD dseg:607F 53 4F 55 4E 44 46 58 00                         cnst_SOUNDFX_File6 db 'SOUNDFX',0       ; DATA XREF: GAME_LoadSpellSound+4Do
// WZD dseg:6087 4E 45 57 53 4F 55 4E 44 00                      cnst_NEWSOUND_File2 db 'NEWSOUND',0     ; DATA XREF: GAME_LoadSpellSound:loc_A7F56o

// WZD dseg:6090
char cnst_RazeCity_Msg2[] = "Do you wish to completely destroy this city?";
// WZD dseg:60BD
char cnst_RESOURCE_File2[] = "RESOURCE";
// WZD dseg:60C6
char cnst_COMPIX_File2[] = "compix";
// WZD dseg:60CD
char cnst_HOTKEY_N_4 = 'N';
// WZD dseg:60CF
char cnst_HOTKEY_R_3 = 'R';
// WZD dseg:60D1
char cnst_HOTKEY_Esc1B = '\x1B';

// WZD dseg:60D3 00                                              align 2

// WZD dseg:60D3                                                 ¿ END:  ovr124 ?






// WZD dseg:C406                                                 ¿ BEGIN:  ?

// WZD dseg:C406 00 00                                           CMB_ImmobileCanAct dw 0                 
// WZD dseg:C408 00 00                                           CMB_WizardCitySiege dw 0                
// WZD dseg:C40A 00 00 00 00 00 00 00 00 00 00 00 00 00 00       CMB_CityName db 0Eh dup(0)              

// WZD dseg:C418
int16_t CMB_CityDamage;

// WZD dseg:C41A 00 00                                           SND_CMB_Silence@ dw 0                   ; LBXE_LoadReplace of SFX_Silence into World_Data@
// WZD dseg:C41C 00 00                                           SND_CMB_Music@ dw 0                     ; LBXE_LoadAppend into World_Data@

// WZD dseg:C41E
int16_t CMB_DEFR_TrueSight;

// WZD dseg:C420
int16_t CMB_ATKR_TrueSight;

// WZD dseg:C422 00 00                                           IMG_CMB_Cancel_Btn@ dw 0                
// WZD dseg:C424 00 00                                           IMG_CMB_InfoBtnLock@ dw 0               
// WZD dseg:C426 00 00                                           IMG_CMB_FleeBtnLock@ dw 0               
// WZD dseg:C428 00 00                                           IMG_CMB_AutoBtnLock@ dw 0               
// WZD dseg:C42A 00 00                                           IMG_CMB_DoneBtnLock@ dw 0               
// WZD dseg:C42C 00 00                                           IMG_CMB_WaitBtnLock@ dw 0               
// WZD dseg:C42E 00 00                                           IMG_CMB_SpllBtnLock@ dw 0               
// WZD dseg:C430 00 00                                           CMB_AIGoesFirst dw 0                    
// WZD dseg:C432 00 00                                           CMB_Auto_ON dw 0                        
// WZD dseg:C434 00 00                                           CRP_CMB_AI_Player_2 dw 0                
// WZD dseg:C436 00 00                                           CRP_CMB_HumanPlayer dw 0                
// WZD dseg:C438 00 00                                           CMB_WizCastAvailable dw 0               
// WZD dseg:C438                                                                                         ; now 1 (during tactical combat init)
// WZD dseg:C438                                                                                         ; enables or disables the Spell button when a finished
// WZD dseg:C438                                                                                         ; unit is selected
// WZD dseg:C43A 00 00                                           CMB_DEFR_First_CE dw 0                  
// WZD dseg:C43A                                                                                         ; now 0 (after tactical BU init)
// WZD dseg:C43A                                                                                         ; defender's first shown combat enchant
// WZD dseg:C43C 00 00                                           CMB_ATKR_First_CE dw 0                  
// WZD dseg:C43C                                                                                         ; now 0 (after tactical BU init)
// WZD dseg:C43C                                                                                         ; attacker's first shown combat enchant
// WZD dseg:C43E 00 00                                           CMB_AI_Player dw 0                      
// WZD dseg:C43E                                                                                         ; when populating the BU table, set to the index of the
// WZD dseg:C43E                                                                                         ; AI player (the other participant being always human)
// WZD dseg:C440 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_DEFR_CE_Labels dw 0Fh dup(0)        
// WZD dseg:C45E 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_ATKR_CE_Labels dw 0Fh dup(0)        

// WZD dseg:C47C
int16_t OVL_Action_Plane;
// WZD dseg:C47E
int16_t OVL_Action_YPos;
// WZD dseg:C480
int16_t OVL_Action_XPos;

// WZD dseg:C482 00 00                                           CMB_ATKR_CE_Count dw 0                  
// WZD dseg:C484 00 00                                           CMB_DEFR_CE_Count dw 0                  
// WZD dseg:C486 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_DEFR_CE_Wnd Active_CE 0Fh dup(<0>)  
// WZD dseg:C4C2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_ATKR_CE_Wnd Active_CE 0Fh dup(<0>)  
// WZD dseg:C4FE 00 00                                           IMG_CMB_TrueLight@ dw 0                 
// WZD dseg:C4FE                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C500 00 00                                           IMG_CMB_Darkness@ dw 0                  
// WZD dseg:C500                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C502 00 00                                           IMG_CMB_WarpReality@ dw 0               
// WZD dseg:C502                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C504 00 00                                           IMG_CMB_BlackPrayer@ dw 0               
// WZD dseg:C504                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C506 00 00                                           IMG_CMB_Wrack@ dw 0                     
// WZD dseg:C506                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C508 00 00                                           IMG_CMB_MetalFires@ dw 0                
// WZD dseg:C508                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C50A 00 00                                           IMG_CMB_Prayer@ dw 0                    
// WZD dseg:C50A                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C50C 00 00                                           IMG_CMB_HighPrayer@ dw 0                
// WZD dseg:C50C                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C50E 00 00                                           IMG_CMB_Terror@ dw 0                    
// WZD dseg:C50E                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C510 00 00                                           IMG_CMB_CallLightng@ dw 0               
// WZD dseg:C510                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C512 00 00                                           IMG_CMB_CounterMgc@ dw 0                
// WZD dseg:C512                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C514 00 00                                           IMG_CMB_MassInvis@ dw 0                 
// WZD dseg:C514                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C516 00 00                                           IMG_CMB_Entangle@ dw 0                  
// WZD dseg:C516                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C518 00 00                                           IMG_CMB_ManaLeak@ dw 0                  
// WZD dseg:C518                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C51A 00 00                                           IMG_CMB_Blur@ dw 0                      
// WZD dseg:C51A                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C51C 00 00                                           CMB_Turn dw 0                           
// WZD dseg:C51E 00 00                                           CMB_HumanUnitsDone dw 0                 
// WZD dseg:C520 00 00                                           CMB_combat_structure dw 0               
// WZD dseg:C520                                                                                         ; a seemingly pointless variable whose state could have
// WZD dseg:C520                                                                                         ; been extrapolated from others
// WZD dseg:C520                                                                                         ;   1 - city battle (defender only for wall spells?)
// WZD dseg:C520                                                                                         ;   2 - sorcery node
// WZD dseg:C520                                                                                         ;   3 - nature node
// WZD dseg:C520                                                                                         ;   4 - chaos node
// WZD dseg:C520                                                                                         ;   6 - sailable tile
// WZD dseg:C522 00 00                                           GUI_CombatWindow@ dw 0                  
// WZD dseg:C522                                                                                         ; 12 LBX_NearAlloc_Next bytes
// WZD dseg:C524 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_TargetRows@ dw 16h dup(0)           
// WZD dseg:C524 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 22 pointers to LBX_NearAlloc_Next
// WZD dseg:C524 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; allocations of 21 bytes each
// WZD dseg:C550 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_BU_Figure_GFX@ dw 12h dup(0)        
// WZD dseg:C550 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+                                        ; array of 18 segment pointers after an LBX_Alloc_Mark
// WZD dseg:C550 00 00 00 00                                                                             ; of the sandbox, one every 38h paragraphs
// WZD dseg:C550                                                                                         ;
// WZD dseg:C550                                                                                         ; WARNING: this allocation is always undone!
// WZD dseg:C574 00                                              db    0
// WZD dseg:C575 00                                              db    0
// WZD dseg:C576 00 00                                           IMG_CMB_Info_Btn@ dw 0                  
// WZD dseg:C576                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C578 00 00                                           IMG_CMB_Flee_Btn@ dw 0                  
// WZD dseg:C578                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C57A 00 00                                           IMG_CMB_Auto_Btn@ dw 0                  
// WZD dseg:C57A                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C57C 00 00                                           IMG_CMB_Done_Btn@ dw 0                  
// WZD dseg:C57C                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C57E 00 00                                           IMG_CMB_Wait_Btn@ dw 0                  
// WZD dseg:C57E                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C580 00 00                                           IMG_CMB_Spell_Btn@ dw 0                 
// WZD dseg:C580                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C582 00 00                                           CMB_SelectedUnit dw 0                   
// WZD dseg:C582                                                                                         ; active battle unit in combat?
// WZD dseg:C582                                                                                         ; 666 - rampage, no ruins
// WZD dseg:C582                                                                                         ; 667 - raiders won (city neutral)
// WZD dseg:C582                                                                                         ; 668 - rampage, created ruins
// WZD dseg:C584 00 00                                           _combat_defender_player dw 0            
// WZD dseg:C586 00 00                                           _combat_attacker_player dw 0            
// WZD dseg:C588 00 00                                           _combat_total_unit_count dw 0           
// WZD dseg:C58A 00 00                                           CMB_CursorBattleUnit dw 0               
// WZD dseg:C58A                                                                                         ; the combat unit display is based on this
// WZD dseg:C58C 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CMB_InfoIcons@ dw 0Eh dup(0)        
// WZD dseg:C58C 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; array of 14 pointers to World_Data@ appended images
// WZD dseg:C5A8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+IMG_CMB_ATK_Icons@ ATK_ICONS <0>        
// WZD dseg:C5A8 00 00                                                                                   ; array of 9 pointers to sandbox appended images
// WZD dseg:C5BA 00 00                                           IMG_GAME_LimboFall@ dw 0                
// WZD dseg:C5BA                                                                                         ; 190h appended paragraphs into the LBX_Sandbox_Segment
// WZD dseg:C5BC 00 00                                           GAME_LimboFall_Stage dw 0               
// WZD dseg:C5BE 00 00                                           IMG_GAME_WizHandsUp@ dw 0               
// WZD dseg:C5BE                                                                                         ; LBXE_LoadReplace into the LBX_Sandbox_Segment
// WZD dseg:C5C0 00 00                                           IMG_GAME_RedSparkle@ dw 0               
// WZD dseg:C5C2 00 00                                           GAME_Conquering_Wiz dw 0                
// WZD dseg:C5C4 00 00                                           GAME_Conquered_Wiz dw 0                 
// WZD dseg:C5C6 00 00                                           SND_GAME_Scream@ dw 0                   
// WZD dseg:C5C6                                                                                         ; appended into the LBX_Sandbox_Segment (~20k)
// WZD dseg:C5C8 00 00                                           GAME_ConqstAnimStage dw 0               
// WZD dseg:C5CA 00 00                                           IMG_GAME_ZappedWiz@ dw 0                
// WZD dseg:C5CA                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C5CC 00 00                                           IMG_GAME_GrowZap@ dw 0                  
// WZD dseg:C5CC                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C5CE 00 00                                           IMG_GAME_Axeboy@ dw 0                   
// WZD dseg:C5CE                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C5D0 00 00                                           IMG_GAME_ConeHead@ dw 0                 
// WZD dseg:C5D0                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C5D2 00 00                                           IMG_GAME_ConqrngWiz@ dw 0               
// WZD dseg:C5D2                                                                                         ; appended into the LBX_Sandbox_Segment
// WZD dseg:C5D4 00 00                                           IMG_GAME_ConqrdWiz@ dw 0                

// WZD dseg:C5D4                                                 ¿ END:  ?







// 
// WZD dseg:C582
int16_t CMB_SelectedUnit;
// WZD dseg:C584
int16_t _combat_defender_player;
// WZD dseg:C586
int16_t _combat_attacker_player;
// WZD dseg:C588
int16_t _combat_total_unit_count;





// WZD dseg:C7AA                                                 ¿ BEGIN:  ovr103 ?

// WZD dseg:C7AA 00 00 00 00 00 00                               IMG_CMB_MoveIcons@ CMOVE_ICONS <0>      ; DATA XREF: CMB_DrawAUWStats+168r ...
// WZD dseg:C7AA                                                                                         ; array of 3 pointers to sandbox appended images
// WZD dseg:C7B0 00 00                                           IMG_CMB_HitBar@ dw 0                    ; DATA XREF: CMB_DrawActiveUnitW+1FAr ...
// WZD dseg:C7B2 00 00                                           IMG_CMB_FX_Figure@ dw 0                 ; DATA XREF: BU_CreateImage+1Aw ...
// WZD dseg:C7B2                                                                                         ; 7Ch paragraphs appended into World_Data@
// WZD dseg:C7B4 00                                              CMB_HeavenlyLight db 0                  ; DATA XREF: CMB_ShowInfoWindow+29w ...
// WZD dseg:C7B5 00                                              CMB_CloudofShadow db 0                  ; DATA XREF: CMB_ShowInfoWindow:loc_83D3Dw ...
// WZD dseg:C7B6 00                                              CMB_CentralStructure db 0               ; DATA XREF: CMB_ShowInfoWindow:loc_83D31w ...
// WZD dseg:C7B7 00                                              db    0
// WZD dseg:C7B8 00 00                                           IMG_CMB_Info_Box@ dw 0                  ; DATA XREF: CMB_ShowInfoWindow+F1w ...
// WZD dseg:C7B8                                                                                         ; appended into World_Data@
// WZD dseg:C7BA 00 00                                           IMG_CMB_Info_Bottom@ dw 0               ; DATA XREF: CMB_ShowInfoWindow+C3w ...
// WZD dseg:C7BA                                                                                         ; appended into World_Data@
// WZD dseg:C7BC 00 00                                           IMG_CMB_Info_Middle@ dw 0               ; DATA XREF: CMB_ShowInfoWindow+DAw ...
// WZD dseg:C7BC                                                                                         ; appended into World_Data@
// WZD dseg:C7BE 00 00                                           IMG_CMB_Info_Top@ dw 0                  ; DATA XREF: CMB_ShowInfoWindow+130w ...
// WZD dseg:C7BE                                                                                         ; appended into the sandbox
// WZD dseg:C7C0 00 00                                           CMB_PerSideInfo dw 0                    ; DATA XREF: CMB_ShowInfoWindow:loc_83E82r ...

// WZD dseg:C7C2
// ; contains a Node_Type flag or -1 (unit bonus aura)
int16_t CMB_node_type;

// WZD dseg:C7C4 00 00                                           CMB_Info_CommonCount dw 0               ; DATA XREF: CMB_ShowInfoWindow+144r ...
// WZD dseg:C7C6 00 00                                           CMB_ExternalFX_Count dw 0               ; DATA XREF: Tactical_Combat__WIP+143w ...
// WZD dseg:C7C8 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+CMB_External_FX@ dw 0Eh dup(0)          ; DATA XREF: CMB_ShowInfoWindow+82w ...
// WZD dseg:C7C8 00 00 00 00 00 00 00 00 00 00 00 00                                                     ; 30 LBX_NearAlloc_Next bytes each
// WZD dseg:C7E4                                                 ; int CMB_InfoWindow_Top
// WZD dseg:C7E4 00 00                                           CMB_InfoWindow_Top dw 0                 ; DATA XREF: CMB_ShowInfoWindow+13Ew ...
// WZD dseg:C7E6                                                 ; int CMB_InfoWindow_Left
// WZD dseg:C7E6 00 00                                           CMB_InfoWindow_Left dw 0                ; DATA XREF: CMB_ShowInfoWindow+138w ...

// WZD dseg:C7E6                                                 ¿ END:  ovr103 ?








// WZD dseg:C896
int16_t * CMB_LeadershipArray;
// WZD dseg:C89A
int16_t * CMB_ResistAllArray;
// WZD dseg:C89E
int16_t * CMB_HolyBonusArray;
// WZD dseg:C8A2
int16_t * CMB_IDK_4PR;






// WZD dseg:C972                                                 ¿ BEGIN: ovr123 - Uninitialized Data ?

// WZD dseg:C972
int16_t CMB_Winner;
// WZD dseg:C974
int16_t GAME_RazeCity;
// WZD dseg:C976
int16_t CMB_Gold_Reward;
// WZD dseg:C978
int16_t CMB_Buildings_Lost;
// WZD dseg:C97A
int16_t * CMB_LostBuildings;
// WZD dseg:C97C
int16_t CMB_Population_Lost;
// WZD dseg:C97E
int16_t CMB_ScrollMsg_Type;

// WZD dseg:C97E                                                 ¿ END: ovr123 - Uninitialized Data ?    






// WZD dseg:D13E
SAMB_ptr CMB_Path_Ys;
// WZD dseg:D140
SAMB_ptr CMB_Path_Xs;
// WZD dseg:D142 00 00                                           CMB_Path_Length dw 0                    ; DATA XREF: BU_Move:loc_77989w ...
// WZD dseg:D144
SAMB_ptr CMB_NearBuffer_3;
// WZD dseg:D146
SAMB_ptr CMB_Path_Costs;
// WZD dseg:D148
SAMB_ptr CMB_ActiveMoveMap;
// WZD dseg:D14A 00 00                                           CMB_Chasm_Anim_Y dw 0                   ; DATA XREF: TILE_CracksCall+1Ew ...
// WZD dseg:D14C 00 00                                           CMB_Chasm_Anim_X dw 0                   ; DATA XREF: TILE_CracksCall+18w ...
// WZD dseg:D14E
SAMB_ptr CMB_Vortex_Array;
// WZD dseg:D152 00 00                                           CMB_Vortex_Count dw 0                   ; DATA XREF: BU_Move:loc_77934r ...
// WZD dseg:D154
SAMB_ptr CMB_Projectiles;
// WZD dseg:D158 00 00                                           CMB_ProjectileCount dw 0                ; DATA XREF: CMB_RangedAnim+464r ...
// WZD dseg:D15A
SAMB_ptr CMB_EntitySprites;
// WZD dseg:D15E 00 00                                           CMB_SpriteCount dw 0                    ; DATA XREF: CMB_DrawEntities:loc_DD3A3r ...





/*
    WIZARDS.EXE  ovr090
*/

// WZD s90p01
// drake178: CMB_TacticalCombat()
int16_t Tactical_Combat__WIP(int16_t combat_attacker_player_idx, int16_t combat_defender_player_idx, int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t did_win;

    did_win = ST_TRUE;

    return did_win;
}



/*
    WIZARDS.EXE  ovr096
*/

// WZD s96p01
/*
Move_Units() |-> Combat()
Lair_Combat() |-> ... |-> Combat()
...
Combat()
    |-> { STK_CaptureCity(), Tactical_Combat(), Strategic_Combat() }

*/
int16_t Combat__WIP(int16_t attacker_player_idx, int16_t defender_player_idx, int16_t troop_count, int16_t troops[])
{
    int16_t Item_List[18];
    int16_t City_Destroyed;
    int16_t Item_Count;
    int16_t Garrison_Size;
    int16_t combat_attacker_player_idx;
    int16_t Battle_Outcome;
    int16_t defender_idx;  // _DI_
    int16_t itr;  // _SI_
    int16_t combat_defender_player_idx;
    int16_t return_value;  // DNE in Dasm


    City_Destroyed = ST_FALSE;

    GAME_RazeCity = ST_FALSE;

    _unit_stack_count = 0;

    combat_attacker_player_idx = attacker_player_idx;
    defender_idx = defender_player_idx;  // unit_idx or player_idx

    for(itr = 0; itr < troop_count; itr++)
    {
        _UNITS[troops[itr]].wx = OVL_Action_OriginX;
        _UNITS[troops[itr]].wy = OVL_Action_OriginY;
    }

    if(combat_attacker_player_idx == NEUTRAL_PLAYER_IDX)
    {
        switch(OVL_Action_Type)
        {
            case 0:  // Stack vs. Stack
            {
                if(_UNITS[defender_idx].owner_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr = 0; itr < troop_count; itr++)
                    {
                        _UNITS[troops[itr]].Status = us_Ready;
                    }
                    goto No_Combat;
                }
            } break;
            case 1:  // Stack vs. City
            {
                if(_CITIES[OVL_Action_Structure].owner_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr = 0; itr < troop_count; itr++)
                    {
                        _UNITS[troops[itr]].Status = us_Ready;
                    }
                    goto No_Combat;
                }
            } break;
            case 5:  // Stack vs. Lair
            {
                for(itr = 0; itr < troop_count; itr++)
                {
                    _UNITS[troops[itr]].Status = us_Ready;
                }
                goto No_Combat;
            } break;
        }
    }

    switch(OVL_Action_Type)
    {
        case 0:  // Stack vs. Stack
        {
            OVL_Action_XPos = _UNITS[defender_idx].wx;
            OVL_Action_YPos = _UNITS[defender_idx].wy;
            OVL_Action_Plane = _UNITS[defender_idx].wp;
            defender_idx = _UNITS[defender_idx].owner_idx;  // NOTE: after this, defender_idx is player_idx for both combat type 0 and 1
        } break;
        case 1:  // Stack vs. City
        {
            // TODO  _players.banner_id+17E8h], BNR_Brown
            OVL_Action_XPos = _CITIES[OVL_Action_Structure].wx;
            OVL_Action_YPos = _CITIES[OVL_Action_Structure].wy;
            OVL_Action_Plane = _CITIES[OVL_Action_Structure].wp;
            Garrison_Size = 0;
            for(itr = 0; itr < _units; itr++)
            {
                if(
                    (_UNITS[itr].wx == OVL_Action_XPos) &&
                    (_UNITS[itr].wy == OVL_Action_YPos) &&
                    (_UNITS[itr].wp == OVL_Action_Plane) &&
                    (_UNITS[itr].owner_idx == defender_idx)
                )
                {
                    Garrison_Size++;
                }

            }

        } break;
        case 5:  // Stack vs. Lair
        {
            // TODO  _players.banner_id+17E8h], BNR_Brown
            Lair_Make_Guardians(OVL_Action_Structure);
            OVL_Action_XPos = _LAIRS[OVL_Action_Structure].wx;
            OVL_Action_YPos = _LAIRS[OVL_Action_Structure].wy;
            OVL_Action_Plane = _LAIRS[OVL_Action_Structure].wp;
        } break;
    }

    if(
        (combat_attacker_player_idx < NEUTRAL_PLAYER_IDX) &&
        (defender_idx < NEUTRAL_PLAYER_IDX) &&
        (
            (OVL_Action_Type == 1) ||
            (OVL_Action_Type == 0)
        )
    )
    {
        if(
            (_players[defender_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_Alliance)
            ||
            (_players[defender_idx].Dipl.Dipl_Status[combat_attacker_player_idx] == DIPL_WizardPact)
        )
        {
            if(combat_attacker_player_idx != _human_player_idx)
            {
                for(itr = 0; itr < troop_count; itr++)
                {
                    _UNITS[troops[itr]].Status = us_Ready;
                }
                goto No_Combat;
            }
            else
            {
                strcpy(GUI_NearMsgString, cnst_TreatyAtk_Msg1);  // "You have a treaty with "
                strcat(GUI_NearMsgString, _players[defender_idx].name);
                strcat(GUI_NearMsgString, cnst_TreatyAtk_Msg2);  // ".  Do you still wish to attack?"
                if(Confirmation_Box(GUI_NearMsgString) != ST_FALSE)
                {
                    if(
                        (combat_attacker_player_idx < 6)
                        &&
                        (defender_idx < 6)
                    )
                    {
                        // drake178: ; BUG: calling these two functions in this order can allow war without declaring or properly applying it
                        Change_Relations(-40, combat_attacker_player_idx, defender_idx, 5, 0, 0);
                        Break_Treaties(defender_idx, combat_attacker_player_idx);
                    }
                }
                else
                {
                    goto No_Combat;
                }
            }

        }
    }

    if(
        (OVL_Action_Type == 1) &&
        (Garrison_Size < 1)
    )
    {
        Battle_Outcome = ST_TRUE;
        Item_Count = 0;
        STK_CaptureCity__WIP(troop_count, &troops[0]);
    }
    else
    {
        if(
            (
                (combat_attacker_player_idx == _human_player_idx)
                ||
                (defender_idx == _human_player_idx)
            )
            &&
            (magic_set.Strategic_Combat == ST_FALSE)
        )
        {
            // TODO  SND_Silence();
            Battle_Outcome = Tactical_Combat__WIP(combat_attacker_player_idx, defender_idx, troops, troop_count, OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane, &Item_Count, &Item_List[0]);
            // TODO  SND_PlayBkgrndTrack();
        }
        else
        {
            Battle_Outcome = Strategic_Combat__WIP(troops, troop_count, OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane, &Item_Count, &Item_List[0]);
            Item_Count = 0;
        }
    }


// @@StartPostCombat
    if(Battle_Outcome == ST_TRUE)
    {
        for(itr = 0; itr < troop_count; itr++)
        {
            if(_UNITS[troops[itr]].owner_idx != 100)  // DEDU  ¿ another case of this field being a status, like 'dead' ?
            {
                _UNITS[troops[itr]].wx = OVL_Action_XPos;
                _UNITS[troops[itr]].wy = OVL_Action_YPos;
                _UNITS[troops[itr]].wp = OVL_Action_Plane;
            }
            else
            {
                _UNITS[troops[itr]].owner_idx = combat_attacker_player_idx;
                _UNITS[troops[itr]].Finished = ST_TRUE;
            }
        }

        item_pool_in_process = ST_TRUE;
        m_item_wx = OVL_Action_XPos;
        m_item_wy = OVL_Action_YPos;
        m_item_wp = OVL_Action_Plane;
        Player_Process_Item_Pool(combat_attacker_player_idx, Item_Count, &Item_List[0]);
        item_pool_in_process = ST_FALSE;

        if(
            (OVL_Action_Type == 1) &&
            (_CITIES[OVL_Action_Structure].owner_idx != combat_attacker_player_idx)
        )
        {
            if(_CITIES[OVL_Action_Structure].size != 0)
            {
                if(
                    (combat_attacker_player_idx < NUM_PLAYERS) &&
                    (defender_idx < NUM_PLAYERS)
                )
                {
                    Change_Relations(-40, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                }

                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX) ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
                )
                {
                    if(
                        (combat_attacker_player_idx < NUM_PLAYERS) &&
                        (defender_idx < NUM_PLAYERS)
                    )
                    {
                        Declare_War(combat_attacker_player_idx, defender_idx);
                    }

                    if(GAME_RazeCity == ST_FALSE)
                    {
                        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);

                        if(combat_attacker_player_idx != _human_player_idx)
                        {
                            _CITIES[OVL_Action_Structure].construction = bt_AUTOBUILD;  // -4 gran vizier
                        }

                    }

                }

            }
            else
            {
                if(
                    (combat_attacker_player_idx != NEUTRAL_PLAYER_IDX) ||
                    ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
                )
                {
                    City_Destroyed = ST_TRUE;
                }
            }
        }

    }
    else
    {
        Player_Process_Item_Pool(defender_idx, Item_Count, &Item_List[0]);
        item_pool_in_process = ST_FALSE;  // drake178: BUG: never set it to 1 in this branch

        if(OVL_Action_Type == 1)
        {
            if(_CITIES[OVL_Action_Structure].owner_idx == defender_idx)
            {
                if(
                    (combat_attacker_player_idx < 6) &&
                    (defender_idx < 6)
                )
                {
                    if(_CITIES[OVL_Action_Structure].size != 0)
                    {
                        if(Player_Fortress_City(defender_idx) != OVL_Action_Structure)
                        {
                            Change_Relations(-20, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                        }
                        else
                        {
                            Change_Relations(-60, combat_attacker_player_idx, defender_idx, 9, OVL_Action_Structure, 0);
                        }
                    }

                }

            }
            else
            {
                if(_CITIES[OVL_Action_Structure].size == 0)
                {
                    City_Destroyed = ST_TRUE;
                }
                else
                {
                    if(GAME_RazeCity == ST_FALSE)
                    {
                        Change_City_Ownership(OVL_Action_Structure, defender_idx);
                    }
                }
            }

        }

    }



    Update_Defender_Hostility(combat_attacker_player_idx, defender_idx);

    if(City_Destroyed == ST_TRUE)
    {
        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);
        Destroy_City(OVL_Action_Structure);
        GAME_RazeCity = ST_FALSE;
    }

    if(GAME_RazeCity == ST_TRUE)
    {
        Change_City_Ownership(OVL_Action_Structure, combat_attacker_player_idx);
        Destroy_City(OVL_Action_Structure);
        GAME_RazeCity = ST_FALSE;
    }

    OVL_Action_Structure = ST_UNDEFINED;
    OVL_Action_Type = ST_UNDEFINED;

    // DONT j_EmptyFxn_o153p24()

    Allocate_Reduced_Map();

    if(
        (combat_attacker_player_idx == _human_player_idx) ||
        (defender_idx == _human_player_idx)
    )
    {
        Set_Entities_On_Map_Window(_map_x, _map_y, _map_plane);
        Full_Draw_Main_Screen();
        Copy_On_To_Off_Page();
    }

    return_value = Battle_Outcome;

    goto Done;


No_Combat:
    OVL_Action_Structure = ST_UNDEFINED;
    OVL_Action_Type = ST_UNDEFINED;
    return_value = 0;
    goto Done;


Done:
    return return_value;
}


// WZD s96p02
// drake178: EZ_CreateGuardStack()
// MoO2: DNE
void Lair_Make_Guardians(int16_t lair_idx)
{
    int16_t guard_count;
    int16_t unit_count;
    int16_t itr;  // _DI_

    // TODO figure out lair guardian count high and low nibbles, so you can make a macro for them
    // TODO make macro for lair guardian count high and low nibbles
    guard_count = (_LAIRS[lair_idx].guard1_count & 0x0F);

    unit_count = 0;

    for(itr = 0; ((itr < guard_count) && (unit_count < MAX_STACK)); itr++)
    {
        if(Create_Unit__WIP(_LAIRS[lair_idx].guard1_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
        {
            if(_LAIRS[lair_idx].guard1_unit_type < ut_Magic_Spirit)
            {
                _UNITS[(_units - 1)].mutations = (_UNITS[(_units - 1)].mutations | UM_UNDEAD);
            }
            unit_count++;
        }
    }

    guard_count = (_LAIRS[lair_idx].guard2_count & 0x0F);

    for(itr = 0; ((itr < guard_count) && (unit_count < MAX_STACK)); itr++)
    {
        if(Create_Unit__WIP(_LAIRS[lair_idx].guard2_unit_type, NEUTRAL_PLAYER_IDX, _LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, 2000) == ST_TRUE)
        {
            if(_LAIRS[lair_idx].guard2_unit_type < ut_Magic_Spirit)
            {
                _UNITS[(_units - 1)].mutations = (_UNITS[(_units - 1)].mutations | UM_UNDEAD);
            }
            unit_count++;
        }
    }

}


// WZD s96p03
// drake178: EZ_ResolveEntry()
/*
    wrapper for combat for lair
    rebuilds troop array
    equivalent of Move_Units() |-> Combat() for Stack/City
    hard-coded with Neutral Player as Defender
*/
int16_t Lair_Combat_Do(int16_t lair_idx, int16_t player_idx)
{
    int16_t troops[9];
    int16_t troop_count;
    int16_t winner;

    Player_Army_At_Square(_LAIRS[lair_idx].wx, _LAIRS[lair_idx].wy, _LAIRS[lair_idx].wp, player_idx, &troop_count, &troops[0]);

    winner = Combat__WIP(player_idx, NEUTRAL_PLAYER_IDX, troop_count, &troops[0]);

    return winner;
}


// WZD s96p04
// drake178: sub_7DE08()

// WZD s96p05
// drake178: WIZ_SetHostile()
void Update_Defender_Hostility(int attacker_player_idx, int defender_player_idx)
{

    _players[defender_player_idx].Hostility[attacker_player_idx] = 2;

    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] >= 3)  // DIPL_War
    {
        _players[defender_player_idx].Hostility[attacker_player_idx] = 3;
    }

    if(_players[defender_player_idx].Dipl.Dipl_Status[attacker_player_idx] == 2)  // DIPL_Alliance
    {
        _players[defender_player_idx].Hostility[attacker_player_idx] = 0;
    }

    _players[defender_player_idx].War_Reeval = (15+ Random(10));

}


// WZD s96p06
// UU_IDK_Main_Screen_Draw()



/*
    WIZARDS.EXE  ovr103
*/

// WZD s103p01
// CMB_ShowInfoWindow()

// WZD s103p02
// CMB_DrawInfoWindow()

// WZD s103p03
// CMB_FillInfoArrays()

// WZD s103p04
// CMB_SetCommonXFX()

// WZD s103p05
// CMB_CountExternalFX()

// WZD s103p06
// ¿ MoO2  Point_Is_In_Some_Nebula_() |-> Point_Is_In_Nebula_N_() ?
/*
    sets node type, if any
*/
void CMB_Set_Node_Type(void)
{
    int16_t itr_auras;  // _SI_
    int16_t node_type;  // _DI_
    int16_t itr_nodes;  // _CX_

    node_type = ST_UNDEFINED;

    for(itr_nodes = 0; ((itr_nodes < NUM_NODES) && (node_type == ST_UNDEFINED)); itr_nodes++)
    {
        if(_NODES[itr_nodes].wp != OVL_Action_Plane)
        {
            continue;
        }

        for(itr_auras = 0; ((itr_auras < _NODES[itr_nodes].power) && (node_type == ST_UNDEFINED)); itr_auras++)
        {
            if(
                (_NODES[itr_nodes].Aura_Xs[itr_auras] == OVL_Action_XPos)
                &&
                (_NODES[itr_nodes].Aura_Ys[itr_auras] == OVL_Action_YPos)
            )
            {
                node_type = _NODES[itr_nodes].type;
            }
        }

    }

    CMB_node_type = node_type;

}


// WZD s103p07
// BU_CreateImage()

// WZD s103p08
// WIZ_BU_SelectNext()

// WZD s103p09
// WIZ_BU_SelectClosest()

// WZD s103p10
// CMB_SpellcastError()

// WZD s103p11
// BU_MoveConfused()

// WZD s103p12
// CMB_LoadResources()

// WZD s103p13
// CMB_SetNearAllocs()

// WZD s103p14
// CMB_SelectCaster()




/*
    WIZARDS.EXE  ovr105
*/

// WZD o105p01
// CMB_WinLoseFlee()

// WZD o105p02
// LBX_IMG_BannerPaint()

// WZD o105p03
// LBX_IMG_Highlight()

// WZD o105p04
// LBX_IMG_FX()

// WZD o105p05
// BU_SetUEColor()

// WZD o105p06
// LBX_IMG_UE_Outline()

// WZD o105p07
void CMB_UpdateTrueSight(void)
{
    int16_t itr_battle_units;  // _CX_

    CMB_ATKR_TrueSight = ST_FALSE;
    CMB_DEFR_TrueSight = ST_FALSE;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].Status == 0) /* Unit_Active */
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                if((battle_units[itr_battle_units].Attribs_1 & 0x08) != 0) /* Imm_Illusion */
                {
                    CMB_ATKR_TrueSight = ST_TRUE;
                }
            }
            else
            {
                if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                {
                    if((battle_units[itr_battle_units].Attribs_1 & 0x08) != 0) /* Imm_Illusion */
                    {
                        CMB_DEFR_TrueSight = ST_TRUE;
                    }
                }
            }
        }
    }

}


// WZD o105p08
// BU_SetVisibility()

// WZD o105p09
// BU_GetCombatEffect()

// WZD o105p10
// BU_SetBaseAnims()

// WZD o105p11
// BU_GetWpnIconIndex()

// WZD o105p12
// BU_GetRngIconIndex()

// WZD o105p13
// CMB_DrawUnitDisplay()



/*
    WIZARDS.EXE  ovr110
*/

// WZD o110p01
/*

*/
void Strategic_Combat_Allocate(void)
{
    int16_t itr;  // _SI_

    // 249 * 16 = 3984 / 110 = 36.2182 ... 36? 2 * max stack = 18 so, ...?
    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 249);

    // 3 * 16 = 48 ... 1PR header, 15 2-byte values
    combat_enchantments = (struct s_COMBAT_ENCHANTMENTS *)Allocate_Next_Block(_screen_seg, 3);

    for(itr = 0; itr < 30; itr++)
    {
        *((char *)&combat_enchantments[itr]) = ST_FALSE;
    }

    CMB_IDK_4PR = (int16_t *)Allocate_Next_Block(_screen_seg, 4);  // header + sub-header + 2 PR ... or header + 3 sub-headers?
    CMB_HolyBonusArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B
    CMB_ResistAllArray  = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B
    CMB_LeadershipArray = (int16_t *)Allocate_Next_Block(_screen_seg, 3);  // sub-header + 2 PR  32 B

}


// WZD o110p02
/*
    "WIP", cause ¿ ?

*/
int16_t Strategic_Combat__WIP(int16_t troops[], int16_t troop_count, int16_t wx, int16_t wy, int16_t wp, int16_t * item_count, int16_t item_list[])
{
    int16_t Weights[36];
    int16_t var_66;
    int16_t var_64;
    int16_t var_62;
    int16_t var_60;
    int16_t var_5E;
    int16_t var_5C;
    int16_t defender_third_nominal_skill;
    int16_t attacker_third_nominal_skill;
    int16_t var_56;
    int16_t var_54;
    int16_t var_52;
    int16_t var_50;
    int16_t var_4E;
    int16_t var_4C;
    int16_t defender_mana_multiplied;
    int16_t attacker_mana_multiplied;
    int16_t HP;
    int16_t special_ranged_attack_strength;  // DNE in Dasm  (re-uses HP)
    int16_t var_44;
    int16_t var_42;
    int16_t var_40;
    int16_t IDK_damage_defender;
    int16_t IDK_damage_attacker;
    int32_t var_3A;
    int32_t var_36;
    int32_t IDK_health_defender__2;
    int32_t IDK_ranged_threat_defender;
    int32_t IDK_melee_threat_defender;
    int32_t IDK_health_attacker__2;
    int32_t IDK_ranged_threat_attacker;
    int32_t IDK_melee_threat_attacker;
    int32_t IDK_health_defender;
    int32_t IDK_health_attacker;
    int16_t combat_structure;
    int16_t Dmg_Array[3];
    int16_t BU_Index;  // ¿ MsgType__BU_Index ?
    int16_t MsgType;  // ¿ MsgType__BU_Index ?
    int16_t unit_idx;
    int16_t _combat_defender_count;
    int16_t winner_player_idx;
    int16_t itr;  // _SI_
    int16_t _combat_attacker_count;  // DNE in Dasm
    int16_t itr_units;  // _SI_
    int16_t itr_battle_units;  // _SI_
    int16_t itr_combat_turns;  // _SI_
    int16_t spell_ranks;  // _DI_
    int16_t did_win;  // DNE in Dasm


    var_5C = 0;
    var_64 = 0;
    var_60 = 0;
    var_5E = 0;
    var_66 = 0;
    var_62 = 0;

    Set_Page_Off();
    Allocate_Reduced_Map();
    Main_Screen_Draw();
    Copy_Off_To_Back();

    OVL_Action_XPos = wx;
    OVL_Action_YPos = wy;
    OVL_Action_Plane = wp;

    _combat_defender_count = 0;
    _combat_attacker_count = troop_count;  // DNE in Dasm

    _combat_total_unit_count = 0;

    Strategic_Combat_Allocate();


    combat_structure = Combat_Structure(wx, wy, wp, ST_TRUE);


    for(itr_units = (_units - 1); itr_units > 0; itr_units--)
    {
        if(
            (_UNITS[itr_units].wx == wx)
            &&
            (_UNITS[itr_units].wy == wy)
            &&
            (_UNITS[itr_units].wp == wp)
            &&
            (_UNITS[itr_units].owner_idx != _UNITS[troops[0]].owner_idx)
        )
        {
            _combat_defender_count++;

            _combat_defender_player = _UNITS[itr_units].owner_idx;

            Load_Battle_Unit(itr_units, &battle_units[_combat_total_unit_count]);

            if(battle_units[_combat_total_unit_count].controller_idx == ST_UNDEFINED)
            {
                battle_units[_combat_total_unit_count].controller_idx = MOO_MONSTER_PLAYER_IDX;
            }

            _combat_total_unit_count++;
        }
    }


    for(itr = 0; itr < troop_count; itr++)
    {
        unit_idx = troops[itr];
        _combat_attacker_player = _UNITS[troops[0]].owner_idx;
        Load_Battle_Unit(unit_idx, &battle_units[_combat_total_unit_count]);
        _combat_total_unit_count++;
    }


    Init_Battlefield_Effects(combat_structure);


    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        BU_Apply_Battlefield_Effects__WIP(&battle_units[itr_battle_units]);
    }


    /*
        "Ocean Combat"
        "Only flying, sailing and swimming units may actually engage in combat over such squares."
    */
    _combat_attacker_count -= CMB_MarkLandLubbers(_combat_attacker_player);
    _combat_defender_count -= CMB_MarkLandLubbers(_combat_defender_player);


    IDK_health_attacker__2 = 0;
    IDK_health_defender__2 = 0;
    IDK_health_attacker = 0;
    IDK_health_defender = 0;
    IDK_melee_threat_attacker = 0;
    IDK_melee_threat_defender = 0;
    IDK_ranged_threat_attacker = 0;
    IDK_ranged_threat_defender = 0;
    IDK_damage_attacker = 0;
    IDK_damage_defender = 0;


    /*
        BEGIN:  ¿ WTF ?
    */
    if((_combat_attacker_player < HUMAN_PLAYER_IDX) || (_combat_attacker_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                battle_units[itr_battle_units].Status = 6;  /* Unit_Gone */
            }
        }
    }

    if((_combat_defender_player < HUMAN_PLAYER_IDX) || (_combat_defender_player > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            {
                battle_units[itr_battle_units].Status = 6;  /* Unit_Gone */
            }
        }
    }
    /*
        END:  ¿ WTF ?
    */


   for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
   {
        if(battle_units[itr_battle_units].Status == bus_Active)
        {
            Weights[itr_battle_units] = (30 - battle_units[itr_battle_units].defense);

            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                // Unit's Current Total Hit Points
                HP = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                IDK_health_attacker += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) == ST_TRUE)
                {
                    IDK_ranged_threat_attacker += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));
                }

                // {100, 101, 102, 103, 104, 105} srat_Thrown, srat_FireBreath, srat_Lightning, srat_StoneGaze, srat_MultiGaze, srat_DeathGaze
                // special ranged attack strength
                // Thrown, Breath, Gaze
                if(battle_units[itr_battle_units].ranged_type < srat_Thrown)
                {
                    special_ranged_attack_strength = 0;
                }
                else
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }

                IDK_melee_threat_attacker += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

                IDK_damage_attacker += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
            }
            else  /* battle_units[itr_battle_units].controller_idx == _combat_defender_player */
            {
                // Unit's Current Total Hit Points
                HP = ((battle_units[itr_battle_units].Cur_Figures * battle_units[itr_battle_units].hits) - battle_units[itr_battle_units].TopFig_Dmg);

                IDK_health_defender += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

                if(Battle_Unit_Has_Ranged_Attack(itr_battle_units) == ST_TRUE)
                {
                    IDK_ranged_threat_defender += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));
                }

                if(battle_units[itr_battle_units].ranged_type < srat_Thrown)
                {
                    special_ranged_attack_strength = 0;
                }
                else
                {
                    special_ranged_attack_strength = battle_units[itr_battle_units].ranged;
                }

                IDK_melee_threat_defender += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

                IDK_damage_defender += ((battle_units[itr_battle_units].hits * battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
            }  /* if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player) */
        }
    }



    /*
        BEGIN:  Mods - Mana / Spell-Ranks
    */

    if(_combat_attacker_player >= _num_players)  /* Neural Player or Monster */
    {
        attacker_mana_multiplied = 0;
    }
    else
    {
        attacker_mana_multiplied = ((10 * _players[_combat_attacker_player].mana_reserve) / Combat_Casting_Cost_Multiplier(_combat_attacker_player));

        if(_players[_combat_attacker_player].Nominal_Skill < attacker_mana_multiplied)
        {
            attacker_mana_multiplied = _players[_combat_attacker_player].Nominal_Skill;
        }

        attacker_third_nominal_skill = (_players[_combat_attacker_player].Nominal_Skill / 3);

        if(attacker_third_nominal_skill > attacker_mana_multiplied)
        {
            attacker_third_nominal_skill = attacker_mana_multiplied;
        }

        var_4C = 0;
        var_50 = 0;
        var_54 = 0;

        if(attacker_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_attacker_player].spellranks[0];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 50);
                var_50 = (spell_ranks * 10);
                var_54 = (spell_ranks * 25);
            }

            if(
                (combat_structure == 3)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 75);
                var_54 = (spell_ranks * 25);
            }
            
            // Chaos
            spell_ranks = _players[_combat_attacker_player].spellranks[2];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 100);
                var_50 = (spell_ranks * 30);
            }

            // Life
            spell_ranks = _players[_combat_attacker_player].spellranks[3];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_50 = (spell_ranks * 15);
                var_54 = (spell_ranks * 40);
            }

            // Death
            spell_ranks = _players[_combat_attacker_player].spellranks[4];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4C = (spell_ranks * 75);
                var_54 = (spell_ranks * 15);
            }

            var_4C = (var_4C / 10);
            var_50 = (var_50 / 10);
            var_54 = (var_54 / 20);

        }  /* if(attacker_mana_multiplied >= 10) */

    }  /* if(_combat_attacker_player >= _num_players) */

    if(_combat_defender_player >= _num_players)
    {
        defender_mana_multiplied = 0;
    }
    else
    {
        defender_mana_multiplied = ((10 * _players[_combat_defender_player].mana_reserve) / Combat_Casting_Cost_Multiplier(_combat_defender_player));

        if(_players[_combat_defender_player].Nominal_Skill < defender_mana_multiplied)
        {
            defender_mana_multiplied = _players[_combat_defender_player].Nominal_Skill;
        }

        defender_third_nominal_skill = (_players[_combat_defender_player].Nominal_Skill / 3);

        if(defender_third_nominal_skill > defender_mana_multiplied)
        {
            defender_third_nominal_skill = defender_mana_multiplied;
        }

        var_4E = 0;
        var_52 = 0;
        var_56 = 0;

        if(defender_mana_multiplied >= 10)
        {
            spell_ranks = _players[_combat_defender_player].spellranks[0];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 50);
                var_52 = (spell_ranks * 10);
                var_56 = (spell_ranks * 25);
            }

            if(
                (combat_structure == 3)
                ||
                (combat_structure == 4)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 75);
                var_56 = (spell_ranks * 25);
            }
            
            // Chaos
            spell_ranks = _players[_combat_defender_player].spellranks[2];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 100);
                var_52 = (spell_ranks * 30);
            }

            // Life
            spell_ranks = _players[_combat_defender_player].spellranks[3];

            if(
                (combat_structure == 2)
                ||
                (combat_structure == 4)
                ||
                (combat_structure == 3)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_52 = (spell_ranks * 15);
                var_56 = (spell_ranks * 40);
            }

            // Death
            spell_ranks = _players[_combat_defender_player].spellranks[4];

            if(
                (combat_structure == cs_SorceryNode)
                ||
                (combat_structure == cs_ChaosNode)
                ||
                (combat_structure == cs_NatureNode)
            )
            {
                spell_ranks = 0;
            }

            if(spell_ranks > 0)
            {
                var_4E = (spell_ranks * 75);
                var_56 = (spell_ranks * 15);
            }

            var_4E = (var_4C / 10);
            var_52 = (var_50 / 10);
            var_56 = (var_54 / 20);

        }
    }  /* if(_combat_defender_player >= _num_players) */

    /*
        END:  Mods - Mana / Spell-Ranks
    */


    IDK_health_attacker__2 = IDK_health_attacker;
    IDK_health_defender__2 = IDK_health_defender;
    /* DEMOHACK */ if (OVL_Action_Type == 5) { IDK_health_attacker__2 = IDK_health_attacker = 0b01111111111111111111111111111111; }

    /*
        BEGIN:  ¿ halve values for Neutral Player ?
    */

    // BUGBUG: should be ||? either or both
    if(
        (_combat_attacker_player != HUMAN_PLAYER_IDX)
        &&
        (_combat_defender_player != HUMAN_PLAYER_IDX)
    )
    {
        if(_combat_attacker_player != NEUTRAL_PLAYER_IDX)
        {
            var_5C = (var_5C / 2);
            var_64 = (var_64 / 2);
            var_60 = (var_60 / 2);
            IDK_health_attacker__2 = (IDK_health_attacker__2 / 2);
            IDK_health_attacker = (IDK_health_attacker / 2);
            IDK_melee_threat_attacker = (IDK_melee_threat_attacker / 2);
            IDK_ranged_threat_attacker = (IDK_ranged_threat_attacker / 2);
            var_4C = (var_4C / 2);
            var_50 = (var_50 / 2);
            var_54 = (var_54 / 2);
        }
        if(_combat_attacker_player != NEUTRAL_PLAYER_IDX)
        {
            var_5E = (var_5E / 2);
            var_66 = (var_66 / 2);
            var_62 = (var_62 / 2);
            IDK_health_defender__2 = (IDK_health_defender__2 / 2);
            IDK_health_defender = (IDK_health_defender / 2);
            IDK_melee_threat_defender = (IDK_melee_threat_defender / 2);
            IDK_ranged_threat_defender = (IDK_ranged_threat_defender / 2);
            var_4E = (var_4E / 2);
            var_52 = (var_52 / 2);
            var_56 = (var_56 / 2);
        }
    }

    /*
        END:  ¿ halve values for Neutral Player ?
    */


    /*
        BEGIN:  Combat Turn
    */

    for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++)
    {
        if((IDK_ranged_threat_attacker > 10) || (IDK_ranged_threat_defender > 10))
        {
            if((IDK_health_attacker > 0) && (IDK_health_defender > 0))
            {
                if(attacker_mana_multiplied > 10)
                {
                    var_64 = ((var_4C * attacker_third_nominal_skill) / 5);
                    var_5C = ((var_50 * attacker_third_nominal_skill) / 5);
                    var_60 = ((var_54 * attacker_third_nominal_skill) / 5);

                    var_64 += ((IDK_ranged_threat_attacker * var_5C) / 100);

                    var_54 = (var_54 / 2);

                    _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);

                    attacker_mana_multiplied -= attacker_third_nominal_skill;

                    if(attacker_third_nominal_skill < attacker_mana_multiplied)
                    {
                        attacker_third_nominal_skill = attacker_mana_multiplied;
                    }
                }
                else
                {
                    var_5C = 0;
                    var_64 = 0;
                    var_60 = 0;
                }

                if(defender_mana_multiplied > 10)
                {
                    var_66 = ((var_4E * defender_third_nominal_skill) / 5);
                    var_5E = ((var_52 * defender_third_nominal_skill) / 5);
                    var_62 = ((var_56 * defender_third_nominal_skill) / 5);

                    var_66 += ((IDK_ranged_threat_defender * var_5E) / 100);

                    var_56 = (var_56 / 2);

                    _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);

                    defender_mana_multiplied -= defender_third_nominal_skill;

                    if(defender_third_nominal_skill < defender_mana_multiplied)
                    {
                        defender_third_nominal_skill = defender_mana_multiplied;
                    }
                }
                else
                {
                    var_5E = 0;
                    var_66 = 0;
                    var_62 = 0;
                }


                var_36 = (((IDK_ranged_threat_attacker + var_64) * Random(10)) / 100);
                var_3A = (((IDK_ranged_threat_defender + var_66) * Random(10)) / 100);

                IDK_health_attacker += var_60;
                IDK_health_defender += var_62;

                IDK_ranged_threat_attacker -= ((var_3A * IDK_ranged_threat_attacker) / IDK_health_attacker);
                IDK_ranged_threat_defender -= ((var_36 * IDK_ranged_threat_defender) / IDK_health_defender);

                IDK_melee_threat_attacker -= ((var_3A * IDK_melee_threat_attacker) / IDK_health_attacker);
                IDK_melee_threat_defender -= ((var_36 * IDK_melee_threat_defender) / IDK_health_defender);

                IDK_health_attacker -= var_3A;
                IDK_health_defender -= var_36;

                if(IDK_ranged_threat_attacker < 0)
                {
                    IDK_ranged_threat_attacker = 0;
                }

                if(IDK_ranged_threat_defender < 0)
                {
                    IDK_ranged_threat_defender = 0;
                }

            }
        }

    }  /* for(itr_combat_turns = 0; itr_combat_turns < 3; itr_combat_turns++) */


    while((IDK_melee_threat_attacker > 10) && (IDK_melee_threat_defender > 10))
    {
        if(attacker_mana_multiplied <= 10)
        {
            var_5C = 0;
            var_64 = 0;
            var_60 = 0;  // ¿ ~== healing ?
        }
        else
        {
            var_64 = ((var_4C * attacker_third_nominal_skill) / 5);
            var_5C = ((var_50 * attacker_third_nominal_skill) / 5);
            var_60 = ((var_50 * attacker_third_nominal_skill) / 5);  // ¿ ~== healing ?
            var_64 += ((IDK_ranged_threat_attacker * var_5C) / 100);
            _players[_combat_attacker_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_attacker_player) * attacker_third_nominal_skill) / 10);
            attacker_mana_multiplied -= (attacker_third_nominal_skill / 2);
            if((attacker_mana_multiplied / 2) < attacker_third_nominal_skill)
            {
                attacker_third_nominal_skill = (attacker_mana_multiplied / 2);
            }
        }

        if(defender_mana_multiplied <= 10)
        {
            var_5E = 0;
            var_66 = 0;
            var_62 = 0;  // ¿ ~== healing ?
        }
        else
        {
            var_66 = ((var_4E * defender_third_nominal_skill) / 5);
            var_5E = ((var_52 * defender_third_nominal_skill) / 5);
            var_62 = ((var_52 * defender_third_nominal_skill) / 5);  // ¿ ~== healing ?
            var_66 += ((IDK_ranged_threat_defender * var_5E) / 100);
            _players[_combat_defender_player].mana_reserve -= ((Combat_Casting_Cost_Multiplier(_combat_defender_player) * defender_third_nominal_skill) / 10);
            defender_mana_multiplied -= (defender_third_nominal_skill / 2);
            if((defender_mana_multiplied / 2) < defender_third_nominal_skill)
            {
                defender_third_nominal_skill = (defender_mana_multiplied / 2);
            }
        }

        var_36 = (((IDK_melee_threat_attacker + var_64) * Random(10)) / 100);
        var_3A = (((IDK_melee_threat_defender + var_66) * Random(10)) / 100);

        IDK_health_attacker += var_60;  // ¿ ~== healing ?
        IDK_health_defender += var_62;  // ¿ ~== healing ?

        IDK_melee_threat_attacker -= ((IDK_melee_threat_attacker * var_3A) / IDK_health_attacker);
        IDK_melee_threat_defender -= ((IDK_melee_threat_defender * var_36) / IDK_health_defender);

        IDK_health_attacker -= var_3A;
        IDK_health_defender -= var_36;

    }  /* while((IDK_melee_threat_attacker > 10) & (IDK_melee_threat_defender > 10)) */

    /*
        END:  Combat Turn
    */


    if(IDK_melee_threat_attacker > 10)
    {
        winner_player_idx = _combat_attacker_player;

        var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);

        var_42 = ((var_44 * IDK_damage_attacker) / 100);

        var_40 = IDK_damage_attacker;
    }

    // BUGBUG: duplicated code?
    if(IDK_melee_threat_attacker > 10)
    {
        winner_player_idx = _combat_attacker_player;
        var_44 = ((100 * IDK_health_attacker) / IDK_health_attacker__2);
        var_42 = ((var_44 * IDK_damage_attacker) / 100);
        var_40 = IDK_damage_attacker;
    }
    else
    {
        if(IDK_melee_threat_defender > 10)
        {
            winner_player_idx = _combat_defender_player;
            var_44 = ((100 * IDK_health_defender) / IDK_health_defender__2);
            var_42 = ((var_44 * IDK_damage_defender) / 100);
            var_40 = IDK_damage_defender;
        }
    }


    for(itr = 0; itr < _combat_total_unit_count; itr++)
    {
        if(battle_units[itr].controller_idx != winner_player_idx)
        {
            battle_units[itr].Status = bus_Dead;  /* Unit_Dead */
            Weights[itr] = 0;
        }
    }


    Dmg_Array[0] = 3;
    Dmg_Array[1] = 0;
    Dmg_Array[2] = 0;

    if(winner_player_idx != ST_UNDEFINED)
    {
        MsgType = ST_UNDEFINED;

        while(var_40 > var_42)
        {
            if(MsgType <= ST_UNDEFINED)
            {
                // ; chooses a random item from a list of 16bit weighted
                // ; chances; condensing the weights such that the total
                // ; fits into a single call of the 9-bit RNG (max 512),
                // ; using repeated divisions by 2 if necessary
                BU_Index = Get_Weighted_Choice(&Weights[0], _combat_total_unit_count);
            }
            else
            {
                BU_Index = MsgType;
            }

            Weights[BU_Index] += 50;

            // ; applies the damage points passed through the array
            // ; to the target battle unit, reducing its figure count
            // ; or marking it dead as necessary (in which case the
            // ; combat victor and true sights are also updated)
            BU_ApplyDamage(BU_Index, &Dmg_Array[0]);

            if(battle_units[BU_Index].Status <= 0)
            {
                MsgType = BU_Index;
            }
            else
            {
                Weights[BU_Index] = 0;
                MsgType = ST_UNDEFINED;
            }

            var_40 -= 3;
        }
    }


    if(winner_player_idx == HUMAN_PLAYER_IDX)
    {
        MsgType = 6;  /* ¿ cmbmsg_DefVictory ? */
    }
    else
    {
        MsgType = 7;  /* ¿ cmbmsg_DefDefeat ?*/
    }



    End_Of_Combat__WIP(winner_player_idx, item_count, item_list, MsgType);



    if(winner_player_idx == _combat_attacker_player)
    {
        did_win = ST_TRUE;
    }
    else
    {
        did_win = ST_FALSE;
    }


    return did_win;
}


// WZD o110p03
// drake178: BU_Heal()

// WZD o110p04
/*
; assigns and returns an arbitrary effective health
; value based on the passed defense and hit points:
;   defense 1:           hp*18/10        (*1.8)
;   defense 2:           hp*21/10        (*2.1)
;   defense 3:           hp*24/10        (*2.4)
;   defense 4+:          hp*(def-2)*(def-2)*3/2
;     (hp*6) - (hp*13.5) - (hp*24) - (hp*37.5)...

Strategic_Combat()
    |-> HP = ((battle_units[itr_battle_units].hits / battle_units[itr_battle_units].Cur_Figures) - battle_units[itr_battle_units].TopFig_Dmg);
    |-> IDK_health_attacker += Get_Effective_Hits(HP, battle_units[itr_battle_units].defense);

HP = ((battle_units[].hits / battle_units[].Cur_Figures) - battle_units[].TopFig_Dmg);

battle_units[].defense = ¿ unit_type_table[_UNITS[].type].defense ?

*/
int16_t Get_Effective_Hits(int16_t hits, int16_t defense)
{
    int16_t effective_hits;  // _SI_

    effective_hits = (hits * 3);

    if(defense > 3)
    {
        effective_hits = (effective_hits * (((defense + -2) * (defense + -2)) / 2));
        // 4:  ((4 - 2) * (4 - 2)) = (2 * 2) =  4 / 2 = 2  200%
        // 5:  ((5 - 2) * (5 - 2)) = (3 * 3) =  9 / 2 = 4  400%
        // 6:  ((6 - 2) * (6 - 2)) = (4 * 4) = 16 / 2 = 8  800%
    }
    else
    {
        switch(defense)
        {
            case 0:
            {
                effective_hits = (effective_hits / 2);  // 50%
            } break;
            case 1:
            {
                effective_hits = ((effective_hits * 3) / 5);  // 60%
            } break;
            case 2:
            {
                effective_hits = ((effective_hits * 7) / 15);  // 70%
            } break;
            case 3:
            {
                effective_hits = ((effective_hits * 4) / 5);  // 80%
            } break;
        }
    }

    return effective_hits;
}

// WZD o110p05
/*

; assigns and returns an arbitrary threat value for the passed ranged combat attributes:
;   ranged 1:      figs*40/7    (*5.7)
;   ranged 2:      figs*40/3   (*13.3)
;   ranged 3:      figs*80/3   (*26.6)
;   ranged 4+:     figs*20*(m-2) (*40)-(*60)...
;   doom:          total*2
;   ap:            total*5/4 (unless doom)
;   touch:         +figs*60 each
;   illusion:      total*5
;   eldritch w:    total*5/4

Strategic_Combat()
    |-> IDK_ranged_threat_defender += Get_Effective_Ranged_Strength(battle_units[itr_battle_units].ranged, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Ranged_ATK_Flags));

battle_units[].ranged
battle_units[].Cur_Figures
(battle_units[].Attack_Flags | battle_units[].Ranged_ATK_Flags)

*/
int16_t Get_Effective_Ranged_Strength(int16_t ranged, int16_t figures, int16_t attack_flags)
{
    int16_t ranged_threat;  // _CX_

    if(ranged == 0)
    {
        return 0;
    }

    if(ranged > 3)
    {
        ranged_threat = ((figures * 20) * (ranged + -2));
    }
    else
    {
        ranged_threat = (figures * 40);

        if(ranged == 1)
        {
            ranged_threat = (ranged_threat / 7);
        }
        else if(ranged == 2)
        {
            ranged_threat = (ranged_threat / 7);
        }
        else if(ranged == 3)
        {
            ranged_threat = (ranged_threat / 7);
        }
    }

    if(attack_flags != 0)
    {
        if((attack_flags & Att_DoomDmg) != 0)
        {
            ranged_threat = (ranged_threat * 2);  // 200%
        }
        else if((attack_flags & Att_ArmorPrc) != 0)
        {
            ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        }

        if((attack_flags & Att_Poison) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_LifeSteal) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_Destruct) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_StnTouch) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_DthTouch) != 0)
        {
            ranged_threat = (ranged_threat + (figures * 60));
        }

        if((attack_flags & Att_Illusion) != 0)
        {
            ranged_threat = (ranged_threat * 5);  // 500%
        }

        // DNE  if((attack_flags & Att_1stStrike) != 0)
        // DNE  {
        // DNE      ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        // DNE  }

        if((attack_flags & Att_EldrWeap) != 0)
        {
            ranged_threat = ((ranged_threat * 5) / 4);  // 125%
        }
    }

    // DNE if(ranged_type >= srat_StoneGaze)
    // DNE {
    // DNE     melee_threat = (melee_threat + 300);
    // DNE     if(ranged_type == srat_MultiGaze)
    // DNE     {
    // DNE         melee_threat = (melee_threat + 900);
    // DNE     }
    // DNE }

    return ranged_threat;
}

// WZD o110p06
// ¿ ~ MoO2  Module: CMBTAI  Expected_Weapon_Damage_(); Get_Effective_Missile_Strength_(); Get_Effective_Beam_Strength_() ?
/*
; assigns and returns an arbitrary threat value for the passed melee combat attributes:
;   melee 1:       figs*40/7    (*5.7)
;   melee 2:       figs*40/3   (*13.3)
;   melee 3:       figs*80/3   (*26.6)
;   melee 4+:      figs*20*(m-2) (*40)-(*60)...
;   short range:   +same*1.5
;   doom:          total*2
;   ap:            total*5/4 (unless doom)
;   touch:         +figs*60 each
;   illusion:      total*5
;   1st strike:    total*5/4
;   eldritch w:    total*5/4
;   gaze:          +300
;   multi-gaze:    +900

Strategic_Combat()
    |-> IDK_melee_threat_attacker += Get_Effective_Melee_Strength(battle_units[itr_battle_units].melee, special_ranged_attack_strength, battle_units[itr_battle_units].Cur_Figures, (battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags), battle_units[itr_battle_units].ranged_type);

battle_units[itr_battle_units].melee
special_ranged_attack_strength
battle_units[itr_battle_units].Cur_Figures
(battle_units[itr_battle_units].Attack_Flags | battle_units[itr_battle_units].Melee_ATK_Flags)
battle_units[itr_battle_units].ranged_type

¿ Calc, Mods, Effects, etc. ?
    battle_units[].melee = ¿ unit_type_table[_UNITS[].type].melee ?
    thrown = ¿ unit_type_table[_UNITS[].type].ranged ?
    battle_units[].Cur_Figures
    (battle_units[].Attack_Flags | battle_units[].Melee_ATK_Flags)
    battle_units[].ranged_type

"thrown" ~== 'Special Ranged Attack Strength' ~== {Thrown, Breath, Gaze}

*/
int16_t Get_Effective_Melee_Strength(int16_t melee, int16_t thrown, int16_t figures, int16_t attack_flags, int16_t ranged_type)
{
    int16_t melee_threat;  // _CX_
    int16_t thrown_threat;  // _DI_

    if(melee == 0)
    {
        return 0;
    }

    if(melee > 3)
    {
        melee_threat = ((figures * 20) * (melee + -2));
    }
    else
    {
        melee_threat = (figures * 40);
        switch(melee)
        {
            case 0:
            {
                melee_threat = 0;
            } break;
            case 1:
            {
                melee_threat = (melee_threat / 7);
            } break;
            case 2:
            {
                melee_threat = (melee_threat / 3);
            } break;
            case 3:
            {
                melee_threat = ((melee_threat * 2) / 3);
            } break;
        }
    }

    thrown_threat = 0;
    if(thrown > 0)
    {
        if(thrown > 3)
        {
            thrown_threat = ((figures * 30) * (thrown + -2));
        }
        else
        {
            thrown_threat = (figures * 60);
            switch(thrown)
            {
                case 0:
                {
                    thrown_threat = 0;
                } break;
                case 1:
                {
                    thrown_threat = (thrown_threat / 7);
                } break;
                case 2:
                {
                    thrown_threat = (thrown_threat / 3);
                } break;
                case 3:
                {
                    melee_threat = ((melee_threat * 2) / 3);
                } break;
            }
        }
    }
    melee_threat += thrown_threat;  // BUG:  += here should just be inside the if > 0

    if(attack_flags != 0)
    {
        if((attack_flags & Att_DoomDmg) != 0)
        {
            melee_threat = (melee_threat * 2);
        }
        else if((attack_flags & Att_ArmorPrc) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }

        if((attack_flags & Att_Poison) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_LifeSteal) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_Destruct) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_StnTouch) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_DthTouch) != 0)
        {
            melee_threat = (melee_threat + (figures * 60));
        }

        if((attack_flags & Att_Illusion) != 0)
        {
            melee_threat = (melee_threat * 5);
        }

        if((attack_flags & Att_1stStrike) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }

        if((attack_flags & Att_EldrWeap) != 0)
        {
            melee_threat = ((melee_threat * 5) / 4);
        }
    }

    if(ranged_type >= srat_StoneGaze)
    {
        melee_threat = (melee_threat + 300);
        if(ranged_type == srat_MultiGaze)
        {
            melee_threat = (melee_threat + 900);
        }
    }

    return melee_threat;
}


/*
    WIZARDS.EXE  ovr112
*/

// WZD o112p01
// CMB_ComposeBookBG()

// WZD o112p02
// CMB_RedrawSpellbook()

// WZD o112p03
// G_CMB_CastSpell()

// WZD o112p04
// CMB_ShowSpellbook()

// WZD o112p05
// CMB_CheckSpellErrors()

// WZD o112p06
// SPL_GetResistMod()

// WZD o112p07
/*
    combat casting cost multiplier
    Cost for Magic Reserve, not Casting Skill

¿ maximum distance is 30, because world is 60 and it wraps ?
¿ 30 is 300% AKA 3x (*3.0) ?
¿ Neutral Player is hard-coded to 10% ?
*/
int16_t Combat_Casting_Cost_Multiplier(int16_t player_idx)
{
    int16_t Y_Distance;
    int16_t modifier;  // _SI_
    int16_t X_Distance;  // _SI_  DNE in Dasm
    int16_t distance;  // _DI_

    if(player_idx == NEUTRAL_PLAYER_IDX)
    {
        modifier = 1;
    }
    else
    {

        if(_FORTRESSES[player_idx].wp != OVL_Action_Plane)
        {
            distance = (WORLD_WIDTH / 2);
        }
        else
        {
            Y_Distance = abs(_FORTRESSES[player_idx].wy - OVL_Action_YPos);

            X_Distance = abs(_FORTRESSES[player_idx].wx - OVL_Action_XPos);

            if(X_Distance > (WORLD_WIDTH / 2))
            {
                X_Distance = (WORLD_WIDTH - X_Distance);
            }

            if(X_Distance > (WORLD_WIDTH / 2))
            {
                X_Distance = (WORLD_WIDTH - X_Distance);
            }

            if(X_Distance < Y_Distance)
            {
                distance = X_Distance;
            }
            else
            {
                distance = Y_Distance;
            }
        }

        if(distance == 0)
        {
            modifier = 5;
        }
        else
        {

            if(_players[player_idx].channeler > 0)
            {
                distance = 2;
            }

            modifier = 10;

            if(distance > 5)
            {
                modifier += 5;
            }

            if(distance > 10)
            {
                modifier += 5;
            }

            if(distance > 15)
            {
                modifier += 5;
            }

            if(distance > 20)
            {
                modifier += 5;
            }

        }

    }

    return modifier;
}


// WZD o112p08
// AITP_EarthToMud()

// WZD o112p09
// AITP_Disrupt()

// WZD o112p10
// AITP_CracksCall()

// WZD o112p11
// UU_AITP_WordofRecall()

// WZD o112p12
// AITP_RecallHero()



/*
    WIZARDS.EXE  ovr113
*/

// WZD o113p01
// CMB_SetTargetCursor()

// WZD o113p02
// CMB_DrawTargetScreen()

// WZD o113p03
// CMB_TargetSpell()

// WZD o113p04
// CMB_RangedAnim()

// WZD o113p05
// CMB_MeleeAnim()

// WZD o113p06
// CMB_ConvSpellAttack()

// WZD o113p07
void BU_ApplyDamage(int16_t battle_unit_idx, int16_t Dmg_Array[])
{
    int16_t Figures_Lost;
    int16_t Total_Damage;
    int16_t itr;  // _DI_


    Total_Damage = 0;

    for(itr = 0; itr < 3; itr++)
    {
        Total_Damage += Dmg_Array[itr];
    }

    if(
        (Total_Damage > 0)
        &&
        (battle_units[battle_unit_idx].Status == 0)  /* Unit_Active */
    )
    {
        for(itr = 0; itr < 3; itr++)
        {
            if((battle_units[battle_unit_idx].damage[itr] + Dmg_Array[itr]) <= 200)
            {
                battle_units[battle_unit_idx].damage[itr] += Dmg_Array[itr];
            }
            else
            {
                battle_units[battle_unit_idx].damage[itr] = 200;
            }
        }
    }

    Total_Damage += battle_units[battle_unit_idx].TopFig_Dmg;

    if(Total_Damage > 0)
    {
        Figures_Lost = (Total_Damage / battle_units[battle_unit_idx].hits);

        if(battle_units[battle_unit_idx].Cur_Figures < Figures_Lost)
        {
            Figures_Lost = battle_units[battle_unit_idx].Cur_Figures;
        }

        battle_units[battle_unit_idx].Cur_Figures -= Figures_Lost;

        battle_units[battle_unit_idx].TopFig_Dmg = (Total_Damage % battle_units[battle_unit_idx].hits);

    }

    if(battle_units[battle_unit_idx].Cur_Figures <= 0)
    {
        battle_units[battle_unit_idx].Cur_Figures = 0;

        CMB_Winner = Check_For_Winner();

        if(BU_IsCombatSummon(battle_unit_idx) == ST_TRUE)
        {
            _UNITS[battle_units[battle_unit_idx].unit_idx].wp = 9;
        }

        if(
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[1])
            ||
            (battle_units[battle_unit_idx].damage[2] >= battle_units[battle_unit_idx].damage[0])
        )
        {
            battle_units[battle_unit_idx].Status = 6;  /* Unit_Gone */
        }
        else
        {
            if(
                (battle_units[battle_unit_idx].damage[1] > battle_units[battle_unit_idx].damage[2])
                ||
                (battle_units[battle_unit_idx].damage[1] >= battle_units[battle_unit_idx].damage[0])
            )
            {
                if(_UNITS[battle_units[battle_unit_idx].unit_idx].wp != 9)
                {
                    battle_units[battle_unit_idx].Status = 5;  /* Unit_Drained */
                }
                else
                {
                    battle_units[battle_unit_idx].Status = 6;  /* Unit_Gone */
                }
            }
            else
            {
                if(
                    (battle_units[battle_unit_idx].damage[0] > battle_units[battle_unit_idx].damage[2])
                    &&
                    (battle_units[battle_unit_idx].damage[0] > battle_units[battle_unit_idx].damage[1])
                )
                {
                    battle_units[battle_unit_idx].Status = 4;  /* Unit_Dead */
                }
            }
        }

        CMB_UpdateTrueSight();
    }

}


// WZD o113p08
// BU_RangedValidate()

// WZD o113p09
// CMB_PlaceArmy()

// WZD o113p10
// BU_SummonDemon()

// WZD o113p11
// BU_MeleeWallCheck()

// WZD o113p12
// CMB_ManaLeak()

// WZD o113p13
// GUI_DrawNearMessage()

// WZD o113p14
// CMB_SpellcastMessage()

// WZD o113p15
// CMB_FindEmptyFigSlot()

// WZD o113p16
// MoO2  Module: COMBAT1  Check_For_Winner_()
/*
    returns player_idx or ST_UNDEFINED
*/
int16_t Check_For_Winner(void)
{
    int16_t attacker_count;  // _SI_
    int16_t defender_count;  // _DI_
    int16_t itr_battle_units;  // _CX_
    int16_t winner_player_idx;  // DNE in Dasm

    attacker_count = 0;
    defender_count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(battle_units[itr_battle_units].Status = 0)  /* Unit_Active */
        {
            if(battle_units[itr_battle_units].controller_idx == _combat_attacker_player)
            {
                if(battle_units[itr_battle_units].Confusion_State != 2)  /* ¿ ? */
                {
                    attacker_count++;
                }
                else
                {
                    defender_count++;
                }
            }
            else
            {
                if(battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                {
                    if(battle_units[itr_battle_units].Confusion_State != 2)  /* ¿ ? */
                    {
                        defender_count++;
                    }
                    else
                    {
                        attacker_count++;
                    }
                }
            }
        }
    }

    if(attacker_count == 0)
    {
        winner_player_idx = _combat_defender_player;
    }
    else if(defender_count == 0)
    {
        winner_player_idx = _combat_attacker_player;
    }
    else
    {
        winner_player_idx = ST_UNDEFINED;
    }

    return winner_player_idx;
}



/*
    WIZARDS.EXE  ovr122
*/

// WZD o122p01
// CMB_AttackRoll()

// WZD o122p02
// CMB_DefenseRoll()

// WZD o122p03
// drake178: BU_ResistRoll()
/*
; calculates the unit's effective Resistance score,
; then makes a resistance check with the passed
; modifier, returning 0 if the unit succeeded, or the
; difference from the target number if it didn't
*/
int16_t BU_ResistRoll__STUB(struct s_BATTLE_UNIT battle_unit, int16_t Save_Mod, int16_t magic_realm)
{

    return 0;

}


// WZD o122p04
// BU_GetEffectiveRES()

// WZD o122p05
// BU_GetATKImmFlags()

// WZD o122p06
// BU_AttackTarget()

// WZD o122p07
// BU_ProcessAttack()

// WZD o122p08
// BU_GetEffectiveDEF()

// WZD o122p09
// BU_GetATKRealm()

// WZD o122p10
/*
    ¿ only applies to city combat ?
    ~== MoO Combat at Colony, where Colony is owned by Defender



*/
/*
; calculates and stores the highest unit aura values
; for each player (Holy Bonus, Resistance to All, and
; Leadership), and applies Heavenly Light, Cloud of
; Shadow, and Eternal Night
;
; BUG: grants +3 Defense to defending units in battle
; condition 1 (else wouldn't even need this argument)
; Attributes: bp-based frame
*/
// ¿ MoO2  Module: COMBINIT  Load_Colony_Defense_() |-> Load_Tactical_Colony_() ... Load_Combat_Satelite_()
/*
    combat_enchantments, CMB_HolyBonusArray, CMB_ResistAllArray, CMB_LeadershipArray, battle_units[].defense
    combat_enchantments - Darkness, True_Light
    +3 Defender defense, if combat_structure == 1
    Hero - Prayer Master
    Hero - Leadership
    CMB_ResistAllArray += CMB_HolyBonusArray
*/
void Init_Battlefield_Effects(int16_t combat_structure)
{
    int16_t Leadership_Value;
    int16_t Prayermaster_Bonus;
    int16_t itr_players;  // _SI_
    int16_t itr_arrays;  // _SI_
    int16_t itr_battle_units;  // _SI_
    int16_t battle_unit_owner_idx;  // _DI_

// WTFmate:  Per the Dasm, battlefield is not even allocated for Strategic_Combat()  (and, I can't find where it's populated, in either case)
// IDGI      if(battlefield->City_Enchants->Cloud_of_Shadow > 0)
// IDGI      {
// IDGI          combat_enchantments->Darkness.Dfndr = 2;
// IDGI      }
// IDGI  
// IDGI      if(battlefield->City_Enchants->Heavenly_Light > 0)
// IDGI      {
// IDGI          combat_enchantments->True_Light.Dfndr = 2;
// IDGI      }

    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {

        if(_players[itr_players].Globals[ETERNAL_NIGHT] > 0)
        {
            if(combat_enchantments->True_Light.Dfndr != 2)
            {
                if(itr_players == _combat_attacker_player)
                {
                    combat_enchantments->Darkness.Attkr = 3;
                }
                else if(itr_players == _combat_defender_player)
                {
                    combat_enchantments->Darkness.Dfndr = 3;
                }
                else
                {
                    combat_enchantments->Darkness.Dfndr = 3;
                    combat_enchantments->Darkness.Attkr = 3;
                }
            }
        }
    }

    for(itr_arrays = 0; itr_arrays < 7; itr_arrays++)
    {
        CMB_HolyBonusArray[itr_arrays] = 0;
        CMB_ResistAllArray[itr_arrays] = 0;
        CMB_LeadershipArray[itr_arrays] = 0;
    }


    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        Leadership_Value = 0;

        if(battle_units[itr_battle_units].Status = 0) /* Unit_Active */
        {

            // BUGBUG: should be checking for City Wall
            if(
                (combat_structure = cs_City)  /* probably just {F,T} */
                &&
                (battle_units[itr_battle_units].controller_idx == _combat_defender_player)
            )
            {
                battle_units[itr_battle_units].defense += 3;
            }

            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;

            if((battle_units[itr_battle_units].Attribs_2 & USA_HOLYBONUS) !=0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > CMB_HolyBonusArray[battle_unit_owner_idx])
                {
                    CMB_HolyBonusArray[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }

            if((battle_units[itr_battle_units].Attribs_2 & USA_RESISTALL) !=0)
            {
                // DEDU  what is it testing/setting here? what's in Spec_Att_Attrib, how'd it get there - memcpy?
                if(battle_units[itr_battle_units].Spec_Att_Attrib > CMB_ResistAllArray[battle_unit_owner_idx])
                {
                    CMB_ResistAllArray[battle_unit_owner_idx] = battle_units[itr_battle_units].Spec_Att_Attrib;
                }
            }

            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot >= 0)
            {

                // Prayermaster
                if(HERO_PRAYERMASTER(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Prayermaster_Bonus = (_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1);
                    if(Prayermaster_Bonus > CMB_ResistAllArray[battle_unit_owner_idx])
                    {
                        CMB_ResistAllArray[battle_unit_owner_idx] = Prayermaster_Bonus;
                    }
                }
                if(HERO_PRAYERMASTER2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Prayermaster_Bonus = (((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) * 3) / 2);
                    if(Prayermaster_Bonus > CMB_ResistAllArray[battle_unit_owner_idx])
                    {
                        CMB_ResistAllArray[battle_unit_owner_idx] = Prayermaster_Bonus;
                    }
                }

                // Leadership
                if(HERO_LEADERSHIP(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Leadership_Value = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 3);
                    if(Leadership_Value > CMB_LeadershipArray[battle_unit_owner_idx])
                    {
                        CMB_LeadershipArray[battle_unit_owner_idx] = Leadership_Value;
                    }
                }
                if(HERO_LEADERSHIP2(battle_unit_owner_idx, _UNITS[battle_units[itr_battle_units].unit_idx].type))
                {
                    Leadership_Value = ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                    if(Leadership_Value > CMB_LeadershipArray[battle_unit_owner_idx])
                    {
                        CMB_LeadershipArray[battle_unit_owner_idx] = Leadership_Value;
                    }
                }

            }

        }
        
    }


    for(itr_players = 0; itr_players < _num_players; itr_players++)
    {
        CMB_ResistAllArray[itr_players] += CMB_HolyBonusArray[itr_players];
    }

}


// WZD o122p11
// BU_Attack()

// WZD o122p12
// BU_GetDistanceFrom()

// WZD o122p13
// AI_BU_GetAttackValue()

// WZD o122p14
int16_t Battle_Unit_Has_Ranged_Attack(int16_t battle_unit_idx)
{
    int16_t is_ranged;  // _SI_

    is_ranged = ST_FALSE;

    if(
        (battle_units[battle_unit_idx].ranged_type > rat_None)
        &&
        (battle_units[battle_unit_idx].ranged_type < srat_Thrown)
    )
    {
        is_ranged = ST_TRUE;
    }

    return is_ranged;
}


// WZD o122p15
// BU_CauseFear()



/*

    WIZARDS.EXE ovr123

*/

// WZD o123p01
// MoO2  Module: COMBINIT  End_Of_Combat_()
/*
    "WIP", cause CMB_CreateUndeadAnim(ut_Zombies) and CMB_CreateUndeadAnim(CMB_GetUndeadCreator(player_idx))

    // ; finishes the active combat, processing all related
    // ; events and effects - including the display of the
    // ; summary scroll and undead creation animations if applicable
    End_Of_Combat(winner_player_idx, item_count, item_list, MsgType);

Diplomatic_Value:
    starts at 0
    adds 20 for Hero Unit
    or Random(20) for Non-Hero Unit and Random(10) for Engineer or Settler
    passes negated value to G_DIPL_Action()
*/
void End_Of_Combat__WIP(int16_t player_idx, int16_t * item_count, int16_t item_list[], int16_t MsgType)
{
    int16_t Buildings_Lost[36];
    int16_t Rare_Foe_Defeated;
    int16_t Diplomatic_Value;
    int16_t Summoned_Unit;
    int16_t Population_Loss_Percent;
    int16_t Destruction_Chance;
    int16_t No_Secondaries;
    int16_t BU_CombatHits;
    char Conv_String[10];
    int16_t Undead_Created;
    int16_t Zombies_Raised;
    int16_t Experience_Gained;
    int16_t Surviving_Unit_Count;
    int16_t battle_unit_owner_idx;
    uint32_t enchantments;
    int16_t itr_battle_units;  // _SI_
    int16_t RazeCity;  // _SI_
    int16_t itr_buildings;  // _SI_
    int16_t itr_population;  // _SI_
    int16_t itr_bldg_msg;  // _SI_
    int16_t IDK_population_lost;  // _DI_
    int16_t bldg_msg_idx;  // _DI_
    int16_t itr_hero_items;  // _DI_

    Rare_Foe_Defeated = 0;

    CMB_ScrollMsg_Type = MsgType;

    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;
    CMB_Gold_Reward = 0;

    // ~ Monsters
    if((player_idx < HUMAN_PLAYER_IDX) || (player_idx > NEUTRAL_PLAYER_IDX))
    {
        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                battle_units[itr_battle_units].Status = bus_Gone;
            }
        }
    }

    *item_count = 0;

    Experience_Gained = 0;
    Zombies_Raised = 0;
    Undead_Created = 0;
    No_Secondaries = ST_FALSE;


    // Lair Guard nibbles
    if(OVL_Action_Type == 5)  /* Lair */
    {
        _LAIRS[OVL_Action_Structure].guard1_count = (_LAIRS[OVL_Action_Structure].guard1_count & 0xF0);  // clear low-nibble
        if((_LAIRS[OVL_Action_Structure].guard2_count & 0x0F) == 0)
        {
            No_Secondaries = ST_TRUE;
        }
        _LAIRS[OVL_Action_Structure].guard2_count = (_LAIRS[OVL_Action_Structure].guard2_count & 0xF0);  // clear low-nibble
    }


    /*
        BEGIN:  
            drake178: process control change effects, regeneration, and recalls, while also counting the surviving units
    */

    Surviving_Unit_Count = 0;

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if((battle_units[itr_battle_units].Combat_Effects & bue_Confusion) != 0)
        {
            if(_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx != player_idx)
            {
                battle_units[itr_battle_units].Status = bus_Dead;
            }
            else
            {
                battle_units[itr_battle_units].controller_idx = _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx;
            }
        }

        if(battle_units[itr_battle_units].controller_idx != player_idx)
        {
            if(battle_units[itr_battle_units].Status == bus_Uninvolved)
            {
                battle_units[itr_battle_units].Status = bus_Dead;
            }
        }
        else
        {
            if(battle_units[itr_battle_units].Status == bus_Uninvolved)
            {
                battle_units[itr_battle_units].Status = bus_Active;
            }
        }

        enchantments = battle_units[itr_battle_units].enchantments;
        enchantments |= _UNITS[battle_units[itr_battle_units].unit_idx].enchantments;
        enchantments |= battle_units[itr_battle_units].item_enchantments;

        if(
            ((battle_units[itr_battle_units].Combat_Effects & bue_Possession) != 0)
            ||
            ((battle_units[itr_battle_units].Combat_Effects & bue_Creature_Binding) != 0)
        )
        {
            if(battle_units[itr_battle_units].controller_idx != player_idx)
            {
                if(battle_units[itr_battle_units].controller_idx != _combat_attacker_player)
                {
                    battle_units[itr_battle_units].controller_idx = _combat_attacker_player;
                }
                else
                {
                    battle_units[itr_battle_units].controller_idx = _combat_defender_player;
                }
            }
            else
            {
                battle_units[itr_battle_units].Status = bus_Gone;
            }
        }

        if(
            (battle_units[itr_battle_units].Status <= bus_Dead)
            ||
            (battle_units[itr_battle_units].Status == bus_Fleeing)
            ||
            (battle_units[itr_battle_units].Status == bus_Recalled)
        )
        {
            if(((battle_units[itr_battle_units].Abilities & UA_REGENERATION) | (enchantments & UE_REGENERATION)) != 0)
            {
                if(battle_units[itr_battle_units].controller_idx == player_idx)
                {
                    battle_units[itr_battle_units].Status = bus_Active;
                }

                battle_units[itr_battle_units].Cur_Figures = battle_units[itr_battle_units].Max_Figures;

                battle_units[itr_battle_units].TopFig_Dmg = 0;
            }
        }

        if(battle_units[itr_battle_units].Status == bus_Recalled)
        {
            battle_unit_owner_idx = battle_units[itr_battle_units].controller_idx;

            _UNITS[battle_units[itr_battle_units].unit_idx].wx = _players[battle_unit_owner_idx].summon_wx;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = _players[battle_unit_owner_idx].summon_wy;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = _players[battle_unit_owner_idx].summon_wp;
            _UNITS[battle_units[itr_battle_units].unit_idx].Finished = ST_TRUE;
            UNIT_RemoveExcess(battle_units[itr_battle_units].unit_idx);
            battle_units[itr_battle_units].Status = bus_Active;
            if(
                (battle_unit_owner_idx == player_idx)
                &&
                (battle_unit_owner_idx == _combat_attacker_player)
            )
            {
                _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = 100;
            }
        }

        if(
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (battle_units[itr_battle_units].Status == bus_Active)
        )
        {
            Surviving_Unit_Count++;
        }

    }  /* for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++) */

    /*
        END:  
    */


   Retreat_From_Combat(player_idx);

   GUI_Multipurpose_Int = 0;


    /*
        BEGIN:  
            drake178: process undead and zombie creation from dead units
                      INCONSISTENT: does not remove enchantments and mutations that are not available for the new unit
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        // Undead
        if(
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (battle_units[itr_battle_units].Status == bus_Drained)
            &&
            (battle_units[itr_battle_units].race == rt_Death)
            &&
            (_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
            &&
            (Surviving_Unit_Count < MAX_STACK)
        )
        {
            battle_units[itr_battle_units].Cur_Figures = battle_units[itr_battle_units].Max_Figures;
            battle_units[itr_battle_units].TopFig_Dmg = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = OVL_Action_XPos;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = OVL_Action_YPos;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = OVL_Action_Plane;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].mutations |= UM_UNDEAD;
            battle_units[itr_battle_units].controller_idx = player_idx;
            battle_units[itr_battle_units].Status = bus_Active;
            Undead_Created++;
            Surviving_Unit_Count++;
            Experience_Gained += 2;
        }
        // Zombies
        if(
            (_players[player_idx].Globals[ZOMBIE_MASTERY] > 0)
            &&
            (battle_units[itr_battle_units].Status == bus_Dead)
            &&
            (battle_units[itr_battle_units].race < rt_Arcane)
            &&
            (_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot == ST_UNDEFINED)
            &&
            (Surviving_Unit_Count < 9)
        )
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                Experience_Gained += 2;
            }

            Zombies_Raised++;

            _UNITS[battle_units[itr_battle_units].unit_idx].type = ut_Zombies;
            _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx = player_idx;
            _UNITS[battle_units[itr_battle_units].unit_idx].XP = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].Level = 0;
            _UNITS[battle_units[itr_battle_units].unit_idx].wx = OVL_Action_XPos;
            _UNITS[battle_units[itr_battle_units].unit_idx].wy = OVL_Action_YPos;
            _UNITS[battle_units[itr_battle_units].unit_idx].wp = OVL_Action_Plane;
            battle_units[itr_battle_units].Cur_Figures = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].Max_Figures = _unit_type_table[ut_Zombies].Figures;
            battle_units[itr_battle_units].TopFig_Dmg = 0;
            battle_units[itr_battle_units].Status = bus_Active;

            Surviving_Unit_Count++;
        }

    }  /* for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++) */

    /*
        END:  
    */


    /*
        BEGIN:  Enemey City
    */

    if(OVL_Action_Type == 1)
    {
        CMB_Population_Lost = 0;
        CMB_Buildings_Lost = 0;
        if(player_idx != _combat_attacker_player)
        {
            CMB_Gold_Reward = 0;
        }
        else
        {
            if(_combat_defender_player != NEUTRAL_PLAYER_IDX)
            {
                _players[_combat_defender_player].fame -= _CITIES[OVL_Action_Structure].size;
                SETMIN(_players[_combat_defender_player].fame, 0);
            }

            if(_combat_defender_player != HUMAN_PLAYER_IDX)
            {
                GUI_Multipurpose_Int = _CITIES[OVL_Action_Structure].size;
            }

            if(_CITIES[OVL_Action_Structure].population == 0)
            {
                GAME_RazeCity = ST_TRUE;
            }
            else
            {
                if(player_idx != HUMAN_PLAYER_IDX)
                {
                    if(player_idx < _num_players)  /* ~== not NEUTRAL_PLAYER_IDX */
                    {
                        // ; decides whether the AI will raze the selected city or not
                        // ; returns 1 if yes, 0 if no
                        // ; INCONSISTENT: Lawful wizards disproportionately
                        // ;  adjust based on whether hidden relation is 0 or not
                        // ; BUG? surviving units are checked without owners
                        // TODO  GAME_RazeCity = AI_Raze_Decision(player_idx, OVL_Action_Structure);
                        GAME_RazeCity = ST_FALSE;
                    }
                }
                else
                {
                    if(magic_set.Raze_City == ST_TRUE)
                    {
                        RazeCity = Raze_City_Prompt(cnst_RazeCity_Msg);
                        if(RazeCity == 0)
                        {
                            GAME_RazeCity = ST_TRUE;
                        }
                    }
                }

            }

            if(GAME_RazeCity == ST_FALSE)
            {
                CMB_Gold_Reward = (_CITIES[OVL_Action_Structure].size - 2);

                SETMIN(CMB_Gold_Reward, 0);

                _players[player_idx].fame += CMB_Gold_Reward;
                
                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int -= CMB_Gold_Reward;
                }

                CMB_Gold_Reward = 0;

                CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);  // the conquering wizard gets a portion of the previous owner's gold reserve as loot.

                if(player_idx < _num_players)
                {
                    Player_Add_Gold(player_idx, CMB_Gold_Reward);
                }

                if(_CITIES[OVL_Action_Structure].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }
            }
            else
            {
                CMB_Gold_Reward = _CITIES[OVL_Action_Structure].size;

                if(player_idx == HUMAN_PLAYER_IDX)
                {
                    GUI_Multipurpose_Int -= CMB_Gold_Reward;
                }

                _players[player_idx].fame -= CMB_Gold_Reward;

                SETMIN(_players[player_idx].fame, 0);

                CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

                if(_CITIES[OVL_Action_Structure].owner_idx < _num_players)
                {
                    _players[_combat_defender_player].gold_reserve -= CMB_Gold_Reward;
                }

                for(itr_buildings = bt_Barracks; itr_buildings < NUM_BUILDINGS; itr_buildings++)
                {
                    if(_CITIES[OVL_Action_Structure].bldg_status[itr_buildings] > bs_NotBuilt)
                    {
                        CMB_Gold_Reward += (bldg_data_table[itr_buildings].construction_cost / 10);
                    }
                }

                if(player_idx < _num_players)
                {
                    Player_Add_Gold(player_idx, CMB_Gold_Reward);
                }

            }

        }  /* if(player_idx != _combat_attacker_player) */



        if(_CITIES[OVL_Action_Structure].population != 0)
        {
            Population_Loss_Percent = (CMB_CityDamage / 2);

            if(player_idx == _combat_attacker_player)
            {
                if(player_idx == NEUTRAL_PLAYER_IDX)
                {
                    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
                    {
                        if(battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                        {
                            if((battle_units[itr_battle_units].Abilities & UA_FANTASTIC) == 0)
                            {
                                Summoned_Unit = ST_FALSE;
                            }
                            else
                            {
                                Summoned_Unit = ST_TRUE;
                            }
                        }
                    }

                    if(Summoned_Unit != ST_TRUE)
                    {
                        CMB_SelectedUnit = 667;  /* 667 - raiders won (city neutral) */
                    }
                    else
                    {
                        Population_Loss_Percent += 50;

                        CMB_SelectedUnit = CTY_RampageVictory();  /* 666 - rampage, no ruins  668 - rampage, created ruins */
                    }

                }
                else
                {
                    Population_Loss_Percent += 10;
                }
            }

            SETMAX(Population_Loss_Percent, 50);

            IDK_population_lost = 0;

            for(itr_population = 0; (_CITIES[OVL_Action_Structure].population - 1) > itr_population; itr_population++)
            {
                if(Random(100) <= Population_Loss_Percent)
                {
                    IDK_population_lost++;
                }
            }

            for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
            {
                Buildings_Lost[itr_buildings] = 0;
            }

            Destruction_Chance = CMB_CityDamage;

            if(player_idx == _combat_attacker_player)
            {
                if(player_idx != NEUTRAL_PLAYER_IDX)
                {
                    Destruction_Chance += 10;
                }
                else
                {
                    Destruction_Chance += 50;
                }
            }

            SETMAX(Destruction_Chance, 75);

            if(CMB_SelectedUnit != 668)  /* 668 - rampage, created ruins */
            {
                CTY_ApplyDamage(OVL_Action_Structure, IDK_population_lost, Destruction_Chance, &Buildings_Lost[0]);
            }

            if(player_idx != _combat_defender_player)
            {
                for(itr_bldg_msg = 0; g_bldg_msg_ctr> itr_bldg_msg; itr_bldg_msg++)
                {
                    if(MSG_Building_Complete[itr_bldg_msg].city_idx == OVL_Action_Structure)
                    {
                        for(bldg_msg_idx = itr_bldg_msg; g_bldg_msg_ctr > itr_bldg_msg; itr_bldg_msg++)
                        {
                            MSG_Building_Complete[bldg_msg_idx].city_idx = MSG_Building_Complete[(bldg_msg_idx + 1)].city_idx;
                        }
                    }
                }
            }

            CMB_Buildings_Lost = 0;

            for(itr_buildings = 0; itr_buildings < NUM_BUILDINGS; itr_buildings++)
            {
                if(Buildings_Lost[itr_buildings] > 0)
                {
                    CMB_Buildings_Lost++;
                }
            }

            CMB_Population_Lost = IDK_population_lost;

            CMB_LostBuildings = &Buildings_Lost[0];

        }  /* if(OVL_Action_Type == 1) */


    }  /* if(_CITIES[OVL_Action_Structure].population != 0) */

    /*
        END:  Enemey City
    */


    /*
        BEGIN:  
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        
        /*
            Lair Combat, Neutral Player won, Neutral Player's Unit, Unit is ~Alive
            move the unit back into the lair unit count and kill it's battle unit record
            not sure about the "No_Secondaries" part
        */
        {
            if(
                (OVL_Action_Type == 5)
                &&
                (player_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                &&
                (battle_units[itr_battle_units].Status == bus_Active)
            )
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[OVL_Action_Structure].guard1_unit_type)
                {
                    _LAIRS[OVL_Action_Structure].guard1_count++;
                    battle_units[itr_battle_units].Status = bus_Dead;
                }
                else if(_UNITS[battle_units[itr_battle_units].unit_idx].type == _LAIRS[OVL_Action_Structure].guard2_unit_type)
                {
                    _LAIRS[OVL_Action_Structure].guard2_count++;
                    battle_units[itr_battle_units].Status = bus_Dead;
                }
                else
                {
                    if(No_Secondaries != ST_TRUE)
                    {
                        battle_units[itr_battle_units].Status = bus_Dead;
                    }
                    else
                    {
                        _LAIRS[OVL_Action_Structure].guard2_unit_type = _UNITS[battle_units[itr_battle_units].unit_idx].type;
                        _LAIRS[OVL_Action_Structure].guard2_count += 0x11;  // add 1 to both the high and low nibbles
                        battle_units[itr_battle_units].Status = bus_Dead;
                        No_Secondaries = ST_FALSE;
                    }
                }
            }
        }

        /*
            @@Diplomatic_Value
        */
        Diplomatic_Value = 0;

        /*
            Current Battle Unit is Alive or Dead
        */
        if(
            (battle_units[itr_battle_units].Status > bus_Active)
            ||
            (_UNITS[battle_units[itr_battle_units].unit_idx].wp == 9)  /* BU Combat Summon */  /* ¿ ~unsummoned ? */
            ||
            (
                (battle_units[itr_battle_units].controller_idx == NEUTRAL_PLAYER_IDX)
                &&
                (OVL_Action_Type != 0)  /* Enemy Army */
                &&
                (
                    (OVL_Action_Type != 1)  /* Enemy City */
                    ||
                    ((battle_units[itr_battle_units].Abilities & UA_FANTASTIC) != 0)
                )
            )
        )
        {

            // it's dead, and it's not ours, so we killed it, and we deserve the credit
            if(battle_units[itr_battle_units].controller_idx != player_idx)
            {
                Experience_Gained += 2;

                if(_unit_type_table[_UNITS[battle_units[itr_battle_units].unit_idx].type].Cost >= 600)  /* ¿ >= the cost of Torin ? */
                {
                    Rare_Foe_Defeated = ST_TRUE;  // later, +1 Fame for "winning a battle where the enemy lost a very rare creature" - MoM-OSG
                }
            }


            if(
                (battle_units[itr_battle_units].controller_idx == _combat_defender_player)
                &&
                (_UNITS[battle_units[itr_battle_units].unit_idx].wp != 9)  /* BU Combat Summon */
            )
            {
                if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
                {
                    Diplomatic_Value += 20;
                }
                else
                {
                    Diplomatic_Value += Random(20);

                    if(
                        (battle_units[itr_battle_units].Construction > 0)
                        ||
                        ((battle_units[itr_battle_units].Abilities & UA_CREATEOUTPOST) != 0)
                    )
                    {
                        Diplomatic_Value += Random(10);
                    }
                }
            }


            /*
                BEGIN: Hero Unit

                    IDA Color: brick red
            */
            // if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
            if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)
            {
                for(itr_hero_items = 0; itr_hero_items < NUM_HERO_ITEMS; itr_hero_items++)
                {

                    // if(_players[BUNITSOWNER()].Heroes[BUNITSHEROSLOT()].Items[itr_hero_items] > -1)
                    if(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] > -1)
                    {
                        // BU Status 6 ~== unsummoned, banished, disintegrated, stoned, cracks called
                        if(battle_units[itr_battle_units].Status == bus_Gone)
                        {
                            Remove_Item(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items]);
                        }
                        else
                        {
                            // only mostly dead
                            if(*item_count < 18)
                            {
                                item_list[*item_count] = _players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items];
                                *item_count += 1;
                            }
                            else
                            {
                                Remove_Item(_players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items]);
                            }
                        }
                    }

                    _players[_UNITS[battle_units[itr_battle_units].unit_idx].owner_idx].Heroes[_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot].Items[itr_hero_items] = -1;
                }

                if(battle_units[itr_battle_units].controller_idx != player_idx)
                {
                    if(battle_units[itr_battle_units].controller_idx == HUMAN_PLAYER_IDX)
                    {
                        GUI_Multipurpose_Int -= ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                    }

                    _players[battle_units[itr_battle_units].controller_idx].fame -= ((_UNITS[battle_units[itr_battle_units].unit_idx].Level + 1) / 2);
                }

            }
            /*
                END: Hero Unit
            */


            // BU Status 6 ~== unsummoned, banished, disintegrated, stoned, cracks called
            if(
                (battle_units[itr_battle_units].Status == bus_Gone)
                ||
                ((_UNITS[battle_units[itr_battle_units].unit_idx].mutations & UM_UNDEAD) != 0)
            )
            {
                UNIT_MarkRemoved(battle_units[itr_battle_units].unit_idx, 2);  // wiped from existence
            }
            else
            {
                UNIT_MarkRemoved(battle_units[itr_battle_units].unit_idx, 0);  // just mostly dead
            }

        }
        else
        {
            /*
                Current Battle Unit is Alive
            */
            SETMIN(battle_units[itr_battle_units].TopFig_Dmg, 0);
            battle_units[itr_battle_units].Extra_Hits = 0;
            battle_units[itr_battle_units].enchantments = 0;
            BU_CombatHits = Battle_Unit_Hit_Points(&battle_units[itr_battle_units]);
            BU_CombatHits -= battle_units[itr_battle_units].hits;
            battle_units[itr_battle_units].TopFig_Dmg -= BU_CombatHits;
            SETMIN(battle_units[itr_battle_units].TopFig_Dmg, 0);
            _UNITS[battle_units[itr_battle_units].unit_idx].Damage = (((battle_units[itr_battle_units].Max_Figures - battle_units[itr_battle_units].Cur_Figures) * (battle_units[itr_battle_units].hits - BU_CombatHits)) + battle_units[itr_battle_units].TopFig_Dmg);
            _UNITS[battle_units[itr_battle_units].unit_idx].moves2 = 0;
        }

    // @@Next_Battle_Unit__2
    }

    /*
        END:  
    */


    if(
        (_combat_defender_player != HUMAN_PLAYER_IDX)
        &&
        (_combat_defender_player != NEUTRAL_PLAYER_IDX)
        &&
        (_combat_attacker_player != NEUTRAL_PLAYER_IDX)
    )
    {
        Change_Relations(-Diplomatic_Value, _combat_attacker_player, _combat_defender_player, 8, ST_NULL, ST_NULL);
    }


    /*
        BEGIN:  
    */

    if(
        (OVL_Action_Type == 5)  // Lair
        &&   
        (player_idx == NEUTRAL_PLAYER_IDX)
    )
    {

        if(
            ((_LAIRS[OVL_Action_Structure].guard1_count & 0x0F) == 0)
            &&
            ((_LAIRS[OVL_Action_Structure].guard2_count & 0x0F) > 0)
        )
        {
            _LAIRS[OVL_Action_Structure].guard1_count = _LAIRS[OVL_Action_Structure].guard2_count;
            _LAIRS[OVL_Action_Structure].guard1_unit_type = _LAIRS[OVL_Action_Structure].guard2_unit_type;
            _LAIRS[OVL_Action_Structure].guard2_count = 0;
            _LAIRS[OVL_Action_Structure].guard2_unit_type = ut_Gargoyles;
        }

        if(
            (Undead_Created > 0)
            &&
            (_LAIRS[OVL_Action_Structure].guard1_unit_type >= ut_Magic_Spirit)
            &&
            (_LAIRS[OVL_Action_Structure].guard2_unit_type >= ut_Magic_Spirit)
        )
        {
            Undead_Created = 0;
        }
    }

    /*
        END:

    */


    /*
        BEGIN:  Fame for defeating 4+ enemy units
    */

    if(
        (player_idx != NEUTRAL_PLAYER_IDX)
        &&
        (Experience_Gained > 8)
    )
    {
        if(player_idx == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int++;
        }

        _players[player_idx].fame++;

        if(player_idx != _combat_attacker_player)
        {
            if(
                (_players[_combat_attacker_player].fame > 20)
                &&
                (_combat_attacker_player == HUMAN_PLAYER_IDX)
            )
            {
                GUI_Multipurpose_Int--;
            }

            _players[_combat_attacker_player].fame--;
        }
        else
        {
            if(
                (_players[_combat_defender_player].fame > 20)
                &&
                (_combat_defender_player == HUMAN_PLAYER_IDX)
            )
            {
                GUI_Multipurpose_Int--;
            }

            _players[_combat_defender_player].fame--;
        }
    }

    /*
        END:  Fame for defeating 4+ enemy units
    */


    if(player_idx == HUMAN_PLAYER_IDX)
    {
        GUI_Multipurpose_Int += Rare_Foe_Defeated;
    }

    _players[player_idx].fame += Rare_Foe_Defeated;



    if(
        (_combat_attacker_player == HUMAN_PLAYER_IDX)
        ||
        (_combat_defender_player == HUMAN_PLAYER_IDX)
    )
    {
        Combat_Results_Scroll();
    }



    /*
        BEGIN:  Experience
    */

    for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
    {
        if(
            (battle_units[itr_battle_units].Status == bus_Active)
            &&
            (battle_units[itr_battle_units].controller_idx == player_idx)
            &&
            (
                (battle_units[itr_battle_units].race < rt_Arcane)
                ||
                (_UNITS[battle_units[itr_battle_units].unit_idx].type == ut_Chosen)
            )
        )
        {
            _UNITS[battle_units[itr_battle_units].unit_idx].XP += Experience_Gained;
        }
    }

    /*
        END:  Experience
    */


    /*
        BEGIN:  Zombies & Undead
    */

    if(
    (_combat_attacker_player == HUMAN_PLAYER_IDX)
    ||
    (_combat_defender_player == HUMAN_PLAYER_IDX)
    )
    {
        if(Zombies_Raised > 0)
        {
            itoa(Zombies_Raised, Conv_String, 10);

            strcpy(GUI_NearMsgString, Conv_String);

            if(Zombies_Raised > 1)
            {
                strcat(GUI_NearMsgString, cnst_Zombie_Msg_1);
            }
            else
            {
                strcat(GUI_NearMsgString, cnst_Zombie_Msg_2);
            }

            strcat(GUI_NearMsgString, cnst_Zombie_Msg_3);

            // TODO  CMB_CreateUndeadAnim(ut_Zombies);
        }

        if(Undead_Created > 0)
        {
            itoa(Undead_Created, Conv_String, 10);

            strcpy(GUI_NearMsgString, Conv_String);

            if(Undead_Created > 1)
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_1);
            }
            else
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_2);
            }

            if(player_idx == HUMAN_PLAYER_IDX)
            {
                strcat(GUI_NearMsgString, cnst_Undead_Msg_3);
            }
            else
            {
                if(player_idx == NEUTRAL_PLAYER_IDX)
                {
                    if(Undead_Created > 1)
                    {
                        strcat(GUI_NearMsgString, cnst_Undead_Msg_4);
                    }
                    else
                    {
                        strcat(GUI_NearMsgString, cnst_Undead_Msg_5);
                    }
                }
                else
                {
                    strcat(GUI_NearMsgString, _players[player_idx].name);
                    strcat(GUI_NearMsgString, cnst_Dot9);
                }
            }

            // TODO  CMB_CreateUndeadAnim(CMB_GetUndeadCreator(player_idx));
        }

    }

    /*
        END:  Zombies & Undead
    */

    if(_combat_attacker_player == HUMAN_PLAYER_IDX)
    {
        // TODO  ¿ odd behavior, what to do ?  Set_Mouse_List(1, mouse_list_hourglass);
    }

}


// WZD o123p02
// CMB_GetUndeadCreator()

// WZD o123p03
void Combat_Results_Scroll_Draw(void)
{
    Copy_Back_To_Off();
    Set_Window(0, 0, SCREEN_XMAX, (_scroll_text_top + _scroll_text_height));
    Clipped_Draw(54, _scroll_text_top, _scroll_paper_seg);
    Reset_Window();
    FLIC_Draw(54, (_scroll_text_top + _scroll_text_height), _combat_results_scroll_bottom_seg);
    Combat_Results_Scroll_Text();
}


// WZD o123p04
// ¿ MoO2  Module: COMBFIND  Strategic_Results_Popup_() ?
// Settings - Help - 'Raze City' "combat results scroll"
void Combat_Results_Scroll(void)
{
    int16_t City_Capture;
    int16_t Hotkey_R2_Index;
    int16_t Hotkey_R1_Index;
    int16_t hotkey_ESC;
    int16_t leave_screen;
    int16_t Keep_City;
    int16_t IDK_popup_timer;  // _SI_
    int16_t input_field_idx;  // _DI_

    City_Capture = ST_FALSE;

    if(CMB_ScrollMsg_Type == 4)
    {
        // SCROLL.LBX  011  CMBLOSE
        _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 11, _screen_seg);
    }
    else
    {
        if(OVL_Action_Type == 1)  // City
        {
            if(
                (CMB_ScrollMsg_Type == 1)
                ||
                (CMB_ScrollMsg_Type == 5)
                ||
                (CMB_ScrollMsg_Type == 6)
            )
            {
                // City, Won
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    // SCROLL.LBX  009  CMBSCRL1
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 9, _screen_seg);
                    City_Capture = ST_TRUE;
                }
                else
                {
                    // SCROLL.LBX  010  CMBWIN
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
                }
            }
            else
            {
                // City, Lost
                if(_combat_attacker_player == HUMAN_PLAYER_IDX)
                {
                    // SCROLL.LBX  010  CMBWIN
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
                }
                else
                {
                    // SCROLL.LBX  009  CMBSCRL1
                    _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 9, _screen_seg);
                }
            }
        }
        else
        {
            if(
                (CMB_ScrollMsg_Type == 1)  // Combat Victory, Tactical Combat
                ||
                (CMB_ScrollMsg_Type == 5)  // Enemy Fled
                ||
                (CMB_ScrollMsg_Type == 6)  // Combat Victory, Strategic Combat
            )
            {
                // SCROLL.LBX  010  CMBWIN
                _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 10, _screen_seg);
            }
            else
            {
                // SCROLL.LBX  011  CMBLOSE
                _scroll_paper_seg = LBX_Reload_Next(scroll_lbx_file__ovr123, 11, _screen_seg);
            }
        }
    }

    // HELP.LBX  001  <no name, no description>
    _combat_results_scroll_bottom_seg = LBX_Reload_Next(help_lbx_file__ovr123, 1, _screen_seg);

    Set_Page_Off();

    _scroll_text_height = Combat_Results_Scroll_Text();

    _scroll_text_top = ((SCREEN_YMAX - (22 + _scroll_text_height)) / 2);

    Clear_Fields();

    Set_Page_Off();

    Combat_Results_Scroll_Draw();

    PageFlipEffect = 3;
    PageFlip_FX();
    PageFlipEffect = 0;
    
    Copy_On_To_Off_Page();

    Assign_Auto_Function(Combat_Results_Scroll_Draw, 1);
    
    leave_screen = ST_FALSE;

    Hotkey_R1_Index = Add_Hot_Key(cnst_HOTKEY_R);
    Hotkey_R2_Index = Add_Hot_Key(cnst_HOTKEY_R_2);
    hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);

    IDK_popup_timer = 0;
    while((IDK_popup_timer < 400) && (leave_screen == ST_FALSE))
    {
        input_field_idx = Get_Input();

        if(input_field_idx == hotkey_ESC)
        {
            leave_screen = ST_TRUE;
        }

        if((input_field_idx == Hotkey_R1_Index) || (input_field_idx == Hotkey_R2_Index))
        {
            if(GAME_RazeCity == ST_FALSE)
            {
                if(City_Capture == ST_TRUE)
                {
                    Keep_City = Raze_City_Prompt(cnst_RazeCity_Msg);

                    if(Keep_City == ST_FALSE)
                    {
                        GAME_RazeCity = ST_TRUE;
                        // ; BUG: this is different than the original penalty, as conquest fame is already applied!
                        // ; BUG: no zero check!
                        _players[HUMAN_PLAYER_IDX].fame -= (_CITIES[OVL_Action_Structure].size + 1);
                        GUI_Multipurpose_Int -= (_CITIES[OVL_Action_Structure].size + 1);

                        // ; (this resets the timeout counter)
                        for(IDK_popup_timer = 3; IDK_popup_timer < NUM_BUILDINGS; IDK_popup_timer++)
                        {
                            if(_CITIES[OVL_Action_Structure].bldg_status[IDK_popup_timer] > bs_NotBuilt)
                            {
                                // ; BUG: will re-award the original looted gold another time!
                                CMB_Gold_Reward += bldg_data_table[IDK_popup_timer].construction_cost;
                            }
                        }

                        Player_Add_Gold(HUMAN_PLAYER_IDX, CMB_Gold_Reward);

                        _scroll_text_height = Combat_Results_Scroll_Text();

                        _scroll_text_top = ((SCREEN_YMAX - (22 + _scroll_text_height)) / 2);

                    }

                    Clear_Fields();

                    hotkey_ESC = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, cnst_HOTKEY_Esc1A, ST_UNDEFINED);
                }
            }
        }

        if(leave_screen == ST_FALSE)
        {
            Set_Page_Off();
            Combat_Results_Scroll_Draw();
            PageFlip_FX();
        }

    }

    Deactivate_Auto_Function();
    Release_Block(_screen_seg);

}


// WZD o123p05
int16_t Combat_Results_Scroll_Text(void)
{
    char message[150];
    char temp_string[20];
    uint8_t colors2[5];
    uint8_t colors1[5];
    int16_t next_x;
    int16_t text_height;  // _SI_
    int16_t itr_colors;  // _DI_
    int16_t itr_buildings;  // _DI_


    CMB_Scroll_MinHeight = 0;


    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors1[itr_colors] = 47;
    }

    for(itr_colors = 0; itr_colors < 5; itr_colors++)
    {
        colors2[itr_colors] = 55;
    }

    colors2[0] = 247;

    colors1[0] = 245;

    text_height = 92;

    Set_Font_Colors_15(4, &colors1[0]);


    switch(CMB_ScrollMsg_Type)
    {
        case 0:
        {

        } break;
        case 1:
        case 6:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 5, 1, 150);  /* "You are triumphant" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 2:
        case 7:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 6, 1, 150);  /* "You have been defeated" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 3:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 7, 1, 150);  /* "Your forces have retreated" */
            Print_Centered(160, (_scroll_text_top + 25), message);
            if(GUI_NearMsgString != 0)
            {
                Set_Font_Colors_15(1, &colors2[0]);
                Set_Font_Spacing_Width(2);
                Print_Paragraph(75, (_scroll_text_top + text_height), 75, GUI_NearMsgString, 0);
                // TODO  text_height += (Get_Paragraph_Max_Height(175, GUI_NearMsgString, 0) + 2);
                text_height += (Get_Paragraph_Max_Height(175, GUI_NearMsgString) + 2);
            }
        } break;
        case 4:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 8, 1, 150);  /*  "All units retreat exhausted" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 5:
        {
            LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 9, 1, 150);  /*  "Your opponent has fled" */
            Print_Centered(160, (25 + _scroll_text_top), message);
        } break;
        case 8:
        {

        } break;
        case 9:
        {

        } break;
        case 10:
        {

        } break;
        case 11:
        {

        } break;
        case 12:
        {
            // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
            strcpy(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
            strcat(GUI_NearMsgString, cnst_CityLost_Msg);  /* " has been conquered" */
            Print_Centered(160, (_scroll_text_top + 25), GUI_NearMsgString);
        } break;
    }


    if(CMB_SelectedUnit == 666)  /* ; 666 - rampage, no ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        LBX_Load_Data_Static(message_lbx_file__ovr123, 0, (SAMB_ptr)message, 10, 1, 150);  /* "The monsters rampage through the city and then disappear into the wilderness" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }

    if(CMB_SelectedUnit == 668)  /* ; 668 - rampage, created ruins */
    {
        Set_Font_Colors_15(1, &colors2[0]);
        // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
        strcpy(message, _CITIES[OVL_Action_Structure].name);
        strcat(message, cnst_NewRuins_Msg);  /* " has been reduced to ruins" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }

    if(CMB_SelectedUnit == 667)  /* ; 667 - raiders won (city neutral) */
    {
        // String_Copy_Far(GUI_NearMsgString, _CITIES[OVL_Action_Structure].name);
        strcpy(message, _CITIES[OVL_Action_Structure].name);
        strcat(message, cnst_CityRaided_Msg);  /* " has fallen to raiders" */
        Print_Paragraph(75, (_scroll_text_top + text_height), 175, message, 2);
        // TODO  text_height += (Get_Paragraph_Max_Height(175, message, 2) + 2));
        text_height += (Get_Paragraph_Max_Height(175, message) + 2);
    }


    if(GUI_Multipurpose_Int != 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        strcpy(message, cnst_ScrlFame_Msg_1);  /* "You have " */
        if(GUI_Multipurpose_Int < 0)
        {
            strcat(message, cnst_ScrlFame_Msg_2);  /* "lost " */
        }
        else
        {
            strcat(message, cnst_ScrlFame_Msg_3);  /* "gained " */
        }
        itoa(abs(GUI_Multipurpose_Int), temp_string, 10);
        strcat(message, temp_string);
        strcat(message, cnst_ScrlFame_Msg_4);  /* " fame." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }


    if(CMB_Gold_Reward > 0)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Spacing_Width(2);
        itoa(CMB_Gold_Reward, temp_string, 10);
        strcpy(message, temp_string);
        strcat(message, cnst_ScrlGold_Msg);  /* " gold pieces were looted." */
        Print_Centered(160, (_scroll_text_top + text_height), message);
        text_height += 9;
    }


    if(GAME_RazeCity == ST_TRUE)
    {
        Set_Font_Colors_15(1, &colors2[0]);
        Set_Font_Spacing_Width(2);
        Print_Centered(160, (_scroll_text_top + text_height), cnst_ScrlRaze_Msg);  /* "The city has been completly destroyed." */
        text_height += 7;
        text_height += 2;
    }
    else
    {
        if(CMB_Population_Lost > 0)
        {
            Set_Font_Colors_15(1, &colors2[0]);
            Set_Font_Spacing_Width(2);
            itoa(CMB_Population_Lost, temp_string, 10);
            strcpy(message, temp_string);
            strcat(message, cnst_ScrlPop_Msg);  /* " thousand inhabitants killed." */
            Print_Centered(160, (_scroll_text_top + text_height), message);
            text_height += 7;
            text_height += 2;
        }

        if(CMB_Buildings_Lost > 0)
        {
            Set_Font_Colors_15(4, &colors1[0]);
            Print_Centered(160, (_scroll_text_top + text_height), cnst_ScrlBldng_Msg);  /* "Destroyed Buildings" */
            text_height += 11;
            Set_Font_Colors_15(1, &colors2[0]);
            Set_Font_Spacing_Width(2);
            G_CMB_NextScrollLineTop = text_height;
            for(itr_buildings = 0; itr_buildings < CMB_Buildings_Lost; itr_buildings++)
            {
                if(CMB_LostBuildings[itr_buildings] > 0)
                {
                    // next_x = Print_Far((75 + ((itr_buildings % 2) * 90)), (_scroll_text_top + text_height), bldg_data_table[CMB_LostBuildings[itr_buildings]]);
                    next_x = Print((75 + ((itr_buildings % 2) * 90)), (_scroll_text_top + text_height), bldg_data_table[CMB_LostBuildings[itr_buildings]].name);
                }
                if((itr_buildings % 2) == 1)
                {
                    text_height += 7;
                }
            }
            text_height += 2;
        }

        // ¿ BUG: this should be outside this if / the second-to-last-thing to happen ?
        SETMIN(text_height, CMB_Scroll_MinHeight);
    }

    return text_height;
}


// WZD o123p06
int16_t CTY_GetConquerGold(int16_t city_idx)
{
    int16_t Empire_Population;
    int16_t Gold_Reward;
    int16_t itr;  // _SI_

    Gold_Reward = 0;

    if(_CITIES[city_idx].owner_idx != NEUTRAL_PLAYER_IDX)
    {
        Empire_Population = 0;

        for(itr = 0; itr < _cities; itr++)
        {
            if(_CITIES[itr].owner_idx == _CITIES[city_idx].owner_idx)
            {
                Empire_Population += _CITIES[itr].population;
            }
        }

        Gold_Reward = ((_CITIES[city_idx].population * _players[_CITIES[city_idx].owner_idx].gold_reserve) / Empire_Population);
    }
    else
    {
        for(itr = 0; _CITIES[city_idx].population > itr; itr++)
        {
            Gold_Reward += Random(10);
        }
    }

    return Gold_Reward;
}


// WZD o123p07
int16_t CTY_RampageVictory(void)
{
    int16_t Unit_Type;
    int16_t Unit_Types;
    int16_t Secondary_Unit;
    int16_t Primary_Unit;
    int16_t Secondary_Count;
    int16_t Primary_Count;
    int16_t itr;  // _DI_
    int16_t idx;  // _SI_
    int16_t return_value;  // DNE in Dasm

    // get next available lair_idx
    idx = ST_UNDEFINED;
    for(itr = 0; ((itr < NUM_LAIRS) && (idx == ST_UNDEFINED)); itr++)
    {
        if(_LAIRS[itr].Intact == ST_FALSE)
        {
            idx = itr;
        }
    }

    Primary_Unit = 0;
    Secondary_Unit = 0;
    Primary_Count = 0;
    Secondary_Count = 0;

    // 50:50 - destroyed city becomes new lair
    // OR no more lair_idx available
    // OR the city was the player's fortress city
    if(
        (Random(2) == 1)
        ||
        (idx == ST_UNDEFINED)
        ||
        (Player_Fortress_City(_CITIES[OVL_Action_Structure].owner_idx) == OVL_Action_Structure)
    )
    {
        return_value = 666;
    }
    else
    {
        Change_City_Ownership(OVL_Action_Structure, NEUTRAL_PLAYER_IDX);
        Destroy_City(OVL_Action_Structure);

        Unit_Types = 0;

        for(itr = 0; _combat_total_unit_count < _combat_total_unit_count; itr++)
        {

            if(
                (battle_units[itr].Status == 0) // Unit_Active
                &&
                (battle_units[itr].controller_idx == NEUTRAL_PLAYER_IDX) // Unit_Active
            )
            {
                battle_units[itr].Status = 4; // Unit_Dead

                Unit_Type = _UNITS[battle_units[itr].unit_idx].type;

                if(Primary_Unit == Unit_Type)
                {
                    Primary_Count++;
                }
                else
                {
                    if(Secondary_Unit == Unit_Type)
                    {
                        Secondary_Count++;
                    }
                    else
                    {
                        if(Unit_Types == 0)
                        {
                            Primary_Unit = Unit_Type;
                            Primary_Count = 1;
                            Unit_Types = 1;
                        }
                        else if(Unit_Types == 1)
                        {
                            Unit_Types = 2;
                            if(_unit_type_table[Unit_Type].Cost < _unit_type_table[Primary_Unit].Cost)
                            {
                                Secondary_Unit = Unit_Type;
                                Secondary_Count = 1;
                            }
                            else
                            {
                                Secondary_Unit = Primary_Unit;
                                Secondary_Count = Primary_Count;
                                Primary_Unit = Unit_Type;
                                Primary_Count = 1;
                            }
                        }
                        else
                        {
                            if(_unit_type_table[Unit_Type].Cost > _unit_type_table[Primary_Unit].Cost)
                            {
                                Secondary_Unit = Primary_Unit;
                                Secondary_Count = Primary_Count;
                                Primary_Unit = Unit_Type;
                                Primary_Count = 1;
                            }
                            else
                            {
                                if(_unit_type_table[Unit_Type].Cost > _unit_type_table[Secondary_Unit].Cost)
                                {
                                    Secondary_Unit = Unit_Type;
                                    Secondary_Count = 1;
                                }
                            }
                        }
                    }
                }
            }
        }

        _LAIRS[idx].guard1_unit_type = Primary_Unit;
        _LAIRS[idx].guard2_unit_type = Secondary_Unit;
        _LAIRS[idx].guard1_count = Primary_Count;
        _LAIRS[idx].guard2_count = Secondary_Count;
        _LAIRS[idx].wx = OVL_Action_XPos;
        _LAIRS[idx].wy = OVL_Action_YPos;
        _LAIRS[idx].wp = OVL_Action_Plane;
        _LAIRS[idx].Intact = ST_TRUE;
        _LAIRS[idx].type = EZ_Ruins;
        _LAIRS[idx].Loot_Gold = CMB_Gold_Reward;
        _LAIRS[idx].Loot_Mana = 0;
        _LAIRS[idx].Spell_n_Special = 0;
        _LAIRS[idx].Misc_Flags = 0;
        _LAIRS[idx].Item_Count = 0;
        _LAIRS[idx].Item_Values[0] = 0;
        _LAIRS[idx].Item_Values[1] = 0;
        _LAIRS[idx].Item_Values[2] = 0;
    }

    return return_value;
}




/*
    WIZARDS.EXE  ovr124
*/

// WZD o124p01
// WIZ_GetLastRangedStr()

// WZD o124p02
// BU_SetCityMovement()

// WZD o124p03
// AI_RestrictToCity()

// WZD o124p04
/*
    "STRUCTURES INFLUENCING COMBAT"

    (only?) used Init_Combat_Conditions() as {F,T} ... if defender == owner then +3 defense  (~ City Walls)

Eh ? set_city_flag?
only called from Strategic Combat and Tactical Combat
Tactical calls it with SetCtyCnd 0
Strategic calls it with SetCtyCnd 1
so, for Tactical, could be city combat but no condition 1?

Maybe not pure happenstance that 2,3,4,6 inherently means it can't be city combat?

maybe not 5 because 5 is Lair?
...which means it is pure happenstance that 5 is neutral player and lair combat?

*/
int16_t Combat_Structure(int16_t wx, int16_t wy, int16_t wp, int16_t set_city_flag)
{
    int16_t combat_structure;  //  _SI_
    int16_t terrain_type;  // _DI_

    combat_structure = cs_NONE;

    if(Terrain_Is_Sailable(wx, wy, wp) != ST_FALSE)
    {
        combat_structure = cs_OceanTerrainType;
    }

    if(OVL_Action_Type == 1)  /* Combat - Enemy City */
    {
        if(_CITIES[OVL_Action_Structure].enchantments[HEAVENLY_LIGHT] > ST_FALSE)
        {
            // TBL_CombatEnchants[] ... Combat_Enchants.True_Light.Dfndr] = 2
        }

        if(_CITIES[OVL_Action_Structure].enchantments[CLOUD_OF_SHADOW] > ST_FALSE)
        {
            // TBL_CombatEnchants[] ... Combat_Enchants.Darkness.Dfndr] = 2
        }

        if(set_city_flag == ST_TRUE)
        {
            combat_structure = cs_City;
        }
    }

    terrain_type = (_world_maps[((wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + wx)] % NUM_TERRAIN_TYPES);

    if(terrain_type == TT_SorcNode)
    {
        combat_structure = cs_SorceryNode;
    }
    
    if(terrain_type == TT_NatNode)
    {
        combat_structure = cs_NatureNode;
    }
    
    if(terrain_type == TT_ChaosNode)
    {
        combat_structure = cs_ChaosNode;
    }

    return combat_structure;
}


// WZD o124p05
/*

Page 98  (PDF Page 103)
"Only flying, sailing and swimming units may actually engage in combat over such squares."
*/
int16_t CMB_MarkLandLubbers(int16_t player_idx)
{

    int16_t unit_count;  // _DI_
    int16_t itr_battle_units;  // _SI_

    unit_count = 0;

    if(Terrain_Is_Sailable(OVL_Action_XPos, OVL_Action_YPos, OVL_Action_Plane) == ST_TRUE)
    {

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(battle_units[itr_battle_units].controller_idx == player_idx)
            {
                if(
                    ((battle_units[itr_battle_units].Abilities & UA_WINDWALKING) == 0)
                    ||
                    ((battle_units[itr_battle_units].enchantments & UE_WINDWALKING))
                    ||
                    (_UNITS[battle_units[itr_battle_units].unit_idx].type == spell_data_table[ut_Floating_Island].Param0)
                )
                {
                    return 0;
                }

            }
        }

        for(itr_battle_units = 0; itr_battle_units < _combat_total_unit_count; itr_battle_units++)
        {
            if(
                (battle_units[itr_battle_units].controller_idx == player_idx)
                &&
                (battle_units[itr_battle_units].carry_capacity == 0)
                &&
                ((battle_units[itr_battle_units].Move_Flags & MV_FLYING) == 0)
                &&
                ((battle_units[itr_battle_units].Move_Flags & MV_SWIMMING) == 0)
            )
            {
                battle_units[itr_battle_units].Status = 3;  /* Unit_NotInvolved */
                unit_count++;
            }

        }

    }

    return unit_count;
}


// WZD o124p06
// AI_CompareArmies()

// WZD o124p07
// BU_LoadMoveSound()

// WZD o124p08
// BU_LoadMeleeSound()

// WZD o124p09
// GAME_LoadSpellSound()

// WZD o124p10
// BU_WallofFire()

// WZD o124p11
// BU_SetBloodAnim()

// WZD o124p12
// BU_ClearBlood()

// WZD o124p13
// BU_IsInCityProper()

// WZD o124p14
// TILE_HasIntactWall()

// WZD o124p15
/*
"While most creature summonings are cast overland, the summoning of Elementals, Phantom Warriors and Phantom Beasts can only be performed during combat."
*/
int16_t BU_IsCombatSummon(int16_t battle_unit_idx)
{
    int16_t unit_type;  // _SI_

    unit_type = _UNITS[battle_units[battle_unit_idx].unit_idx].type;

    // ovr124:0BFE 26 39 B7 C8 0C                                  cmp     es:[bx+(Fire_Elemental*24h)+s_SPELL_DATA.Param0], _SI_unit_type
    // field_CA8 / 24h = 5Ah  90 Fire_Elemental
    // field_438 / 24h = 1Eh  30 Earth_Elemental  
    // field_870 / 24h = 3Ch  60 Phantom_Beast
    // field_654 / 24h = 2Dh  45 Phantom_Warriors
    // field_948 / 24h = 42h  66 Air_Elemental
    if(
        (spell_data_table[90].Param0 == unit_type)
        ||
        (spell_data_table[30].Param0 == unit_type)
        ||
        (spell_data_table[60].Param0 == unit_type)
        ||
        (spell_data_table[45].Param0 == unit_type)
        ||
        (spell_data_table[66].Param0 == unit_type)
        ||
        (unit_type == ut_Demon)
    )
    {
        return ST_TRUE;
    }
    else
    {
        return ST_FALSE;
    }

}


// WZD o124p16
// BU_GetHalfMoves()

// WZD o124p17
/*
    "WIP", cause AI_Raze_Decision(Stack_Owner, OVL_Action_Structure);
*/
void STK_CaptureCity__WIP(int16_t troop_count, int16_t troops[])
{
    int16_t City_Owner;
    int16_t Stack_Owner;  // _DI_
    int16_t itr;  // _SI_

    Unit_View_Allocate();  // ¿ if/where used ?

    Set_Page_Off();
    Main_Screen_Draw();
    Copy_Off_To_Back();

    Stack_Owner = _UNITS[troops[0]].owner_idx;

    City_Owner = _CITIES[OVL_Action_Structure].owner_idx;

    GUI_Multipurpose_Int = 0;

    if(City_Owner < _num_players)
    {
        _players[City_Owner].fame -= _CITIES[OVL_Action_Structure].size;

        if(City_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int -= _CITIES[OVL_Action_Structure].size;
        }

        if(_players[City_Owner].fame < 0)
        {
            _players[City_Owner].fame = 0;
        }
    }

    if(_CITIES[OVL_Action_Structure].population == 0)
    {
        GAME_RazeCity = ST_TRUE;
    }
    else
    {
        GAME_RazeCity = ST_FALSE;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            if(magic_set.Raze_City == ST_TRUE)
            {
                if(Raze_City_Prompt(cnst_RazeCity_Msg2) == 0)
                {
                    GAME_RazeCity = ST_TRUE;
                }
            }
        }
        else
        {
            if(Stack_Owner < _num_players)
            {
                // TODO GAME_RazeCity = AI_Raze_Decision(Stack_Owner, OVL_Action_Structure);
            }
        }

    }

    if(GAME_RazeCity == ST_FALSE)
    {
        CMB_Gold_Reward = (_CITIES[OVL_Action_Structure].size - 2);

        if(CMB_Gold_Reward < 0)
        {
            CMB_Gold_Reward = 0;
        }

        _players[Stack_Owner].fame += CMB_Gold_Reward;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int += CMB_Gold_Reward;
        }

        CMB_Gold_Reward = 0;

        CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

        if(Stack_Owner < _num_players)
        {
            Player_Add_Gold(Stack_Owner, CMB_Gold_Reward);
        }

        if(City_Owner < _num_players)
        {
            _players[City_Owner].gold_reserve -= CMB_Gold_Reward;

            if(_players[City_Owner].gold_reserve < 0)
            {
                _players[City_Owner].gold_reserve = 0;
            }
        }

    }
    else
    {
        CMB_Gold_Reward = _CITIES[OVL_Action_Structure].size;

        if(Stack_Owner == HUMAN_PLAYER_IDX)
        {
            GUI_Multipurpose_Int += CMB_Gold_Reward;
        }

        if(Stack_Owner < _num_players)
        {
            _players[Stack_Owner].fame -= CMB_Gold_Reward;

            if(_players[Stack_Owner].fame < 0)
            {
                _players[Stack_Owner].fame = 0;
            }
        }

        CMB_Gold_Reward = CTY_GetConquerGold(OVL_Action_Structure);

        if(City_Owner < _num_players)
        {
            _players[City_Owner].gold_reserve -= CMB_Gold_Reward;

            if(_players[City_Owner].gold_reserve < 0)
            {
                _players[City_Owner].gold_reserve = 0;
            }
        }

        for(itr = 3; itr < 36; itr++)
        {
            if(_CITIES[OVL_Action_Structure].bldg_status[itr] > bs_NotBuilt)
            {
                CMB_Gold_Reward += (bldg_data_table[itr].construction_cost / 10);
            }
        }

        if(Stack_Owner < _num_players)
        {
            Player_Add_Gold(Stack_Owner, CMB_Gold_Reward);
        }

    }


    battle_units = (struct s_BATTLE_UNIT *)Allocate_First_Block(_screen_seg, 63);

    if(
        (_UNITS[troops[0]].owner_idx == NEUTRAL_PLAYER_IDX)
        &&
        ((_unit_type_table[_UNITS[troops[0]].type].Abilities & UA_FANTASTIC) != 0)
    )
    {
        for(itr = 0; itr < troop_count; itr++)
        {
            battle_units[itr].controller_idx = NEUTRAL_PLAYER_IDX;
            battle_units[itr].Status = 0;  // Unit_Active
            battle_units[itr].unit_idx = troops[itr];
        }

        _combat_total_unit_count = troop_count;

        CMB_SelectedUnit = CTY_RampageVictory();

        for(itr = 0; itr < troop_count; itr++)
        {
            UNIT_MarkRemoved(troops[itr], 0);
        }

    }


    CMB_Population_Lost = 0;

    if(Stack_Owner == HUMAN_PLAYER_IDX)
    {
        CMB_ScrollMsg_Type = 1;  // victory
    }

    if(City_Owner == HUMAN_PLAYER_IDX)
    {
        CMB_ScrollMsg_Type = 12; // city lost
    }

    CMB_Population_Lost = 0;
    CMB_Buildings_Lost = 0;

    if(
        (Stack_Owner == HUMAN_PLAYER_IDX)
        ||
        (City_Owner == HUMAN_PLAYER_IDX)
    )
    {
        Combat_Results_Scroll();
    }

}

// WZD o124p18
// AI_FightorFlight()

// WZD o124p19
// AI_Raze_Decision()

// WZD o124p20
int16_t Raze_City_Prompt(char * message)
{
    int16_t Keep_City;
    int16_t Label_Ctrl_Index;
    int16_t Esc_Hotkey_Index;
    int16_t Raze_Button_Index;
    int16_t NoRaze_Button_Index;
    int16_t input_field_idx;
    int16_t Dialog_Box_Height;
    int16_t para_height;  // _SI_
    int16_t leave_screen;  // _DI_

    Save_Alias_Colors();

    Set_Font_Colors_15(0, &COL_Dialog_Text[0]);

    // DONT EMM_Sandbox2VGAFILEH();


    // RESOURCE.LBX, 000  CONFMBAK
    // RESOURCE.LBX, 001  CONFMBAK
    confirmation_background_top_seg = LBX_Reload(cnst_RESOURCE_File2, 0, _screen_seg);
    confirmation_background_bottom_seg = LBX_Reload_Next(cnst_RESOURCE_File2, 1, _screen_seg);

    // COMPIX.LBX, 081  BASE       no raze button
    // COMPIX.LBX, 082  BASE       raze button
    confirmation_button_yes_seg = LBX_Reload_Next(cnst_COMPIX_File2, 81, _screen_seg);
    confirmation_button_no_seg = LBX_Reload_Next(cnst_COMPIX_File2, 82, _screen_seg);


    Copy_On_To_Off_Page();

    message_box_text = message;

    Set_Font_Style(4, 4, 15, ST_NULL);

    para_height = Get_Paragraph_Max_Height(166, message);

    Dialog_Box_Height = (34 + para_height);

    message_box_x = (68 + confirmation_box_x_offset);

    message_box_y = (confirmation_box_y_offset + ((SCREEN_HEIGHT - Dialog_Box_Height) / 2));

    Clear_Fields();

    NoRaze_Button_Index = Add_Button_Field((message_box_x + 101), (message_box_y + para_height + 15), "", confirmation_button_yes_seg, 'N', ST_UNDEFINED);

    Raze_Button_Index = Add_Button_Field((message_box_x + 18), (message_box_y + para_height + 15), "", confirmation_button_no_seg, 'R', ST_UNDEFINED);

    Label_Ctrl_Index = Add_Hidden_Field(message_box_x, message_box_y, (message_box_x + 185), (message_box_y + 63), ST_NULL, ST_UNDEFINED);

    Esc_Hotkey_Index = Add_Hidden_Field(0, 0, SCREEN_XMAX, SCREEN_YMAX, '\x1B', ST_UNDEFINED);

    Assign_Auto_Function(Raze_City_Prompt_Draw, 1);

    leave_screen = ST_FALSE;

    while(leave_screen == ST_FALSE)
    {
        input_field_idx = abs(Get_Input());

        if(
            (input_field_idx == ST_UNDEFINED)
            ||
            (input_field_idx == Raze_Button_Index)
        )
        {
            leave_screen = ST_TRUE;
            Keep_City = ST_FALSE;
        }

        if(input_field_idx == NoRaze_Button_Index)
        {
            leave_screen = ST_TRUE;
            Keep_City = ST_TRUE;
        }

        Raze_City_Prompt_Draw();
        PageFlip_FX();
    }

    Deactivate_Auto_Function();
    Clear_Fields();
    Restore_Alias_Colors();
    Reset_Window();
    // DONT  EMM_VGAFILEH2Sandbox()

    return Keep_City;
}


// WZD o124p21
// drake178: ; byte-identical to GUI_DrawConfrmDialog()  AKA Confirmation_Box_Draw()  GENDRAW.C
/*
    === Confirmation_Box_Draw()
        presumably copied to this overlay to avoid swapping
*/
void Raze_City_Prompt_Draw(void)
{
    int16_t paragraph_height;

    Set_Font_Style(4, 4, 15, ST_NULL);

    paragraph_height = Get_Paragraph_Max_Height(166, message_box_text);

    Set_Page_Off();

    Set_Window(0, 0, SCREEN_XMAX, (message_box_y + paragraph_height + 12));

    Clipped_Draw(message_box_x, message_box_y, confirmation_background_top_seg);

    Reset_Window();

    FLIC_Draw(message_box_x, (message_box_y + paragraph_height + 10), confirmation_background_bottom_seg);

    Set_Font_Colors_15(4, &COL_ConfirmShadows[0]);
    Set_Font_Style(4, 15, 15, ST_NULL);
    Print_Paragraph((message_box_x + 10), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned
    Print_Paragraph((message_box_x +  9), (message_box_y + 10), 166, message_box_text, 0);  // print_type 0: Print Left Aligned

    Set_Alias_Color(18);
    Set_Font_Colors_15(4, &COL_Dialog_Text[0]);
    Set_Font_Style(4, 4, 15, ST_NULL);
    Print_Paragraph((message_box_x +  9), (message_box_y +  9), 166, message_box_text, 0);  // print_type 0: Print Left Aligned

}
