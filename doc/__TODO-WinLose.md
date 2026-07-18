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



OON XREF:  Change_City_Ownership() |-> j_WIZ_Conquer() |-> WIZ_Conquer()
OON XREF:  Change_City_Ownership() |-> j_WIZ_Conquer() |-> WIZ_Conquer() |-> CP_Is_Dead()
...
    forfeited = WIZ_Banishment__STUB(city_owner_idx, player_idx);
    WIZ_Conquest__WIP(city_owner_idx, player_idx);
    GAME_LimboFallAnim__STUB(city_owner_idx);
    GAME_PlayVictoryAnim__STUB(_human_player_idx);
    GAME_OVER();
    GAME_EXE_Swap(cnst_MAGIC_EXE_File, cnst_MAGICEXE_arg0, cnst_ZeroString_7, cnst_ZeroString_7);

[ ] Change_City_Ownership()
[ ]     |-> j_Resolve_Wizard_Conquest()
[ ]         |-> Resolve_Wizard_Conquest()
[ ]             |-> Banish_Wizard()
[x]                 |-> Conquest_Animation()
[x]                 |-> Return_Animation()
[x]                 |-> Conquest_Spells()
[x]             |-> Conquest_Animation()
[x]             |-> Conquest_Spells
[ ]             |-> GAME_LimboFallAnim__STUB(city_owner_idx);
[ ]             |-> GAME_PlayVictoryAnim__STUB(_human_player_idx);
[ ]             |-> GAME_OVER();
[x]             |-> s01p16_empty_function();
[ ]             |-> GAME_EXE_Swap(cnst_MAGIC_EXE_File, cnst_MAGICEXE_arg0, cnst_ZeroString_7, cnst_ZeroString_7);

[x] Conquest_Animation()
[x]     |-> Conquest_Animation_Draw()
[x]          |-> GAME_DrawConquerors();
[x]          |-> GAME_Conqest_Scene2();
[x]          |-> GAME_DrawZappedWiz(1);
[x]          |-> GAME_DrawZapping();
[x]          |-> GAME_DrawZappedWiz(0);
[x]          |-> Conquest_Animation_Draw_End_Pose();
    // WZD 093p02   void Conquest_Animation(int16_t city_owner_idx, int16_t player_idx);
    // WZD 093p03   void Conquest_Animation_Draw(void);
    // WZD 093p04   void Conquest_Animation_Draw_Conquerors_Approach(void);
    // WZD 093p05   void Conquest_Animation_Draw_Setup_Zap_Scene(void);
    // WZD 093p06   void Conquest_Animation_Draw_Zapped_Wizard(int16_t flag);
    // WZD 093p07   void Conquest_Animation_Draw_Zap_Strike(void);
    // WZD 093p08   void Conquest_Animation_Draw_End_Pose(void);

[x] Return_Animation()
[x]     |-> Return_Animation_Draw()



/*
    WIZARDS.EXE  ovr093
*/

// WZD 093p01
void Resolve_Wizard_Conquest(int16_t city_owner_idx, int16_t player_idx, int16_t city_idx);

// WZD 093p02
void Conquest_Animation(int16_t city_owner_idx, int16_t player_idx);

// WZD 093p03
void Conquest_Animation_Draw(void);

// WZD 093p04
void GAME_DrawConquerors(void);

// WZD 093p05
void GAME_Conqest_Scene2(void);

// WZD 093p06
void GAME_DrawZappedWiz(int16_t flag);

// WZD 093p07
void GAME_DrawZapping(void);

// WZD 093p08
void sub_79907(void);

// WZD 093p09
int16_t CP_Is_Dead(void);

// WZD 093p10
void GAME_PlayVictoryAnim__STUB(int16_t player_idx);

// WZD 093p11
// GAME_Draw_WIN_Anim()

// WZD 093p12
void GAME_LimboFallAnim__STUB(int16_t player_idx);

// WZD 093p13
// GAME_DrawLimboFall()

// WZD 093p14
int16_t Banish_Wizard(int16_t loser_idx, int16_t winner_idx);

// WZD 093p15
// GAME_ReturnDialog()

// WZD 093p16
// GAME_DrawReturnAnim()
