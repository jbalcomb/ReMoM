




m_diplomac_player_idx

_players[].Dipl.Hidden_Rel[]
_players[].Dipl.Visible_Rel[]
_players[].Dipl.Dipl_Status[]

_players[].Dipl.Treaty_Interest[]
_players[].Dipl.Trade_Interest[]
_players[].Dipl.Peace_Interest[]

TBL_AI_PRS_IDK_Mod[_players[].Personality]

m_diplomacy_message_record_data
m_diplomsg_1_record_sub_number
G_DIPL_ComposedMessage
G_DIPL_TempMessage


## m_diplomacy_message_record_data
DIPLOMSG record
    G_DiploMsg_E0_Field0    {0,1,2}
    G_DiploMsg_E0_Field2    {0,1,2,3,4,5}
    G_DiploMsg_E0_Field4    {0,1,2,3,4,5,6,7,8,9,10,12,15}  no 11, 13, 14
set by LBX_Load_Data_Static(), in IDK_DIPLOMSG_s732D9()
    and passed to IDK_Diplomacy_Background_Music__STUB(m_diplomacy_message_record_data.IDK_mood)
¿ good, bad, neutral ? message type/mood
tested against word_42E74, updates if different
    0 or 1 plays sound_buffer1
    2 plays sound_buffer2

word_42E80 = m_diplomacy_message_record_data.IDK_group

if(_num_players == 2)
    m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 2);
else
    m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 1);



## m_diplomsg_1_record_sub_number

IDK_DIPLOMSG_s732D9()
    if(_num_players == 2)
        m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 2);
    else
        m_diplomsg_1_record_sub_number = (Random(m_diplomacy_message_record_data.IDK_count) - 1);
    LBX_Load_Data_Static(diplomsg_lbx_file__ovr86, 1, (SAMB_ptr)G_DIPL_ComposedMessage, m_diplomsg_1_record_sub_number + (diplomsg_1_record_number * 15), 1, 200);

XREF:
    Diplomacy_Screen__WIP+8B      mov     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED
    sub_6F51A+118                 mov     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED
    sub_70A1A+66                  mov     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED        
    IDK_Dipl_s72690+8B            mov     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED        
    IDK_Dipl_s72690+11D           mov     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED        
    DIPL_AI_To_AI+263             mov     dx, [m_diplomsg_1_record_sub_number]            
    IDK_DIPLOMSG_s732D9+18        cmp     [m_diplomsg_1_record_sub_number], e_ST_UNDEFINED
    IDK_DIPLOMSG_s732D9:loc_7333A mov     [m_diplomsg_1_record_sub_number], ax            
    IDK_DIPLOMSG_s732D9+64        cmp     [m_diplomsg_1_record_sub_number], 0             
    IDK_DIPLOMSG_s732D9+6B        mov     [m_diplomsg_1_record_sub_number], 0             
    IDK_DIPLOMSG_s732D9+81        mov     dx, [m_diplomsg_1_record_sub_number]            



## G_DIPL_ComposedMessage
G_DIPL_TempMessage

IDK_DIPLOMSG_s732D9()
...load
DIPL_LoadTalkGFX()
...allocate

¿ MoO2  Module: MSG  _native_msg ?

XREF:
    DIPL_LoadTalkGFX+48           mov     [G_DIPL_ComposedMessage], ax
    IDK_DIPLOMSG_s732D9+88        push    [G_DIPL_ComposedMessage]    
    IDK_DIPLOMSG_s732D9:loc_7337D mov     bx, [G_DIPL_ComposedMessage]
    IDK_DIPLOMSG_s732D9+373       mov     bx, [G_DIPL_ComposedMessage]
    IDK_DIPLOMSG_s732D9+38F       push    [G_DIPL_ComposedMessage]    
    IDK_DIPLOMSG_s732D9:loc_73685 push    [G_DIPL_ComposedMessage]    

## G_DIPL_TempMessage

XREF:
    Diplomacy_Screen_Draw__WIP+83  push    [G_DIPL_TempMessage]
    sub_6ECD9+3F                   push    [G_DIPL_TempMessage]
    sub_6ECD9+64                   push    [G_DIPL_TempMessage]
    sub_6ECD9+73                   push    [G_DIPL_TempMessage]
    sub_6ED5D+58                   push    [G_DIPL_TempMessage]
    sub_6ED5D+77                   push    [G_DIPL_TempMessage]
    sub_6ED5D+8B                   push    [G_DIPL_TempMessage]
    IDK_Diplo_Scrn+A5              push    [G_DIPL_TempMessage]
    sub_6EFA5+28                   push    [G_DIPL_TempMessage]
    sub_6F3E4+94                   push    [G_DIPL_TempMessage]
    sub_6F6BB+100                  push    [G_DIPL_TempMessage]
    sub_6F6BB+10F                  push    [G_DIPL_TempMessage]
    sub_6F6BB+123                  push    [G_DIPL_TempMessage]
    sub_6F6BB+132                  push    [G_DIPL_TempMessage]
    sub_6F6BB+141                  push    [G_DIPL_TempMessage]
    sub_6F6BB+19E                  push    [G_DIPL_TempMessage]
    _sub_6F90E_Draw+5A             push    [G_DIPL_TempMessage]
    sub_6F982+28                   push    [G_DIPL_TempMessage]
    DIPL_LoadTalkGFX+3B            mov     [G_DIPL_TempMessage], ax
    sub_71B90+F9                   push    [G_DIPL_TempMessage]
    sub_71B90+3E8                  push    [G_DIPL_TempMessage]
    sub_72131+201                  push    [G_DIPL_TempMessage]
    sub_72131:loc_72452            push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+179             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+198             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+214             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+324             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+354             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+363             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+39A             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6+3B9             push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6_Draw+5A         push    [G_DIPL_TempMessage]
    DIPL_sub_72DB6_Draw+7E         push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+AD         mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+B5         mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_00 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:loc_733B9  push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:loc_733C4  push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_01 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_02 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+134        mov     ax, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+161        mov     ax, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_05 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_07 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_08 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_09 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_10 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+208        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_11 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+22D        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+24A        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_12 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_13 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_14 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_15 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+2C4        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+2E1        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_16 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_17 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_18 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_19 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:sw_cmsg_20 mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9:loc_7365B  mov     bx, [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+38B        push    [G_DIPL_TempMessage]
    IDK_DIPLOMSG_s732D9+3B0        push    [G_DIPL_TempMessage]




## m_diplomac_player_idx








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









OSG

Page 365  (PDF Page 366)

A Computer Player's Reaction Modifier Temporay Variables  
Besides a wizard's fairly stable Core Reaction, most of the diplomatic decisions she makes are also affected by more volatile temporary modifier variables.  
These apply in each of the following decision making areas and are kept track of separately:  
• Temporary Treaty Modifier (for Wizard's Pacts and Alliances)  
• Temporary Peace Modifier (for Peace Treaties)  
• Temporary Exchange Modifier (for spell exchanges)  
• Temporary Cold Shoulder Modifier (for their leader's patience)  

Page 366  (PDF Page 367)

"No" Really Means "Stop Asking"

Every parlay, no matter its outcome, inflicts a -10 point penalty to each of these fom temporary modifiers. (In effect, you're wearing down that wizard's patience.)  
If your diplomatic offer is rejected, then the temporary modifier for the area you were specifically rejected in receives a -30 point penalty instead of the usual -10 point penalty that would have occurred had she agreed.  
These temporary modifier penalties are cumulative.  
They are adjusted back toward zero at the rate of 10 points per turn per category.  
The principle behind these temporary modifiers is quite simple.  
They're designed to prevent you from annoying computer players with incessant demands on their diplomatic time.  
They simulate that point where computer players simply get tired of dealing with you and, consequently, will be more likely to say "No" to any of your proposals.  
It might help to think of tl1ese temporary modifiers as "patience modifiers" instead.  
