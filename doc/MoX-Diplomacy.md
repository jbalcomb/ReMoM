


word_42E8C  ==>  m_IDK_diplomatic_order
word_42E8E  ==>  m_IDK_diplomatic_flag


¿ computer player's Core Reaction ?
¿ Diplomatic Relations Scale ?

Table 25.1 The Diplomatic Relations Scale
Relationship Level (Name)
Diplomacy Point (DP)
`relationship_level = ((100 + _players[HUMAN_PLAYER_IDX].Dipl.Visible_Rel[m_diplomac_player_idx]) / 20);`
(-100, 220)
{ {   -100,      -80,   -60,      -40 }, {    -20,       0,      20 }, {   40,       60,       80,     100 } }
{ {      0,       20,    40,       60 }, {     80,     100,     120 }, {  140,      160,      180,     200 } }
{ {      0,        1,     2,        3 }, {      4,       5,       6 }, {    7,        8,        9,      10 } }
{ { Hatred, Troubled, Tense, Restless }, { Uneasy, Neutral, Relaxed }, { Calm, Peaceful, Friendly, Harmony } }
{ { red                               }, { yellow                   }, { green                             } }



What am I seeing?


MoO2  DIPLOMAC  Limit_Treaty_Modifiers_()
    SETMAX _player[].treaty_modifier  120
    SETMIN _player[].treaty_modifier -200

MoO2  DIPLOMAC  Find_Worst_Modifier_()
XREF:
    Diplomacy_Screen_()
    Get_Main_Repulsive_Diplomacy_Choices_()
    Get_Main_Diplomacy_Choices_()
    Get_Demand_Response_()


MoO2  Diplomacy_Screen_()
    _ambassador_option = {0,1}
    _response_message
...looks like IDK_DiplSts_s70570() 



should be
background
portrait
gargoyles
mouth talking
...
greeting
first list
    How may I serve you:
        Propose Treaty
        Threaten/Break Treaty
        Offer Tribute
        Exchange Spells
        Good Bye
then
    You propose a treaty:
        Wizard Pact
        Alliance
        Peace Treaty
        Declaration of War on Another Wizard
        Break Alliance With Another Wizard
        Forget It


...
...conclusions...
    calls to 







## m_IDK_diplomatic_flag


## m_IDK_diplomatic_order

Diplomacy_Screen__WIP()
    m_IDK_diplomatic_order = _players[HUMAN_PLAYER_IDX].Dipl.Dipl_Action[m_diplomac_player_idx];

XREF:
    Diplomacy_Screen__WIP+58        mov     [m_IDK_diplomatic_order], ax                                          
    Diplomacy_Screen__WIP+61        cmp     [m_IDK_diplomatic_order], 54                                          
    Diplomacy_Screen__WIP+68        cmp     [m_IDK_diplomatic_order], 1                                           
    Diplomacy_Screen__WIP:loc_6EAE4 cmp     [m_IDK_diplomatic_order], 0                                           
    Diplomacy_Screen__WIP+91        cmp     [m_IDK_diplomatic_order], 45                                          
    Diplomacy_Screen__WIP+98        cmp     [m_IDK_diplomatic_order], 49                                          
    Diplomacy_Screen__WIP:loc_6EB0F cmp     [m_IDK_diplomatic_order], 2                                           
    Diplomacy_Screen__WIP+A6        cmp     [m_IDK_diplomatic_order], 47                                          
    Diplomacy_Screen__WIP:loc_6EB1D mov     ax, [m_IDK_diplomatic_order]                                          
    Diplomacy_Screen__WIP+B3        mov     [m_IDK_diplomatic_order], 42                                          
    Diplomacy_Screen__WIP+F6        push    [m_IDK_diplomatic_order]                                              
    Diplomacy_Screen_Draw__WIP+75   push    [m_IDK_diplomatic_order]                                              
    IDK_Diplo_Scrn+157              cmp     [m_IDK_diplomatic_order], 71                                          
    sub_6F51A:loc_6F549             mov     [m_IDK_diplomatic_order], si                                          
    sub_6F51A+70                    mov     [m_IDK_diplomatic_order], ax                                          
    sub_6F51A+A1                    mov     [m_IDK_diplomatic_order], 58                                          
    sub_6F51A:loc_6F5C3             mov     [m_IDK_diplomatic_order], 57                                          
    sub_6F51A+E6                    mov     [m_IDK_diplomatic_order], 54                                          
    sub_6F51A:loc_6F608             mov     [m_IDK_diplomatic_order], si                                          
    sub_6F51A:loc_6F610             mov     [m_IDK_diplomatic_order], si                                          
    sub_6F51A+122                   push    [m_IDK_diplomatic_order]                    ; diplomsg_0_record_number
    IDK_DiplSts_s70570:loc_705CF    mov     [m_IDK_diplomatic_order], 44                                          
    IDK_DiplSts_s70570+72           mov     [m_IDK_diplomatic_order], 42                                          
    IDK_DiplSts_s70570:loc_705EA    mov     [m_IDK_diplomatic_order], 43                                          
    sub_70A1A:loc_70A6F             mov     [m_IDK_diplomatic_order], 2Fh ; '/'                                   
    IDK_Dipl_s72690+58              mov     [m_IDK_diplomatic_order], ax                                          
    IDK_Dipl_s72690+61              cmp     [m_IDK_diplomatic_order], 54                                          
    IDK_Dipl_s72690+68              cmp     [m_IDK_diplomatic_order], 1                                           
    IDK_Dipl_s72690:loc_72704       cmp     [m_IDK_diplomatic_order], 0                                           
    IDK_Dipl_s72690+91              cmp     [m_IDK_diplomatic_order], 45                                          
    IDK_Dipl_s72690+98              cmp     [m_IDK_diplomatic_order], 50                                          
    IDK_Dipl_s72690:loc_7272F       cmp     [m_IDK_diplomatic_order], 2                                           
    IDK_Dipl_s72690+A6              cmp     [m_IDK_diplomatic_order], 47                                          
    IDK_Dipl_s72690:loc_7273D       mov     ax, [m_IDK_diplomatic_order]                                          
    IDK_Dipl_s72690+B3              mov     [m_IDK_diplomatic_order], 42                                          
    IDK_Dipl_s72690+11A             mov     [m_IDK_diplomatic_order], ax                                          
    IDK_Dipl_s72690:loc_727C1       cmp     [m_IDK_diplomatic_order], 45                                          
    IDK_Dipl_s72690:loc_727DB       cmp     [m_IDK_diplomatic_order], 46                                          
    IDK_Dipl_s72690:loc_727F5       cmp     [m_IDK_diplomatic_order], 49                                          
    IDK_Dipl_s72690:loc_72859       cmp     [m_IDK_diplomatic_order], 47                                          
    IDK_Dipl_s72690:loc_7286E       cmp     [m_IDK_diplomatic_order], 45                                          
    IDK_Dipl_s72690+1E5             cmp     [m_IDK_diplomatic_order], 46                                          
    IDK_Dipl_s72690+1EC             cmp     [m_IDK_diplomatic_order], 49                                          
    IDK_Dipl_s72690:loc_728B9       cmp     [m_IDK_diplomatic_order], 47                                          
    IDK_Dipl_s72690+243             cmp     [m_IDK_diplomatic_order], 48                                          
    IDK_Dipl_s72690+276             cmp     [m_IDK_diplomatic_order], 45                                          
    IDK_Dipl_s72690+27D             cmp     [m_IDK_diplomatic_order], 46                                          
    IDK_Dipl_s72690:loc_7292B       cmp     [m_IDK_diplomatic_order], 49                                          
    IDK_Dipl_s72690:loc_72949       cmp     [m_IDK_diplomatic_order], 48                                          
    IDK_Dipl_s72690+2E0             push    [m_IDK_diplomatic_order]                                              
    DIPL_sub_72DB6+C                push    [m_IDK_diplomatic_order]                                              
    DIPL_sub_72DB6+39               mov     bx, [m_IDK_diplomatic_order]                                          
    DIPL_sub_72DB6+11C              push    [m_IDK_diplomatic_order]                                              
    DIPL_sub_72DB6:loc_72F5F        cmp     [m_IDK_diplomatic_order], 2                                           
    DIPL_sub_72DB6+1B0              cmp     [m_IDK_diplomatic_order], 48                                          
    DIPL_sub_72DB6+209              cmp     [m_IDK_diplomatic_order], 48                                          
    DIPL_sub_72DB6+3E8              cmp     [m_IDK_diplomatic_order], 47                                          











// WZD o74p02
void Mirror_Screen_Draw(void)

    strcat(GUI_String_1, _treaty_type_names[_players[_human_player_idx].Dipl.Dipl_Status[mirror_screen_player_idx]]);





## _treaty_type_names[]

{"No Treaty", "Wizard Pact", "Alliance", "War", "Final War"}



// MagicScr.C
// WZD dseg:35BA
extern char * _treaty_type_names[5];



// WZD dseg:3617                                                 BEGIN:  _treaty_type_names
// WZD dseg:3617
extern char aNoTreaty[];
// WZD dseg:3621
extern char aWizardPact_1[];
// WZD dseg:362D
extern char aAlliance_1[];
// WZD dseg:3636
extern char aWar[];
// WZD dseg:363A
extern char aFinalWar[];
// WZD dseg:363A                                                 END:  _treaty_type_names

...

// WZD dseg:35BA
char * _treaty_type_names[5] =
{
    aNoTreaty,
    aWizardPact_1,
    aAlliance_1,
    aWar,
    aFinalWar
};

...

// WZD dseg:3617                                                 BEGIN:  _treaty_type_names
// WZD dseg:3617
char aNoTreaty[] = "No Treaty";
// WZD dseg:3621
char aWizardPact_1[] = "Wizard Pact";
// WZD dseg:362D
char aAlliance_1[] = "Alliance";
// WZD dseg:3636
char aWar[] = "War";
// WZD dseg:363A
char aFinalWar[] = "Final War";
// WZD dseg:363A                                                 END:  _treaty_type_names





##### Naming Things Is Hard

Relations
Treaties
Personality
Objective
magic summary screen
mirrors (or gems)
current status of your relationship with a particular wizard.  
the communication screen
form or break treaties
enemy wizard
portrait
a detailed character sheet
wizard
Enemy Wizard Statistics
opponents eventually introduce themselves
a list of overland enchantments
no icon ... rivals ... in a neutral state
not at war, nor ... formal peace treaty, wizard pact or alliance
scroll icon ... wizard pact or alliance
crossed swords icon ... state of war exists
peace symbol icon ... peace treaty
no icon ... neutral state


BACKGRND.LBX, 004  MAGEVIEW    wizardview backgrn

BACKGRND.LBX, 018  MIRROR      diplomacy mirror


MAGIC.LBX,  060     DIPLICON   blue wizpak
MAGIC.LBX,  061     DIPLICON   blue alliance
MAGIC.LBX,  062     DIPLICON   blue war
MAGIC.LBX,  063     DIPLICON   green wizpaq
MAGIC.LBX,  064     DIPLICON   green alliance
MAGIC.LBX,  065     DIPLICON   green war
MAGIC.LBX,  066     DIPLICON   purple wizpack
MAGIC.LBX,  067     DIPLICON   purple alliance
MAGIC.LBX,  068     DIPLICON   purple war
MAGIC.LBX,  069     DIPLICON   red whizpaq
MAGIC.LBX,  070     DIPLICON   red alliance
MAGIC.LBX,  071     DIPLICON   red war
MAGIC.LBX,  072     DIPLICON   yellow wizpax
MAGIC.LBX,  073     DIPLICON   yellow alliance
MAGIC.LBX,  074     DIPLICON    yellow war


// WZD dseg:3648
char str_Relations[] = "Relations:";
char str_Treaties[] = "Treaties:";
char str_Personality[] = "Personality:";
char str_Objective[] = "Objective:";

// WZD dseg:35C4
char aHate[] = "Hate";                         
char aTroubled[]  db 'Troubled',0
char aTense[]  db 'Tense',0
char aRestless[]  db 'Restless',0
char aUnease[]  db 'Unease',0
char aNeutral[]  db 'Neutral',0
char aRelaxed[]  db 'Relaxed',0
char aCalm[]  db 'Calm',0
char aPeaceful_0[]  db 'Peaceful',0
char aFriendly[]  db 'Friendly',0
char aHarmony[]  db 'Harmony',0


Page 30  (PDF Page 35)

Magic

Clicking on the magic button takes you to the magic summary screen.  
This screen contains portraits of all known enemy wizards (there are no faces in the frames at the beginning of the game, but don’t worry, your opponents eventually introduce themselves), a list of overland enchantments, information on your magic power base, current spells you are researching or casting and how your available magic power is divided between researching, reserves and spell casting skill.  
The magic summary screen also provides a means to communicate with other wizards, to obtain information about other wizards, to adjust how your magic power is being allocated and to use alchemy to interconvert gold and magic power.  

ENEMY WIZARDS

Quickly determine the banner color of an opposing wizard. The wizards’ faces are rimmed by the color of their banner type.  
· Determine the status of your relationship with a wizard. Below the wizards’ faces, phrases like "neutral" or "hate" summarize the current status of your relationship with a particular wizard.  
· Obtain information about an enemy wizard by right-clicking on his or her portrait. Doing this calls up a detailed character sheet for that wizard; see Enemy Wizard Statistics.  
· Communicate with an enemy wizard by clicking on his or her portrait. This calls up the communication screen from which you can form or break treaties, exchange spells and take other diplomatic actions. For more information, see The Art of Diplomacy.  


Page 31  (PDF Page 36)

Observe the status of wizards’ relationships with each other. To the
right of each wizard’s portrait you may see a series of icons
including scrolls, doves and crossed swords. These icons are colorcoded
to match the banner colors of the other wizards, including
yours. A scroll icon indicates that there is a wizard pact or alliance
in effect between the wizard in the portrait and the wizard whose
banner color matches the ribbon on the icon. Similarly, the crossed
swords icon indicates a state of war exists, while a peace symbol
icon indicates a peace treaty is currently in effect. If no icon
appears, those rivals are in a neutral state (i.e., not at war, nor
under the terms of a formal peace treaty, wizard pact or alliance).  
See The Art of Diplomacy for more details.  


Page 68  (PDF Page 73)

THE ART OF DIPLOMACY

