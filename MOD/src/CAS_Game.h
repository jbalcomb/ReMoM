/*
    CAS_Game -- the CAS built-ins that read and change the game, for CAS.c.

    Scripts use Caster of Magic for Windows (CoM2) IDs: the constants Warlord's MASTER.CAS defines for buildings, retorts,
    realms, city and global enchantments, ores, overland tiles, events and difficulty.  CoM2 renumbered some of 1.31's
    lists and added entries 1.31 does not have, so each list goes through a table here.  An ID for something ReMoM does
    not have (a CoM2-only building, retort, enchantment or ore) reads as absent: 0, or -1 where the built-in returns an
    owner.

    Other conventions, from CoM2's Scripts.TXT:
      - cities are 1 .. MAXCITY() and units 1 .. MAXUNIT() (ReMoM index + 1); 0 is "none" (CITYONTILE of an empty
        square) and reads as owner -1 / value 0, because scripts read from it before checking; wizards are 0 (human)
        .. NOFWIZARDS()
      - planes are 1 (Arcanus) and 2 (Myrror); x wraps around the map, y outside it means "no tile"
      - GETCITYDATA / SETCITYDATA and GETWIZARDDATA / SETWIZARDDATA are 100 integer slots per city / wizard that belong to
        the scripts; they are saved with the game (sections "CASCITYD" and "CASWIZD ")
*/

#ifndef CAS_GAME_H
#define CAS_GAME_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* A game built-in: integer arguments in, one integer out (ignored for commands).  Returns 0 on success; on an error
   writes a message to err and returns -1, which fails the script. */
typedef int (*CAS_Game_Fn)(const int32_t * args, int nargs, int32_t * result, char * err, size_t err_size);

int CAS_Game_Difficulty(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Landsize(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Nofwizards(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Maxcity(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Cityowner(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Cityrace(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Isbuilt(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Gold(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Mana(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Getcitydata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Setcitydata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Getwizarddata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Setwizarddata(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Cityenchant(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Books(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Spellstate(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Hasglobal(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Oreinrange(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Terraininrange(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Planesizex(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Planesizey(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Cityontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Isfortress(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Banished(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Defeated(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Population(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Popunits(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Scoringoption(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Retort(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Buildingsowned(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Eventturn(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Eventval(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Treaty(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Contact(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Relation(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Homerace(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Researching(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Nofunitsontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Unitontile(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);
int CAS_Game_Getstat(const int32_t * a, int n, int32_t * result, char * err, size_t err_size);

/* Zeroes the script slots (GETCITYDATA / GETWIZARDDATA) and registers them with the save file. */
void CAS_Game_Reset_Slots(void);

#ifdef __cplusplus
}
#endif

#endif  /* CAS_GAME_H */
