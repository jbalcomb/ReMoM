#ifndef MOX_DEF_H
#define MOX_DEF_H

#include "MoX_BASE.H"

#ifndef STU_DEBUG
#define DLOG
#endif

#ifndef	MOX_DBG_BREAK
	#ifdef	_DEBUG
		#define	MOX_DBG_BREAK	__debugbreak
	#else
		#define	MOX_DBG_BREAK
	#endif
#endif

#define VIDEO_MODE_MODE_Y
// #define VIDEO_MODE_MODE_Y_2x
// #define VIDEO_MODE_MODE_Y_2x_XBGR
#define VIDEO_MODE       3  // vm_Mode_Y
// #define VIDEO_MODE       5  // vm_Mode_Y_2x
// #define VIDEO_MODE       6  // vm_Mode_Y_2x_XBGR



#define MOUSE_BUTTON_LEFT   1
#define MOUSE_BUTTON_RIGHT  2

#define INVALID_FIELD       -1000


#define AbsVal(_val_) ( (_val_) < 0 ? (-(_val_)) : (_val_) )

// Define macro to swap two numbers
// #define SWAP(x, y, temp) temp = x; x = y; y = temp;  ... macro expansion(temp = x; x = y; y = temp;)
// x = x + y - ( y = x );
// #define SWAP(x, y, T) do { T SWAP = x; x = y; y = SWAP; } while (0)  ...  invoke: SWAP(a, b, int);
#define SWAP(x, y) (x ^= y ^= x ^= y)
// temp = x2; x2 = x1; x1 = temp;
// temp = y2; y2 = y1; y1 = temp;
#define SWAP_X() (temp = x2; x2 = x1; x1 = temp;)
#define SWAP_Y() (temp = y2; y2 = y1; y1 = temp;)
#define SWAP_SHORT(_v1_,_v2_,_t_) ((_t_) = (_v2_); (_v2_) = (_v1_); (_v1_) = (_t_);)

#define UPPERCASE(_character_) do { if(((_character_) > 96) && ((_character_) < 123) ) { (_character_) -= 32; } } while (0)


/*
#define MIN(a, b) (((a) <= (b)) ? (a) : (b))
#define MIN(a, b) (((a) <  (b)) ? (a) : (b))
...JL vs. JLE...
...JL would take b when a is equal to be
...that does not *feel* logical

*/
/*
1oom  (ReMoO1)
C:\STU\developp\1oom\src\comp.h
¿ ADDSATT(), SUBSATT(), SUBSAT0() ?

Eh?
    Min(), Max(), Clamp()
    SetMin(), SetMax(), SetRange()
*/
#ifndef MIN
#define MIN(a, b)           ( ((a) <= (b)) ? (a) : (b) )
// #define MIN(a, b) (((a) <  (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a, b)           ( ((a) >= (b)) ? (a) : (b) )
#endif
#ifndef IS_EVEN
#define IS_EVEN(_number_)   ( ((_number_) % 2 == 0) )
#endif
#ifndef IS_ODD
#define IS_ODD(_number_)    ( ((_number_) % 2 == 1) )
#endif
#ifndef SETMIN
#define SETMIN(a, b) do { if ((b) > (a)) { (a) = (b); } } while (0)
// #define SETMIN(a, b) do { if ((a) < (b)) { (a) = (b); } } while (0)
#endif
#ifndef SETMAX
#define SETMAX(a, b) do { if ((b) < (a)) { (a) = (b); } } while (0)
#endif
#ifndef SETRANGE
#define SETRANGE(a, b, c) do { if (((c) <= (b)) || ((b) > (a))) { (a) = (b); } else if ((c) < (a)) { (a) = (c); } } while (0)
#endif
// e.g., _CITIES[itr].size = ((_CITIES[itr].population + 3) / 4);
#ifndef BUCKET
#define BUCKET(_value_, _size_) do { (((_value_) + ((_size_) - 1)) / (_size_) )} while (0)
#endif
// `1oom`
#define ADDSATT(_v_, _n_, _top_) do { int _t_; _t_ = (_v_) + (_n_); SETMIN(_t_, (_top_)); (_v_) = _t_; } while (0)
#define SUBSATT(_v_, _n_, _bot_) do { int _t_; _t_ = (_v_) - (_n_); SETMAX(_t_, (_bot_)); (_v_) = _t_; } while (0)
#define SUBSAT0(_v_, _n_) SUBSATT(_v_, _n_, 0)
#define TBLLEN(_t_) (sizeof((_t_)) / sizeof((_t_)[0]))

#define CLIPPED()  (x2 < screen_window_x1 || x1 > screen_window_x2 || y2 < screen_window_y1 || y1 > screen_window_y2)

// TODO  // ¿ Fastest Possible Intel-CPU Absolute Value *Function* ?
// TODO  #define ABS(x)  ( x = (x >= 0) ? x : -x );
// TODO  
// TODO  #define sgn(x)  ( (x < 0) ? -1 : ((x > 0) ? 1 : 0) )    /* macro to return the sign of a number */




/*
    Macros to translate coordinates from/to 'Movement Map', 'World Map', 'Reduced Map'
        ...
        ¿ including main_map_grid ?
        ¿ including reduced_map_grid ?
    
    MMAPX2WMAPX vs. MMX2WMX vs. MX2WX
    MMAPY2WMAPY vs. MMY2WMY vs. MY2WY
    ¿ congnitive effort to visually differentiate between MMAP, WMAP, RMAP, given the one character difference ?
    ¿ congnitive effort to eschew 'mx' as 'Mouse X', given the generally accepted practice in the industry ?
    ¿ lazy cunts and thier retarded concern for typing less characters ?

*/
#define MMAPX2WMAPX(_mmap_x_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH ) }
#define MMAPY2WMAPY(_mmap_y_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT ) }
#define MMX2WMX(_mmx_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH  ) }
#define MMY2WMY(_mmy_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT  ) }
#define MX2WX(_mx_) { ( (_map_x + _main_map_grid_x) / WORLD_WIDTH  ) }
#define MY2WY(_my_) { ( (_map_y + _main_map_grid_y) / WORLD_HEIGHT  ) }


/*
    Number <==> Index
*/
#define NUMBER2INDEX(_value_) ( (_value_) - 1 )
#define INDEX2NUMBER(_value_) ( (_value_) + 1 )
#define NUM2IDX(_value_)      ( (_value_) - 1 )
#define IDX2NUM(_value_)      ( (_value_) + 1 )
#define N2I(_value_)          ( (_value_) - 1 )
#define I2N(_value_)          ( (_value_) + 1 )


/*/
    _FORTRESS accessor macros
    FORT ~== _FORTRESS
    X,Y,P ~== wx,wy,wp
*/
#define FORTX() ( _FORTRESSES[player_idx].wx )
#define FORTY() ( _FORTRESSES[player_idx].wy )
#define FORTP() ( _FORTRESSES[player_idx].wp )


#define CITYX() ( _CITIES[city_idx].wx )
#define CITYY() ( _CITIES[city_idx].wy )
#define CITYP() ( _CITIES[city_idx].wp )

#define CITIESX() ( _CITIES[itr_cities].wx )
#define CITIESY() ( _CITIES[itr_cities].wy )
#define CITIESP() ( _CITIES[itr_cities].wp )

/*
    _UNITS[] accessor macros
    depends on `unit_idx` local variable
    UNIT...()
    X,Y,P,O,T,L
    wx, wy, wp, owner_idx, type, level

*/
#define UNITX() ( _UNITS[unit_idx].wx  )
#define UNITY() ( _UNITS[unit_idx].wy  )
#define UNITP() ( _UNITS[unit_idx].wp  )
#define UNITOWNER() ( _UNITS[unit_idx].owner_idx )
#define UNITTYPE()  ( _UNITS[unit_idx].type      )
#define UNITLEVEL() ( _UNITS[unit_idx].level     )

// if(_unit_type_table[_UNITS[unit_idx].type].Transport > 0)

#define HEROTYPE()  ( _UNITS[unit_idx].type      )
#define HEROSLOT()  ( _UNITS[unit_idx].Hero_Slot )

// "Hero" ~== "Hero Unit"
// "Hero Item" vs. "Item" ... _players[].Heroes[].Items[hero_item_idx] vs. _ITEMS[item_idx]
// _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]
// HEROITEM(_UNITS[unit_idx].owner_idx, _UNITS[unit_idx].Hero_Slot, 0)
#define HEROITEM(_px_,_hx_,_ix_)  ( _players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0] )

#define NAY_HERO()  ( _UNITS[unit_idx].Hero_Slot <= ST_UNDEFINED )
#define YAY_HERO()  ( _UNITS[unit_idx].Hero_Slot > ST_UNDEFINED  )

/*
    "Battle Unit" - Combat

    Unit Index for _UNITS[]
    Battle Unit Index for _battle_units[]  (AKA _combat_data[])
    ¿ Battle Unit Owner vs. Unit Owner ?
    e.g.,
        battle_units[itr_battle_units].unit_idx
            BUNITIDX(itr_battle_units)
        _UNITS[battle_units[itr_battle_units].unit_idx].owner_idx
            UNITOWNER(BUNITIDX(itr_battle_units))
        _UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot
            HEROSLOT(BUNITIDX(itr_battle_units))

¿ UX for Unit Index ?
¿ BX for Battle Unit Index ?

Battle Unit is (not) Hero Unit
-reqd: Battle Unit Index
NAYBHERO()
YAYBHERO()
if(_UNITS[battle_units[itr_battle_units].unit_idx].Hero_Slot > -1)

*/

// #define BU_UI()            ( battle_units[bunit_idx].unit_idx  )
// #define BU_OI()            ( battle_units[bunit_idx].owner_idx )

#define BATTLE_UNIT_OWNER(_bux_)             ( _UNITS[battle_units[(_bux_)].unit_idx].owner_idx )
#define BATTLE_UNIT_HEROSLOT(_bux_)          ( _UNITS[battle_units[(_bux_)].unit_idx].Hero_Slot )
#define BATTLE_UNIT_UNIT_INDEX(_bux_)        ( battle_units[(_bux_)].unit_idx  )
#define BATTLE_UNIT_CONTROLLER_INDEX(_bux_)  ( battle_units[(_bux_)].controller_idx )
#define BATTLE_UNIT_STATUS(_bux_)  ( battle_units[(_bux_)].Status )

// #define BUNITIDX(_bux_)    do { battle_units[(_bux_)].unit_idx } while (0)
#define BUNITUNIT()         ( battle_units[battle_unit_idx].unit_idx   )
#define BUNITCONTROLLER()   ( battle_units[battle_unit_idx].controller_idx  )
#define BUNITOWNER()        ( _UNITS[BUNITUNIT()].owner_idx )
#define BUNITHEROSLOT()     ( _UNITS[BUNITUNIT()].Hero_Slot )

#define BUNITSUNIT()        ( battle_units[itr_battle_units].unit_idx  )
#define BUNITSCONTROLLER()  ( battle_units[itr_battle_units].controller_idx )
#define BUNITSOWNER()       ( _UNITS[BUNITSUNIT()].owner_idx )
#define BUNITSHEROSLOT()    ( _UNITS[BUNITSUNIT()].Hero_Slot )



#define POC_MACRO_1         silly
#define POC_MACRO_2         printf("hello, world\n")
#define POC_MACRO_3()       printf("hello, world\n")
#define POC_MACRO_4(_txt_)  printf(_txt_)


// #define CID customer_record.account.cid
// #define TOOFAR array[MAX_ARRAY_SIZE]
// 
// #define FOR_LOOP(n)  for(i=0; i<(n); i++)
// int i;
// FOR_LOOP(3)
// {
//   puts("Inside for loop\n");
// }





#define HU_LEADERSHIP()  ( (_HEROES[hero_owner_idx][hero_type].Abilities_LO & HSA_LO_LEADERSHIP) != 0)
// #define HERO_LEADERSHIP(_pi_,_ht_)  ( (_HEROES[(_pi_)][(_ht_)].Abilities_LO & HSA_LO_LEADERSHIP) != 0)
// #define HERO_LEADERSHIP(_player_idx_,_hero_type_)     ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEADERSHIP    ) != 0)
// #define HERO_LEADERSHIP2(_player_idx_,_hero_type_)    ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEADERSHIP2   ) != 0)
// #define HERO_LEGENDARY(_player_idx_,_hero_type_)      ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEGENDARY     ) != 0)
// #define HERO_LEGENDARY2(_player_idx_,_hero_type_)     ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEGENDARY2    ) != 0)
// #define HERO_BLADEMASTER(_player_idx_,_hero_type_)    ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_BLADEMASTER   ) != 0)
// #define HERO_BLADEMASTER2(_player_idx_,_hero_type_)   ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_BLADEMASTER2  ) != 0)
// #define HERO_ARMSMASTER(_player_idx_,_hero_type_)     ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_ARMSMASTER    ) != 0)
// #define HERO_ARMSMASTER2(_player_idx_,_hero_type_)    ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_ARMSMASTER2   ) != 0)
// #define HERO_CONSTITUTION(_player_idx_,_hero_type_)   ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_CONSTITUTION  ) != 0)
// #define HERO_CONSTITUTION2(_player_idx_,_hero_type_)  ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_CONSTITUTION2 ) != 0)
// #define HERO_MIGHT(_player_idx_,_hero_type_)          ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_MIGHT         ) != 0)
// #define HERO_MIGHT2(_player_idx_,_hero_type_)         ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_MIGHT2        ) != 0)
// #define HERO_ARCANE_POWER(_player_idx_,_hero_type_)   ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_ARCANE_POWER  ) != 0)
// #define HERO_ARCANE_POWER2(_player_idx_,_hero_type_)  ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_ARCANE_POWER2 ) != 0)
// #define HERO_SAGE(_player_idx_,_hero_type_)           ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_SAGE          ) != 0)
// #define HERO_SAGE2(_player_idx_,_hero_type_)          ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_SAGE2         ) != 0)
// #define HERO_PRAYERMASTER(_player_idx_,_hero_type_)   ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_PRAYERMASTER  ) != 0)
// #define HERO_PRAYERMASTER2(_player_idx_,_hero_type_)  ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_PRAYERMASTER2 ) != 0)
// #define HERO_AGILITY2(_player_idx_,_hero_type_)       ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_AGILITY2      ) != 0)
// #define HERO_LUCKY(_player_idx_,_hero_type_)          ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_LUCKY         ) != 0)
// #define HERO_CHARMED(_player_idx_,_hero_type_)        ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_CHARMED       ) != 0)
// #define HERO_NOBLE(_player_idx_,_hero_type_)          ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_NOBLE         ) != 0)
// #define HERO_FEMALE(_player_idx_,_hero_type_)         ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_FEMALE        ) != 0)
// #define HERO_AGILITY(_player_idx_,_hero_type_)        ( ( _HEROES[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_AGILITY       ) != 0)

// DELETE  #define HERO_LEADERSHIP(_player_idx_,_hero_type_)     ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEADERSHIP    ) != 0)
// DELETE  #define HERO_LEADERSHIP2(_player_idx_,_hero_type_)    ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEADERSHIP2   ) != 0)
// DELETE  #define HERO_LEGENDARY(_player_idx_,_hero_type_)      ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEGENDARY     ) != 0)
// DELETE  #define HERO_LEGENDARY2(_player_idx_,_hero_type_)     ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_LEGENDARY2    ) != 0)
// DELETE  #define HERO_BLADEMASTER(_player_idx_,_hero_type_)    ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_BLADEMASTER   ) != 0)
// DELETE  #define HERO_BLADEMASTER2(_player_idx_,_hero_type_)   ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_BLADEMASTER2  ) != 0)
// DELETE  #define HERO_ARMSMASTER(_player_idx_,_hero_type_)     ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_ARMSMASTER    ) != 0)
// DELETE  #define HERO_ARMSMASTER2(_player_idx_,_hero_type_)    ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_ARMSMASTER2   ) != 0)
// DELETE  #define HERO_CONSTITUTION(_player_idx_,_hero_type_)   ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_CONSTITUTION  ) != 0)
// DELETE  #define HERO_CONSTITUTION2(_player_idx_,_hero_type_)  ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_CONSTITUTION2 ) != 0)
// DELETE  #define HERO_MIGHT(_player_idx_,_hero_type_)          ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_LO & HSA_LO_MIGHT         ) != 0)
// DELETE  #define HERO_MIGHT2(_player_idx_,_hero_type_)         ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_MIGHT2        ) != 0)
// DELETE  #define HERO_ARCANE_POWER(_player_idx_,_hero_type_)   ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_ARCANE_POWER  ) != 0)
// DELETE  #define HERO_ARCANE_POWER2(_player_idx_,_hero_type_)  ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_ARCANE_POWER2 ) != 0)
// DELETE  #define HERO_SAGE(_player_idx_,_hero_type_)           ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_SAGE          ) != 0)
// DELETE  #define HERO_SAGE2(_player_idx_,_hero_type_)          ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_SAGE2         ) != 0)
// DELETE  #define HERO_PRAYERMASTER(_player_idx_,_hero_type_)   ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_PRAYERMASTER  ) != 0)
// DELETE  #define HERO_PRAYERMASTER2(_player_idx_,_hero_type_)  ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_PRAYERMASTER2 ) != 0)
// DELETE  #define HERO_AGILITY2(_player_idx_,_hero_type_)       ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_AGILITY2      ) != 0)
// DELETE  #define HERO_LUCKY(_player_idx_,_hero_type_)          ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_LUCKY         ) != 0)
// DELETE  #define HERO_CHARMED(_player_idx_,_hero_type_)        ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_CHARMED       ) != 0)
// DELETE  #define HERO_NOBLE(_player_idx_,_hero_type_)          ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_NOBLE         ) != 0)
// DELETE  #define HERO_FEMALE(_player_idx_,_hero_type_)         ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_FEMALE        ) != 0)
// DELETE  #define HERO_AGILITY(_player_idx_,_hero_type_)        ( ( p_heroes[(_player_idx_)][(_hero_type_)].Abilities_HI & HSA_HI_AGILITY       ) != 0)

#define HERO_LEADERSHIP(_player_idx_,_hero_type_)     ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LEADERSHIP    ) != 0)
#define HERO_LEADERSHIP2(_player_idx_,_hero_type_)    ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LEADERSHIP2   ) != 0)
#define HERO_LEGENDARY(_player_idx_,_hero_type_)      ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LEGENDARY     ) != 0)
#define HERO_LEGENDARY2(_player_idx_,_hero_type_)     ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LEGENDARY2    ) != 0)
#define HERO_BLADEMASTER(_player_idx_,_hero_type_)    ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_BLADEMASTER   ) != 0)
#define HERO_BLADEMASTER2(_player_idx_,_hero_type_)   ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_BLADEMASTER2  ) != 0)
#define HERO_ARMSMASTER(_player_idx_,_hero_type_)     ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_ARMSMASTER    ) != 0)
#define HERO_ARMSMASTER2(_player_idx_,_hero_type_)    ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_ARMSMASTER2   ) != 0)
#define HERO_CONSTITUTION(_player_idx_,_hero_type_)   ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_CONSTITUTION  ) != 0)
#define HERO_CONSTITUTION2(_player_idx_,_hero_type_)  ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_CONSTITUTION2 ) != 0)
#define HERO_MIGHT(_player_idx_,_hero_type_)          ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_MIGHT         ) != 0)
#define HERO_MIGHT2(_player_idx_,_hero_type_)         ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_MIGHT2        ) != 0)
#define HERO_ARCANE_POWER(_player_idx_,_hero_type_)   ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_ARCANE_POWER  ) != 0)
#define HERO_ARCANE_POWER2(_player_idx_,_hero_type_)  ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_ARCANE_POWER2 ) != 0)
#define HERO_SAGE(_player_idx_,_hero_type_)           ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_SAGE          ) != 0)
#define HERO_SAGE2(_player_idx_,_hero_type_)          ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_SAGE2         ) != 0)
#define HERO_PRAYERMASTER(_player_idx_,_hero_type_)   ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_PRAYERMASTER  ) != 0)
#define HERO_PRAYERMASTER2(_player_idx_,_hero_type_)  ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_PRAYERMASTER2 ) != 0)
#define HERO_AGILITY2(_player_idx_,_hero_type_)       ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_AGILITY2      ) != 0)
#define HERO_LUCKY(_player_idx_,_hero_type_)          ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_LUCKY         ) != 0)
#define HERO_CHARMED(_player_idx_,_hero_type_)        ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_CHARMED       ) != 0)
#define HERO_NOBLE(_player_idx_,_hero_type_)          ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_NOBLE         ) != 0)
#define HERO_FEMALE(_player_idx_,_hero_type_)         ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_FEMALE        ) != 0)
#define HERO_AGILITY(_player_idx_,_hero_type_)        ( ( _HEROES2[(_player_idx_)]->heroes[(_hero_type_)].abilities & HSA_AGILITY       ) != 0)


/*
Macro to get little-endian, 32-bit, unsigned value
item powers from s_ITEM structure
via `item_idx` or specified item index

offset to item powers field is 0x2E (46d)

item_powers = GET_4B_OFS((uint8_t*)&_ITEMS[item_idx], 0x2E);
*/
#define ITEM_OFS_POWERS  0x2E
#define GET_ITEM_POWERS(_item_idx_)  ( GET_4B_OFS((uint8_t*)&_ITEMS[(_item_idx_)], ITEM_OFS_POWERS) )

// #define ITEM_POWER(_item_idx_,_item_power_)  ( ((_ITEMS[(_item_idx_)].Powers & (1 << (_item_power_))) != 0) )
#define ITEM_POWER(_item_idx_,_item_power_)  ( ((GET_4B_OFS((uint8_t*)&_ITEMS[(_item_idx_)], 0x2E) & (1 << (_item_power_))) != 0) )

// ip_Invulnerability = 32 ... warning C4293: '<<': shift count negative or too big, undefined behavior
// 1000 0000 0000 0000 0000 0000 0000 0000
//                                       ^ bit 1  ((_ITEMS[(_item_idx_)].Powers & (1 << (0))) != 0)
// ^ bit 32 ((_ITEMS[(_item_idx_)].Powers & (1 << (31))) != 0)
// ((_ITEMS[(_item_idx_)].Powers & (1 << (_item_power_))) != 0)

// Item_Charges = _ITEMS[_players[_UNITS[unit_idx].owner_idx].Heroes[_UNITS[unit_idx].Hero_Slot].Items[0]].embed_spell_cnt;

// #define UNIT_ENCHANTMENT(_unit_idx_,_item_power_)  ( ((_ITEMS[(_item_idx_)].Powers & (1 << (_item_power_))) != 0) )



/*
    Accessor Macros
        _world_map
            Terrain Type
        TBL_Terr_Specials

        _map_square_flags  AKA TBL_Terrain_Flags
            Volcano Player, Road - Regular, Road - Enchanted, Corruption

*/
// #define GET_TERRAIN_TYPE(_value_, _size_) do { GET_2B_OFS(_world_maps, ((wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2))) } while (0)
/*
    gets the uint16_t 'Terrain Type' from _world_maps
*/
#define GET_TERRAIN_TYPE(_wx_, _wy_, _wp_)  ( GET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2))) )
// terrain_type = (*( (uint16_t *)(_world_maps + ( (wp * WORLD_SIZE) + (wy * WORLD_WIDTH) + (wx) )) ) % NUM_TERRAIN_TYPES);
// NOWORKIE  #define TERRAIN_TYPE(_wx_, _wy_, _wp_)      ( (*( (uint16_t *)(_world_maps + ( ((_wp_) * WORLD_SIZE) + ((_wy_) * WORLD_WIDTH) + (_wx_) )) ) % NUM_TERRAIN_TYPES) )
#define TERRAIN_TYPE_INDEX(_wx_, _wy_, _wp_)  ( GET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2)))                     )
#define TERRAIN_TYPE(_wx_, _wy_, _wp_)        ( GET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2))) % NUM_TERRAIN_TYPES )

// #define SET_TERRAIN_TYPE(_terrain_type_) do { SET_2B_OFS(_world_maps, ((wp * WORLD_SIZE * 2) + (wy * WORLD_WIDTH * 2) + (wx * 2)), (_terrain_type_)) } while (0)
/*
    sets the uint16_t 'Terrain Type' in _world_maps
*/
#define SET_TERRAIN_TYPE(_wx_, _wy_, _wp_, _terrain_type_)  ( SET_2B_OFS(_world_maps, (((_wp_) * WORLD_SIZE * 2) + ((_wy_) * WORLD_WIDTH * 2) + ((_wx_) * 2)), (_terrain_type_)) )


/*
    Terrain Special

...( & 0x0F) ~== 'any mineral'
*/
#define GET_TERRAIN_SPECIAL(_wx_, _wy_, _wp_)  ( *(TBL_Terr_Specials + ((_wp_) * WORLD_SIZE) + ((_wy_) * WORLD_WIDTH) + (_wx_)) )


//            _square_explored[((0 * WORLD_SIZE) + (itr_y * WORLD_WIDTH) + itr_x)] = 0xF;  /* 0b11111111 */
//            _square_explored[((1 * WORLD_SIZE) + (itr_y * WORLD_WIDTH) + itr_x)] = 0xF;  /* 0b11111111 */
// MoO2  Clear_Planet_Explored_Flags_()
// MoO2  Set_Planet_Explored_Flags_()
#define GET_SQUARE_EXPLORED(_wx_, _wy_, _wp_)           ( _square_explored[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ]              )
#define SET_SQUARE_EXPLORED(_wx_, _wy_, _wp_, _value_)  ( _square_explored[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ] = (_value_)  )

#define UNEXPLORED  0x0  /* no bits are set */  /* test as 'any bits are set' */
#define EXPLORED    0xF  /* ¿ all bits are set ? */  /* test as 'all bits are set' */

#define SQUARE_UNEXPLORED(_wx_, _wy_, _wp_)             (  GET_SQUARE_EXPLORED((_wx_), (_wy_), (_wp_)) == UNEXPLORED  )
/* Intuitive?  explored means NOT unexplored */
#define SQUARE_EXPLORED(  _wx_, _wy_, _wp_)             (  !SQUARE_UNEXPLORED( (_wx_), (_wy_), (_wp_))                )
/* ¿ ...none, any, all... ? */


/*
    Map Square Flags

#define MSF_ROAD         0x08
#define TF_Enc_Road     0x10
#define MSF_CORRUPTION   0x20
#define TF_Unk_40       0x40
#define TF_NoRoad80     0x80  // ~== -1 ST_UNDEFINED
#define TF_AnyRoad      0x18
*/
#define MAP_SQUARE_FLAG(_wx_, _wy_, _wp_)           ( _map_square_flags[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ]             )
#define GET_MAP_SQUARE_FLAG(_wx_, _wy_, _wp_)           ( _map_square_flags[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ]             )
#define SET_MAP_SQUARE_FLAG(_wx_, _wy_, _wp_, _value_)  ( _map_square_flags[ ( ( (_wp_) * WORLD_SIZE ) + ( (_wy_) * WORLD_WIDTH ) + (_wx_) ) ] = (_value_) )

#define CLEAR_MAP_SQUARE_FLAGS_VOLCANO_PLAYER_IDX()  ( SET_MAP_SQUARE_FLAG(wx, wy, wp, (GET_MAP_SQUARE_FLAG(wx, wy, wp) & 0b11111000)) )

#define MAP_SQUARE_HAS_NO_ROAD(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & TF_NoRoad80 )
#define MAP_SQUARE_HAS_ROAD(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & TF_AnyRoad )
#define MAP_SQUARE_HAS_ROAD_REGULAR(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & TF_Road )
#define MAP_SQUARE_HAS_ROAD_ENCHANTED(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & TF_Enc_Road )
#define MAP_SQUARE_HAS_CORRUPTION(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & MSF_CORRUPTION )
#define MAP_SQUARE_HAS_UNKOWN(_wx_, _wy_, _wp_)  ( GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & TF_Unk_40 )

#define MAP_SQUARE_CORRUPT(_wx_, _wy_, _wp_)  ( (GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & MSF_CORRUPTION) == 1 )
#define MAP_SQUARE_UNCORRUPT(_wx_, _wy_, _wp_)  ( (GET_MAP_SQUARE_FLAG((_wx_), (_wy_), (_wp_)) & MSF_CORRUPTION) == 0 )

// ovr094:06CF 26 F6 07 20                                     test    [byte ptr es:bx], TF_Corruption
// 26 F6 07 40                                     test    [byte ptr es:bx], TF_Unk_40



// something or other for color difference/distance
#define REMAP_THRESHOLD  21



#define HUMAN_PLAYER_IDX                   0
#define NEUTRAL_PLAYER_IDX                 5
#define MOO_MONSTER_PLAYER_IDX             6

#define NUM_BANNER_COLORS                  6
#define NEUTRAL_PLAYER_BANNER_COLOR_IDX    5
#define NEUTRAL_PLAYER_BANNER_COLOR       50

/*
WZD dseg:7011

uint8_t COL_City_Banner[] = { ... };

COL_City_Banner0    db  96,  97,  98,  99, 100  ¿ ~ blue ?
COL_City_Banner1    db  72,  73,  74,  75,  76  ¿ ~ green ?
COL_City_Banner2    db 205, 206, 207, 208, 209  ~ purple
COL_City_Banner3    db 201, 165, 203, 166,  45  ~ red
COL_City_Banner4    db 209, 210, 211, 212, 213  ~ yellow
UU_COL_City_Banner5 db  29,  29,  28,  28,  27  ¿ ?
UU_COL_City_Banner6 db  29,  29,  28,  28,  27  ¿ ?


// ¿ MoO2: ~ remap_picture_colors ?
// WZD dseg:00AA 69 6A 6B 6C 6D                                  COL_Banners_P0 db 105, 106, 107, 108, 109
// WZD dseg:00AF 49 4A 4B 4C 4D                                  COL_Banners_P1 db 73, 74, 75, 76, 77
// WZD dseg:00B4 79 7A 7B 7C 7D                                  COL_Banners_P2 db 121, 122, 123, 124, 125
// WZD dseg:00B9 C9 A5 CB A6 2D                                  COL_Banners_P3 db 201, 165, 203, 166, 45
// WZD dseg:00BE A0 A1 A2 B2 B4                                  COL_Banners_P4 db 160, 161, 162, 178, 180
// WZD dseg:00C3 30 31 32 33 34                                  COL_Banners_P5 db 48, 49, 50, 51, 52
uint8_t COL_Banners[] = {
    0x69, 0x6A, 0x6B, 0x6C, 0x6D,
    0x49, 0x4A, 0x4B, 0x4C, 0x4D,
    0x79, 0x7A, 0x7B, 0x7C, 0x7D,
    0xC9, 0xA5, 0xCB, 0xA6, 0x2D,
    0xA0, 0xA1, 0xA2, 0xB2, 0xB4,
    0x30, 0x31, 0x32, 0x33, 0x34
};

// WZD dseg:00C8
uint8_t COL_Banners2[36] = {
    98, 216, 123, 43, 179, 50,
    201,  0, 165,  0, 203,  0,
    121,  0, 122,  0, 123,  0,
     13,  0,  14,  0,  15,  0,
     73,  0,  74,  0,  75,  0,
    105,  0, 106,  0, 107,  0
};

*/
#define BANNER_COLOR_BLUE                172
#define BANNER_COLOR_GREEN               216
#define BANNER_COLOR_PURPLE              124
#define BANNER_COLOR_RED                 201
#define BANNER_COLOR_YELLOW              211
#define BANNER_COLOR_BROWN                50
#define BANNER_COLOR_BLUE_171            171
#define BANNER_COLOR_BLUE_172            172
#define BANNER_COLOR_PURPLE_123          123
#define BANNER_COLOR_PURPLE_124          124
#define BANNER_COLOR_PURPLE_205          205
#define BANNER_COLOR_YELLOW_210          210
#define BANNER_COLOR_YELLOW_211          211

#define MAX_UNIT_COUNT                  1000    /* AKA MAX_UNITS  used by Create_Unit__WIP(); City_Apply_Production(); Determine_Offer(); */

#define MAX_CITY_POPULATION 25
#define MAX_CITY_POPULATION_NEUTRAL_PLAYER 8  // NOTE: *should* be ((_difficulty + 1) * 2))  (({0,1,2,3} + 1) * 2) = {2,4,6,8}
#define MAX_CITY_SIZE        5

#define MAX_LEVEL_HERO       7



// World (Grid) Dimensions, in Map Sqaures
#define WORLD_WIDTH         60
#define WORLD_HEIGHT        40
#define MAP_WIDTH           12
#define MAP_HEIGHT          10
#define MOVEMENT_MAP_WIDTH  12
#define MOVEMENT_MAP_HEIGHT 10
#define REDUCED_MAP_WIDTH   58
#define REDUCED_MAP_HEIGHT  30
/*
    World / Movement-Map Square Dimensions
*/
#define SQUARE_WIDTH        20  // in Pixels
#define SQUARE_HEIGHT       18  // in Pixels



#define CARTOGRAPHER_MAP_WIDTH  (WORLD_WIDTH  * 4)
#define CARTOGRAPHER_MAP_HEIGHT (WORLD_HEIGHT * 4)



#ifdef VIDEO_MODE_MODE_Y
#define SCREEN_WIDTH    320
#define SCREEN_HEIGHT   200
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     319
#define SCREEN_YMAX     199
#define CURSOR_WIDTH     16
#define CURSOR_HEIGHT    16
#endif
#ifdef VIDEO_MODE_MODE_Y_2x
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif
#ifdef VIDEO_MODE_MODE_Y_2x_XBGR
#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   400
#define SCREEN_XMIN       0
#define SCREEN_YMIN       0
#define SCREEN_XMAX     639
#define SCREEN_YMAX     399
#define CURSOR_WIDTH     32
#define CURSOR_HEIGHT    32
#endif


/*
    SimTex Colors
*/

#define MINIMAP_MARK_COLOR  15
#define REDUCED_MAP_BOX_COLOR   13

// Unit_Window_Draw_Unit_Attributes()
#define DAMAGE_BAR_GREEN        0xD9  // 217
#define DAMAGE_BAR_YELLOW       0xD3  // 211
#define DAMAGE_BAR_RED          0xCA  // 202

/* ¿ Normal vs. Hero ? */
#define EXPERIENCE_LEVEL_SILVER     
#define EXPERIENCE_LEVEL_GOLD       
#define EXPERIENCE_LEVEL_RED        



/*
    MoM / MoX Palette / Color-Map (CMAP) / Color-Table (CTBL)
    7?
*/
#define PALETTE_EMPERATO      0  /* Main Screen;  ¿ "EMPERATO - main game palette" ? */
#define PALETTE_LOADSAVE      1  /* Load/Save Screen;  ¿ "LOADSAVE - load save palette" ? */
#define PALETTE_ARCANUS       2  /* Main Menu Screen;  ¿ "ARCANUS - Magic Castle View" ? */



#define ST_TRANSPARENT       0  /*  */
#define ST_BLACK             1  /*  */

#define ST_LIGHT_GRAY        7  /* used for {nodes, lairs, towers} on the Cartographer Screen */

#define ST_GRAY              8

#define ST_LIGHT_MAGENTA    13

#define ST_WHITE            15
// "silver" used for experience level rings/circles?
// same as reduced map box?
// white/bright white for reduced map mark, and GP/MP font color, and outline/aura color for unit enchantment outline?

#define ST_BROWN            16  /* used for the Neutral Player's cities on the 'Cartographer Screen' */

// main palette  EMPERATO
#define ROAD_BUILD_BLUE     108  /* {108, 109, 110, 111} */
#define ROAD_BUILD_RED      200  /* {200, 201, 202, 203} */

// ¿ which palette ?
// Production Screen / Build Table
#define PAL_GREY_DARK  4  // ¿ font color ?
#define PAL_GREY_LIGHT 5  // ¿ font color ?

#define PAL_TURQUOISE_LIGHT  244
#define PAL_TURQUOISE_DARK   248
#define PAL_TURQUOISE_DIM    251
#define PAL_TURQUOISE_BRIGHT 255


/*
    EGA / VGA Colors
        $00 Black
        $01 Blue
        $02 Green
        $03 Cyan
        $04 Red
        $05 Magenta
        $06 Brown
        $07 White
        $08 Gray
        $09 Light Blue
        $0A Light Green
        $0B Light Cyan
        $0C Light Red
        $0D Light Magenta
        $0E Yellow
        $0F Bright White
*/




#define CLROFF() { Set_Page_Off(); Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0); }
#define CLRON()  { Set_Page_On();  Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 0); }

// MoO2  Darken_Fill()
// MoO2  Darken_Main_Screen_Background_()
#define DARKEN_SCREEN()  { Gradient_Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, 3, ST_NULL, ST_NULL, ST_NULL, ST_NULL); }

// TODO  GUI_String_1 = (char *)Near_Allocate_First(100);
// TODO  GUI_String_2 = (char *)Near_Allocate_Next(100);

// TODO  full_screen_ESC_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, hotkey_ESC, ST_UNDEFINED);
// char hotkey_ESC[] = "\x1B";





#endif /* MOX_DEF_H */
