#ifndef ST_VGA_H
#define ST_VGA_H


#define VGA_Min_X 0                             // dseg:41B2
#define VGA_Min_Y 0                             // dseg:41B4
#define VGA_Max_X 319                           // dseg:41B6
#define VGA_Max_Y 199                           // dseg:41B8
#define DEFAULT_FONTS_FILE "FONTS.LBX"          // dseg:41BA
// dseg:41C4 00 A0                         VGA_DisplayBuffer_BaseAddress dw 0A000h
// dseg:41C6 00 00                         VGA_Display_Frame dw 0
// dseg:41C8 0F 0E 0C 08                   VGA_LeftBits db  15, 14, 12,  8         ; bit 0 = plane 0 means this is reverse-ordered
// dseg:41C8                                                                       ; ~= byte  leftmasks[4] = {15,14,12,8};
// dseg:41CC 01 03 07 0F                   VGA_RightBits db   1,  3,  7, 15        ; bit 0 = plane 0 means this is reverse-ordered
// dseg:41CC                                                                       ; ~= byte rightmasks[4] = {1,3,7,15};
// dseg:41D0 01 02 04 08                   VGA_WriteMapMasks db   1,  2,  4,  8    ; 
// dseg:41D0                                                                       ; ~= byte pixmasks[4] = {1,2,4,8};
// dseg:41D4 00 00 00 00 00 00 00 00 00 00+UU_Wave_Colors db 40h dup(0)
// dseg:4214 29 24 3F 05 62 6D 57 2F 53 11+UU_RNG_7bit_Lookup db  29h, 24h, 3Fh,   5, 62h, 6Dh, 57h, 2Fh, 53h, 11h, 4Ah,2 dup( 72h), 3Ch, 6Ah, 6Ch, 33h, 27h, 5Ch, 3Dh,   8, 0Dh, 3Fh, 1Ah, 25h, 5Fh, 0Eh, 1Dh,   7, 38h, 48h, 5Fh, 33h, 13h, 4Eh, 49h, 44h, 3Ch, 0Ch, 27h, 20h,   4, 5Bh, 7Eh, 0Ah, 39h, 26h, 20h, 5Dh, 55h, 4Ch, 7Dh, 17h, 76h, 46h, 3Ch, 14h, 0Eh, 0Ah, 0Bh, 1Dh, 5Ch, 2Fh, 33h, 20h, 1Bh, 51h, 6Fh, 41h, 79h, 37h, 7Eh, 13h, 4Ah, 33h, 77h, 1Fh, 7Eh, 4Ah, 5Dh, 2Dh, 50h, 15h, 73h, 45h, 41h, 67h, 51h, 6Ch, 45h, 31h, 38h, 33h, 3Ch, 22h, 23h, 76h, 23h, 12h, 1Eh, 62h, 0Ch, 20h, 5Bh, 31h, 4Bh, 1Ah,   3, 3Ah, 73h, 1Eh, 4Ah, 2Ch,   1, 7Fh, 46h, 1Ah, 56h, 6Ah,   0, 33h, 6Bh, 4Ah, 4Dh, 54h, 40h, 68h, 57h, 3Fh, 15h, 57h, 7Fh, 2Eh, 5Dh, 0Fh, 67h,   4, 70h, 58h, 4Ah, 62h, 7Fh, 6Ah, 10h, 61h, 4Eh, 52h, 1Fh, 1Eh, 1Dh, 17h,2 dup( 73h), 67h, 1Eh, 71h,   5, 50h, 4Bh, 78h,   2, 58h, 69h, 3Ah, 2Dh, 54h, 4Ch, 4Ah, 13h, 1Fh, 34h, 75h, 1Fh, 0Dh, 75h, 56h, 54h, 20h, 55h, 25h, 5Ah, 7Fh, 36h, 50h, 33h, 23h, 75h, 4Dh, 50h, 54h, 11h, 2Eh, 48h, 54h, 10h, 76h, 67h, 5Ah, 1Eh; 0
// dseg:4214 4A 72 72 3C 6A 6C 33 27 5C 3D+db  2Bh, 66h, 41h, 78h, 2Ch, 79h,   2,   8, 45h, 0Eh, 60h, 51h,   1, 55h, 62h, 0Eh, 3Fh, 7Ch,   6, 16h,   8, 3Ch, 34h,   3, 20h, 18h, 71h, 13h, 5Bh, 65h, 55h, 4Fh, 32h,   6, 3Fh, 6Ah, 16h, 79h, 47h, 6Bh,   5, 16h, 74h, 0Fh, 5Ah, 17h, 30h, 68h, 69h, 55h, 78h,2 dup( 4Bh), 51h, 58h, 69h, 77h; 199
// dseg:4314 00 00                         VGA_Outline_Color dw 0
// dseg:4316 00 00                         VGA_Skip_AA_Pixels dw 0
extern unsigned int UU_g_VGA_TextDraw_Initd;    // dseg:4318
// dseg:431A 00 00                         VGA_FltBlock_Count dw 0
// dseg:431C FF FF                         VGA_BlinkState dw 0FFFFh                ; 0 - normal, 1 - reverse, -1 - disabled

extern unsigned int gsa_VGAFILEH_Header;        // dseg:A7D4    alloc in VGA_DAC_Init
extern unsigned int gsa_PaletteFlags;           // dseg:A7D6    alloc in VGA_DAC_Init
extern unsigned int gsa_ShadingColors;          // dseg:A7D8  ; 5,376 bytes (1500h) into the palette record
extern unsigned int gsa_ReplacementColors;      // dseg:A7DA      alloc in VGA_DAC_Init
extern unsigned int gsa_PaletteSaved;           // dseg:A7DC    alloc in VGA_DAC_Init
extern unsigned int gsa_Palette;                // dseg:A7DE    alloc in VGA_DAC_Init
extern char g_PaletteLbxFileName[];             // dseg:A7E0  ; set in VGA_DAC_Init
extern unsigned int gsa_IntensityScaleTable;    // dseg:A7F0    alloc in VGA_DAC_Init
extern unsigned int gsa_gsa_PaletteLbxEntry;    // dseg:A7F2
extern unsigned int gsa_PaletteLbxEntry;        // dseg:A7F4    alloc in VGA_DAC_Init
extern unsigned int gsa_FontStyleData;          // dseg:A816    alloc & load in VGA_DAC_Init
extern unsigned int gsa_BorderStyleData;        // dseg:A818    alloc & load in VGA_DAC_Init
extern unsigned int gsa_Palette_Cursor_Data;    // dseg:A81A  ; 500h into the palette entry
extern unsigned int UU_gsa_Palette_Data;        // dseg:A81C  ; 400h into the palette entry
extern unsigned int gsa_Palette_Font_Colors;    // dseg:A81E  ; 300h into the palette entry, 16 arrays of 16 colors
extern void far * gfp_VGA_TextLine_Starts;    // dseg:A820
extern void far * gfp_VGA_TextLine_Tops;      // dseg:A824
extern void far * gfp_VGA_TextLine_Rights;    // dseg:A828
extern void far * gfp_VGA_TextLine_Lefts;     // dseg:A82C
extern unsigned int gsa_VGA_TextDraw_Lines;     // dseg:A830



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    seg015
*/
// _s15p01
void VGA_SetModeY(void);

/*
    seg019
*/
void VGA_TextDraw_Init(void);  // _s19p14.asm


#ifdef __cplusplus
}
#endif

#endif /* ST_VGA_H */
