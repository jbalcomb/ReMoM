SPLMASTR-Spell_Of_Mastery_Lose.md

Let's review Spell_Of_Mastery_Lose(), Spell_Of_Mastery_Lose_Draw(), and Spell_Of_Mastery_Lose_Load().

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose.asm       (ground truth — IDA Pro 5.5 disassembly)
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose.c         (Gemini translation of the .asm — second opinion, NOT ground truth)

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Draw.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Draw.c

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Load.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\WIZARDS\ovr138\Spell_Of_Mastery_Lose_Load.c

NX_j_Spell_Of_Mastery_Lose_Load()
    |-> Spell_Of_Mastery_Lose_Load()

NX_j_Spell_Of_Mastery_Lose()
    |-> Spell_Of_Mastery_Lose()

Next_Turn_Proc__WIP()
Cast_Spell_Overland_Do()
AI_Next_Turn()
    |-> j_Cast_Spell_Overland()
        |-> Cast_Spell_Overland()
            |-> j_Spell_Of_Mastery()
                |-> Spell_Of_Mastery()
                    |-> Spell_Of_Mastery_Lose()
                        |-> Spell_Of_Mastery_Lose_Load()
                        |-> Spell_Of_Mastery_Lose_Draw()
                            |-> Vanish_Bitmap__WIP()
                                |-> RNG_GFX_Random__WIP()
                                |-> rnd_bitfiddle__1oom()
                        |-> Lose_Animation()
                            |-> Lose_Animation_Draw()

MoO2
    Play_Diplomatic_Win_Flic_()     |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
    Play_Normal_Win_Flic_()         |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
    Play_Antaran_Win_Flic_()        |-> Play_Cinematic_() ... Open_Smacker_Animation(); Draw_Smacker_Animation(); Close_Smacker_Animation(); 
...why no Play_.*_Lose_Flic_()? ...Play_Loose_Flic_()?

MUSIC.LBX
// MUSIC.LBX, 109 "MOM64 XM"    "Winning Military"
// MUSIC.LBX, 110 "MOM66 XM"    "Losing Military"
// MUSIC.LBX, 111 "MOM65 XM"    "Losing Magic"

SPELLOSE.LBX

---
