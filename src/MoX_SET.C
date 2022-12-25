
#include "MoX_TYPE.H"
#include "MoX_DEF.H"

#include "MoX_SET.H"

#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), fseek(); */
#include <string.h>     /* strcpy() */



uint8_t _magic_set[466];



// WZD o125p04
void Set_Default_Game_Settings(void)
{
    int16_t itr;

    magic_set.SoundFX = 1;
    magic_set.BG_Music = 1;
    magic_set.Event_Music = 1;
    magic_set.Enemy_Moves = 1;
    magic_set.Enemy_Spells = 1;
    magic_set.Random_Events = 1;
    magic_set.EoT_Wait = 1;
    magic_set.EoT_Summary = 1;
    magic_set.Raze_City = 1;
    magic_set.Spell_Book_Order = 1;
    magic_set.Strategic_Combat = 0;
    magic_set.Spell_Anims = 1;
    magic_set.City_Spell_Events = 1;
    magic_set.Ovl_Spell_Events = 1;
    magic_set.Summoning_Events = 1;
    magic_set.Auto_Unit_Info = 1;
    magic_set.Show_Node_Owners = 1;
    magic_set.Expanding_Help = 1;
    magic_set.Movement_Anims = 1;

    magic_set.Difficulty = 0;
    magic_set.Opponents = 4;
    magic_set.MagicPower = 1;
    magic_set.LandSize = 1;

    magic_set.Input_Type = 1;
    magic_set.Snd_Channels = 2;

    for(itr = 0; itr < NUM_SAVES; itr++)
    {
        magic_set.Have_Save[itr] = 0;
    }

    for(itr = 0; itr < NUM_HOF_ENTRIES; itr++)
    {
        magic_set.HallofFame_Scores[itr] = 0;
        magic_set.HallofFame_Races[itr] = 0;
        strcpy(magic_set.HallofFame_Names[itr], "");
    }

}


// WZD o125p03
void Load_MAGIC_SET(void)
{
    char file_name[30];
    FILE * file_pointer;
    int itr;

    if( (DIR(&file_name, "MAGIC.SET") == 0) || (LOF("MAGIC_SET") != 466) )
    {
        Set_Default_Game_Settings();
        file_pointer = fopen("MAGIC_SET","wb");
        fwrite(&magic_set, 466, 1, file_pointer);
    }
    else
    {
        file_pointer = fopen("MAGIC_SET","rb");
        fread(&magic_set, 466, 1, file_pointer);
    }
    fclose(file_pointer);


    if(magic_set.Movement_Anims != ST_FALSE) { magic_set.Movement_Anims = ST_TRUE; }
    if(magic_set.Raze_City != ST_FALSE) { magic_set.Raze_City = ST_TRUE; }

    
    if(magic_set.HallofFame_Scores == 0)
    {
        for(itr = 0; itr < NUM_HOF_ENTRIES; itr++)
        {
            magic_set.HallofFame_Scores[itr] = 0;
            magic_set.HallofFame_Races[itr] = 0;
            strcpy(magic_set.HallofFame_Names[itr], "");
        }
    }

    file_pointer = fopen("MAGIC_SET","wb");
    fwrite(&magic_set, 466, 1, file_pointer);
    fclose(file_pointer);

}



void Read_MAGIC_SET(void)
{
    FILE * file_pointer;
    file_pointer = fopen("MAGIC.SET", "rb");
    fread(&_magic_set, 466, 1, file_pointer);
    fclose(file_pointer);
}

void Write_MAGIC_SET(void)
{
    FILE * file_pointer;
    file_pointer = fopen("MAGIC.SET", "wb");
    fwrite(&_magic_set, 466, 1, file_pointer);
    fclose(file_pointer);
}