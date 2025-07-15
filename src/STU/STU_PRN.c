/*
    STU Print

        Print Game-Data



    ¿ everything that means to use sprintf() ?

_CITIES[]
_players[]
_UNITS[]

*/

#include "STU_TYPE.H"

#include "../MOX/MOM_Data.h"

#include <stdio.h>
#include <string.h>



char unit_struct_buffer[4096];


// Define a sample struct
struct Person {
    char name[50];
    int age;
    float height;
};


// int8_t  owner_idx
// int8_t  moves2_max
// uint8_t type
// int8_t  Hero_Slot
// int8_t  Finished
// int8_t  moves2
// int8_t  dst_wx
// int8_t  dst_wy
// int8_t  Status
// int8_t  Level
// uint8_t pad2B_0Dh
// int16_t XP
// int8_t  Move_Failed
// int8_t  Damage
// int8_t  Draw_Priority
// uint8_t pad2B_13h
// int16_t in_tower
// int8_t  Sight_Range
// int8_t  mutations
// uint32_t enchantments
// int8_t  Rd_Constr_Left
// int8_t  Rd_From_X
// int8_t  Rd_From_Y
// uint8_t pad2B_1Fh
char _unit_struct_fmt[] = "unit_idx: %d, wx: %d, wy: %d, wp: %d, owner_idx: %d, moves2_max: %d, type: %u, Hero_Slot: %d, Finished: %d, moves2: %d, dst_wx: %d, dst_wy: %d, Status: %d, Level: %d, XP: %d, Move_Failed: %d, Damage: %d, Draw_Priority: %d, in_tower: %d, Sight_Range: %d, mutations: %d, enchantments: %04X, Rd_Constr_Left: %d, Rd_From_X: %d, Rd_From_Y: %d";

void Print_Unit_Structure(int16_t unit_idx)
{

    memset(unit_struct_buffer, 0, 4096);

    snprintf(
        unit_struct_buffer,
        sizeof(unit_struct_buffer),
        _unit_struct_fmt,
        unit_idx,
        _UNITS[unit_idx].wx,
        _UNITS[unit_idx].wy,
        _UNITS[unit_idx].wp,
        _UNITS[unit_idx].owner_idx,
        _UNITS[unit_idx].moves2_max,
        _UNITS[unit_idx].type,
        _UNITS[unit_idx].Hero_Slot,
        _UNITS[unit_idx].Finished,
        _UNITS[unit_idx].moves2,
        _UNITS[unit_idx].dst_wx,
        _UNITS[unit_idx].dst_wy,
        _UNITS[unit_idx].Status,
        _UNITS[unit_idx].Level,
        _UNITS[unit_idx].XP,
        _UNITS[unit_idx].Move_Failed,
        _UNITS[unit_idx].Damage,
        _UNITS[unit_idx].Draw_Priority,
        _UNITS[unit_idx].in_tower,
        _UNITS[unit_idx].Sight_Range,
        _UNITS[unit_idx].mutations,
        _UNITS[unit_idx].enchantments,
        _UNITS[unit_idx].Rd_Constr_Left,
        _UNITS[unit_idx].Rd_From_X,
        _UNITS[unit_idx].Rd_From_Y
    );

    // Print the content of the buffer to the console
    printf("%s\n", unit_struct_buffer);

}

void Print_Example(void)
{

    // Create an instance of the struct
    struct Person person1 = {"Alice", 30, 1.65};

    // Declare a buffer to store the formatted string
    char buffer[200]; // Ensure buffer is large enough for the formatted string


    // Use sprintf to format the struct data into the buffer
    // It's generally safer to use snprintf to prevent buffer overflows
    snprintf(
        buffer,
        sizeof(buffer),
        "Name: %s, Age: %d, Height: %.2f meters",
             person1.name, person1.age, person1.height
            );

    // Print the content of the buffer to the console
    printf("%s\n", buffer);

}
