#ifndef MOX_TYPE_H
#define MOX_TYPE_H



// #include <inttypes.h>
// #include <stdbool.h>

// ¿ #include <cstdint.h> ?
#include <stdint.h>
// #define _STDINT
// #ifndef _STDINT
// typedef signed char        int8_t;
// typedef short              int16_t;
// typedef int                int32_t;
// typedef long long          int64_t;
// typedef unsigned char      uint8_t;
// typedef unsigned short     uint16_t;
// typedef unsigned int       uint32_t;
// typedef unsigned long long uint64_t;
// #endif



typedef unsigned char * byte_ptr;
typedef unsigned char * SAMB_ptr;
typedef long long       SAMB_INT;  // TODO  finish this out - ~ pointer to pict or int for -1/INVALID, but 16-bit DOS 2-byte vs. 64-bit Win 8-byte (short vs. long long)

typedef unsigned char       byte;
typedef unsigned short int  word;
typedef unsigned int        dword;



/* CLAUDE  Unified "impassable tile / infinite cost" sentinel for the three shortest-path solvers (overland Move_Path_Find, combat Combat_Move_Path_Find, new-game Find_Shortest_Path). Single home here, where every one of those translation units already reaches MOX_TYPE.h; replaces the former duplicate `#define INF 0xFF` in Combat.h, MovePath.c, and MAPGEN.c. IS_INF()'s uint8_t cast makes the test robust against signed sign-extension when reading a stored 0xFF out of a signed byte array -- use the bare INF where you specifically need the OG's raw signed comparison preserved. */
#define INF                     0xFF
#define IS_INF(_value_)         ((uint8_t)(_value_) == (uint8_t)INF)



#endif /* MOX_TYPE_H */
