










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

