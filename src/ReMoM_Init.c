/**
 * ReMoM_Init.c — Shared engine initialization for ReMoM and HeMoM.
 *
 * Extracted from MOM_main() in ReMoM.c. This is the core init sequence
 * that both the full game and the headless tool need:
 *   CONFIG.MOM -> sound config -> Init_Drivers -> Allocate_Data_Space -> resources
 */

#include "../MoX/src/CFG.h"
#include "../MoX/src/DOS.h"
#include "../MoX/src/EMS/EMS.h"
#include "../MoX/src/EXIT.h"
#include "../MoX/src/Graphics.h"
#include "../MoX/src/LBX_Load.h"
#include "../MoX/src/MOX_BASE.h"
#include "../MoX/src/MOM_DAT.h"
#include "../MoX/src/MOX_DEF.h"
#include "../MoX/src/MOX_SET.h"
#include "../MoX/src/MOX_TYPE.h"
#include "../MoX/src/SOUND.h"
#include "../MoX/src/Timer.h"

#include "../MoX/src/capture.h"
#include "../MoX/src/Fields.h"
#include "../MoX/src/Fonts.h"

#include "../MoM/src/ALLOC.h"
#include "../MoM/src/Init.h"
#include "../MoM/src/LOADER.h"
#include "../MoM/src/Settings.h"

#include <stdio.h>

#include "../ext/stu_compat.h"

#include "../STU/src/STU_LOG.h"
#include "../STU/src/STU_GRAF.h"
#include "../platform/include/Platform.h"

#include "ReMoM_Init.h"

#ifdef STU_DEBUG
#include "../STU/src/STU_DBG.h"
#endif


extern char MOM_FONT_FILE[];



int ReMoM_Preflight_Game_Data(void)
{
    /* Small, high-signal set: if these can't be resolved, the player has not
       supplied the game data.  Kept minimal so a legitimately-present install
       never false-fails.  (Aligned with the installer PRD's presence check.) */
    static const char * const required[] =
    {
        "FONTS.LBX", "MAINSCRN.LBX", "WIZARDS.LBX", "SPELLDAT.LBX"
    };
    const int required_count = (int)(sizeof(required) / sizeof(required[0]));
    char missing_list[256];
    char message[768];
    int n_missing = 0;
    int i;

    missing_list[0] = '\0';
    for(i = 0; i < required_count; i++)
    {
        FILE * fp = STU_GRAF_Open_Asset(required[i], "rb");
        if(fp != NULL)
        {
            fclose(fp);
        }
        else
        {
            if(n_missing > 0)
            {
                stu_strcat(missing_list, ", ");
            }
            stu_strcat(missing_list, required[i]);
            n_missing++;
        }
    }

    if(n_missing == 0)
    {
        return 0;
    }

    snprintf(message, sizeof(message),
             "ReMoM could not find your Master of Magic game data.\n\n"
             "Missing: %s\n\n"
             "Copy every .LBX file and CONFIG.MOM from your original\n"
             "Master of Magic v1.31 installation into the same folder as\n"
             "the ReMoMber executable (or set the REMOM_DATA_DIR environment\n"
             "variable to point at them).\n\n"
             "See PLAYING.md for details.",
             missing_list);

    LOG_ERROR(LOG_CAT_GENERAL, "Preflight: missing game data: %s", missing_list);
    Platform_Show_Error("ReMoM - Missing Game Data", message);
    return 1;
}



void ReMoM_Init_Engine(void)
{
    char found_file[LEN_STRING] = { 0 };
    char file_name[LEN_MAIN_SAVE_FILE_NAME] = { 0 };
    int16_t DIGI_ID = 0;
    int16_t MIDI_ID = 0;
    int16_t DIGI_DMA = 0;
    int16_t DIGI_IRQ = 0;
    int16_t DIGI_IO = 0;
    int16_t DIGI_DRV = 0;
    int16_t MIDI_IRQ = 0;
    int16_t MIDI_IO = 0;
    int16_t itr_savegams = 0;
    FILE *  file_handle = 0;
    int16_t MIDI_DMA = 0;
    int16_t MIDI_DRV = 0;

    EMM_Pages_Reserved = 158;

    // TODO(JimBalcomb,20260502)  MoO2  main_() |-> Check_Command_Line_Parameters_() ... if(strstr_(_cmd_line_saveset, aSeed) != 0) { _cmd_line_seed = Get_Value_From_String_(aSeed, _cmd_line_saveset); }

    if(DIR(str_CONFIG_MOM, found_file) == 0)
    {
        config_mom.MIDI_IO = 0;
        config_mom.MIDI_ID = 0;
        config_mom.MIDI_IRQ = 0;
        config_mom.DIGI_IO = 0;
        config_mom.DIGI_IRQ = 0;
        config_mom.DIGI_DMA = 0;
        config_mom.DIGI_ID = 0;
        config_mom.NIU_0E = 0;
        config_mom.NIU_10 = 0;
        config_mom.NIU_12 = 0;
    }
    else
    {
        file_handle = stu_fopen_ci(str_CONFIG_MOM, str_RB);
        stu_fread(&config_mom, sizeof(struct s_CONFIG_MOM_18), 1, file_handle);
        stu_fclose(file_handle);
    }

    Load_MAGIC_SET();

#ifdef STU_DEBUG
    DBG_Print_MAGIC_SET("ReMoM_Init_Engine after Load_MAGIC_SET");
#endif

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
        DOS_PrintString__STUB(str_Initializing_Roland_Drivers);
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

    magic_set.input_type = 1;  /* OG-MoM v1.31 is hard-coded to 'keyboard and mouse' */

    for(itr_savegams = 1; itr_savegams < 9; itr_savegams++)
    {
        if(magic_set.Have_Save[(itr_savegams - 1)] != ST_FALSE)
        {
            stu_itoa(itr_savegams, found_file, 10);
            stu_strcpy(file_name, str_SAVE_NAME);
            stu_strcat(file_name, found_file);
            stu_strcat(file_name, str_SAVE_EXT);
            DIR(file_name, found_file);
            if(found_file[0] == '\0')
            {
                magic_set.Have_Save[(itr_savegams - 1)] = ST_FALSE;
                stu_strcpy(magic_set.Save_Names[(itr_savegams - 1)], empty_string__MAIN);
                file_handle = stu_fopen_ci(str_MAGIC_SET, str_WB);
                fwrite(&magic_set, sizeof(struct s_MAGIC_SET), 1, file_handle);
                fclose(file_handle);
            }
        }
    }

    EMM_Set_Minimum(2700);
    RAM_Set_Minimum(583);
    magic_set.input_type = 1;
    magic_set.sound_channels = 2;
    Init_Drivers(magic_set.input_type, magic_set.sound_channels, MOM_FONT_FILE, MIDI_DRV, MIDI_IO, MIDI_IRQ, MIDI_DMA, DIGI_DRV, DIGI_IO, DIGI_IRQ, DIGI_DMA);

    Release_Version();

    Allocate_Data_Space(6100);
    
    Disable_Cancel();

    Load_Palette(0, ST_UNDEFINED, 0);
    Apply_Palette();

    Load_MGC_Resources();

    Load_TERRSTAT();
    Load_SPELLDAT();

    /*
        Load_WZD_Resources() relocated -- this is the line that turned the MGC path into the merged path.
        It belongs in a WIZARDS.EXE-path function that --load/--continue will
        dispatch to. Until that Run_WIZARDS() split lands, --load/--continue
        are broken: no WZD resources loaded.
        Readded in the section case scr_Continue in MOM_SCR.c
    */
}
