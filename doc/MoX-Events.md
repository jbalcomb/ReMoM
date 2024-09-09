
MoX / MoM
Events

EVENTS.C/.H




## Event Status

~ Nothing, Starting, Ongoing, Oneoff, Ending


e.g., Population Boom

"Population Boom!  A sudden population boom doubles the population growth rate of the \8B of \x80."
"The population boom in \x80 has ended."
""
"\8C's \8B of \x80 population is booming, doubling their population growth rate."

|-> Show_Event_Message()
    |-> Get_Event_Message()
        event_message_record_idx = (((m_current_event - 1) * 7) + EVNT_MessageIndex);

The event ends...
    if Random(20) && events_table->Population_Boom_Duration > 3
        events_table->Population_Boom_Status = 0;
        EVNT_MessageIndex = 1;
        Show_Event_Message();

if(events_table->Population_Boom_Status == 1)
    Show_Event_Message();
    events_table->Population_Boom_Status = 2;
    events_table->Population_Boom_Duration = 0;


Disjunction
EVNT_MessageIndex = 3;
OON record
"Disjunction!  The fabric of magic has been torn asunder destroying all overland enchantments."


## Event Message Type

~ Started, Ended, Happened, Ongoing, Elsewhere

```c
if(events_table->Rebellion_Player == _human_player_idx)
{
    m_event_message_type = 0;
}
else
{
    m_event_message_type = 4;
}
```
So, ...
0 is it is happening to you
4 is it is happening to someone else






##### Naming Things Is Hard

Bad Moon
Conjunction
Depletion
Disjunction
Donation
Good Moon
Great Meteor
Mana Short
New Minerals
Piracy
Plague
Population Boom
Rebellion



EVENTS.LBX
000  METEOR
001  GIFT
002  DISJUNCT
003  MARRIAGE
004  EARTHQUA
005  PIRACY
006  PLAGUE
007  REBELLIO
008  DONATION
009  DEPLET
010  MINERALS
011  POPBOOM
012  MOON        good moon
013  BADMOON
014  CNJRED
015  CNJGREEN
016  CNJBLUE
017  MANASHOR


Page 105  (PDF Page 110)

Events  

In real life, things are rarely quiet for long.  
Life on Arcanus and Myrror is no different from life on Earth in that respect.  
The following list briefly describes the sorts of interesting events that can keep things from getting too routine.  
(Although, if you are not really in the mood for much excitement, feel free to toggle off the random events button; see Game Settings.)  
Every year brings with it a certain chance that an unusual event will occur.  
As years without events pass, the probability of a random event occurring increases.  
If your wizard is doing well (winning), there is an increased chance that a negative event will occur, while the opposite holds true if your wizard is losing.  
All events are announced to you as they occur.  
If the particular event is a lasting one, you are also notified when it ends.  

Bad Moon: A blood red, evil moon appears in the night skies of Arcanus
and Myrror, increasing the magic power that flows from evil temples,
those owned by practitioners of death magic, by 50%. At the same
time, the magic power flowing from good temples, those owned by
practitioners of life magic, decreases to half its usual amount. There
is a 5% probability of the event ending every game turn.

Conjunction: The skies over Arcanus and Myrror occasionally exhibit an
unusual phenomenon: triads of stars of similar color (red, blue or
green) appear to rise in the skies. When they do, the dominant color
of the stars in the triad mysteriously influences the magic nodes on
the worlds, doubling the magic power obtained from nodes of the
same color as the stars and halving the magic power emanating from
all other nodes. There is a 5% probability every game turn that this
phenomenon ends.

Depletion: Occasionally deposits of minerals are depleted. When this
happens, the mineral disappears from the land, never again to
contribute to the local town’s resources.
Diplomatic Marriage: The ruler of a neutral city appears, bearing an
offer for unity through a diplomatic marriage. If you agree, the
neutral city becomes yours.

Disjunction: On rare occasions, a massive flux of energy from an asyet-
unidentified source tears asunder the fabric of magic itself! Well,
it need hardly be said that the consequences of this event are
disastrous, as wizards see all their hard-placed global enchantments
destroyed along with the woven threads of magic.

Donation: A generous and wealthy merchant decides to donate a large
amount of gold to a wizard.

Earthquake: While most of the earthquakes on Arcanus and Myrror are
due to wizardly feuds, the odd earthquake occasionally happens
naturally, with effects transpiring in the same manner as the spell
(see earthquake in the Spellbook supplement).

The Gift: A god offers a powerful ancient relic to a wizard.

Good Moon: A pure silver moon appears in the night skies of Arcanus
and Myrror, increasing the magic power that flows from good temples
by 50%. At the same time, the magic power flowing from evil
temples decreases to half its usual amount. There is a 5% probability
every game turn of ending the event.

Great Meteor: An enormous meteor strikes a city, with consequences like
those following the casting of call the void (see the Spellbook
supplement).

Mana Short: All sources of magic power dry up completely. No player
can draw magic power from any source during a mana short. Note
that enchantments and fantastic creatures that require mana for
maintenance still require it during a mana short! Take care that you
spend the magic power from your reserves carefully during this
event. Every turn there is a 5% probability that the event ends.

New Minerals: A new mineral source is discovered near a city. Potential
new minerals include coal, gold, gems, mithril, adamantium and
quork crystals.

Piracy: Pirates stage a raid and make off with between 30% and 50% of
a wizard’s gold reserves!

Plague: A virulent plague breaks out in a city, causing a loss of lives and
increasing local unrest. The effects are the same as the spell
pestilence (see the Spellbook supplement), except that a plague
cannot be dispelled. There is a 5% probability of the plague ending
every game turn.

Population Boom: The population growth rate of a city doubles. There
is a 5% probability every game turn that the growth rate returns
to normal.

Rebellion: Your local popularity in one of the cities you own plummets.
Rebels seize power and the city, along with any military units there,
becomes neutral. All fantastic creatures in the city are banished.
