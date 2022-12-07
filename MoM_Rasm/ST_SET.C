
#include "MoX_TYPE.H"    /* word */

#include "ST_SET.H"     /* magic_set */

#include <STDIO.H>      /* FILE; fclose(), fopen(), fread(), fseek(); */

#include <STRING.H>     /* strcpy() */


/*

SEE: MoM-MAGIC_SET.md


*/

// o49p01  Settings Screen
// o49p02  Settings Screen Draw
// o49p03   Load Settings
//          Save Settings
// o49p04   Reset Settings
// o49p05   Load Settings Screen Help

// MGC o49p03
void Load_MAGIC_SET(void)
{
    char file_name[30];
    FILE * file_pointer;
    int itr;

    if(DIR(&file_name,"MAGIC.SET") == 0)
    {
        Set_Default_Game_Settings();
        // Save_MAGIC_SET();
        file_pointer = fopen("MAGIC_SET","WB");
        fwrite(&magic_set, 466, 1, file_pointer);
    }
    else
    {
        file_pointer = fopen("MAGIC_SET","RB");
        fread(&magic_set, 466, 1, file_pointer);
    }
    fclose(file_pointer);

    /*

    if( !(magic_set.Movement_Anims - 1) <= 0 )
        magic_set.Movement_Anims = 1
    if( !(magic_set.Movement_Anims - 0) >= 0 )
        magic_set.Movement_Anims = 1
    if( !(magic_set.Raze_City] - 1) <= 0 )
        magic_set.Raze_City = 1
    if( !(magic_set.Raze_City] - 0) >= 0 )
        magic_set.Raze_City = 1
    */
    
    if(magic_set.HallofFame_Scores == 0)
    {
        for(itr = 0; itr < 10; itr++)
        {
            magic_set.HallofFame_Scores[itr] = 0;
            magic_set.HallofFame_Races[itr] = 0;
            // magic_set.HallofFame_Names[itr] = 0;
            strcpy(magic_set.HallofFame_Names[itr].Name,'\0');
        }
    }

}
void SAVE_MAGIC_SET(void)
{
    FILE * file_pointer;

    file_pointer = fopen("MAGIC_SET","WB");

    fwrite(&magic_set, 466, 1, file_pointer);

    fclose(file_pointer);
}