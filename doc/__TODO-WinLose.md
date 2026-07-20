__TODO-WinLose.md

MoM - Win / Lose

When/How?

Banishment
...attack second-to-last player's Fortress City and be the Combat Winner

Cast 'Spell of Mastery'



CP Banish CP
HP Banish CP
CP Banish HP

HP Banish last CP
CP Banish last HP

HP cast 'Spell of Return'

CP cast 'Spell of Mastery'
HP cast 'Spell of Mastery'


WIZARDS.EXE
ovr093
CONQUEST.c



OON XREF:  Change_City_Ownership() |-> j_Resolve_Wizard_Conquest() |-> Resolve_Wizard_Conquest()
OON XREF:  Change_City_Ownership() |-> j_Resolve_Wizard_Conquest() |-> Resolve_Wizard_Conquest() |-> CP_Is_Dead()

[ ] Change_City_Ownership()
[x]     |-> j_Resolve_Wizard_Conquest()
[x]         |-> Resolve_Wizard_Conquest()
[x]             |-> Banish_Wizard()
[x]                 |-> Conquest_Animation()
[x]                 |-> Return_Animation()
[x]                 |-> Conquest_Spells()
[x]             |-> Conquest_Animation()
[x]             |-> Conquest_Spells
[x]             |-> Lose_Animation()
[x]             |-> Win_Animation()
[x]             |-> End_Of_Game_Score();
[x]             |-> s01p16_empty_function();
[❌]             |-> Respawn()

¿ should be Banish_Animation() ? ¿ Conquest is unclear relative to Lose/Win ?
[x] Conquest_Animation()
[x]     |-> Conquest_Animation_Draw()
[x]          |-> Conquest_Animation_Draw_Conquerors_Approach();
[x]          |-> Conquest_Animation_Draw_Setup_Zap_Scene();
[x]          |-> Conquest_Animation_Draw_Zapped_Wizard();
[x]          |-> Conquest_Animation_Draw_Zap_Strike();
[x]          |-> Conquest_Animation_Draw_End_Pose();

[x] Return_Animation()
[x]     |-> Return_Animation_Draw()

[x] Lose_Animation()
[x]     |-> Lose_Animation_Draw()

[x] Win_Animation()
[x]     |-> Win_Animation_Draw()

[x] End_Of_Game_Score()
[x]     |-> End_Of_Game_Score_Draw()

spell of return over?
spell of mastery win * lose
switch EXE



// WZD s30p03
[x] Shear_Bitmap_Y
// WZD s30p04
// LBX_IMG_VertWarp
// WZD s30p05
// LBX_IMG_HorzWarp

// WZD o138p01
void Spell_Of_Mastery_Lose_Load(int16_t wizard_id)
// WZD o138p02
void Spell_Of_Mastery_Lose_Draw(void)
// WZD o138p03
void Spell_Of_Mastery_Lose(void)

[x] Cast_Spell_Of_Mastery()
[x]     |-> Cast_Spell_Of_Mastery_Draw()

// WZD o138p06
void Spell_Of_Mastery_Load(void)
// WZD o138p07
void Spell_Of_Mastery_Draw(void)
// WZD o138p08
void Spell_Of_Mastery(int16_t player_idx)
