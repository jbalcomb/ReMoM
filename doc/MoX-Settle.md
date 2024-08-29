
Unit Action Special Settle

Create Outpost
Create City

1oom

MoO2
Module: COLONIZE
    code (0 bytes) Make_New_Colony_Or_Outpost_
        Address: 01:000E5EB3
    code (0 bytes) Make_New_Colony_
        Address: 01:000E6071
    code (0 bytes) Make_New_Outpost_
        Address: 01:000E607F
Module: INITGAME
    code (0 bytes) Init_Colony_
        Address: 01:00012D75





Main_Screen()
    switch(special_action_flag)
        Do_Build_Outpost()
            STK_SettleTile()
                Create_Outpost()
                    |-> Map_Square_Survey()
                    |-> Init_Outpost()
                    |-> UNIT_MarkRemoved()


int16_t STK_SettleTile(int16_t troop_count, int16_t troops[])
    |-> Create_Outpost(unit_wx, unit_wy, unit_wp, unit_race, unit_owner, unit_idx)

int16_t Create_Outpost(int16_t outpost_wx, int16_t outpost_wy, int16_t outpost_wp, int16_t outpost_race, int16_t outpost_owner, int16_t settler_unit_idx)



## ¿ Logical Separation ?

Init_Outpost()
    creates the records and zeroes out the values
    returns {F,T}

Create_Outpost()
    checks Map_Square_Survey() to see if the map square is even a valid option
    calls Init_Outpost() and assumes failure means the city count limit has been reached
    sets the *global* _city_idx, which seems equivalent to MoO2's Set_Colony_Globals_()
    *consumes* the Settler unit
    returns {F,T}


STK_SettleTile()
    checks if the Stack has a Settler and calls out to attempt the outpost creation process
    takes troops, so ~ "Army_..."

¿ ~ Army ... Settle ... Map Sqaure ?
~ MoO2  Colonize_Planet_()
~ MoO2  Make_New_Colony_Or_Outpost_()















#####  Naming Things Is Hard

Build outposts
settlers
build outposts
build button



Page 44  (PDF Page 49)

Build outposts: settlers can build outposts (click on build button)
on map squares that are at least three map squares from any
other city.


Page 62  (PDF Page 67)

You can either build settlers (see Production Screen) to
establish outposts or conquer existing cities.

THE OUTPOST
To build a new city, you first have to go to the production screen in
one of your other cities (see City Screen) and click on the settlers
button. When you have constructed a settlers unit (note that it “costs” a
townsperson to build this unit), decide on the best location for your new
city.

When you have selected a suitable site for a new city, send your
settlers to that map square and select the build option. The settlers then
quickly build an outpost (which you get to name) and populate it (i.e.,
the settlers disappear).


TABLE I

STANDARD AND RACE-SPECIFIC UNITS

Creature        Race    Building    Melee   Ranged  Defense Resistance  Hits    Movement    Special
(# of Figures)          Cost        Attack  Attack                                          Abilities
Settlers (1)    -       60          -       -       1       4           10      1           Creates Outposts
