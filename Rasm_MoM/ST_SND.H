#ifndef ST_SND_H
#define ST_SND_H

/*
    Simtex Sound
        + Audio Interface Library (AIL)
*/

// dseg:4D7A SND_MIDI_Drv_Index dw 0
// dseg:4D7C SND_Digi_Drv_Index dw 0
// dseg:4D7E g_SND_AIL_running dw 0
// dseg:4D80 ; int SND_MIDI_Drv_handle
// dseg:4D80 SND_MIDI_Drv_handle dw 0FFFFh
// dseg:4D82 ; int SND_Digi_Drv_Handle
// dseg:4D82 SND_Digi_Drv_Handle dw 0FFFFh
// dseg:4D84 ; int SND_Music_Seq_Handle
// dseg:4D84 SND_Music_Seq_Handle dw 0FFFEh
// dseg:4D86 dw 0FFFEh
// dseg:4D88 ; void far *SND_AIL_State_Table
// dseg:4D88 SND_AIL_State_Table dd 0
// dseg:4D8C align 8

extern int g_SND_Driver_Count;                          // dseg:4D90

// dseg:4D92 cnst_TOKENS db 'AIDT',0                 ; BLASTER.C static variable
// dseg:4D97 align 2
// dseg:4D98 cnst_BASE dw    10h,3 dup(   0Ah)                 ; 0 ; BLASTER.C static variable
// dseg:4DA0 cnst_TARGETS dw offset static_t.default_IO,offset static_t.default_IRQ,offset static_t.default_DMA,offset static_type ; BLASTER.C static variable
// dseg:4DA8 cnst_SNDDRV_FileName db 'SNDDRV.LBX'
// dseg:4DB2 cnst_ZeroString_2 db 0
// dseg:4DB3 cnst_tbank_prefix db 'FAT.',0
// dseg:4DB8 cnst_RB8 db 'rb',0                      ; should use dseg:2836
// dseg:4DBB cnst_BLASTER_envvar db 'BLASTER',0      ; BLASTER.C character constant
// dseg:4DC3 cnst_BLASTER_numbers db '0123456789ABCDEF',0 ; BLASTER.C character constant

// dseg:4DD4 cnst_SND_Error01 db 'You selected an invalid sound driver, ',0
// dseg:4DFB cnst_SND_Error02 db 'reconfigure hardware options',0
// dseg:4E18 cnst_SND_Error2 db 'You selected an invalid digi driver, ',0
// dseg:4E3E cnst_SND_Error3 db 'Sound driver is not a valid XMIDI driver, ',0
// dseg:4E69 cnst_SND_Error4 db 'Digi driver is not a valid DSP driver, ',0
// dseg:4E91 cnst_SND_Error51 db 'Music ',0
// dseg:4E98 cnst_SND_Error52 db 'I/O parameters are incorrect, ',0
// dseg:4EB7 cnst_SND_Error6 db 'Music I/O parameters are incorrect, ',0
// dseg:4EDC cnst_SND_Error8 db 'Digi Driver ',0
// dseg:4EE9 cnst_SND_Error9 db 'Not a valid'
// dseg:4EF4 cnst_SND_Error92 db ' sound file',0
// dseg:4F00 cnst_SND_ErrorA db 'Couldn',27h,'t load XMIDI file',0
// dseg:4F19 cnst_SND_ErrorB db 'Sequence did not register',0
// dseg:4F33 cnst_SND_ErrorC db 'Timbre not found',0
// dseg:4F44 cnst_SND_ErrorD db 'Could not load',0
// dseg:4F53 cnst_SND_ErrorF db 'Not a valid',0
// dseg:4F5F cnst_SND_Error10 db 'Could not load sound file',0


// dseg:A8A6 static_tlen dw 0
// dseg:A8A8 static_t AIL_drvr_desc <0>              ; BLASTER.C static variable
// dseg:A8C0 static_type dw 0                        ; BLASTER.C static variable
// dseg:A8C2 static_GTL GTL_Header_Record <0>
// dseg:A8C8 SND_tbank_suffix db 16h dup(   0)
// dseg:A8DE SND_TLoad_Buffer dd 0                   ; 14h LBX_Alloc_Space paragraphs
// dseg:A8E2 ; void far *SND_Timbre_Data
// dseg:A8E2 SND_Timbre_Data@ dd 0
// dseg:A8E6 ; void far *SND_AIL_Digi_Driver
// dseg:A8E6 SND_AIL_Digi_Driver@ dd 0
// dseg:A8EA ; void far *SND_AIL_MIDI_Driver
// dseg:A8EA SND_AIL_MIDI_Driver@ dd 0
// dseg:A8EE SND_Digi_drvr_desc dd 0
// dseg:A8F2 SND_MIDI_drvr_desc dd 0
// dseg:A8F6 ; void far *SND_Digi_Data
// dseg:A8F6 SND_Digi_Data@ dd 0
// dseg:A8FA ; void far *SND_Music_Data
// dseg:A8FA SND_Music_Data@ dd 0
// dseg:A8FE SND_Timbre_Bank_File db 10h dup(   0)
// dseg:A90E SND_TBank_Handle dw 0

#endif /* ST_SND_H */
