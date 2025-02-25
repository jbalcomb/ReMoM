/*
    WIZARDS.EXE
        seg038  "SOUND"

MoO2
    Module: sound

*/

#include "SOUND.H"

#include "Allocate.H"
#include "MoX_DEF.H"
#include "MoX_TYPE.H"

#include <string.h>     /* memset(), strcat(), strcpy(); */



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
int16_t SND_MIDI_Drv_handle = ST_UNDEFINED;

// WZD dseg:82B0
int16_t SND_Digi_Drv_Handle = ST_UNDEFINED;

// WZD dseg:82B2
int16_t SND_Music_Seq_Handle = -2;

// WZD dseg:82B4 FE FF                                           dw 0FFFEh

// WZD dseg:82B6 00 00 00 00                                     SND_AIL_State_Table dd 0                ; DATA XREF: Audio_Init__STUB+58Cw ...
// WZD dseg:82BA 00                                              db 0
// WZD dseg:82BB 00                                              db 0
// WZD dseg:82BC 00                                              db 0
// WZD dseg:82BD 00                                              db 0

// WZD dseg:82BE
int16_t SND_Driver_Count;

// WZD dseg:82C0 41 49 44 54 00                                  cnst_TOKENS db 'AIDT',0                 ; DATA XREF: _AIL_board_ID:loc_3229Cr
// WZD dseg:82C0                                                                                         ; BLASTER.C static variable
// WZD dseg:82C5 00                                              db    0
// WZD dseg:82C6 10 00 0A 00 0A 00 0A 00                         cnst_BASE dw 10h, 3 dup(0Ah)            ; DATA XREF: _AIL_board_ID+CAr ...
// WZD dseg:82C6                                                                                         ; BLASTER.C static variable
// WZD dseg:82CE BE E8 C0 E8 C2 E8 CA E8                         cnst_TARGETS dw offset static_t.def_IO, offset static_t.def_IRQ, offset static_t.def_DMA, offset static_type
// WZD dseg:82CE                                                                                         ; DATA XREF: _AIL_board_ID+F8r
// WZD dseg:82CE                                                                                         ; BLASTER.C static variable

// WZD dseg:82D6
char snddrv_lbx_file__seg038[] = "SNDDRV.LBX";

// WZD dseg:82E0
char str_empty_string__seg038[] = "";

// WZD dseg:82E1 46 41 54 2E 00                                  cnst_tbank_prefix db 'FAT.',0           ; DATA XREF: Audio_Init__STUB+58Fo
// WZD dseg:82E6 72 62 00                                        cnst_RB8 db 'rb',0                      ; DATA XREF: Play_Sound__STUB:loc_31D5Fo
// WZD dseg:82E6                                                                                         ; should use dseg:28b8
// WZD dseg:82E9 42 4C 41 53 54 45 52 00                         cnst_BLASTER_envvar db 'BLASTER',0      ; DATA XREF: _AIL_board_ID+9o
// WZD dseg:82E9                                                                                         ; BLASTER.C character constant
// WZD dseg:82F1 30 31 32 33 34 35 36 37 38 39 41 42 43 44 45 46+cnst_BLASTER_numbers db '0123456789ABCDEF',0
// WZD dseg:82F1 00                                                                                      ; DATA XREF: _AIL_board_ID+BCr
// WZD dseg:82F1                                                                                         ; BLASTER.C character constant

// // char * str_allocation_errors[] =
// // ISO C++11 does not allow conversion from string literal to 'char *'clang(-Wwritable-strings)
// char const * str_allocation_errors[] =
// WZD dseg:8302
char cnst_SND_Error01[] = "You selected an invalid sound driver, ";
// WZD dseg:8329
char cnst_SND_Error02[] = "reconfigure hardware options";
// WZD dseg:8346 59 6F 75 20 73 65 6C 65 63 74 65 64 20 61 6E 20+cnst_SND_Error2 db 'You selected an invalid digi driver, ',0
// WZD dseg:8346 69 6E 76 61 6C 69 64 20 64 69 67 69 20 64 72 69+                                        ; DATA XREF: SND_Error:loc_32397o
// WZD dseg:836C 53 6F 75 6E 64 20 64 72 69 76 65 72 20 69 73 20+cnst_SND_Error3 db 'Sound driver is not a valid XMIDI driver, ',0
// WZD dseg:836C 6E 6F 74 20 61 20 76 61 6C 69 64 20 58 4D 49 44+                                        ; DATA XREF: SND_Error:loc_3239Co
// WZD dseg:8397 44 69 67 69 20 64 72 69 76 65 72 20 69 73 20 6E+cnst_SND_Error4 db 'Digi driver is not a valid DSP driver, ',0
// WZD dseg:8397 6F 74 20 61 20 76 61 6C 69 64 20 44 53 50 20 64+                                        ; DATA XREF: SND_Error:loc_323A1o
// WZD dseg:83BF 4D 75 73 69 63 20 00                            cnst_SND_Error51 db 'Music ',0          ; DATA XREF: SND_Error:loc_323A6o
// WZD dseg:83C6 49 2F 4F 20 70 61 72 61 6D 65 74 65 72 73 20 61+cnst_SND_Error52 db 'I/O parameters are incorrect, ',0
// WZD dseg:83C6 72 65 20 69 6E 63 6F 72 72 65 63 74 2C 20 00                                            ; DATA XREF: SND_Error+64o
// WZD dseg:83E5 4D 75 73 69 63 20 49 2F 4F 20 70 61 72 61 6D 65+cnst_SND_Error6 db 'Music I/O parameters are incorrect, ',0
// WZD dseg:83E5 74 65 72 73 20 61 72 65 20 69 6E 63 6F 72 72 65+                                        ; DATA XREF: SND_Error:loc_323ABo
// WZD dseg:840A 44 69 67 69 20 44 72 69 76 65 72 20 00          cnst_SND_Error8 db 'Digi Driver ',0     ; DATA XREF: SND_Error:loc_323B2o
// WZD dseg:8417 4E 6F 74 20 61 20 76 61 6C 69 64                cnst_SND_Error9 db 'Not a valid'        ; DATA XREF: SND_Error:loc_323C6o
// WZD dseg:8422 20 73 6F 75 6E 64 20 66 69 6C 65 00             cnst_SND_Error92 db ' sound file',0     ; DATA XREF: SND_Error+93o
// WZD dseg:842E 43 6F 75 6C 64 6E 27 74 20 6C 6F 61 64 20 58 4D+cnst_SND_ErrorA db 'Couldn',27h,'t load XMIDI file',0
// WZD dseg:842E 49 44 49 20 66 69 6C 65 00                                                              ; DATA XREF: SND_Error:loc_323CBo
// WZD dseg:8447 53 65 71 75 65 6E 63 65 20 64 69 64 20 6E 6F 74+cnst_SND_ErrorB db 'Sequence did not register',0
// WZD dseg:8447 20 72 65 67 69 73 74 65 72 00                                                           ; DATA XREF: SND_Error:loc_323D0o
// WZD dseg:8461 54 69 6D 62 72 65 20 6E 6F 74 20 66 6F 75 6E 64+cnst_SND_ErrorC db 'Timbre not found',0 ; DATA XREF: SND_Error:loc_323D5o
// WZD dseg:8472 43 6F 75 6C 64 20 6E 6F 74 20 6C 6F 61 64 00    cnst_SND_ErrorD db 'Could not load',0   ; DATA XREF: SND_Error:loc_323DAo
// WZD dseg:8481 4E 6F 74 20 61 20 76 61 6C 69 64 00             cnst_SND_ErrorF db 'Not a valid',0      ; DATA XREF: SND_Error:loc_323E1o
// WZD dseg:848D
char cnst_SND_Error10[] = "Could not load sound file";

// WZD dseg:84A7 00                                              align 2

// WZD dseg:84A7                                                 END:  seg038 - Initialized Data  (SOUND)



/*
    WIZARDS.EXE  seg038
*/

// WZD s38p01
// drake178: _AIL_load_timbre()
// _AIL_load_timbre()

// WZD s38p02
// drake178: Audio_Init()
/*
; initializes the sound system, loads drivers,
; and sets up the Audio Interface Library resources
; returns 1
*/
int16_t Audio_Init__STUB(int16_t midi_driver, int16_t sound_channels, int16_t MIDI_IO, int16_t MIDI_IRQ, int16_t MIDI_DMA, int16_t digi_driver, int16_t Digi_IO, int16_t Digi_IRQ, int16_t Digi_DMA)
{
    int16_t Use_Digi_DMA;
    int16_t Use_Digi_IRQ;
    int16_t Use_Digi_IO;
// Prev_Digi_IRQ= word ptr -2Ch
// Prev_Digi_DMA= word ptr -2Ah
// Prev_Digi_IO= word ptr -28h
// Prev_MIDI_DMA= word ptr -26h
// Prev_MIDI_IRQ= word ptr -24h
// Prev_MIDI_IO= word ptr -22h
    int16_t Use_MIDI_DMA;
    int16_t Use_MIDI_IRQ;
    int16_t Use_MIDI_IO;
// State_Table_Size= dword ptr -1Ah
// Timbre_Cache= dword ptr -16h
    int16_t digi_set;
    SAMB_ptr Sound_Data_Space;
// State_Tbl_Seg= word ptr -0Eh
// Timbre_Cache_Seg= word ptr -0Ah
    int16_t midi_set;
// GMIDI_Timbre_Init= word ptr -6
// Roland_Timbre_Init= word ptr -4
// SB_Type= word ptr -2

    midi_set = ST_FALSE;
    digi_set = ST_FALSE;

    AIL_mdi_driver = midi_driver;
    AIL_dig_driver = digi_driver;

    if(audio_initialized == ST_FALSE)
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





// AIL_mdi_driver], SND_SBPro
// AIL_mdi_driver], SND_SBPro2
// call    near ptr _AIL_board_ID          ; returns the Sound Blaster (Pro) type from the BLASTER
//                                         ; environment variable, or 0 if none; copied from the
//                                         ; BLASTER example of the Miles/IBM AIL (board_ID
//                                         ; function, ln 131-171); forces the entire IS.CAS file
//                                         ; to be linked by its single macro call to isalnum()
//                                         ; source: miles214\examples\blaster.c
// mov     [bp+SB_Type], ax





    audio_initialized = ST_TRUE;

    Sound_Data_Space = Allocate_Space(20);  // 20 PR  320 B

    // WTF  SND_TLoad_Buffer = SA_MK_FP0(Sound_Data_Space);





    return ST_TRUE;
}


// WZ 8s38p03
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
void Play_Sound__STUB(SAMB_ptr voc)
{



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

    if(SND_Music_Seq_Handle != -2)
    {
        Percent = 0;

        // // ; returns the status of the specified sequence
        // // ; source: miles214\disk3\ail.asm
        // if(_AIL_sequence_status(SND_MIDI_Drv_handle, SND_Music_Seq_Handle) == SEQ_PLAYING)
        // {
        //     // ; sets the XMIDI volume scaling factor, ramping up/down
        //     // ; source: miles214\disk3\ail.asm
        //     _AIL_set_rel_volume(SND_MIDI_Drv_handle, SND_Music_Seq_Handle, Percent, 1000);
        // }
    }

    if(
        (SND_Digi_Drv_Handle != -1)
        &&
        (AIL_dig_driver > SND_NONE)
    )
    {
        // // ; silences output and resets any sound buffers
        // // ; source: miles214\disk3\ail.asm
        // _AIL_stop_digi_play(SND_Digi_Drv_Handle);
    }

}

// WZD s38p07
// drake178: SND_Get_Seq_Status()
// SND_Get_Seq_Status()

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
        if(SND_Digi_Drv_Handle == ST_UNDEFINED)
        {
            if(AIL_dig_driver > SND_NONE)
            {
                // jle     short $+2
            }
        }
        return;
    }

    if(SND_Music_Seq_Handle != -2)
    {
        // TODO  // ; returns the status of the specified sequence
        // TODO  // ; source: miles214\disk3\ail.asm
        // TODO  if(_AIL_sequence_status(SND_MIDI_Drv_handle, SND_Music_Seq_Handle) == SEQ_PLAYING)
        // TODO  {
        // TODO      // ; stops playback of a registered XMIDI sequence
        // TODO      // ; source: miles214\disk3\ail.asm
        // TODO      _AIL_stop_sequence(SND_MIDI_Drv_handle, SND_Music_Seq_Handle);
        // TODO  }
        // TODO  // ; releases a registered sequence handle for re-use
        // TODO  // ; source: miles214\disk3\ail.asm
        // TODO  _AIL_release_seq_hnd(SND_MIDI_Drv_handle, SND_Music_Seq_Handle);
        SND_Music_Seq_Handle = -2;
    }

    if(SND_Digi_Drv_Handle != ST_UNDEFINED)
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

    SND_MIDI_Drv_handle = ST_UNDEFINED;
    SND_Digi_Drv_Handle = ST_UNDEFINED;
    audio_initialized = ST_FALSE;

}


// WZD s38p10
// drake178: UU_SND_LegacyFn()
// NX_UU_SND_LegacyFn()

// WZD s38p11
// drake178: _AIL_board_ID()
// _AIL_board_ID()

// WZD s38p12
// drake178: SND_Error()
/*
; generates an audio error message and quits the game
*/
void Sound_Error__STUB(int16_t error)
{
    char string[120];

    // cmp     bx, 16                          ; switch 17 cases
    switch(error)
    {
        case 0:
        {
            strcpy(string, cnst_SND_Error01);
            strcpy(string, cnst_SND_Error02);
        } break;
        // ...
        // ...
        // ...
        case 16:
        {
            strcpy(string, cnst_SND_Error10);
        } break;
    }

    // TODO  Exit_With_Message(string);

}
