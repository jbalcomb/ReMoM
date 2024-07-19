
Rebels
Unrest



MoO2
    Module: INVASION
        Colony_N_Militia_()
        Get_Rebellion_Info_()
        Check_Rebellion_()
        Check_All_Rebellions_()
MoO2 INITGAME







e.g., Capital of Banbury
unrest_races =  0  for human and human
unrest_taxes = 30  1.5 gold, 30% unrest - "Tax Per Population" 
NAY _CITIES[city_idx].enchantments[FAMINE]
_CITIES[city_idx].population == 21
21 * 30 = 630 / 100 = 6.3 truncated = 6

*"pacifying effects"*
*"religious institutions"*
Pacify = 0
Pacify_Religion = 0
YAY SHRINE    += 1
YAY TEMPLE    += 1
YAY PARTHENON += 1
YAY CATHEDRAL += 1
NAY infernal_power || divine_power += 50%
...pacify == 4
YAY ORACLE          += 2
YAY ANIMISTS_GUILD  += 1
NAY GREAT_WASTING   = -1  BUGBUG
NAY ARMAGEDDON      = -2  BUGBUG
NAY DARK_RITUALS    -= 1
NAY PESTILENCE      -= 2
NAY CURSED_LANDS    -= 1
NAY JUST_CAUSE      += 1
NAY GAIAS_BLESSING  += 2
...pacify == 7
Pacify_Police = 0
...6 pikemen / 2 += 3












Load_WZD_Resources()
    |-> Terrain_Init()



## Terrain_Init()
XREF:
NX_j_Terrain_Init()
Load_WZD_Resources()

```c
    // // TBL_Unrest[0] = LBX_Load_Data(terrstat_lbx_file, 1, 0, 1, 196);
    // TBL_Unrest[0] = LBX_Load_Data("TERRSTAT.LBX", 1, 0, 1, 196);
    // for(itr = 1; itr < 14; itr++)
    // {
    //     TBL_Unrest[itr] = TBL_Unrest[itr - 1] + (14 * 16);
    // }
    // can't do like they did, because pointers are 8 bytes now
    TBL_Unrest_Hack = LBX_Load_Data(terrstat_lbx_file, 1, 0, 1, 196);
    for(itr = 0; itr < 14; itr++)
    {
        TBL_Unrest[itr] = (SAMB_ptr)(TBL_Unrest_Hack + (14 * itr));
    }
```

```
mov     ax, 196
push    ax                              ; record_size
mov     ax, 1
push    ax                              ; num_recs
xor     ax, ax
push    ax                              ; start_rec
mov     ax, 1
push    ax                              ; entry_num
mov     ax, offset terrstat_lbx_file    ; "TERRSTAT"
push    ax                              ; file_name
call    LBX_Load_Data                   ; LBX_RecordLoader()  ; LBX_Name, LBX_Entry, 0, 0, 0, 1, 196
add     sp, 0Ah
mov     [bp+tmp_sa_TBL_Unrest], ax
push    [bp+tmp_sa_TBL_Unrest]          ; addr
call    SA_MK_FP0
pop     cx
mov     [word ptr TBL_Unrest+2], dx
mov     [word ptr TBL_Unrest], ax
mov     _SI_itr, 1
jmp     short @@Loop_13
@@Loop_13:
cmp     _SI_itr, 14
jl      short @@Loop_1to13
@@Loop_1to13:
mov     bx, _SI_itr
mov     cl, 2                           ; sizeof
shl     bx, cl                          ; +4, because for si=1 to ...
mov     ax, word ptr (TBL_Unrest-2)[bx]
mov     dx, word ptr (TBL_Unrest-4)[bx]
add     dx, 14                          ; record size
mov     bx, _SI_itr
mov     cl, 2                           ; sizeof
shl     bx, cl                          ; writing from [2] to [14]
mov     [word ptr (TBL_Unrest+2)+bx], ax
mov     [word ptr TBL_Unrest+bx], dx
inc     _SI_itr
```
```c
    SAMB_ptr TBL_Unrest[14];
    SAMB_ptr tmp_sa_TBL_Unrest;
    tmp_sa_TBL_Unrest = LBX_Load_Data(terrstat_lbx_file, 1, 0, 1, 196);
    TBL_Unrest = SA_MK_FP0(tmp_sa_TBL_Unrest);
    for(itr = 1; itr < 14; itr++)
    {
        // ¿¿¿
        TBL_Unrest[itr] = (TBL_Unrest[(itr - 1)] + 14);
        // ???
    }
```

City_Rebel_Count()

mov     ax, [bp+city_owner_idx]
mov     dx, size s_WIZARD
imul    dx
mov     bx, ax
mov     al, [_players.Cptl_Race+bx]
cbw
mov     cl, 2
shl     ax, cl
mov     bx, ax
les     bx, [TBL_Unrest+bx]
mov     ax, _DI_city_idx
mov     dx, size s_CITY
imul    dx
push    es
les     si, [_CITIES]
add     si, ax
mov     al, [es:si+s_CITY.race]
cbw
add     bx, ax
pop     es
TBL_Unrest[_players[city_owner_idx].Cptl_Race][_CITIES[city_idx].race]
mov     al, [es:bx]
cbw
mov     dx, 10
imul    dx
push    ax
mov     ax, [bp+city_owner_idx]
mov     dx, size s_WIZARD
imul    dx
mov     bx, ax
mov     bx, [_players.tax_rate+bx]
shl     bx, 1
mov     ax, [tax_unrest_pct_table+bx]
pop     dx
add     ax, dx
mov     [bp+unrest_percent], ax

EVNT_PickCity()

_players[player_idx].capital_race
cbw
mov     cl, 2
shl     ax, cl
mov     bx, ax

les     bx, [TBL_Unrest+bx]
mov     ax, [bp+Picked_Index]
mov     dx, size s_CITY
imul    dx
push    es
les     si, [_CITIES]
add     si, ax
mov     al, [es:si+s_CITY.race]
cbw
add     bx, ax
pop     es
if(... > 1) { ... }




enum e_RACE_TYPE
    rt_Barbarian
    rt_Beastman
    rt_Dark_Elf
    rt_Draconian
    rt_Dwarf
    rt_Gnoll
    rt_Halfling
    rt_High_Elf
    rt_High_Man
    rt_Klackon
    rt_Lizardman
    rt_Nomad
    rt_Orc
    rt_Troll

/*
    Race Type
    00
    01
    02
    03
    04
    05
    06
    07
    08   Human
    09
    10
    11
    12
    13
*/
int8_t race_natural_dispositions[200] =
{
/*                          HU KL              */
	0, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 0, 0, 1,  /*    */
	1, 0, 2, 2, 2, 0, 1, 2, 1, 2, 1, 1, 1, 2, 
	1, 2, 0, 2, 3, 2, 2, 4, 2, 2, 2, 2, 2, 3, 
	1, 2, 2, 0, 2, 1, 1, 1, 1, 2, 1, 1, 1, 2,
	1, 2, 3, 2, 0, 1, 0, 3, 0, 2, 1, 0, 3, 4, 
	1, 0, 2, 1, 1, 0, 0, 1, 1, 2, 1, 1, 0, 0, 
	1, 1, 2, 1, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 
	1, 2, 4, 1, 3, 1, 0, 0, 0, 2, 1, 0, 2, 3, 
	1, 1, 2, 1, 0, 1, 0, 0, 0, 2, 1, 0, 0, 1,  /* HU */
	2, 2, 2, 2, 2, 2, 2, 2, 2,-2, 2, 2, 2, 2,  /* KL */
	1, 1, 2, 1, 1, 1, 0, 1, 1, 2, 0, 1, 1, 1, 
	0, 1, 2, 1, 0, 1, 0, 0, 0, 2, 1, 0, 0, 1, 
	0, 1, 2, 1, 3, 0, 0, 2, 0, 2, 1, 0, 0, 0, 
	1, 2, 3, 2, 4, 0, 0, 3, 1, 2, 1, 1, 0, 0
};


signed char TERRSTAT_LBX_001[200] =
{
	0x01, 0x00, 0xC4, 0x00, 
	0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x00, 0x00, 0x01, 
	0x01, 0x00, 0x02, 0x02, 0x02, 0x00, 0x01, 0x02, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02, 
	0x01, 0x02, 0x00, 0x02, 0x03, 0x02, 0x02, 0x04, 0x02, 0x02, 0x02, 0x02, 0x02, 0x03, 
	0x01, 0x02, 0x02, 0x00, 0x02, 0x01, 0x01, 0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x02,
	0x01, 0x02, 0x03, 0x02, 0x00, 0x01, 0x00, 0x03, 0x00, 0x02, 0x01, 0x00, 0x03, 0x04, 
	0x01, 0x00, 0x02, 0x01, 0x01, 0x00, 0x00, 0x01, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00, 
	0x01, 0x01, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 
	0x01, 0x02, 0x04, 0x01, 0x03, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x02, 0x03, 
	0x01, 0x01, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x01, 
	0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0xFE, 0x02, 0x02, 0x02, 0x02,
	0x01, 0x01, 0x02, 0x01, 0x01, 0x01, 0x00, 0x01, 0x01, 0x02, 0x00, 0x01, 0x01, 0x01, 
	0x00, 0x01, 0x02, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x01, 0x00, 0x00, 0x01, 
	0x00, 0x01, 0x02, 0x01, 0x03, 0x00, 0x00, 0x02, 0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 
	0x01, 0x02, 0x03, 0x02, 0x04, 0x00,	0x00, 0x03, 0x01, 0x02, 0x01, 0x01, 0x00, 0x00
};





##### Naming Things Is Hard

the pacifying effects (ability to reduce unrest in a city’s populace)
religious institutions



Page 14  (PDF Page 19)
Divine Power dramatically improves the effectiveness of shrines,
temples, parthenons and cathedrals in cities owned by a wizard.
The magic power obtained from these city-based sources is
increased by 50%, and the pacifying effects (ability to reduce unrest
in a city’s populace) of these institutions are enhanced. A wizard
must have at least four spell ranks in life magic before you can
choose Divine Power. This special ability costs two spell picks.

Infernal Power is the death magic counterpart to Divine Power. All
shrines, temples, parthenons and cathedrals in cities owned by the
wizard yield 50% more than the usual amount of magic power.
These institutions are also more effective than normal at reducing
local unrest. A wizard must have a minimum of four spell ranks in
death magic to choose Infernal Power. This special ability costs two
spell picks.


Page 17  (PDF Page 22)

Natural Dispositions:  
Some races are inclined to get along with other races even after they capture one another’s cities or after one of their cities is captured.  
Others races tend to seethe with anger and rebellion when their cities are taken and have, in general, such an intense aversion to other races that even when they conquer other races’ cities, this hatred generates a large rebellious contingent in the populace.  
Generally, halflings are the most congenial race of all and very little unrest develops among their townsfolk when their cities are controlled by other races, or when they control other races’ cities.  
Barbarians, gnolls, high men, lizardmen, nomads and, surprisingly, orcs are also pretty easy going vis-à-vis occupation policies.  
Dark elves, dwarves, high elves, klackons and trolls, however, can create plenty of unrest when their cities are occupied or their race conquers another race’s city among this listing.  
Particular animosity exists between the high and dark elves, between the dwarves and elves, and among the trolls, dwarves and elves.  
Any combination of these is likely to require a conqueror to build several religious institutions just to keep the rebel populace in check.  
(See City Population, Building Types and Imperialism for more information.)  


Page 38  (PDF Page 43)

TAX COLLECTOR
Clicking on the tax collector button takes you to the tax window in
which you can set the taxes for the people in the cities you control.
Tax rates can be set in increments of one-half gold coin per
townsperson. Tax rates at the beginning of the game are set at one gold
piece per townsperson. Elevating taxes increases gold revenues, but
decreases productivity by increasing the level of unrest in a city.
Decreasing taxes has the opposite effect (see Gold).


Page 53  (PDF Page 58)

CITY POPULATION

On occasion, civil unrest may grow to the point that some of your workers rebel.  
The rebels, shown in a group to the right of the productive workers, refuse to do any productive work.  
However, they still manage to consume just as much food as they did before.  
You cannot change rebels into workers or farmers.  
You can only address the condition that spawned them to end their rebellious state.  


Page 55  (PDF Page 60)

GOLD

While the tax rate affects how much gold you are earning per turn, tax rates also affect the productivity of your workers.  
This is reflected in the level of unrest in your towns.  
Higher tax rates generate more dissatisfied townsfolk (rebels) at two times the tax rate,  
  while certain institutions (shrines, temples, parthenons and cathedrals)  
  and spells (Gaia's blessing and stream of life) decrease the level of unrest by decreasing the number of rebels.  
Furthermore, the wizard special abilities Divine Power and Infernal Power make local religious institutions more effective at decreasing unrest (see Special Abilities section).  
Note that certain spells (including cursed lands, pestilence, famine and evil presence) increase unrest levels.  


Page 64  (PDF Page 69)

Conquering a city generally causes a certain degree of additional
unrest there. If the conquering race (defined as the race of the city
where the wizard’s enchanted fortress resides) is different from that of
the captured city, again, unrest may result. Conquered cities, then,
generally have a higher base unrest level (i.e., number of rebels) than
cities that have been built up from outposts. As a quick guide, additional
rebels following a city’s capture range from 0 to 4 figures in its
population row. Halflings, as either conquering races or conquered
races, tend to cause the smallest increases in unrest levels; dark elves,
klackons, high elves and dwarves, on the other hand, tend to cause the
largest increases in local unrest levels. One way to handle any
heightened levels of unrest is by quickly building religious institutions
(shrines, temples, parthenons or cathedrals).  
See Building Types and Select Races for a few more details.


Page 107  (PDF Page 112)

Plague: A virulent plague breaks out in a city, causing a loss of lives and
increasing local unrest. The effects are the same as the spell
pestilence (see the Spellbook supplement), except that a plague
cannot be dispelled. There is a 5% probability of the plague ending
every game turn.


(PDF Page 119)

Klackons  
If wizard’s fortress is klackon city, reduces unrest by 2  
