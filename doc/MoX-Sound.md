


"Sound Driver"  MIDI/XMIDI  *Music*  XMI files
"Digital Sound Driver" *Sound*  VOC files



Audio_Init__STUB()  ==>  Audio_Init__WIP()

SND_Get_Seq_Status()  ==>  Midi_Sequence_Status()


Sound_Data_Space  ==>  timb_seg
SND_TLoad_Buffer  ==>  timb_ptr
SND_TBank_Handle  ==>  GTL

SND_Timbre_Data  ==>  timb

SND_Music_Seq_Handle  ==>  sequence_handle

AIL_mdi_driver
AIL_dig_driver
SND_AIL_Digi_Driver  ==>  
SND_AIL_MIDI_Driver  ==>  
SND_Digi_drvr_desc  ==>  
SND_MIDI_drvr_desc  ==>  
SND_Music_Data  ==>  midi_sound_buffer
SND_Digi_Data  ==>  digi_sound_buffer
SND_MIDI_Drv_Handle  ==>  midi_driver_handle
SND_Digi_Drv_Handle  ==>  digi_driver_handle




DEAD
 D  A  E  D
13 10 14 13


// enum File_Signatures
// SA_MEMSIG1      = 0x12FA
// SA_MEMSIG2      = 0x4ECF
// EXE_Sig         = 0x5A4D
// e_FLIC_Decoded  = 0xDE0A
// VOC_FormSig     = 0xDEAD
// SND_FileSig     = 0xDEAF
// LBX_FileSig     = 0xFEAD


J:\STU_J\STU-Archive\STU__devel\STU-LBX_Library\LibLBX\lib_lbx.h
    LBX_RECORD_TYPE_SOUND_XMIDI = 2,          /* 0xDEAF 0x0001 */
    LBX_RECORD_TYPE_SOUND_VOC = 3,            /* 0xDEAF 0x0002 */
J:\STU_J\STU-Archive\STU__devel\STU-LBX_Library\LibLBX\lib_lbx_record_type.c
if (bytes_1to2 == LBX_SOUND_MAGIC)
if (bytes_3to4 == LBX_SOUND_XMIDI_TYPE_MAGIC)
if (bytes_3to4 == LBX_SOUND_VOC_TYPE_MAGIC)



mov     ax, 20
push    ax                              ; LBX_Entry
mov     ax, offset snddrv_lbx_file__seg038 ; "SNDDRV.LBX"
push    ax                              ; LBX_Name
call    
Roland_Timbre_Init = LBX_Load();
SNDDRV.LBX, 20
D3 C3 02 00 
5B C7 02 00 
0002C3D3
0002C75B



void far *load_global_timbre(FILE *GTL, unsigned bank, unsigned patch)
    unsigned far *timb_ptr;
    timb_ptr = farmalloc(len);     // allocate memory for timbre ..
    *timb_ptr = len;         





MGC
_AIL_   ~ seg035
SOUND   ~ seg036
WZD
_AIL_   ~ seg047
SOUND   ~ seg038

MoO1 ORION
seg038
MoO1 STARMAP


MGC Audio_Init__WIP()
XREF:
    Init_Drivers+4F
    Init_Drivers+81
    UU_Legacy_Startup+28
    UU_Legacy_Startup+44

WZD Audio_Init__WIP()
    Init_Drivers+4F
    Init_Drivers+81
    UU_Legacy_Startup:loc_18756
    UU_Legacy_Startup+44

MoO1 ORION

MoO1 STARMAP


Init.C
Init_Drivers()
    if(input_type == ST_UNDEFINED)
        // /* TODO */  Audio_Init__STUB(0, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1);  // IDGI two parameters too many
        /* TODO */  Audio_Init__STUB(0, 0, -1, -1, -1, -1, -1, -1, -1);
    else
        Audio_Init__STUB(midi_driver, sound_channels, MIDI_IO, MIDI_IRQ, MIDI_DMA, digi_driver, Digi_IO, Digi_IRQ, Digi_DMA);



## Audio_Init__WIP()


### Detect Device
AIL_mdi_driver > SND_Speaker
vs.
midi_set != ST_FALSE
calls _AIL_detect_device() with passed in parameters
vs.
calls _AIL_detect_device() with saved MIDI DMA, IO, and IRQ

// C:\STU\devel\Audio Interface Library (AIL)\AIL2\A214_D3\VOCPLAY.C
```c
if(!AIL_detect_device(hdriver,desc->default_IO,desc->default_IRQ,desc->default_DMA,desc->default_DRQ))
{
    printf("Sound hardware not found.\n");
    AIL_shutdown(NULL);
    exit(1);
}
```
