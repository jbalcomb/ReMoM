#ifndef MOX_GAM_H
#define MOX_GAM_H

#include "MoM.H"



/*
Level and Status (signed integer)
from	to	
0x0000	0x0008	hero level - 1
available hero

0xffec	-	recruited or killed by irreversible damage hero
0xfff7	0xffff	0 - hero level
dead hero

0x01	1	Hero ability bitmask
0x80	Blademaster*
0x40	Blademaster
0x10	Legendary*
0x08	Legendary
0x02	Leadership*
0x01	Leadership
0x03	0x01	1	Hero ability bitmask
0x80	Might
0x20	Constitution*
0x10	Constitution
0x04	Armsmaster*
0x02	Armsmaster
0x04	0x01	1	Hero ability bitmask
0x40	Sage*
0x20	Sage
0x08	Arcane Power*
0x04	Arcane Power
0x01	Might*

0x05	0x01	1	Hero ability or gender bitmask
0x80	Agility
0x40	Female
0x20	Noble
0x10	Charmed
0x08	Lucky
0x04	Agility*
0x02	Prayermaster*
0x01	Prayermaster
*/

// struct s_SAVE_GAM
// {
//     /* 00 */ struct s_HERO p0_heroes[35];
// 
// };



#ifdef __cplusplus
extern "C" {
#endif

// DELETEME  extern struct s_SAVE_GAM save_gam;


void TST_Validate_GameData(void);
void Read_SAVE_GAM(void);
void Write_SAVE_GAM(void);


// WZD o50p01
void Save_SAVE_GAM(int16_t save_gam_idx);

// WZD o50p02
void Load_SAVE_GAM(int16_t save_gam_idx);


#ifdef __cplusplus
}
#endif

#endif  /* MOX_GAM_H */
