#ifndef MOX_BASE_H
#define MOX_BASE_H



#pragma warning (disable : 4101)  // Warning C4101 unreferenced local variable
// #pragma warning (disable : 4701)
#pragma warning (disable : 4703)  // Error C4703 potentially uninitialized local pointer variable used
#pragma warning (disable : 6001)  // Warning C6001 Using uninitialized memory '...'.
#pragma warning(disable : 28183)  // Warning C28183 '...' could be '0', and is a copy of the value found in '...':  this does not adhere to the specification for the function 'fread'.



#define ST_FAILURE       0
#define ST_SUCCESS      -1

#define ST_FALSE         0
#define ST_TRUE          1

#define ST_NULL          0

#define ST_NONE          0
#define ST_UNDEFINED    -1  /* ¿ "Empty" ? (// Army List Screen - "Empty Hero Slot") */
/* Elsewhere, "NOT IN USE" ¿ for Fields ? ¿ Debug for outline of fields on screen */



#define SZ_16K_B            16384
#define SZ_32K_B            32768
#define SZ_PARAGRAPH_B         16



#endif /* MOX_BASE_H */
