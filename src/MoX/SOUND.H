/*
    WIZARDS.EXE
        seg038  "SOUND"

MoO2
    Module: sound

*/
#ifndef SOUND_H
#define SOUND_H



#include "MoX_TYPE.H"



#ifdef __cplusplus
extern "C" {
#endif



/*
FFFFFFFF ; enum e_Sound_Hardware
FFFFFFFF No_Sound_HW  = 0
FFFFFFFF PC_Speaker  = 1
FFFFFFFF AdLib  = 16
FFFFFFFF SB_Orig  = 18
FFFFFFFF SB_Pro  = 24
FFFFFFFF Pro_Audio_S  = 25
FFFFFFFF SB_Pro_OPL3  = 32
FFFFFFFF Pro_Audio_S_16  = 33
FFFFFFFF AdLib_Gold  = 34
FFFFFFFF Roland_MT32  = 48
FFFFFFFF Roland_LAPC  = 49
FFFFFFFF General_MIDI  = 56
*/

enum e_SND_DRV
{
    SND_NONE     =  0,
    SND_Speaker  =  1,
    SND_AdLib    =  2,
    SND_SB       =  3,
    SND_SBPro    =  4,
    SND_SBPro2   =  5,
    SND_PAS      =  6,
    SND_PAS16    =  7,
    SND_ALG      =  8,
    SND_Roland   =  9,
    SND_GMIDI    = 10
};




/*
MUSIC.LBX
Entries: 116
{  0, ..., 115 }

*/
enum MUSIC_Tracks
{
    MUSIC_Good_Arcane           =   0, /* "MOM81 XM"    "Good Arcane"                   */
    MUSIC_Good_Chaos            =   1, /* "MOM103 X"    "Good Chaos"                    */
    MUSIC_Good_Death            =   2, /* "MOM75 XM"    "Good Death"                    */
    MUSIC_Good_Life             =   3, /* "MOM77 XM"    "Good Life"                     */
    MUSIC_Good_Nature           =   4, /* "MOM73 XM"    "Good nature"                   */
    MUSIC_Good_Sorcery          =   5, /* "MOM79 XM"    "Good Sorcery"                  */
    MUSIC_Bad_Arcane            =   6, /* "MOM82 XM"    "Bad Arcane"                    */
    MUSIC_Bad_Chaos             =   7, /* "MOM104 X"    "Bad Chaos"                     */
    MUSIC_Bad_Death             =   8, /* "MOM76 XM"    "Bad Death"                     */
    MUSIC_Bad_Life              =   9, /* "MOM78 XM"    "Bad Life"                      */
    MUSIC_Bad_Nature            =  10, /* "MOM74 XM"    "Bad Nature"                    */
    MUSIC_Bad_Sorcery           =  11, /* "MOM80 XM"    "Bad Sorcery"                   */
    MUSIC_SoM_Started           =  12, /* "MOM62 XM"    "Spell Mastery Started"         */
    MUSIC_Weak_Summon           =  13, /* "MOM56 XM"    "Weak Creature Summons"         */
    MUSIC_Avg_Summon            =  14, /* "MOM57 XM"    "Avg Creature Summons"          */
    MUSIC_Powerful_Summon       =  15, /* "MOM58 XM"    "Powerful Creature Summ"        */
    MUSIC_Mighty_Summon         =  16, /* "MOM59 XM"    "Mighty Creature Summon"        */
    MUSIC_Spell_Failure         =  17, /* "MOM53 XM"    "Spell Failure Theme"           */
    MUSIC_Upkeep_Warning        =  18,
    MUSIC_Eternal_Night         =  19,
    MUSIC_Evil_Omens            =  20,
    MUSIC_Zombie_Mastery        =  21,
    MUSIC_Aura_of_Majesty       =  22,
    MUSIC_Wind_Mastery          =  23,
    MUSIC_Suppress_Magic        =  24,
    MUSIC_Time_Stop             =  25,
    MUSIC_Nature_Awareness      =  26,
    MUSIC_Force_of_Nature       =  27,
    MUSIC_Herb_Mastery          =  28,
    MUSIC_Chaos_Surge           =  29,
    MUSIC_Doom_Mastery          =  30,
    MUSIC_Great_Wasting         =  31,
    MUSIC_Meteor_Storm          =  32,
    MUSIC_Armageddon            =  33,
    MUSIC_Tranquility           =  34,
    MUSIC_Life_Force            =  35,
    MUSIC_Crusade               =  36,
    MUSIC_Just_Cause            =  37,
    MUSIC_Holy_Arms             =  38,
    MUSIC_Wrath_of_God          =  39,
    MUSIC_New_Spell             =  40,
    MUSIC_Merlin_Good           =  41,
    MUSIC_Merlin_Bad            =  42,
    MUSIC_Raven_Good            =  43,
    MUSIC_Raven_Bad             =  44,
    MUSIC_Sharee_Good           =  45,
    MUSIC_Sharee_Bad            =  46,
    MUSIC_LoPan_Good            =  47,
    MUSIC_LoPan_Bad             =  48,
    MUSIC_Jafar_Good            =  49,
    MUSIC_Jafar_Bad             =  50,
    MUSIC_Oberic_Good           =  51,
    MUSIC_Oberic_Bad            =  52,
    MUSIC_Rjak_Good             =  53,
    MUSIC_Rjak_Bad              =  54,
    MUSIC_Sssra_Good            =  55,
    MUSIC_Sssra_Bad             =  56,
    MUSIC_Tauron_Good           =  57,
    MUSIC_Tauron_Bad            =  58,
    MUSIC_Freya_Good            =  59,
    MUSIC_Freya_Bad             =  60,
    MUSIC_Horus_Good            =  61,
    MUSIC_Horus_Bad             =  62,
    MUSIC_Ariel_Good            =  63,
    MUSIC_Ariel_Bad             =  64,
    MUSIC_Tlaloc_Good           =  65,
    MUSIC_Tlaloc_Bad            =  66,
    MUSIC_Kali_Good             =  67,
    MUSIC_Kali_Bad              =  68,
    MUSIC_WIN_By_Magic          =  69,
    MUSIC_Hiring_Screen         =  70,
    MUSIC_Merlin_Cmbt1          =  71,
    MUSIC_Merlin_Cmbt2          =  72,
    MUSIC_Raven_Cmbt1           =  73,
    MUSIC_Raven_Cmbt2           =  74,
    MUSIC_Sharee_Cmbt1          =  75,
    MUSIC_Sharee_Cmbt2          =  76,
    MUSIC_LoPan_Cmbt1           =  77,
    MUSIC_LoPan_Cmbt2           =  78,
    MUSIC_Jafar_Cmbt1           =  79,
    MUSIC_Jafar_Cmbt2           =  80,
    MUSIC_Oberic_Cmbt1          =  81,
    MUSIC_Oberic_Cmbt2          =  82,
    MUSIC_Rjak_Cmbt1            =  83,
    MUSIC_Rjak_Cmbt2            =  84,
    MUSIC_Sssra_Cmbt1           =  85,
    MUSIC_Sssra_Cmbt2           =  86,
    MUSIC_Tauron_Cmbt1          =  87,
    MUSIC_Tauron_Cmbt2          =  88,
    MUSIC_Freya_Cmbt1           =  89,
    MUSIC_Freya_Cmbt2           =  90,
    MUSIC_Horus_Cmbt1           =  91,
    MUSIC_Horus_Cmbt2           =  92,
    MUSIC_Ariel_Cmbt1           =  93,
    MUSIC_Ariel_Cmbt2           =  94,
    MUSIC_Tlaloc_Cmbt1          =  95,
    MUSIC_Tlaloc_Cmbt2          =  96,
    MUSIC_Kali_Cmbt1            =  97,
    MUSIC_Kali_Cmbt2            =  98,
    MUSIC_Bkrgnd_Good           =  99,
    MUSIC_Bkgrnd_Neutral        = 100,
    MUSIC_Bkgrnd_Negative       = 101,
    MUSIC_Combat_1              = 102,
    MUSIC_Combat_2              = 103,
    MUSIC_Main_Menu             = 104,
    MUSIC_Site_Discovery        = 105,
    MUSIC_Events                = 106,
    MUSIC_New_Outpost           = 107,
    MUSIC_Bldng_Finished        = 108,
    MUSIC_WIN_Military          = 109,
    MUSIC_LOSE_Military         = 110,
    MUSIC_LOSE_By_Magic         = 111,
    MUSIC_Intro                 = 112,
    MUSIC_Fanf_not_used         = 113,
    MUSIC_Bad_Event             = 114,
    MUSIC_Fanfare               = 115
};


enum SOUND_FX
{
    SFX_Std_Btn_Clk  = 0, 
    SFX_IntroT2  = 1, 
    SFX_Left_Btn_Clk  = 2, 
    SFX_IntroT3  = 3, 
    SFX_IntroM  = 4, 
    SFX_Troops  = 5, 
    SFX_Cavalry  = 6, 
    SFX_Flyer_S  = 7, 
    SFX_Flyer_L  = 8, 
    SFX_Monster_S  = 9, 
    SFX_Monster_L  = 10, 
    SFX_Teleport  = 11, 
    SFX_Tunneling  = 12, 
    SFX_Monster_M  = 13, 
    SFX_Empty1  = 14, 
    SFX_ATK_Catapult  = 15, 
    SFX_ATK_Cannon  = 16, 
    SFX_ATK_Bows  = 17, 
    SFX_ATK_Slings  = 18, 
    SFX_ATK_Lightning  = 19, 
    SFX_ATK_Fire  = 20, 
    SFX_ATK_Illusion  = 21, 
    SFX_ATK_Death  = 22, 
    SFX_ATK_Ice  = 23, 
    SFX_ATK_Holy  = 24, 
    SFX_ATK_Drow  = 25, 
    SFX_ATK_Sprite  = 26, 
    SFX_ATK_LightningBolt  = 27, 
    SFX_Gen_Nature  = 28, 
    SFX_Gen_Sorcery  = 29, 
    SFX_Gen_Chaos  = 30, 
    SFX_Gen_Life  = 31, 
    SFX_Gen_Death  = 32, 
    SFX_Gen_Arcane  = 33, 
    SFX_FemaleScream  = 34, 
    SFX_MaleScream  = 35, 
    SFX_MaleScream2  = 36, 
    SFX_Empty2  = 37, 
    SFX_Empty3  = 38, 
    SFX_StarFires  = 39, 
    SFX_PlaneShift  = 40, 
    SFX_RaiseDead  = 41, 
    SFX_DispelEvil  = 42, 
    SFX_HolyWord  = 43, 
    SFX_StreamofLife  = 44, 
    SFX_Inspirations  = 45, 
    SFX_Prosperity  = 46, 
    SFX_AstralGate  = 47, 
    SFX_AltarofBattle  = 48, 
    SFX_Consecration  = 49, 
    SFX_Weakness  = 50, 
    SFX_BlackSleep  = 51, 
    SFX_MindTwist  = 52, 
    SFX_Possession  = 53, 
    SFX_WarpNode  = 54, 
    SFX_BlackWind  = 55, 
    SFX_DrainPower  = 56, 
    SFX_Subversion  = 57, 
    SFX_CruelUnminding  = 58, 
    SFX_AnimateDead  = 59, 
    SFX_DarkRituals  = 60, 
    SFX_EvilPresence  = 61, 
    SFX_CursedLands  = 62, 
    SFX_Pestilence  = 63, 
    SFX_CloudofShadow  = 64, 
    SFX_Famine  = 65, 
    SFX_WallofDarkness  = 66, 
    SFX_Petrify  = 67, 
    SFX_Web  = 68, 
    SFX_Earth2Mud  = 69, 
    SFX_IceStorm  = 70, 
    SFX_CracksCall  = 71, 
    SFX_Earthquake  = 72, 
    SFX_Transmute  = 73, 
    SFX_NaturesEye  = 74, 
    SFX_EarthGate  = 75, 
    SFX_GaiasBlessing  = 76, 
    SFX_DispelMagic  = 77, 
    SFX_WordofRecall  = 78, 
    SFX_Vertigo  = 79, 
    SFX_DisenchantTrue  = 80, 
    SFX_SpellBlast  = 81, 
    SFX_Whirlwind  = 82, 
    SFX_Banish  = 83, 
    SFX_Confusion  = 84, 
    SFX_Stasis  = 85, 
    SFX_EnchantRoad  = 86, 
    SFX_MindStorm  = 87, 
    SFX_CreatureBinding  = 88, 
    SFX_FlyingFortress  = 89, 
    SFX_SpellWard  = 90, 
    SFX_Disrupt  = 91, 
    SFX_Shatter  = 92, 
    SFX_Fireball  = 93, 
    SFX_WarpCreature  = 94, 
    SFX_WarpWood  = 95, 
    SFX_Firestorm  = 96, 
    SFX_WarpLightning  = 97, 
    SFX_RaiseVolcano  = 98, 
    SFX_Disintegrate  = 99, 
    SFX_DoomBolt  = 100, 
    SFX_CallChaos  = 101, 
    SFX_CalltheVoid  = 102, 
    SFX_Corruption  = 103, 
    SFX_WallofFire  = 104, 
    SFX_ChaosRift  = 105, 
    SFX_Silence  = 106, 
    SFX_CombatSummon  = 107, 
    SFX_ChangeTerrain  = 108, 
    SFX_MoveFortress  = 109, 
    SFX_EarthLore  = 110, 
    SFX_WallofStone  = 111, 
    SFX_Disjunction  = 112, 
    SFX_SpellBinding  = 113, 
    SFX_Resurrection  = 114, 
    SFX_IntroT1  = 115, 
    SFX_Unknown116  = 116, 
    SFX_Unknown117  = 117, 
    SFX_Unknown118  = 118
};



/*
    WIZARDS.EXE  seg038
*/

// WZD s38p01
// _AIL_load_timbre()

// WZD s38p02
int16_t Audio_Init__STUB(int16_t midi_driver, int16_t sound_channels, int16_t MIDI_IO, int16_t MIDI_IRQ, int16_t MIDI_DMA, int16_t digi_driver, int16_t Digi_IO, int16_t Digi_IRQ, int16_t Digi_DMA);

// WZD s38p03
void Play_Sound__STUB(SAMB_ptr voc);

// WZD s38p04
void Set_Music_Volume__STUB(int16_t volume);

// WZD s38p05
int16_t Get_Music_Volume__STUB(void);

// WZD s38p06
void Stop_All_Sounds__STUB(void);

// WZD s38p07
// SND_Get_Seq_Status()

// WZD s38p08
void Stop_Music__STUB(void);

// WZD s38p09
void Audio_Uninit__STUB(void);

// WZD s38p10
// UU_SND_LegacyFn()

// WZD s38p11
// _AIL_board_ID()

// WZD s38p12
void Sound_Error__STUB(int16_t error);



#ifdef __cplusplus
}
#endif

#endif  /* SOUND_H */
