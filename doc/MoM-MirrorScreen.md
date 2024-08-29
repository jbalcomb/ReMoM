
Mirrow Screen

HLPENTRY.LBX  "wizview scrn Help"

Main_Screen()
    input_field_idx == hotkey_idx_F9
    |-> Advisor_Screen(8)
Advisor_Screen()
    switch(input_advisor_idx) { ... }
    case 8:  /* Mirror        (F9 )*/
    |-> Mirror_Screen(0, 150, 60, 180, 90);
Magic_Screen()
    Right-Click Gem (Mirror)
    |-> 


struct s_WIZARD
/* 064 */ int8_t retorts[18];
AKA struct s_RETORTS Retorts
    alchemy
    warlord
    chaos_mastery
    nature_mastery
    sorcery_mastery
    infernal_power
    divine_power
    sage_master
    channeler
    myrran
    archmage
    mana_focusing
    node_mastery
    famous
    runemaster
    conjurer
    charismatic
    artificer





##### Naming Things Is Hard

Relations
Treaties
Personality
Objective


BACKGRND.LBX, 004  MAGEVIEW    wizardview backgrn

BACKGRND.LBX, 018  MIRROR      diplomacy mirror


// WZD dseg:3648
char str_Relations[] = "Relations:";
char str_Treaties[] = "Treaties:";
char str_Personality[] = "Personality:";
char str_Objective[] = "Objective:";
