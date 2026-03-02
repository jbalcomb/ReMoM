/*
    STU Extract
    This module contains functions for extracting data from the game files, such as LBX archives.
    This is useful for debugging, modding, and understanding the game data formats.
    It may also be used for generating new game data or converting data to more modern formats.
    The functions in this module are not intended to be used in the main game loop, but rather as
    standalone utilities or tools for working with the game data.
    For example, Dump_TERRTYPE_LBX() can be used to read the TERRTYPE.LBX file and output its contents
    in a human-readable format, which can help with understanding the terrain types and how they are used
    in the game.

*/

#include "STU_XTRC.h"
#include "STU_FIO.h"
#include "../../MoX/src/LBX_Load.h"

/*
TERRTYPE.LBX
5 * 512

*/
void Dump_TERRTYPE_LBX(void)
{
    int16_t * terrtype = 0;
    char terrtype_lbx_file[] = "TERRTYPE.LBX";

    terrtype = (int16_t *)Near_Allocate_First((5 * 512));

    LBX_Load_Data_Static(terrtype_lbx_file, 0, (SAMB_ptr)terrtype, 0, 5, 512);

    /*
        The first record is used for Ocean, Tundra, and Desert.
    */

    STU_File_Open(NULL, "TERRTYPE_DUMP.txt", "w");
    for(int i = 0; i < 5; i++)
    {
        fprintf(stdout, "Record %d:\n", i);
        for(int j = 0; j < 512; j++)
        {
            fprintf(stdout, "%d ", terrtype[i * 512 + j]);
        }
        fprintf(stdout, "\n");
    }

    /*
        The second record is used for Mountain and Hills,
        but, for Hills, it is offset 16 values into the record.
        My current theory is that Hills was mean to be the third record.
        That might also account for the oddity of there being 16 and only 16 values in the third record.
    */

}
