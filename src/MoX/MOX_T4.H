/*
    WIZARDS.EXE
        ...
        seg001
        ...
        seg009
        ...
*/

#ifndef MOX_UNORG_H
#define MOX_UNORG_H

#include "MoX_Lib.H"

#ifdef __cplusplus
extern "C" {
#endif



extern int16_t current_screen;
extern int16_t previous_screen;



extern int16_t PageFlipEffect;



// MGC dseg:27FF                                                 BEGIN: seg001 - Initialized Data  (main)

// MGC dseg:27FF 43 4F 4E 46 49 47 2E 4D 4F 4D 00                CONFIG_MOM db 'CONFIG.MOM',0            ; DATA XREF: _main+12o ...
// MGC dseg:280A 52 75 6E 20 49 4E 53 54 41 4C 4C 20 74 6F 20 63+cnst_ConfigErr db 'Run INSTALL to configure MASTER OF MAGIC.',0Ah
// MGC dseg:280A 4F 46 20 4D 41 47 49 43 2E 0A 0A 00             db 0Ah,0
// MGC dseg:2836 72 62 00                                        cnst_ReadBinary db 'rb',0               ; DATA XREF: _main:@@Found_ConfigMomo ...
// MGC dseg:2836                                                                                         ; "read binary" open mode constant

// MGC dseg:2839 49 6E 69 74 69 61 6C 69 7A 69 6E 67 20 52 6F 6C+cnst_MIDI_Init db 'Initializing Roland Drivers...$',0
extern char str_Initializing_Roland_Drivers[];

// MGC dseg:2859 53 41 56 45 00                                  cnst_SAVE db 'SAVE',0                   ; DATA XREF: _main+1D9o ...
// MGC dseg:285E 2E 47 41 4D 00                                  cnst_SAVE_ext db '.GAM',0               ; DATA XREF: _main+1F7o ...
// MGC dseg:2863 4D 41 47 49 43 2E 53 45 54 00                   cnst_Set_File db 'MAGIC.SET',0          ; DATA XREF: _main+241o
// MGC dseg:286D 77 62 00                                        cnst_WB db 'wb',0                       ; DATA XREF: _main+23Do
// MGC dseg:2870 46 4F 4E 54 53 2E 4C 42 58 00                   fonts_lbx_file__main db 'FONTS.LBX',0   ; DATA XREF: _main+2AFo
// MGC dseg:287A
extern char music_lbx__main[];
// MGC dseg:2880 54 68 61 6E 6B 20 79 6F 75 20 66 6F 72 20 70 6C+str_quit_message__main db 'Thank you for playing Master of Magic!',0
// MGC dseg:2880 61 79 69 6E 67 20 4D 61 73 74 65 72 20 6F 66 20+                                        ; DATA XREF: Menu_Screen_Control+D1o

// MGC dseg:2880                                                 END: seg001 - Initialized Data  (main)



/*
    WIZARDS.EXE seg001
*/

// WZD s01p01
// WZD s01p02

// WZD s01p03
void PageFlip_FX(void);

// WZD s01p04
void Fade_Out(void);

// WZD s01p05
void Fade_In(void);

// WZD s01p06
// WZD s01p07
// WZD s01p08
// WZD s01p09
// WZD s01p10

// WZD s01p11
void s01p11_empty_function(void);

// WZD s01p12
void s01p12_empty_function(void);

// WZD s01p13
void Play_Left_Click__STUB(void);

// WZD s01p14
void Play_Standard_Click__STUB(void);

// WZD s01p15
void Play_Left_Click__DUPE(void);

// WZD s01p16
void s01p16_empty_function(void);



/*
    WIZARDS.EXE  seg009
*/

// WZD s09p07
// void String_Copy_Far(uint16_t dst_ofst, char * dst_sgmt, uint16_t src_ofst, char * src_sgmt);
void String_Copy_Far(char * dst, char * src);
void String_Copy_Far__ASM(unsigned short int dst_ofst, unsigned short int dst_sgmt, unsigned short int src_ofst, unsigned short int src_sgmt);



#ifdef __cplusplus
}
#endif

#endif  /* MOX_UNORG_H */
