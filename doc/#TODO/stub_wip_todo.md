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
- [ ] AI_Research_Picker__STUB — NOT FOUND: no definition in source; only a comment reference at CITYCALC.c:988 (verified 2026-07-21)
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
- [x] Screen_Flic_Capture — was Screen_Flic_Capture__STUB; suffix dropped in source, def capture.c:132 (verified 2026-07-21)
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
- [x] Find_Undead_Creator_Type — was CMB_GetUndeadCreator; done-done 2026-07-23 (Combat.c:15644; faithful 1:1, builds clean; B1 cross-player hero-slot index preserved — see doc/#CodeReview/SPLMASTR-Undead_Animation.md)
- [ ] AI_BU_AssignAction — renamed from AI_BU_AssignAction__WIP; impl (CMBTAI.c:968), needs done-done review (name verified 2026-07-21)
- [x] AI_BU_GetAttackValue__STUB
- [ ] AI_BU_ProcessAction — renamed from AI_BU_ProcessAction__WIP; impl (CMBTAI.c:268), needs done-done review (name verified 2026-07-21)
- [ ] AI_BU_SelectAction__WIP — renamed to Choose_Target_And_Action (CMBTAI.c:1153); body substantive, needs done-done review
- [ ] AI_CMB_PlayTurn__WIP — renamed to Auto_Cast_Spell_And_Do_Combat_Turn (Combat.c:4508); body substantive, needs done-done review
- [ ] AI_CombatSpellList — impl (Combat.c:25959), needs done-done review; AI Combat Spell layer
- [ ] AI_EvaluateCmbtSpell — impl (Combat.c:24632), needs done-done review; AI Combat Spell layer
- [x] AI_FightorFlight__STUB
- [x] AI_GetCombatRallyPt — was AI_GetCombatRallyPt__WIP; suffix dropped in source, def CMBTAI.c:658 (verified 2026-07-21)
- [ ] AI_MoveBattleUnits__WIP
- [ ] AI_RestrictToCity__WIP
- [ ] AI_SelectCmbtSpell — impl (Combat.c:24565), needs done-done review; AI Combat Spell layer
- [x] AI_SetBasicAttacks — was AI_SetBasicAttacks__WIP; suffix dropped in source, def CMBTAI.c:103 (verified 2026-07-21)
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
- [x] Battle_Unit_Action — done-done 2026-08-04 (walkthrough doc doc/#CodeReview/Combat-Battle_Unit_Action.md; renamed from Battle_Unit_Action__WIP, Combat.c:3259; 3 findings raised and fixed across 2 subfunctions, incl. Battle_Unit_Has_Flight dropping _UNITS[].enchantments so overland Flight never registered; Battle_Unit_Is_Within_City compared and faithful; builds clean 2026-08-04)
- [x] Battle_Unit_Attack — done-done 2026-08-04 (walkthrough doc doc/#CodeReview/Combat-Battle_Unit_Attack.md; renamed from Battle_Unit_Attack__WIP, Combat.c:17247; 9 findings raised and fixed across the function and 3 subfunctions, incl. Wall Crusher never writing the destroyed wall and 9 of 13 projectile types rendering as a lightning bolt with no sound; Melee_Animation, Set_Gibs, Clear_Gibs and Reload_Melee_Sound compared and faithful; builds clean 2026-08-04)
- [x] Battle_Unit_Special_Stats — done-done 2026-08-05 (walkthrough doc doc/#CodeReview/Combat-Init_Prepare_Etc.md; renamed from BU_Apply_Battlefield_Effects__WIP and moved to COMBINIT.c:1189; was ~92% unwritten, all thirty effect blocks written and checked against 1137 asm lines; 2 findings raised and fixed (Shatter store order had repaired an OGBUG; an else-if where the original has two ifs); node-aura realm pairing fixed via e_COMBAT_NODE_TYPE)
- [x] BU_ApplyDamage  AKA BU_ApplyDamage__WIP__SEGRAX
- [x] Battle_Unit_Attack_Target — done-done 2026-08-04 (walkthrough doc doc/#CodeReview/Combat-Battle_Unit_Attack_Target.md; renamed from BU_AttackTarget__WIP, Combat.c:16013; 4 findings fixed incl. a missing ~120-line ranged-Haste block and an out-of-bounds write into the caller's array; Check_Attack_Ranged and Apply_Fear_Attack compared and faithful; builds clean 2026-08-04)
- [x] BU_CheckFlight__WIP — done-done 2026-08-03 (walkthrough doc doc/#CodeReview/Combat-Move_Battle_Unit.md; renamed Battle_Unit_Is_Airborne, Combat.c:5938; 1:1 with ovr098/BU_CheckFlight__WIP.asm)
- [ ] Battle_Unit_Curse_Effects
- [ ] BU_GetInstaMoveType__WIP
- [ ] Battle_Unit_Has_Spell_Ability
- [x] BU_IsVisible__STUB
- [x] BU_MeleeWallCheck — done-done 2026-08-04 (walkthrough doc doc/#CodeReview/Combat-Battle_Unit_Action.md; renamed Check_Attack_Melee_City_Wall, Combat.c:14086; R3 split three Move_Flags tests back apart per asm:23/32/41; Flying Fortress OGBUG confirmed legit, teleport/merging OGBUG disproved; builds clean 2026-08-04)
- [ ] BU_MoveConfused__WIP
- [x] BU_ProcessAttack__WIP — done-done 2026-08-04 (walkthrough doc doc/#CodeReview/Combat-Battle_Unit_Process_Attack.md; renamed Battle_Unit_Process_Attack, Combat.c:16429; 24 findings raised and fixed across the function and 12 subfunctions, incl. an infinite loop and a damage-doubling heal; builds clean 2026-08-04)
- [x] BU_SetBaseAnims__WIP — done-done 2026-08-03 (walkthrough doc doc/#CodeReview/Combat-Move_Battle_Unit.md; renamed Battle_Unit_Set_Animation_Flags, Combat.c:9411; Param0 -> unit_type union arm fixed; D2 dead-argument divergence recorded)
- [ ] BU_SetCityMovement__WIP
- [x] BU_SetVisibility — done-done 2026-08-03 (walkthrough doc doc/#CodeReview/Combat-Move_Battle_Unit.md; renamed Battle_Unit_Set_Invisibility_Effect, Combat.c:9286; 1:1 with ovr105/BU_SetVisibility__WIP.asm)
- [x] BU_SortSlowestFirst__WIP
- [x] Check_For_Winner — was Check_For_Winner__WIP; suffix dropped in source, def Combat.c:9134 (verified 2026-07-21)
- [x] Allocate_Combat_Base_Blocks — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:23980, faithful against ovr163/CMB_BaseAllocs__WIP.asm
- [ ] CMB_CE_Refresh__WIP — NOT FOUND: no definition in source; appears only in comments (verified 2026-07-21)
- [ ] Combat_Compose_Background
- [ ] CMB_ComposeBookBG__WIP
- [x] Combat_Screen_Load_Resources — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:7297, faithful against ovr103/CMB_LoadResources__WIP.asm
- [x] Begin_Combat_Turn — done-done 2026-08-05 (walkthrough doc doc/#CodeReview/Combat-Init_Prepare_Etc.md; renamed from CMB_PrepareTurn__WIP, Combat.c:2635; 1 finding fixed: the Web block was hoisted out of its guard and dropped action = bua_Finished; Add_City_Damage_From_Battle_Units_Within compared and faithful)
- [ ] Combat_Next_Turn — renamed from CMB_ProgressTurnFlow__WIP (Combat.c:4528); body substantive, needs done-done review
- [x] Item_Powers_To_Attack_Attributes — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); COMBINIT.c:224, faithful against ovr116/BU_Apply_Item_Attack_Specials.asm
- [x] Spawn_Dark_Wall_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from CMB_SpawnDarkWall__WIP, Combat.c:20216, faithful against ovr153/CMB_SpawnDarkWall.asm
- [x] Spawn_Figure_Entity — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from CMB_SpawnFigure__WIP, Combat.c:20538, faithful against ovr153/CMB_SpawnFigure__WIP.asm
- [x] Spawn_Fire_Wall_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from CMB_SpawnFireWall__WIP, Combat.c:20357, faithful against ovr153/CMB_SpawnFireWall.asm
- [x] Spawn_Stone_Wall_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from CMB_SpawnStoneWall__WIP, Combat.c:20097, faithful against ovr153/CMB_SpawnStoneWall.asm
- [x] Spawn_Structure_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from CMB_SpawnStructures__WIP, Combat.c:20038, faithful against ovr153/CMB_SpawnStructures.asm
- [x] Build_Battlefield — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:21787, faithful against ovr154/CMB_Terrain_Init__WIP.asm
- [x] Set_Terrain_Tile_Types — done-done 2026-08-08 (doc/#CodeReview/Combat-Generate_Combat_Map.md); renamed from CMB_TileGen__WIP, Combat.c:22678, faithful against ovr154/CMB_TileGen__WIP.asm
- [x] Prepare_All_Battle_Units — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); renamed from CMB_Units_Init__WIP, Combat.c:5041, faithful against ovr098/CMB_Units_Init__WIP.asm; 2 OGBUGs preserved
- [x] Combat — done-done 2026-08-13 (doc/#CodeReview/Combat-Combat.md); renamed from Combat__WIP, Combat.c:3101, faithful against ovr096/Combat__WIP.asm; 1 finding and 5 deviations applied, builds clean; Lair_Make_Guardians (Combat.c:3447), Lair_Combat_Do (Combat.c:3487) and Update_Defender_Hostility (Combat.c:3503) walked with it and are faithful
- [x] Combat_Figure_Effect — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from Combat_Figure_Effect__WIP, Combat.c:7561, faithful against ovr105/Combat_Figure_Effect__WIP.asm
- [x] Combat_Grid_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); renamed from Combat_Grid_Entities__WIP, Combat.c:5899, faithful against ovr099/Combat_Grid_Entities__WIP.asm
- [x] Combat_Grid_Entity_Create — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); Combat.c:20942, faithful against ovr153/Combat_Grid_Entity_Create__WIP.asm
- [x] Combat_Screen__WIP — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen.md); renamed Combat_Screen, Combat.c:1405, faithful against ovr090/Combat_Screen__WIP.asm
- [x] Combat_Screen_Map_Draw — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); Combat.c:19602, faithful against ovr153/Combat_Screen_Map_Draw__WIP.asm
- [x] Combat_Screen_Map_Draw_Entities — done-done 2026-08-08 (doc/#CodeReview/Combat-Combat_Screen_Map_Draw.md); Combat.c:19908, faithful against ovr153/Combat_Screen_Map_Draw_Entities__WIP.asm
- [ ] Combat_Spell_Target_Screen__WIP
- [x] Combat_Cast_Spell — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); CMBMAGIC.c:218, faithful against ovr112/Combat_Cast_Spell__WIP.asm (1,379 asm lines); walked phase-for-phase 2026-07-07, verdict carried when Combat-Combat_Cast_Spell.md was retired into the session doc
- [x] Combat_Cast_Spell_Error — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); Combat.c:6992, faithful against ovr103/zCombat_Cast_Spell_Error.asm (26 asm lines)
- [x] Combat_Cast_Spell_Message — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); Combat.c:12369, faithful against ovr113/Combat_Cast_Spell_Message.asm (276 asm lines)
- [x] Combat_Cast_Spell_With_Caster — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); Combat.c:7206, faithful against ovr103/zCombat_Cast_Spell_With_Caster.asm (111 asm lines); swapped branch arms and two switch statements reconstructed as if chains, all fixed
- [x] Combat_Casting_Cost_Multiplier — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); CMBMAGIC.c:1448, faithful against ovr112/Combat_Casting_Cost_Multiplier.asm (109 asm lines); duplicated wrap correction, min/max inversion and two flattened early returns, all fixed
- [x] Do_Legal_Spell_Check — done-done 2026-08-18 (doc/#CodeReview/Combat-Spell_Cast.md); CMBMAGIC.c:1072, faithful against ovr112/Do_Legal_Spell_Check__WIP.asm (767 asm lines); two inverted controller_idx tests fixed
- [x] End_Of_Combat — done-done 2026-08-10 (doc/#CodeReview/Combat-End_Of_Combat.md); renamed from End_Of_Combat__WIP, Combat.c:14752, faithful against ovr123/End_Of_Combat__WIP.asm; review covers the whole ovr123 overlay plus the ovr098 retreat chain (Retreat_From_Combat, Process_Retreating_Units, Build_Flee_Loss_Message) and ovr124/Raze_Check — 11 functions, 6,028 asm lines, 44 findings
- [x] Generate_Combat_Map — done-done 2026-08-08 (doc/#CodeReview/Combat-Generate_Combat_Map.md); renamed from Generate_Combat_Map__WIP, Combat.c:21438, faithful against ovr154/Generate_Combat_Map.asm; all eight subfunctions implemented, renamed and walked
- [x] Move_Battle_Unit__WIP — done-done 2026-08-03 (walkthrough doc doc/#CodeReview/Combat-Move_Battle_Unit.md; suffix already dropped in source, Combat.c:3067; instant-move flat-2 deduction and move_anim_ctr increment fixed; D1 pacing/sound-size divergences recorded)
- [ ] STK_CaptureCity__WIP
- [x] Build_Flee_Loss_Message — done-done 2026-08-10 (doc/#CodeReview/Combat-End_Of_Combat.md); renamed from STK_ComposeFleeLost__STUB, Combat.c:5195, faithful against ovr098/STK_ComposeFleeLost__STUB.asm
- [x] Strategic_Combat — done-done 2026-08-11 (doc/#CodeReview/Combat-Strategic_Combat.md); renamed from Strategic_Combat__WIP, Combat.c:8214, faithful against ovr110/Strategic_Combat__STUB.asm; 8 findings and 5 deviations applied, all locals named, frame order preserved, builds clean; Strategic_Combat_Allocate (Combat.c:8196) walked with it
- [ ] UU15_AITP_Disintegrate — MISSING (comment-only, Combat.c:12245); likely not needed (Disintegrate targeted inline in AITP_Combat_Spell's resistance-kill case); verify no caller, then strike
- [ ] WIZ_GetLastRangedStr__WIP

- [x] Add_Combat_Enchantment_Fields — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:4730, faithful against ovr098/Add_Combat_Enchantment_Fields.asm (103 asm lines)
- [x] Auto_Cast_Spell_And_Do_Combat_Turn — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:3650, faithful against ovr098/Auto_Cast_Spell_And_Do_Combat_Turn.asm (40 asm lines)
- [x] Combat_Cache_Write — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:22825, faithful against ovr154/Combat_Cache_Write.asm (64 asm lines)
- [x] Combat_Info_Effects_Count — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:6569, faithful against ovr103/Combat_Info_Effects_Count.asm (139 asm lines)
- [x] Combat_Node_Type — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:6654, faithful against ovr103/Combat_Node_Type.asm (82 asm lines)
- [x] Combat_Screen_Draw — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:5207, faithful against ovr099/Combat_Screen_Draw.asm (644 asm lines)
- [x] Combat_Structure — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:16227, faithful against ovr124/Combat_Structure.asm (84 asm lines)
- [x] Draw_Combat_Unit_Display — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:7977, faithful against ovr105/Draw_Combat_Unit_Display.asm (566 asm lines)
- [x] Draw_Spell_Information_Window — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:5595, faithful against ovr099/Draw_Spell_Information_Window.asm (125 asm lines)
- [x] Load_Combat_Terrain_Pictures — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:22885, faithful against ovr163/Load_Combat_Terrain_Pictures.asm (161 asm lines)
- [x] Map_Tile_EMS_Page_As_Sandbox — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:20387, faithful against ovr153/EMM_TILEX_Init__HACK.asm (41 asm lines)
- [x] Move_Confused — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:7042, faithful against ovr103/Move_Confused.asm (77 asm lines)
- [x] Next_Battle_Unit — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:6778, faithful against ovr103/Next_Battle_Unit.asm (89 asm lines)
- [x] Switch_Active_Battle_Unit — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:2709, faithful against ovr091/Switch_Active_Battle_Unit.asm (16 asm lines)
- [x] Turn_Off_Auto_Combat — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:5120, faithful against ovr098/Turn_Off_Auto_Combat.asm (12 asm lines)
- [x] Undeployable_Battle_Units_On_Water — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:16278, faithful against ovr124/Undeployable_Battle_Units_On_Water.asm (135 asm lines)
- [x] Update_Combat_Enchantments_Icon_And_Help — done-done 2026-08-15 (doc/#CodeReview/Combat-Combat_Screen.md); Combat.c:3547, faithful against ovr098/Update_Combat_Enchantments_Icon_And_Help.asm (108 asm lines)
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
- [ ] Change_Relations — renamed from Change_Relations__WIP; impl (DIPLOMAC.c:3450), needs done-done review (name verified 2026-07-21)
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
- [ ] NPC_To_NPC_Treaty_Negotiations — renamed from NPC_To_NPC_Treaty_Negotiations__STUB; impl (DIPLOMAC.c:4894), needs done-done review (name verified 2026-07-21)

## EMM
- [ ] EMM_ReleaseAll__STUB

## EXIT
- [ ] DBG_Close_ERROR_LOG__STUB
- [ ] DBG_Open_ERROR_LOG__STUB

## Explore
- [ ] Update_Scouted_And_Contacted — renamed from Update_Scouted_And_Contacted__WIP; impl (Explore.c:292), needs done-done review (name verified 2026-07-21)

## Fields
- [ ] GUI_MoveDlgHighlight__STUB

## FLIC_Draw
- [x] Shear_Bitmap_Y            // WZD s30p03
- [x] Shear_Array_Bitmap_Y      // WZD s30p04; was LBX_IMG_VertWarp; done-done 2026-07-23 (FLIC_Draw.c:696; faithful 1:1, builds clean)
- [x] Shear_Array_Bitmap_X      // WZD s30p05; was LBX_IMG_HorzWarp; done-done 2026-07-23 (FLIC_Draw.c:813; faithful 1:1, builds clean; B1-B3 OG bugs preserved)
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
- [x] Inverse_Gray_Scale_Bitmap — done-done 2026-08-08; renamed from LBX_IMG_RevGrayscale__STUB, FLIC_Draw.c:3983, faithful against seg033/LBX_IMG_RevGrayscale.asm  // WZD s33p17
- [x] DUPE_Inverse_Gray_Scale_Bitmap — done-done 2026-08-08; FLIC_Draw.c:4048, byte-identical copy, faithful against seg033/UU_DUP_RevGrayscale.asm  // WZD s33p18
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
- [ ] Spellbook_Load_Small_Pictures — renamed from Spellbook_Load_Small_Pictures__WIP; impl (LOADER.c:1614), needs done-done review (name verified 2026-07-21)

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
- [ ] Generate_Neutral_Cities — renamed from Generate_Neutral_Cities__WIP; impl (MAPGEN.c:5511), needs done-done review (name verified 2026-07-21)

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
- [ ] Diplomacy_Growth_For_Enchantments — renamed from Diplomacy_Growth_For_Enchantments__WIP; impl (NEXTTURN.c:4125), needs done-done review (name verified 2026-07-21)
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
- [x] Combat_Spellbook_Build
- [ ] Build_Spellbook
- [ ] SBK_DrawSpellDecode__STUB
- [ ] BigBook_Load_Spell_Descriptions
- [ ] SmlBook_Compose__WIP
- [x] SmlBook_Draw — was SmlBook_Draw__WIP; suffix dropped in source, def Spellbook.c:1526 (verified 2026-07-21)
- [x] SmlBook_PageTurn__WIP
- [ ] UU_IDK_turn_to_cast__STUB

- [x] Combat_Compose_Spellbook_Background — done-done 2026-08-16 (doc/#CodeReview/Combat-Combat_Spellbook_Screen.md); CMBMAGIC.c:157, faithful against ovr112/CMB_ComposeBookBG__WIP.asm (24 asm lines)
- [x] Combat_Spellbook_Mana_Adder_Draw — done-done 2026-08-16 (doc/#CodeReview/Combat-Combat_Spellbook_Screen.md); SPLMASTR.c:677, faithful against ovr136/Combat_Spellbook_Mana_Adder_Draw.asm (259 asm lines)
- [x] Combat_Spellbook_Mana_Adder_Load — done-done 2026-08-16 (doc/#CodeReview/Combat-Combat_Spellbook_Screen.md); SPLMASTR.c:650, faithful against ovr136/Combat_Spellbook_Mana_Adder_Load.asm (85 asm lines)
- [x] Combat_Spellbook_Mana_Adder_Screen — done-done 2026-08-16 (doc/#CodeReview/Combat-Combat_Spellbook_Screen.md); SPLMASTR.c:732, faithful against ovr136/Combat_Spellbook_Mana_Adder_Screen.asm (341 asm lines)
## Spells128
- [ ] WIZ_AddSpellRank__WIP
- [x] Conquest_Spells

## Spells131
- [ ] Combat_Spell_Animation__WIP

## Spells133
- [ ] Apply_Call_Chaos__WIP
- [ ] BU_LifeDrain__WIP
- [ ] Cast_Call_Chaos__WIP
- [x] Combat_Spell_Counter_Message — done-done 2026-08-17 (doc/#CodeReview/Combat-Combat_Spell_Dispel.md); Spells133.c:532, faithful against ovr133/Combat_Spell_Dispel_Attempt.asm (375 asm lines); R1 declaration order fixed
- [ ] Combat_Spell_Animation_Generic__WIP
- [ ] TILE_BoltFromAbove__WIP
- [x] Combat_Spell_Dispel_Attempt — renamed to Combat_Spell_Dispel_Attempt (Spells133.c:653); implemented, compiles clean 2026-07-07

- [x] Combat_Spell_Counter_Message_Box_Draw — done-done 2026-08-17 (doc/#CodeReview/Combat-Combat_Spell_Dispel.md); Combat.c:12364, faithful against ovr113/Combat_Spell_Dispel_Attempt.asm (142 asm lines)
- [x] Combat_Spell_Dispel_Attempt — done-done 2026-08-17 (doc/#CodeReview/Combat-Combat_Spell_Dispel.md); Spells133.c:651, faithful against ovr133/Combat_Spell_Dispel_Attempt.asm (58 asm lines)
## Spells137
- [ ] Cast_Spell_City_Enchantment_Animation_1__WIP
- [ ] Cast_Spell_City_Enchantment_Animation_2__WIP
- [x] Summon_Animation — was IDK_SummonAnim; done-done 2026-07-21 (walkthrough doc/#CodeReview/Spells137-Summon_Animation.md; faithful 1:1)
- [x] Summon_Animation_Draw — was IDK_SummonAnim_Draw; done-done 2026-07-21
- [x] Summon_Animation_Load — was IDK_SummonAnim_Load; done-done 2026-07-21 (former R1 monster-index resolved faithful; B1 type_or_other==0 gap preserved)

## SPLMASTR
- [x] Cast_Spell_Of_Mastery
- [x] Cast_Spell_Of_Mastery_Draw
- [x] Spell_Of_Mastery_Lose_Load
- [x] Spell_Of_Mastery_Lose_Draw
- [x] Spell_Of_Mastery_Lose
- [x] Spell_Of_Mastery_Load
- [x] Spell_Of_Mastery_Draw
- [x] Spell_Of_Mastery
- [x] Undead_Animation — was CMB_CreateUndeadAnim; done-done 2026-07-23 (walkthrough doc/#CodeReview/SPLMASTR-Undead_Animation.md; faithful 1:1, builds clean)
- [x] Undead_Animation_Draw — was CMB_DrawUndeadAnim; done-done 2026-07-23
- [x] Undead_Animation_Load — was CMB_LoadUndeadAnim; done-done 2026-07-23 (segment globals declared+renamed; uy_BeastSwordsmen resolved to MONSTER.LBX 47)
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
