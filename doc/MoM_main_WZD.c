#include "../../MoX/src/MOX_CFG.h"
#include "../../MoX/src/MOX_DEF.h"
#include "../../MoX/src/MOX_SET.h"
#include "../../MoX/src/MOX_SND.h"
#include "../../MoX/src/MOX_TYPE.h"
int WZD_main(int argc, char * argv[])
{
    int16_t found_file = 0;
    int16_t file_name = 0;
    int16_t DIGI_ID = 0;
    int16_t MIDI_ID = 0;
    int16_t DIGI_DMA = 0;
    int16_t DIGI_IRQ = 0;
    int16_t DIGI_IO = 0;
    int16_t DIGI_DRV = 0;
    int16_t MIDI_IRQ = 0;
    int16_t MIDI_IO = 0;
    int16_t itr_savegams = 0;
    int16_t file_handle = 0;
    int16_t EMS_Pages_Left = 0;
    int16_t MIDI_DMA = 0;  // _DI_
    int16_t MIDI_DRV = 0;  // _SI_
    EMM_Pages_Reserved = 158;
    if(DIR(str_CONFIG_MOM, found_file) == 0)
    {
        Exit_With_Message(str_CONFIG_MOM_ERROR);
    }
    file_handle = fopen(str_CONFIG_MOM, str_RB);
    fread(&config_mom, sizeof(struct s_CONFIG_MOM_18), 1, file_handle);
    fclose(file_handle);
    Load_MAGIC_SET();
    MIDI_IO = config_mom.MIDI_IO;
    MIDI_IRQ = config_mom.MIDI_IRQ;
    MIDI_DMA = ST_UNDEFINED;
    MIDI_ID = config_mom.MIDI_ID;
    switch(MIDI_ID)
    {
        case 0:
        {
            MIDI_DRV = SND_NONE;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 1:
        {
            MIDI_DRV = SND_Speaker;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 16:
        {
            MIDI_DRV = SND_AdLib;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 18:
        {
            MIDI_DRV = SND_SB;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 24:
        {
            MIDI_DRV = SND_SBPro;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 25:
        {
            MIDI_DRV = SND_PAS;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 32:
        {
            MIDI_DRV = SND_SBPro2;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 33:
        {
            MIDI_DRV = SND_PAS16;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 34:
        {
            MIDI_DRV = SND_ALG;
            MIDI_IO = ST_UNDEFINED;
            MIDI_IRQ = ST_UNDEFINED;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 48:
        {
            MIDI_DRV = SND_GMIDI;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 49:
        {
            MIDI_DRV = SND_Roland;
            MIDI_DMA = ST_UNDEFINED;
        } break;
        case 56:
        {
            MIDI_DRV = SND_GMIDI;
            MIDI_DMA = ST_UNDEFINED;
        } break;
    }
    if(MIDI_DRV == SND_Roland)
    {
        DOS_PrintString__STUB(str_Initializing_Roland_Drivers);  // "Initializing Roland Drivers...$"
        Mark_Time();
        Release_Time(2);
    }
    DIGI_IO = config_mom.DIGI_IO;
    DIGI_IRQ = config_mom.DIGI_IRQ;
    DIGI_DMA = config_mom.DIGI_DMA;
    DIGI_ID = config_mom.DIGI_ID;
    switch(DIGI_ID)
    {
        case 0:
        {
            DIGI_DRV = SND_NONE;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 1:
        {
            DIGI_DRV = SND_Speaker;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 16:
        {
            DIGI_DRV = SND_NONE;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 18:
        {
            DIGI_DRV = SND_SB;
        } break;
        case 24:
        {
            DIGI_DRV = SND_SBPro;
        } break;
        case 25:
        {
            DIGI_DRV = SND_PAS;
            DIGI_IO = ST_UNDEFINED;
        } break;
        case 32:
        {
            DIGI_DRV = SND_SBPro2;
            MIDI_IRQ = DIGI_IRQ;
            MIDI_DMA = DIGI_DMA;
        } break;
        case 33:
        {
            DIGI_DRV = SND_PAS16;
            DIGI_IO = ST_UNDEFINED;
        } break;
        case 34:
        {
            DIGI_DRV = SND_ALG;
        } break;
        case 48:
        case 49:
        {
            DIGI_DRV = SND_Roland;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
        case 56:
        {
            DIGI_DRV = SND_GMIDI;
            DIGI_IO = ST_UNDEFINED;
            DIGI_IRQ = ST_UNDEFINED;
            DIGI_DMA = ST_UNDEFINED;
        } break;
    }
    if(MIDI_DRV == SND_AdLib)
    {
        DIGI_DRV = SND_AdLib;
    }
    if((magic_set.sound_channels > 2) || (magic_set.sound_channels < 0))
    {
        magic_set.sound_channels = 0;
    }
    magic_set.input_type = 1;
    // ¿ MoO2  Check_For_Saved_Games() ?
    for(itr_savegams = 1; itr_savegams < 9; itr_savegams++)
    {
        if(magic_set.Have_Save[itr_savegams] != ST_FALSE)
        {
            SDL_itoa(itr_savegams, found_file, 10);
            strcpy(file_name, SAVE_NAME);
            strcat(file_name, found_file);
            strcat(file_name, SAVE_EXT);
            DIR(file_name, found_file);
            if(found_file == 0)
            {
                magic_set.Have_Save[(itr_savegams - 1)] = ST_FALSE;
                strcpy(magic_set.Save_Names[(itr_savegams - 1)], empty_string__MAIN);
                file_handle = fopen(str_MAGIC_SET, str_WB);
                fwrite(&magic_set[0], sizeof(struct s_MAGIC_SET), 1, file_handle);
                fclose(file_handle);
            }
        }
    }
    EMS_SetMinKB(2700);
    RAM_SetMinKB(583);
    magic_set.input_type = 1;
    magic_set.sound_channels = 2;
    Init_Drivers(magic_set.input_type, magic_set.sound_channels, fonts_lbx_file, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, DIGI_DRV, DIGI_IO, DIGI_IRQ, DIGI_DMA);
    Release_Version();
    // HERE: MGC === WZD
    Enable_Cancel();
    Allocate_Data_Space(4600);
    // HERE: MGC ~== WZD
    Load_SAVE_GAM(8);
    Load_WZD_Resources();
    EMS_Pages_Left = STU_INT(0x67, 5901);
    if(EMS_Pages_Left != 0)
    {
        g_EmmHndl_OVERXYZ = EMM_MakeNamedHandle(EMS_Pages_Left, ehn_OVERXYZ);
        EMM_ReleaseHandle(g_EmmHndl_OVERXYZ);
        __OvrInitEms(0, 0, EMS_Pages_Left);
    }
    else
    {
        g_EmmHndl_OVERXYZ = 0;  // ¿ NULLL ?
    }
    Load_Palette(0, ST_UNDEFINED);
    Calculate_Remap_Colors();
    Set_Button_Down_Offsets(1, 1,);
    Cycle_Palette_Color__STUB(198, 40, 0, 0, 63, 0, 0, 1);  // (color_num, red_min, green_min, blue_min, red_max, green_max, blue_max, step_value)
    Apply_Palette();
    Fade_In();
    current_screen = scr_Main_Screen
    _players[NEUTRAL_PLAYER_IDX].banner_id = 5;  // enum Banner_Color {BNR_Brown = 5 }
    Clear_Fields();
    Loaded_Game_Update_WZD();
    GAME_SoM_Cast_By = ST_UNDEFINED;
    Screen_Control();
    s01p16_empty_function();
    Exit_With_Size();
}
