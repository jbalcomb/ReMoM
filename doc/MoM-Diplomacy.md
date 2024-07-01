


    // ovr085:14A0 C7 87 5E A0 00 00                               mov     [_players.Dipl.Unknown_194h+bx], 0
    //                         ^ = 0
    // 0000006C Unknown_194h dw 6 dup(?)
    // 00000128 Dipl WIZ_DIPL ?
    // 128 + 6C = 194
    // dseg:9ECA
    // A0 5E - 9ECA = 194
    // players[] + Dipl + Unknown_194h
    // 9ECA + 128 + 6C
    // C7 87 5E A0
    // mov [_players.Dipl.Unknown_194h+bx]
    _players[attacker_idx].Dipl.Unknown_194h[defender_idx] = 0;
    _players[defender_idx].Dipl.Unknown_194h[attacker_idx] = 0;

    // 0000002A Visible_Rel db 6 dup(?)
    // 128 + 2A = 152
    //ovr085:14D8 C6 87 1C A0 9C                                  mov     [_players.Dipl.Visible_Rel+bx], -100
    //                        ^ = -100

/*
80 BF 12 A1
cmp [_players.Dipl.IDK_MoO1_bounty_collect+bx]
C6 87 12 A1
mov [_players.Dipl.IDK_MoO1_bounty_collect+bx]

ovr088:09AE C6 87 E2 A0 00                                  mov     [_players.Dipl.field_F0+bx], 0
ovr088:09BF C6 87 88 A0 00                                  mov     [_players.Dipl.Tribute_Spell+bx], 0
ovr088:09D4 C7 87 8E A0 00 00                               mov     [_players.Dipl.field_9C+bx], 0

C6 87 E2 A0
80 BF E2 A0

*/





End_Of_Combat()
    |-> Change_Relations()

1oom
game_battle_finish()
    |-> game_diplo_battle_finish()

// STARMAP.EXE  ovr070  p08
void game_diplo_act(struct game_s *g, int dv, player_id_t pi, player_id_t pi2, int dtype, uint8_t pli1, int16_t dp2)

// STARMAP.EXE  ovr070  p10
void game_diplo_break_treaty(struct game_s *g, player_id_t breaker, player_id_t victim)



ovr085:1593
89 87 74 A3
mov [_players.War_Reeval+bx], ax


## Penalty for Breaking an Alliance or a Wizard Pact

Break_Treaties()
    checks players[] treaties[] for Wizard Pact or Alliance
    -10 for Wizard Pact, -20 for Alliance
    adds that to Hidden_Rel[]
    if Alliance, also adds it to Default_Rel[]


Page 68  (PDF Page 73)
A quick note on reputation:
every treaty you break labels you as untrustworthy
and permanently worsens your relations with all wizards
(although it has the greatest effect on the wizard with whom you broke the treaty).
Just remember that every time you break a treaty
  (even accidentally!),
  the rest of the world knows and remembers.




# MoM OSG

Diplomacy Points (DPs)

Starting (Base) Diplomacy Point Levels

Computer Players Core Reaction

Core Reaction Personality Modifiers

Computer Player's Reaction Modifier Temporary Variables
Temporary Treaty Modifier
Temporary Peace Modifier
Temporary Exchnage Modifier
Temporary Cold Shoulder Modifier

Temporary Modifiers: Charismatic Advantage

Oath Breaker Penalties





6 sets of data
player 0 is human player
player 5 is neutral player



struct s_WIZARD
    /* 0128 */ struct s_WIZ_DIPL Dipl;
    /* 049E */ uint16_t Hostility[6];
    /* 04AA */ uint16_t War_Reeval;

struct s_WIZ_DIPL
    /* 0000 */ int8_t Contacted[6];
    /* 0006 */ int16_t Treaty_Interest[6];
    /* 0012 */ int16_t Peace_Interest[6];
    /* 001E */ int16_t Trade_Interest[6];
    /* 002A */ int8_t Visible_Rel[6];
    /* 0030 */ int8_t Dipl_Status[6];
    /* 0036 */ int16_t DA_Strength[6];
    /* 0042 */ int8_t Dipl_Action[6];
    /* 0048 */ int16_t DA_Spell[6];
    /* 0054 */ int8_t DA_City[6];
    /* 005A */ int8_t Default_Rel[6];
    /* 0060 */ int8_t Contact_Progress[6];
    /* 0066 */ int8_t Broken_Treaty[6];
    /* 006C */ int16_t Unknown_194h[6];
    /* 0078 */ int8_t Hidden_Rel[6];
    /* 007E */ int8_t Unknown_1A6h[24];
    /* 0096 */ int8_t Tribute_Spell[6];
    /* 009C */ int8_t Unknown_1C4h[90];
    /* 00F6 */ int16_t Tribute_Gold[6];
    /* 0102 */ int8_t Unknown_22Ah[30];
    /* 0120 */ int8_t Unknown_248h[6];
    /* 0126 */ int8_t Unknown_24Eh[6];
    /* 012C */ int8_t G_Warning_Progress[6];



##### Naming Things is Hard

Personality:
    Aggressive, Chaotic, Lawful, Maniacal, Peaceful, Ruthless
Objective:
    Expansionist, Militarist, Perfectionist, Pragmatist, Theurgist

Diplomatic actions
positively
negatively
Positive diplomatic actions improve relations
negative diplomatic actions harm relations

personalities
objectives
enemy wizards

status of your relations


Meeting Your Neighbors
ENEMY WIZARD STATISTICS
ENEMY WIZARD PERSONALITY TYPES
the personalities and objectives of enemy wizards

Page 68  (PDF Page 73)
THE ART OF DIPLOMACY


Trading, Treaties, Alliances and Pacts

a series of conversational options
conversational option
Propose Treaty
    Wizard Pact
    Alliance
    Peace Treaty
    Declaration of War on Another Wizard
    Break Alliance
    Forget It
Threaten/Break Treaty
    Break Wizard Pact
    Break Alliance
    Threaten to Attack
    Forget It
Offer Tribute
Exchange Spells
Good-bye
