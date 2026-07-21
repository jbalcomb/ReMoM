# STUB and WIP Function TODO List

## AdvsrScr
- [ ] Cartograph_Screen_Draw_Map__WIP
- [ ] Cartographer_Screen__WIP
- [ ] Cartographer_Screen_Draw__WIP

## AIDATA
- [ ] AI_Magic_Strategy__WIP

## AIDUDES
- [ ] AI_Landmass_Values_And_Strengths
- [ ] AI_Next_Turn
- [ ] CONTX_CreateChains__WIP
- [ ] CONTX_CreateLChains__WIP
- [ ] CONTXXX_Map

## AIMOVE
- [x] AI_Stacks_Assign_Target
- [x] AI_CanSettleOffPlane__STUB — renamed to AI_Find_Tower_To_Settle_Elsewhere; body was already real (mis-named), compiles clean as of 2026-05-30
- [x] AI_Evaluate_Continents
- [x] AI_Stacks_Garrison_Sites
- [x] AI_Stacks_Move_Out_NonMilitary_Garrisoned
- [x] AI_Choose_War_Landmass
- [x] AI_Stacks_Wartime_Ocean_Movement_And_Cleanup
- [x] AI_Stacks_Roamers_Target_Or_Deploy
- [x] AI_Stacks_Order_To_War_Landmass
- [x] AI_Stacks_Ferry_Add_Location
- [x] AI_Stacks_Survey_Expedition_Forces
- [x] AI_Stacks_Survey_Expedition_Forces_Stack
- [x] AI_Stacks_Reorder_Settle_Elsewhere — renamed from AI_SendToColonize__WIP; done-done 2026-06-03 (R1 + Y-bounds fixed, GEMINI variant deleted)
- [ ] AI_Set_Unit_Orders
- [x] AI_Reevaluate_Continent
- [x] AI_Stacks_Relocate_Roamers
- [x] AI_Stacks_Ocean_Landmass_Orders
- [x] AI_Stacks_Stage_Expedition_Forces
- [x] AI_Stacks_Setup_Ferry
- [ ] TILE_AI_FindEmptyLnd__WIP
- [ ] Next_Nearest_Ferry_Square
- [ ] TILE_NextFreeLand

## AISPELL
- [ ] AI_Research_Picker__STUB
- [x] AI_Spell_Select — done-done 2026-06-11 (walkthrough doc, `__STUB` suffix dropped, GEMINI deleted, variable casing normalized, 9 leaf pickers stubbed `return 0;`)
- [ ] AITP_Change_Terrain
- [ ] AITP_Spell_Ward
- [ ] AITP_Spell_Ward
- [ ] AITP_Disjunction
- [ ] AITP_Attack_Wizard
- [ ] AITP_Unit_Enchantment
- [ ] AITP_City_Enchantment

## capture
- [x] F11_Key__WIP
- [x] Screen_Flic_Capture__STUB
- [x] VGA_GetDACBlock
- [x] VGA_ReadScreenLine
- [x] Screen_Picture_Capture()  // WZD s24p07  // MoO2  Module: capture  Screen_Flic_Capture()
- [x] Capture_Screen_Block()    // WZD s24p08

## City_ovr55
- [ ] Resource_Breakdown__STUB
- [ ] Resource_Breakdown_Draw__STUB

## CITYCALC
- [ ] WIZ_Get_Astr_ArmyStr__STUB
- [ ] WIZ_Get_Astr_Power__STUB
- [ ] WIZ_Get_Astr_Resrch__STUB
- [ ] WIZ_Get_Nation_Size__STUB

## CITYSCAP
- [ ] Cityscape_Roads_1__WIP
- [ ] Cityscape_Roads_2__WIP
- [ ] Cityscape_Roads_3__WIP
- [ ] Cityscape_Roads_4__WIP
- [ ] Cityscape_Roads_5__WIP
- [ ] Cityscape_Window__WIP

## CityScr
- [ ] City_Screen__WIP
- [ ] City_Screen_Add_Fields__WIP
- [ ] City_Screen_Draw2__WIP
- [ ] City_Screen_Draw__WIP

## Combat
- [ ] AI_BU_AssignAction__WIP — renamed to AI_BU_AssignAction (CMBTAI.c:968); body substantive, needs done-done review
- [x] AI_BU_GetAttackValue__STUB
- [ ] AI_BU_ProcessAction__WIP — renamed to AI_BU_ProcessAction (CMBTAI.c:268); body substantive (holds the AI combat cast call at CMBTAI.c:521), needs done-done review
- [ ] AI_BU_SelectAction__WIP — renamed to Choose_Target_And_Action (CMBTAI.c:1153); body substantive, needs done-done review
- [ ] AI_CMB_PlayTurn__WIP — renamed to Auto_Cast_Spell_And_Do_Combat_Turn (Combat.c:4508); body substantive, needs done-done review
- [ ] AI_CombatSpellList — impl (Combat.c:25959), needs done-done review; AI Combat Spell layer
- [ ] AI_EvaluateCmbtSpell — impl (Combat.c:24632), needs done-done review; AI Combat Spell layer
- [x] AI_FightorFlight__STUB
- [x] AI_GetCombatRallyPt__WIP
- [ ] AI_MoveBattleUnits__WIP
- [ ] AI_RestrictToCity__WIP
- [ ] AI_SelectCmbtSpell — impl (Combat.c:24565), needs done-done review; AI Combat Spell layer
- [x] AI_SetBasicAttacks__WIP
- [ ] AI_SetCombatRealms — impl (Combat.c:25792), needs done-done review; AI Combat Spell layer
- [ ] AI_UnitThreatRealms — impl (Combat.c:25860), needs done-done review; AI Combat Spell layer
- [x] AITP_Combat_Spell — done-done 2026-07-07 (walkthrough doc doc/ComputerPlayer/Combat-AITP_Combat_Spell.md; faithful 1:1 to AITP_CombatSpell__STUB.asm; R1 summon-exhaustion + R2 buff-mask + R3 case-16 write-back fixed; builds clean)
- [ ] AITP_CracksCall — impl (Combat.c:14763), needs done-done review; AI Combat Spell layer
- [ ] AITP_DarknessLight — impl (Combat.c:12065), needs done-done review; AI Combat Spell layer
- [ ] AITP_DispelMagic — impl (Combat.c:12260), needs done-done review; AI Combat Spell layer
- [ ] AITP_Disrupt — impl (Combat.c:14708), needs done-done review; AI Combat Spell layer
- [ ] AITP_EarthToMud — impl (Combat.c:14588), needs done-done review; AI Combat Spell layer
- [ ] AITP_Healing — impl (Combat.c:12099), needs done-done review; AI Combat Spell layer
- [ ] AITP_HolyWord — impl (Combat.c:13068), needs done-done review; AI Combat Spell layer
- [ ] AITP_RecallHero — impl (Combat.c:14876), needs done-done review; AI Combat Spell layer
- [ ] AITP_WarpCreature — impl (Combat.c:12196), needs done-done review; AI Combat Spell layer
- [ ] AITP_WarpWood — impl (Combat.c:12144), needs done-done review; AI Combat Spell layer
- [ ] Battle_Unit_Action__WIP
- [ ] Battle_Unit_Attack__WIP
- [ ] BU_Apply_Battlefield_Effects__WIP
- [x] BU_ApplyDamage  AKA BU_ApplyDamage__WIP__SEGRAX
- [ ] BU_AttackTarget__WIP
- [ ] BU_CheckFlight__WIP
- [ ] BU_GetCombatEffect__WIP
- [ ] BU_GetInstaMoveType__WIP
- [ ] BU_HasSpellAbility__WIP
- [x] BU_IsVisible__STUB
- [ ] BU_MeleeWallCheck__WIP
- [ ] BU_MoveConfused__WIP
- [ ] BU_ProcessAttack__WIP
- [ ] BU_SetBaseAnims__WIP
- [ ] BU_SetCityMovement__WIP
- [ ] BU_SetVisibility__WIP
- [x] BU_SortSlowestFirst__WIP
- [x] Check_For_Winner__WIP
- [ ] CMB_BaseAllocs__WIP
- [ ] CMB_CE_Refresh__WIP
- [ ] CMB_ComposeBackgrnd__WIP
- [ ] CMB_ComposeBookBG__WIP
- [ ] CMB_LoadResources__WIP
- [ ] CMB_PrepareTurn__WIP
- [ ] Combat_Next_Turn — renamed from CMB_ProgressTurnFlow__WIP (Combat.c:4528); body substantive, needs done-done review
- [ ] CMB_SetNearAllocs__WIP
- [ ] CMB_SpawnDarkWall__WIP
- [ ] CMB_SpawnFigure__WIP
- [ ] CMB_SpawnFireWall__WIP
- [ ] CMB_SpawnStoneWall__WIP
- [ ] CMB_SpawnStructures__WIP
- [ ] CMB_Terrain_Init__WIP
- [ ] CMB_TileGen__WIP
- [ ] CMB_Units_Init__WIP
- [ ] Combat__WIP
- [x] Combat_Cast_Spell — done-done 2026-07-07 (walkthrough doc in doc/#CodeReview/; renamed from Combat_Cast_Spell__WIP; R1 payment-index + R2 AI-branch/spellbook-gating fixed; Dispel_Attempt/Counter_Message arg orders verified vs asm; whole tree builds clean)
- [ ] Combat_Figure_Effect__WIP
- [ ] Combat_Grid_Entities__WIP
- [ ] Combat_Grid_Entity_Create__WIP
- [ ] Combat_Screen__WIP
- [ ] Combat_Screen_Map_Draw__WIP
- [ ] Combat_Screen_Map_Draw_Entities__WIP
- [ ] Combat_Spell_Target_Screen__WIP
- [ ] Do_Legal_Spell_Check__WIP
- [ ] End_Of_Combat__WIP
- [ ] Generate_Combat_Map__WIP
- [ ] Move_Battle_Unit__WIP
- [ ] STK_CaptureCity__WIP
- [ ] STK_ComposeFleeLost__STUB
- [ ] Strategic_Combat__WIP
- [ ] UU15_AITP_Disintegrate — MISSING (comment-only, Combat.c:12245); likely not needed (Disintegrate targeted inline in AITP_Combat_Spell's resistance-kill case); verify no caller, then strike
- [ ] WIZ_GetLastRangedStr__WIP

## CONQUEST
- [x] Conquest_Animation_Draw
- [x] Conquest_Animation_Draw_Setup_Zap_Scene
- [x] Conquest_Animation_Draw_Conquerors_Approach
- [x] Conquest_Animation_Draw_Zapped_Wizard
- [x] Conquest_Animation_Draw_Zap_Strike
- [x] CP_Is_Dead
- [x] Lose_Animation
- [x] Win_Animation
- [x] Conquest_Animation_Draw_End_Pose
- [x] Banish_Wizard
- [x] Resolve_Wizard_Conquest
- [x] Conquest_Animation

## DIPLOMAC
- [ ] Change_Relations__WIP
- [ ] DIPL_sub_72DB6__STUB
- [ ] Diplomacy_Exchange_Spell__WIP
- [ ] Diplomacy_Propose_Treaty__WIP
- [ ] Diplomacy_Screen__WIP
- [ ] Diplomacy_Screen_Draw__WIP
- [ ] Diplomacy_Screen_Load__WIP
- [ ] IDK_Npc_Counteroffer__STUB
- [ ] IDK_Npc_Target_Player__STUB
- [ ] Npc_Proposal__WIP
- [ ] NPC_To_Human_Diplomacy
- [ ] NPC_To_NPC_Treaty_Negotiations__STUB

## EMM
- [ ] EMM_ReleaseAll__STUB

## EXIT
- [ ] DBG_Close_ERROR_LOG__STUB
- [ ] DBG_Open_ERROR_LOG__STUB

## Explore
- [ ] Update_Scouted_And_Contacted__WIP

## Fields
- [ ] GUI_MoveDlgHighlight__STUB

## FLIC_Draw
- [x] Shear_Bitmap_Y            // WZD s30p03
- [ ] LBX_IMG_VertWarp              // WZD s30p04
- [ ] LBX_IMG_HorzWarp              // WZD s30p05
- [ ] UU_LBX_IMG_ColorShift         // WZD s30p07
- [ ] UU_LBX_IMG_CReplaceRect       // WZD s30p08
- [ ] VGA_WndDrawRotateImg()        // WZD s30p20
- [ ] VGA_WndDrawImageRect()        // WZD s30p21
- [ ] UU_VGA_WndDrawTransform()     // WZD s30p22
- [ ] VGA_RotateRect()              // WZD s30p23
- [ ] UU_LBX_IMG_CropRect()         // WZD s30p29
- [ ] UU_LBX_IMG_FullGScale()       // WZD s30p31
- [ ] UU_LBX_IMG_ExtGScaleEC()      // WZD s30p32
- [ ] UU_LBX_IMG_ExtGrayScale()     // WZD s30p33
- [ ] UU_LBX_IMG_DrawRect()         // WZD s30p45
- [ ] VGA_DrawPartEmsImg()          // WZD s32p01
- [ ] EMM_MapNextIMGPages()         // WZD s32p02
- [ ] VGA_DrawPartEmsImg_R()        // WZD s32p03
- [ ] Add_Picture_To_Bitmap_EMM()   // WZD s33p03
- [ ] EMM_MapNextIMGPages2()        // WZD s33p04
- [ ] UU_LBX_IMG_GetIntensity()     // WZD s33p10
- [ ] Vanish_Bitmap__WIP()          // WZD s33p15
- [ ] Gray_Scale_Bitmap()           // WZD s33p16 // MoO2  Module: bitmap  Gray_Scale_Bitmap()
- [ ] LBX_IMG_RevGrayscale__STUB()  // WZD s33p17
- [ ] UU_DUP_RevGrayscale()         // WZD s33p18
- [ ] RNG_GFX_Random__WIP()         // WZD s33p19

## Fonts
- [ ] VGA_SlideColors__STUB

## Graphics
- [ ] DOS_PrintString__STUB
- [x] WZD s14p11 UU_Interlaced_Fill
- [x] WZD s14p12 UU_VGA_DrawRect
- [x] WZD s14p13 UU_VGA_WndDrawRect
- [x] WZD s14p14 UU_VGA_DrawDblRect

## INITGAME
- [x] Init_CP_Strategy
- [ ] NEWG_FinalizeTables__WIP

## Input
- [ ] Process_Direction_Key__STUB

## Items
- [ ] AI_Process_Item_Pool__STUB

## Lair
- [ ] Lair_Combat__WIP

## LOADER
- [ ] Spellbook_Load_Small_Pictures__WIP

## MainScr
- [ ] CTY_CheckSpellWard__STUB
- [ ] EarthGateTeleport__WIP
- [ ] Eval_Move_Path__WIP
- [ ] RP_CTY_CheckSpellWard__STUB
- [ ] sdl2_Play_Background_Music__WIP

## MAPGEN
- [ ] Build_Connectivity_Graph
- [ ] Build_Connectivity_Graphs
- [ ] Generate_Home_City__WIP
- [ ] Generate_Neutral_Cities__WIP

## NewGame
- [ ] GAME_WizardsLaunch__WIP
- [ ] NEWG_DrawDefShelf__WIP
- [ ] NEWG_FinalizeTables__WIP
- [ ] Newgame_Screen_1__WIP
- [ ] Newgame_Screen_2__WIP
- [ ] Newgame_Screen_3__WIP
- [ ] Newgame_Screen_3_Draw__WIP
- [ ] Newgame_Screen_4__WIP
- [ ] Newgame_Screen_4_Draw__WIP
- [ ] Newgame_Screen_6__WIP
- [ ] Newgame_Screen_6_Draw__WIP
- [ ] Newgame_Screen_7__WIP
- [ ] Newgame_Screen_7_Draw__WIP

## NEXTTURN
- [ ] Create_Unit
- [ ] Diplomacy_Growth_For_Enchantments__WIP
- [ ] Players_Apply_Upkeeps__WIP
- [ ] WIZ_MatchFoodUpkeep__WIP
- [ ] WIZ_MatchManaUpkeep__WIP

## OverSpel
- [x] Cast_Spell_Overland — done-done 2026-06-10 (walkthrough doc, Nature's Wrath wired, GEMINI variant deleted, `__WIP` suffix dropped)
- [ ] IDK_Get_Global_Enchant_Index__STUB

## ProdScr
- [ ] Building_Allows_List__WIP

## REPORT
- [ ] Chancellor_Screen__WIP
- [ ] Chancellor_Screen_Scroll_Draw__WIP

## SBookScr
- [ ] Dissolve_Main_Screen
- [ ] Spell_Animation_Load_Sound_Effect__WIP
- [ ] Spell_Animation_Screen__WIP

## SCastScr
- [ ] Spell_Casting_Screen__WIP

## SCORE
- [x] End_Of_Game_Score_Draw
- [x] End_Of_Game_Score

## SETTLE
- [ ] AI_Execute_Orders
- [ ] AI_Unit_Army_Do_Road
- [x] AI_UNIT_Meld__WIP — renamed to AI_Unit_Army_Do_Meld; body implemented in SETTLE.c:224
- [ ] AI_Unit_Army_Do_Move
- [ ] AI_Unit_Army_Do_Ferry
- [x] AI_Unit_Army_Do_Settle — renamed from AI_UNIT_Settle__WIP; body implemented; GEMINI variant deleted 2026-06-04

## SOUND
- [ ] Audio_Error__STUB
- [ ] Audio_Init__WIP
- [ ] Audio_Uninit__STUB
- [ ] Get_Music_Volume__STUB
- [ ] Set_Music_Volume__STUB
- [ ] Stop_All_Sounds__STUB
- [ ] Stop_Music__STUB

## Spellbook
- [ ] Apprentice_Screen__WIP
- [x] BigBook_PageTurn__WIP
- [ ] BigBook_Compose__WIP
- [ ] CmbBook_Compose__WIP
- [ ] CmbBook_Draw__WIP
- [ ] Combat_Spellbook_Build__WIP
- [ ] Build_Spellbook
- [ ] SBK_DrawSpellDecode__STUB
- [ ] SBK_LoadSpellDescs__WIP
- [ ] SmlBook_Compose__WIP
- [x] SmlBook_Draw__WIP
- [x] SmlBook_PageTurn__WIP
- [ ] UU_IDK_turn_to_cast__STUB

## Spells128
- [ ] WIZ_AddSpellRank__WIP
- [x] Conquest_Spells

## Spells131
- [ ] Combat_Spell_Animation__WIP

## Spells133
- [ ] Apply_Call_Chaos__WIP
- [ ] BU_LifeDrain__WIP
- [ ] Cast_Call_Chaos__WIP
- [x] CMB_CounterMessage__STUB — renamed to Combat_Spell_Counter_Message (Spells133.c:532); implemented, compiles clean 2026-07-07
- [ ] Combat_Spell_Animation_Generic__WIP
- [ ] TILE_BoltFromAbove__WIP
- [x] WIZ_DispelAttempt__STUB — renamed to Combat_Spell_Dispel_Attempt (Spells133.c:653); implemented, compiles clean 2026-07-07

## Spells137
- [ ] Cast_Spell_City_Enchantment_Animation_1__WIP
- [ ] Cast_Spell_City_Enchantment_Animation_2__WIP

## SPLMASTR
- [x] Cast_Spell_Of_Mastery
- [x] Cast_Spell_Of_Mastery_Draw
- [x] Spell_Of_Mastery_Lose_Load
- [x] Spell_Of_Mastery_Lose_Draw
- [x] Spell_Of_Mastery_Lose
- [ ] Spell_Target_Global_Enchantment_Bind__WIP
- [ ] Spell_Target_Global_Enchantment_Disjunct__WIP

## TEXTBOX
- [ ] Draw_Help_Entry__WIP

## UnitStat
- [ ] Building_Allows_List__WIP
- [ ] Thing_View_Draw__WIP

## UNITSTK
- [ ] RdBd_UNIT_MoveStack

## UnitView
- [ ] UNIT_GetDependants__WIP

## Util  (seg022)
- [ ] UU_DBG_SetSelectSetting()
- [ ] UU_DBG_SelectDialog()
- [x] Draw_Expanding_Bitmap()

## Video
- [ ] PageFlip_Dissolve

## WZD_o059
- [ ] Do_Plane_Button__WIP

## WZD_o143
- [ ] EVNT_FindNewMineral__STUB
- [ ] EVNT_TargetDepletion__STUB
- [ ] TILE_GenerateOre__STUB
- [ ] TILE_ReplaceMinerals__STUB
