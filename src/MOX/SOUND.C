/*
    WIZARDS.EXE
        seg038  "SOUND"

MoO2
    Module: sound

*/

#include "SOUND.H"

#include "AIL.H"

#include "Allocate.H"
#include "GFILE.H"
#include "LBX_Load.H"
#include "MOX_DEF.H"
#include "MOX_TYPE.H"

#include <string.h>     /* memset(), strcat(), strcpy(); */



enum e_AUDIO_ERROR
{
    SND_bad_driver              =  0,
    SND_driver_error            =  1,
    SND_invalid_Digi_driver     =  2,
    SND_Invalid_XMIDI_driver    =  3,
    SND_not_DSP_driver          =  4,
    SND_Invalid_Music_IO        =  5,
    SND_Bad_Music_IO            =  6,
    SND_Invalid_Digi_IO         =  7,
    SND_Bad_Digi_IO             =  8,
    SND_Not_SFX_File            =  9,
    SND_Not_XMIDI_File          = 10,
    SND_Sequence_Failure        = 11,
    SND_Timbre_Missing          = 12,
    SND_Couldnt_Load_File       = 13,
    SND_Invalid_File            = 14,
    SND_Not_Sound_File          = 15,
    SND_Couldnt_Load_SND_File   = 16
};


// // C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\AIL.H
// // typedef struct
// // {
// //    unsigned min_API_version;
// //    unsigned drvr_type;
// //    char data_suffix[4];
// //    void far *dev_name_table;
// //    int default_IO;
// //    int default_IRQ;
// //    int default_DMA;
// //    int default_DRQ;
// //    int service_rate;
// //    unsigned display_size;
// // }  
// // drvr_desc;
// typedef struct
// {
//     /* 00 */  uint16_t min_API_version;
//     /* 02 */  uint16_t drvr_type;
//     /* 04 */  int16_t data_suffix;
//     /* 08 */  void * dev_name_table;
//     /* 0C */  int16_t default_IO;
//     /* 0E */  int16_t default_IRQ;
//     /* 10 */  int16_t default_DMA;
//     /* 12 */  int16_t default_DRQ;
//     /* 14 */  int16_t svc_rate;
//     /* 16 */  uint16_t display_size;
//     /* 18 */
// }  AIL_drvr_desc;



// WZD dseg:82A8                                                 BEGIN:  seg038 - Initialized Data  (SOUND)

// WZD dseg:82A8
int16_t AIL_mdi_driver = 0;

// WZD dseg:82AA
int16_t AIL_dig_driver = 0;

// WZD dseg:82AC
// MoO2  Module: sound  signed integer (4 bytes) soundOn  Address: 02:0017C380
// MoO2  Module: sound  signed integer (4 bytes) audioInitialized  Address: 02:0017C39C
int16_t audio_initialized = ST_FALSE;

// WZD dseg:82AE
int16_t midi_driver_handle = ST_UNDEFINED;

// WZD dseg:82B0
int16_t digi_driver_handle = ST_UNDEFINED;

// WZD dseg:82B2
int16_t sequence_handle = -2;

// WZD dseg:82B4 FE FF                                           dw 0FFFEh

// WZD dseg:82B6
char * state_table_pointer = ST_NULL;

// WZD dseg:82BA 00                                              db 0
// WZD dseg:82BB 00                                              db 0
// WZD dseg:82BC 00                                              db 0
// WZD dseg:82BD 00                                              db 0

// WZD dseg:82BE
int16_t SND_Driver_Count = 0;

// WZD dseg:82C0
char BLASTER__tokens[] = "AIDT";

// WZD dseg:82C5 00                                              align 2

// WZD dseg:82C6
int16_t BLASTER__base[] = { 16, 10, 10, 10 };
// WZD dseg:82C6                                                                                         ; BLASTER.C static variable
// WZD dseg:82CE BE E8 C0 E8 C2 E8 CA E8                         cnst_TARGETS dw offset static_t.default_IO, offset static_t.default_IRQ, offset static_t.default_DMA, offset static_type
// WZD dseg:82CE                                                                                         ; DATA XREF: _AIL_board_ID+F8r
// WZD dseg:82CE                                                                                         ; BLASTER.C static variable

// WZD dseg:82D6
char snddrv_lbx_file__seg038[] = "SNDDRV.LBX";

// WZD dseg:82E0
/*
BCPP string optimizer - null terminator for snddrv_lbx_file__seg038[]
*/
char str_empty_string__seg038[] = "";

// WZD dseg:82E1
char str_prefix_GTL_filename[] = "FAT.";
// WZD dseg:82E6 72 62 00                                        cnst_RB8 db 'rb',0                      ; DATA XREF: Play_Sound__WIP:loc_31D5Fo
// WZD dseg:82E6                                                                                         ; should use dseg:28b8
// WZD dseg:82E9 42 4C 41 53 54 45 52 00                         cnst_BLASTER_envvar db 'BLASTER',0      ; DATA XREF: _AIL_board_ID+9o
// WZD dseg:82E9                                                                                         ; BLASTER.C character constant
// WZD dseg:82F1 30 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46+cnst_BLASTER_numbers db '0123456789ABCDEF',0
// WZD dseg:82F1 00                                                                                      ; DATA XREF: _AIL_board_ID+BCr
// WZD dseg:82F1                                                                                         ; BLASTER.C character constant

// WZD dseg:8302
// drake178: cnst_SND_Error01 ... cnst_SND_Error10
// // char * str_allocation_errors[] =
// // ISO C++11 does not allow conversion from string literal to 'char *'clang(-Wwritable-strings)
// char const * str_allocation_errors[] =
static char * audio_error_messages[] =
{
    "You selected an invalid sound driver, ",
    "reconfigure hardware options",
    "You selected an invalid digi driver, ",
    "Sound driver is not a valid XMIDI driver, ",
    "Digi driver is not a valid DSP driver, ",
    "Music ",
    "I/O parameters are incorrect, ",
    "Music I/O parameters are incorrect, ",
    "Digi Driver ",
    "Not a valid",
    " sound file",
    "Couldn',27h,'t load XMIDI file",
    "Sequence did not register",
    "Timbre not found",
    "Could not load",
    "Not a valid",
    "Could not load sound file"
};

// WZD dseg:84A7 00                                              align 2

// WZD dseg:84A7                                                 END:  seg038 - Initialized Data  (SOUND)



// WZD dseg:E8B0                                                 BEGIN:  seg038 - Uninitialized Data  (SOUND)

// WZD dseg:E8B0 00 00                                           STATIC__len dw 0

// WZD dseg:E8B2 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+static_t AIL_drvr_desc <0>              ; DATA XREF: dseg:BLASTER__targetso ...
// WZD dseg:E8B2 00 00 00 00 00 00 00 00                                                                 ; BLASTER.C static variable

// WZD dseg:E8CA
int16_t BLASTER__type;
// WZD dseg:E8CA                                                                                         ; BLASTER.C static variable

// WZD dseg:E8CC 00 00 00 00 00 00                               STATIC__GTL_hdr GTL_Header_Record <0>

// WZD dseg:E8D2
char SND_tbank_suffix[22];

// WZD dseg:E8E8
uint16_t * timb_ptr;

// WZD dseg:E8EC 00 00 00 00                                     SND_Timbre_Data@ dd 0                   ; DATA XREF: Play_Sound__WIP+116w ...
char * timb;

// WZD dseg:E8F0
SAMB_ptr SND_AIL_Digi_Driver;
// WZD dseg:E8F4
SAMB_ptr SND_AIL_MIDI_Driver;
// WZD dseg:E8F8
drvr_desc * SND_Digi_drvr_desc;
// WZD dseg:E8FC
drvr_desc * SND_MIDI_drvr_desc;
// WZD dseg:E900
SAMB_ptr digi_sound_buffer;
// WZD dseg:E904
SAMB_ptr midi_sound_buffer;

// WZD dseg:E908
char GTL_filename[16];

// WZD dseg:E918
// drake178: SND_TBank_Handle
/*
Global Timbre Library file
*/
FILE * GTL;

// WZD dseg:E918                                                 END:  seg038 - Uninitialized Data  (SOUND)



/*
    WIZARDS.EXE  seg038
*/

// WZD s38p01
// drake178: _AIL_load_timbre()
// _AIL_load_timbre()
/*
NOT an AIL library function

loads a timbre into a global buffer so it can be
installed to an XMIDI device (non-stdio static memory
modification of a function from the AIL examples)
returns a pointer to the timbre data or NULL
source: miles214\examples\xplay.c
C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\MIXDEMO.C
*/
// C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\MIXDEMO.C
// C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\XPLAY.C
#include <stdio.h>      /* FILE; fclose(), fopen(), fread(), frite(), fseek(); */
/***************************************************************/

//
// Standard C routine for Global Timbre Library access
//

void far *load_global_timbre(FILE *GTL, unsigned bank, unsigned patch)
{
    unsigned far * timb_ptr;
    static unsigned len;

    static struct                   // GTL file header entry structure
    {
        char patch;
        char bank;
        unsigned long offset;
    }
    GTL_hdr;

    if (GTL==NULL) return NULL;     // if no GTL, return failure

    // rewind(GTL);                    // else rewind to GTL header
    gfseek(GTL, 0L, SEEK_SET);

    do                              // search file for requested timbre
    {
        gfread(&GTL_hdr,sizeof(GTL_hdr),1,GTL);
        if(GTL_hdr.bank == -1) 
            return NULL;            // timbre not found, return NULL
    }
    while ((GTL_hdr.bank != bank) ||
          (GTL_hdr.patch != patch));       

    gfseek(GTL,GTL_hdr.offset,SEEK_SET);    
    gfread(&len,2,1,GTL);           // timbre found, read its length

    // timb_ptr = farmalloc(len);      // allocate memory for timbre ..
    *timb_ptr = len;
                                    // and load it
    gfread((timb_ptr+1),len-2,1,GTL);       

    // if(ferror(GTL))                 // return NULL if any errors
    //     return NULL;                // occurred
    // else
    //     return timb_ptr;            // else return pointer to timbre
    return timb_ptr;
}
   
/***************************************************************/


// WZD s38p02
// drake178: Audio_Init()
/*
; initializes the sound system, loads drivers,
; and sets up the Audio Interface Library resources
; returns 1
*/
int16_t Audio_Init__WIP(int16_t midi_driver, int16_t sound_channels, int16_t MIDI_IO, int16_t MIDI_IRQ, int16_t MIDI_DMA, int16_t digi_driver, int16_t Digi_IO, int16_t Digi_IRQ, int16_t Digi_DMA)
{
    int16_t Use_Digi_DMA;
    int16_t Use_Digi_IRQ;
    int16_t Use_Digi_IO;
    int16_t Prev_Digi_IRQ = 0;
    int16_t Prev_Digi_DMA = 0;
    int16_t Prev_Digi_IO = 0;
    int16_t Prev_MIDI_DMA = 0;
    int16_t Prev_MIDI_IRQ = 0;
    int16_t Prev_MIDI_IO = 0;
    int16_t Use_MIDI_DMA;
    int16_t Use_MIDI_IRQ;
    int16_t Use_MIDI_IO;
    uint32_t state_table_size = 0;  // AIL  unsigned long state_size;
    char * timbre_cache_pointer = 0;
    int16_t digi_set;
    SAMB_ptr timb_seg;
    char * state_table = 0;  // AIL  char far *state[8];
    char * timbre_cache = 0;
    int16_t midi_set;
// GMIDI_Timbre_Init= word ptr -6
    SAMB_ptr Roland_Timbre_Init = 0;
    int16_t sound_blaster_type = 0;
    uint16_t timbre_cache_size = 0;  // _DI_  AIL  unsigned tc_size;

    midi_set = ST_FALSE;
    digi_set = ST_FALSE;

    AIL_mdi_driver = midi_driver;
    AIL_dig_driver = digi_driver;

    if(audio_initialized != ST_FALSE)
    {
        return ST_TRUE;
    }

    if(
        (MIDI_IO != ST_UNDEFINED)
        ||
        (MIDI_IRQ != ST_UNDEFINED)
        ||
        (MIDI_DMA != ST_UNDEFINED)
    )
    {
        midi_set = ST_TRUE;
        Use_MIDI_IO = MIDI_IO;
        Use_MIDI_IRQ = MIDI_IRQ;
        Use_MIDI_DMA = MIDI_DMA;
    }


    if(
        (Digi_IO != ST_UNDEFINED)
        ||
        (Digi_IRQ != ST_UNDEFINED)
        ||
        (Digi_DMA != ST_UNDEFINED)
    )
    {
        digi_set = ST_TRUE;
        Use_Digi_IO = Digi_IO;
        Use_Digi_IRQ = Digi_IRQ;
        Use_Digi_DMA = Digi_DMA;
    }

    if(
        (midi_driver == SND_NONE)
        &&
        (digi_driver == SND_NONE)
    )
    {
        SND_Driver_Count = 0;
        return ST_TRUE;
    }


    if(
        (sound_channels == 1)
        &&
        (digi_driver == SND_NONE)
    )
    {
        SND_Driver_Count = 0;
        return ST_TRUE;
    }

    if(midi_driver == SND_Speaker)
    {
        SND_Driver_Count = 1;
    }
    else
    {
        SND_Driver_Count = 1;

        if(
            (sound_channels == 2)
            &&
            (AIL_mdi_driver > SND_Speaker)
        )
        {
            SND_Driver_Count = 2;
        }
        else if(digi_driver != SND_NONE)
        {
            SND_Driver_Count = 1;
        }
        else
        {
            SND_Driver_Count = 0;
            return ST_TRUE;            
        }
    }

    if(
        (AIL_mdi_driver == SND_SBPro)
        ||
        (AIL_mdi_driver == SND_SBPro2)
    )
    {
        // TODO  sound_blaster_type = BLASTER__board_ID();
        sound_blaster_type = 3;

        if(sound_blaster_type == ST_NULL)
        {
            Audio_Error__STUB(0);
        }

/*
   static char *FM_driver_names[] =
      {
         "SBFM.ADV",
         "SBP1FM.ADV",
         "SBFM.ADV",
         "SBP2FM.ADV",
      };
   static char *DSP_driver_names[] =
      {
         "SBDIG.ADV",
         "SBPDIG.ADV",
         "SBDIG.ADV",
         "SBPDIG.ADV",
      };
   static char *board_names[] =
      {
         "Sound Blaster V1.5 or earlier",
         "Sound Blaster Pro (Yamaha YM3812 version)",
         "Sound Blaster V2.0",
         "Sound Blaster Pro (Yamaha YMF262/OPL3 version)",
      };
*/
        switch(sound_blaster_type)
        {
            case 0:  /* "Sound Blaster V1.5 or earlier" */
            {
                AIL_mdi_driver = SND_SB;
                midi_driver = SND_SB;
            } break;
            case 1:  /* "Sound Blaster Pro (Yamaha YM3812 version)" */
            {
                AIL_mdi_driver = SND_SBPro;
                midi_driver = SND_SBPro;
            } break;
            case 2:  /* "Sound Blaster V1.5 or earlier" */
            {
                AIL_mdi_driver = SND_SB;
                midi_driver = SND_SB;
            } break;
            case 3:  /* "Sound Blaster Pro (Yamaha YMF262/OPL3 version)" */
            {
                AIL_mdi_driver = SND_SBPro2;
                midi_driver = SND_SBPro2;
            } break;

        }

    }

    if(AIL_mdi_driver > SND_Speaker)
    {
        // SND_AIL_MIDI_Driver = SA_MK_FP0(LBX_Load(snddrv_lbx_file__seg038, 1, midi_driver));
        SND_AIL_MIDI_Driver = LBX_Load(snddrv_lbx_file__seg038, ((midi_driver - 1) * 2));
    }

    if(AIL_dig_driver > SND_NONE)
    {
        // SND_AIL_Digi_Driver = SA_MK_FP0(LBX_Load(snddrv_lbx_file__seg038, 1, AIL_dig_driver));
        SND_AIL_Digi_Driver = LBX_Load(snddrv_lbx_file__seg038, (((AIL_dig_driver - 1) * 2) + 1));
    }

    AIL_startup();

    if(AIL_mdi_driver > SND_Speaker)
    {
        digi_driver_handle = AIL_register_driver(SND_AIL_MIDI_Driver);
    }

    if(AIL_dig_driver > SND_NONE)
    {
        digi_driver_handle = AIL_register_driver(SND_AIL_Digi_Driver);
    }

    if(
        (midi_driver_handle == -1)
        &&
        (AIL_mdi_driver > SND_Speaker)
    )
    {
        AIL_shutdown(0);
        SND_Driver_Count = 0;
        Audio_Error__STUB(SND_driver_error);
    }

    if(
        (digi_driver_handle == -1)
        &&
        (AIL_dig_driver > SND_NONE)
    )
    {
        AIL_shutdown(0);
        SND_Driver_Count = 0;
        Audio_Error__STUB(SND_invalid_Digi_driver);
    }

    if(AIL_mdi_driver > SND_Speaker)
    {
        SND_MIDI_drvr_desc = AIL_describe_driver(midi_driver_handle);
    }

    if(AIL_dig_driver > SND_NONE)
    {
        SND_Digi_drvr_desc = AIL_describe_driver(digi_driver_handle);
    }

    if(
        (SND_MIDI_drvr_desc->drvr_type != XMIDI_DRVR)
        &&
        (AIL_mdi_driver > SND_Speaker)
    )
    {
        AIL_shutdown(0);
        SND_Driver_Count = 0;
        Audio_Error__STUB(SND_Invalid_XMIDI_driver);
    }

    if(
        (SND_MIDI_drvr_desc->drvr_type != DSP_DRVR)
        &&
        (AIL_dig_driver > SND_NONE)
    )
    {
        AIL_shutdown(0);
        SND_Driver_Count = 0;
        Audio_Error__STUB(SND_not_DSP_driver);
    }

    if(midi_set != ST_FALSE)
    {
        if(MIDI_DMA != ST_UNDEFINED)
        {
            Prev_MIDI_DMA = SND_MIDI_drvr_desc->default_DMA;
            SND_MIDI_drvr_desc->default_DMA = Use_MIDI_DMA;
        }

        if(MIDI_IO != ST_UNDEFINED)
        {
            Prev_MIDI_IO = SND_MIDI_drvr_desc->default_IO;
            SND_MIDI_drvr_desc->default_IO = Use_MIDI_IO;
        }
        if(MIDI_IRQ != ST_UNDEFINED)
        {
            Prev_MIDI_IRQ = SND_MIDI_drvr_desc->default_IRQ;
            SND_MIDI_drvr_desc->default_IRQ = Use_MIDI_IRQ;
        }
    }
    if(digi_set != ST_FALSE)
    {
        if(Digi_DMA != ST_UNDEFINED)
        {
            Prev_Digi_DMA = SND_Digi_drvr_desc->default_DMA;
            SND_Digi_drvr_desc->default_DMA = Use_Digi_DMA;
        }
        if(Digi_IO != ST_UNDEFINED)
        {
            Prev_Digi_IO = SND_Digi_drvr_desc->default_IO;
            SND_Digi_drvr_desc->default_IO = Use_Digi_IO;
        }
        if(Digi_IRQ != ST_UNDEFINED)
        {
            Prev_Digi_IRQ = SND_Digi_drvr_desc->default_IRQ;
            SND_Digi_drvr_desc->default_IRQ = Use_Digi_IRQ;
        }
    }

    /*
        BEGIN:  Detect Device
    */
    if(AIL_mdi_driver > SND_Speaker)
    {
        if(!AIL_detect_device(midi_driver_handle, SND_MIDI_drvr_desc->default_IO, SND_MIDI_drvr_desc->default_IRQ, SND_MIDI_drvr_desc->default_DMA, SND_MIDI_drvr_desc->default_DRQ))
        {
            if(midi_set != ST_FALSE)
            {
                SND_MIDI_drvr_desc->default_DMA = Prev_MIDI_DMA;
                SND_MIDI_drvr_desc->default_IO = Prev_MIDI_IO;
                SND_MIDI_drvr_desc->default_IRQ = Prev_MIDI_IRQ;
                if(!AIL_detect_device(midi_driver_handle, SND_MIDI_drvr_desc->default_IO, SND_MIDI_drvr_desc->default_IRQ, SND_MIDI_drvr_desc->default_DMA, SND_MIDI_drvr_desc->default_DRQ))
                {
                    AIL_shutdown(NULL);
                    SND_Driver_Count = 0;
                    Audio_Error__STUB(SND_Invalid_Music_IO);
                }
            }
            else
            {
                AIL_shutdown(NULL);
                SND_Driver_Count = 0;
                Audio_Error__STUB(SND_Bad_Music_IO);
            }
        }
    }

    if(AIL_dig_driver > SND_NONE)
    {
        if(!AIL_detect_device(digi_driver_handle, SND_Digi_drvr_desc->default_IO, SND_Digi_drvr_desc->default_IRQ, SND_Digi_drvr_desc->default_DMA, SND_Digi_drvr_desc->default_DRQ))
        {
            if(digi_set != ST_FALSE)
            {
                SND_Digi_drvr_desc->default_DMA = Prev_Digi_DMA;
                SND_Digi_drvr_desc->default_IO = Prev_Digi_IO;
                SND_Digi_drvr_desc->default_IRQ = Prev_Digi_IRQ;
                if(!AIL_detect_device(digi_driver_handle, SND_Digi_drvr_desc->default_IO, SND_Digi_drvr_desc->default_IRQ, SND_Digi_drvr_desc->default_DMA, SND_Digi_drvr_desc->default_DRQ))
                {
                    AIL_shutdown(NULL);
                    SND_Driver_Count = 0;
                    Audio_Error__STUB(SND_Bad_Digi_IO);
                }
            }
            else
            {
                AIL_shutdown(NULL);
                SND_Driver_Count = 0;
                Audio_Error__STUB(SND_Invalid_Digi_IO);
            }
        }
    }

    /*
        END:  Detect Device
    */


    /*
        BEGIN:  Init Driver
    */
    if(AIL_dig_driver > SND_NONE)
    {
        AIL_init_driver(digi_driver_handle, SND_Digi_drvr_desc->default_IO, SND_Digi_drvr_desc->default_IRQ, SND_Digi_drvr_desc->default_DMA, SND_Digi_drvr_desc->default_DRQ);
    }

    if(AIL_mdi_driver > SND_Speaker)
    {
        AIL_init_driver(midi_driver_handle, SND_MIDI_drvr_desc->default_IO, SND_MIDI_drvr_desc->default_IRQ, SND_MIDI_drvr_desc->default_DMA, SND_MIDI_drvr_desc->default_DRQ);
    }
    /*
        END:  Init Driver
    */


    
    state_table_size = AIL_state_table_size(midi_driver_handle);

    state_table = (char *)Allocate_Space(((state_table_size / 16) + 1));

    // state_table_pointer = SA_MK_FP0(state);
    state_table_pointer = state_table;


    // Get name of Global Timbre Library file by appending suffix 
    // supplied by XMIDI driver to GTL filename prefix

    strcpy(GTL_filename, str_prefix_GTL_filename);

    String_Copy_Far(SND_tbank_suffix, SND_MIDI_drvr_desc->data_suffix);

    strcat(GTL_filename, SND_tbank_suffix);

    // Set up local timbre cache; open Global Timbre Library file

    timbre_cache_size = AIL_default_timbre_cache_size(midi_driver_handle);

    if(timbre_cache_size > 2000)
    {
        timbre_cache_size = 2000;
    }

    timbre_cache = (char *)Allocate_Space(((state_table_size / 16) + 1));

    // timbre_cache_pointer = SA_MK_FP0(timbre_cache);
    timbre_cache_pointer = timbre_cache;

    // AIL  AIL_define_timbre_cache(hdriver,tc_addr,tc_size);
    AIL_define_timbre_cache(midi_driver_handle, timbre_cache_pointer, timbre_cache_size);



    audio_initialized = ST_TRUE;



    timb_seg = Allocate_Space(20);  // 20 PR  320 B

    // timb_ptr = SA_MK_FP0(timb_seg);
    timb_ptr = (uint16_t *)timb_seg;

    if(AIL_mdi_driver == SND_Roland)
    {
        Roland_Timbre_Init = LBX_Load(snddrv_lbx_file__seg038, 20);
        Play_Sound__WIP(Roland_Timbre_Init);
    }

    while(Midi_Sequence_Status() != SEQ_DONE) { }

    return ST_TRUE;
}


// WZD s38p03
// drake178: SND_PlayFile()
// MoO2  Module: sound  function (0 bytes) Play_Sound_Buffer  Address: 01:001122C0
// MoO2  Module: sound  function (0 bytes) Play_Sound  Address: 01:00112399
// MoO2  Module: sound  function (0 bytes) Play_Sound_File  Address: 01:00113475
/*
; initiates sound playback: either MIDI or digital,
; depending on the supplied file
; returns 1 if nothing is actually played
; quits on real errors
*/
/*

*/
int16_t Play_Sound__WIP(SAMB_ptr sound_buffer)
{
    int16_t timbre_required = 0;
    int16_t sound_type = 0;  // _DI_
    uint16_t bank = 0;  // DNE in Dasm
    uint16_t patch = 0;  // DNE in Dasm

    if(
        (audio_initialized == ST_FALSE)
        ||
        (SND_Driver_Count == 0)
    )
    {
        return 1;
    }

    if(SND_Driver_Count == 2)
    {

        // if(farpeekw(sound_buffer, 0) != SNDMAGSIG)
        if(GET_2B_OFS(sound_buffer, 0) != SNDMAGSIG)
        {
            AIL_shutdown(NULL);
            Audio_Error__STUB(SND_Not_SFX_File);
        }

        sound_type = GET_2B_OFS(sound_buffer, 2);

        sound_buffer = (sound_buffer + SZ_PARAGRAPH_B);

        switch(sound_type)
        {
            case 1:
            {

                if(AIL_mdi_driver < SND_AdLib)
                {
                    return 1;
                }

                Stop_Music__STUB();
    
                // sound_buffer_ptr = SA_MK_FP0(sound_buffer);
                midi_sound_buffer = sound_buffer;
    
                if(midi_sound_buffer == ST_NULL)
                {
                    AIL_shutdown(0);
                    Audio_Error__STUB(SND_Not_XMIDI_File);
                }
    
                // if ((hseq[i] = AIL_register_sequence(hdriver,buffer,i,state[i],NULL)) == -1)
                // if ((hseq = AIL_register_sequence(hdriver,buffer,seqnum,state,NULL)) == -1)
                if(sequence_handle = AIL_register_sequence(midi_driver_handle, midi_sound_buffer, 0, state_table_pointer, NULL) == -1)
                {
                    Audio_Error__STUB(SND_Sequence_Failure);
                }
    
                if(AIL_mdi_driver != SND_GMIDI)
                {
                    GTL = gfopen(GTL_filename, "rb");
                    while((timbre_required = AIL_timbre_request(midi_driver_handle,sequence_handle)) != 0xffff)
                    {
                        bank = timbre_required / 256;
                        patch = timbre_required % 256;
                        timb = (char *)load_global_timbre(GTL, bank, patch);
                        if (timb != NULL)
                        {
                            AIL_install_timbre(midi_driver_handle, bank, patch, timb);
                        }
                        else
                        {
                            AIL_shutdown(NULL);
                            Audio_Error__STUB(SND_Timbre_Missing);
                        }
                    }
                    if(GTL != NULL) gfclose(GTL);
                    AIL_start_sequence(midi_driver_handle, sequence_handle); 
                }
            } break;
            case 2:
            {
                if(AIL_dig_driver == SND_NONE)
                {
                    return 1;
                }

                // digi_sound_buffer = SA_MK_FP0(sound_buffer);
                digi_sound_buffer = sound_buffer;
    
                if(digi_sound_buffer == ST_NULL)
                {
                    AIL_shutdown(0);
                    Audio_Error__STUB(SND_Couldnt_Load_File);
                }

                // if(farpeekw((sound_buffer - 1), 4) == SNDVOCMAGSIG)
                if(GET_2B_OFS((sound_buffer - SZ_PARAGRAPH_B), 4) != SNDVOCMAGSIG)
                {
                    if(
                        (digi_driver_handle != -1)
                        &&
                        (AIL_dig_driver > SND_NONE)
                    )
                    {
                        AIL_stop_digital_playback(digi_driver_handle);
                    }

                    // farpokew((sound_buffer - 1), 4, SNDVOCMAGSIG)
                    SET_2B_OFS((sound_buffer - SZ_PARAGRAPH_B), 4, SNDVOCMAGSIG);

                    AIL_format_VOC_file(digi_driver_handle, digi_sound_buffer, -1);

                }

                AIL_play_VOC_file(digi_driver_handle, digi_sound_buffer, -1);

                AIL_start_digital_playback(digi_driver_handle);
            } break;
            default:
            {
                Audio_Error__STUB(SND_Invalid_File);
            } break;
        }



    }

    if(SND_Driver_Count == 1)
    {
        if(AIL_dig_driver == SND_NONE)
        {
            return 1;
        }
        // if(farpeekw(sound_buffer, 0) != SNDMAGSIG)
        if(GET_2B_OFS(sound_buffer, 0) != SNDMAGSIG)
        {
            AIL_shutdown(NULL);
            Audio_Error__STUB(SND_Not_Sound_File);
        }
        sound_type = GET_2B_OFS(sound_buffer, 2);
        sound_buffer = (sound_buffer + SZ_PARAGRAPH_B);
        if(sound_type == 2)
        {
            // digi_sound_buffer = SA_MK_FP0(sound_buffer);
            digi_sound_buffer = sound_buffer;
            if(digi_sound_buffer == ST_NULL)
            {
                AIL_shutdown(0);
                Audio_Error__STUB(SND_Couldnt_Load_SND_File);
            }
            // if(farpeekw((sound_buffer - 1), 4) == SNDVOCMAGSIG)
            if(GET_2B_OFS((sound_buffer - SZ_PARAGRAPH_B), 4) != SNDVOCMAGSIG)
            {
                if((digi_driver_handle != -1) && (AIL_dig_driver > SND_NONE))
                {
                    AIL_stop_digital_playback(digi_driver_handle);
                }
                AIL_format_VOC_file(digi_driver_handle, digi_sound_buffer, -1);
                // farpokew((sound_buffer - 1), 4, SNDVOCMAGSIG)
                SET_2B_OFS(sound_buffer, 4, SNDVOCMAGSIG);
            }
            AIL_play_VOC_file(digi_driver_handle, digi_sound_buffer, -1);
            AIL_start_digital_playback(digi_driver_handle);
        }
    }



}


// WZD s38p04
// drake178: UU_SND_SetMIDIVol()
// MoO2  Module: sound  function (0 bytes) Set_Music_Volume  Address: 01:0011302C
/*
; Unused in MoM
;
; if there is active MIDI playback, adjusts the volume
; to the desired scaling factor
*/
void Set_Music_Volume__STUB(int16_t volume)
{



}


// WZD s38p05
// drake178: UU_SND_GetMIDIVol()
// MoO2  Module: sound  function (0 bytes) Get_Music_Volume
/*
; Unused in MoM
;
; if there is active MIDI playback, returns the current
; volume scaling factor
*/
int16_t Get_Music_Volume__STUB(void)
{
    int16_t volume;



    volume = 100;



    return volume;
}


// WZD s38p06
// drake178: SND_Silence()
// MoO2  Module: sound  function (0 bytes) Stop_All_Sounds  Address: 01:00112816
void Stop_All_Sounds__STUB(void)
{
    int16_t Percent;

    if(audio_initialized == ST_FALSE)
    {
        return;
    }

    if(sequence_handle != -2)
    {
        Percent = 0;

        // // ; returns the status of the specified sequence
        // // ; source: miles214\disk3\ail.asm
        // if(_AIL_sequence_status(midi_driver_handle, sequence_handle) == SEQ_PLAYING)
        // {
        //     // ; sets the XMIDI volume scaling factor, ramping up/down
        //     // ; source: miles214\disk3\ail.asm
        //     _AIL_set_rel_volume(midi_driver_handle, sequence_handle, Percent, 1000);
        // }
    }

    if(
        (digi_driver_handle != -1)
        &&
        (AIL_dig_driver > SND_NONE)
    )
    {
        // // ; silences output and resets any sound buffers
        // // ; source: miles214\disk3\ail.asm
        // _AIL_stop_digi_play(digi_driver_handle);
    }

}

// WZD s38p07
// drake178: SND_Get_Seq_Status()
int16_t Midi_Sequence_Status(void)
{
    int16_t sequence_status = 0;

    if(
        (audio_initialized == ST_FALSE)
        ||
        (SND_Driver_Count < 2)
    )
    {
        return SEQ_DONE;
    }

    sequence_status = AIL_sequence_status(midi_driver_handle, sequence_handle);
    
    return sequence_status;

}


// WZD s38p08
// drake178: SND_Stop_Music()
/*
; stops any MIDI playback in progress and unregisters
; the sequence from the AIL so it can be discarded
*/
/*

*/
void Stop_Music__STUB(void)
{

    if(
        (audio_initialized == ST_FALSE)
        ||
        (AIL_mdi_driver == SND_NONE)
        ||
        (SND_Driver_Count == 0)
    )
    {
        return;
    }

    if(AIL_mdi_driver == SND_Speaker)
    {
        if(digi_driver_handle == ST_UNDEFINED)
        {
            if(AIL_dig_driver > SND_NONE)
            {
                // jle     short $+2
            }
        }
        return;
    }

    if(sequence_handle != -2)
    {
        // TODO  // ; returns the status of the specified sequence
        // TODO  // ; source: miles214\disk3\ail.asm
        // TODO  if(_AIL_sequence_status(midi_driver_handle, sequence_handle) == SEQ_PLAYING)
        // TODO  {
        // TODO      // ; stops playback of a registered XMIDI sequence
        // TODO      // ; source: miles214\disk3\ail.asm
        // TODO      _AIL_stop_sequence(midi_driver_handle, sequence_handle);
        // TODO  }
        // TODO  // ; releases a registered sequence handle for re-use
        // TODO  // ; source: miles214\disk3\ail.asm
        // TODO  _AIL_release_seq_hnd(midi_driver_handle, sequence_handle);
        sequence_handle = -2;
    }

    if(digi_driver_handle != ST_UNDEFINED)
    {
        // jz      short $+2
    }

}


// WZD s38p09
// drake178: SND_Shutdown()
// MoO2  Module: sound  function (0 bytes) Audio_Uninit  Address: 01:0011215B
/*
; stops the Audio Interface Library and zeroes out the related main global variables
*/
void Audio_Uninit__STUB(void)
{

    if(
        (AIL_mdi_driver != ST_FALSE)
        &&
        (AIL_dig_driver != ST_FALSE)
        &&
        (audio_initialized != ST_FALSE)
    )
    {
        // ; quick shutdown of all AIL resources
        // ; source: miles214\disk3\ail.asm
        // TODO  _AIL_shutdown(str_empty_string__seg038);
    }

    midi_driver_handle = ST_UNDEFINED;
    digi_driver_handle = ST_UNDEFINED;
    audio_initialized = ST_FALSE;

}


// WZD s38p10
// drake178: UU_SND_LegacyFn()
// NX_UU_SND_LegacyFn()

// WZD s38p11
// drake178: _AIL_board_ID()
/*
*/
/*
C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\READ.ME
C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\BLASTER.C

*/
// // int16_t BLASTER__board_ID(void)
// int16_t BLASTER__board_ID(drvr_desc *desc, char *FM_driver_name, char *DSP_driver_name, char *board_name)
// {
//    int16_t i,j,k,m,d,num,p;
//    char *env;
//    char string[128];
//    static drvr_desc t;
//    static int16_t type;
//    static char tokens[] =
//       {
//       "AIDT"
//       };
//    static int16_t base[] =
//       {
//       16,10,10,10
//       };
//    static int16_t * targets[] =
//       {
//          &t.default_IO,&t.default_IRQ,&t.default_DMA,&type
//       };
//    static char *FM_driver_names[] =
//       {
//          "SBFM.ADV",
//          "SBP1FM.ADV",
//          "SBFM.ADV",
//          "SBP2FM.ADV",
//       };
//    static char *DSP_driver_names[] =
//       {
//          "SBDIG.ADV",
//          "SBPDIG.ADV",
//          "SBDIG.ADV",
//          "SBPDIG.ADV",
//       };
//    static char *board_names[] =
//       {
//          "Sound Blaster V1.5 or earlier",
//          "Sound Blaster Pro (Yamaha YM3812 version)",
//          "Sound Blaster V2.0",
//          "Sound Blaster Pro (Yamaha YMF262/OPL3 version)",
//       };
// 
//     env = getenv("BLASTER");
//     if (env==NULL) return 0;
// 
//     strncpy(string,env,127);
//     if (!strlen(string)) return 0;
// 
//     strupr(string);
// 
//     t = *desc;
// 
//     for (m=0;m<strlen(string);m++)
//         {
//         if ((m != 0) && (string[m] != ' ')) continue;
// 
//         m += (string[m] == ' '); k = string[m];
// 
//         for (i=0;i<4;i++)
//             if (k==tokens[i])
//             {
//             p = m + 1;
//             num = 0;
// 
//             do
//                 {
//                 d = string[p++];
// 
//                 for (j=0;j<base[i];j++)
//                     if (toupper(d) == "0123456789ABCDEF"[j])
//                         num = (num * base[i]) + j;
//                 }
//             while (isalnum(d));
// 
//             *targets[i] = num;
//             break;
//             }
//         }
//                      
//     *desc = t;
// 
//     if (!type) return 0;
// 
//     if (type > 4) type=4;
// 
//     if (FM_driver_name != NULL)
//         strcpy(FM_driver_name,FM_driver_names[type-1]);
// 
//     if (DSP_driver_name != NULL)
//         strcpy(DSP_driver_name,DSP_driver_names[type-1]);
// 
//     if (board_name != NULL)
//         strcpy(board_name,board_names[type-1]);
// 
//     return type;
// }


// WZD s38p12
// drake178: SND_Error()
/*
; generates an audio error message and quits the game
*/
void Audio_Error__STUB(int16_t error)
{
    char string[120];

    switch(error)
    {
        case 0:
        {
            strcpy(string, audio_error_messages[0]);  //  cnst_SND_Error01
            strcpy(string, audio_error_messages[1]);  //  cnst_SND_Error02
        } break;
        // ...
        // ...
        // ...
        case 16:
        {
            strcpy(string, audio_error_messages[16]);  //  cnst_SND_Error10
        } break;
    }

    // TODO  Exit_With_Message(string);

}
