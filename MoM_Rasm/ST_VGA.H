#ifndef ST_VGA_H
#define ST_VGA_H


#define CLRSCR0() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDrawAddress(); }
#define CLRSCR1() { VGA_DrawFilledRect(0,0,319,199,0); VGA_SetDirectDraw();  }

//     VGA_LoadPalette(EMPERATO, -1, 0); VGA_DAC_Write();
#define SETPAL(_idx_) { VGA_LoadPalette(_idx_, -1, 0); VGA_DAC_Write();  }


// 3DAh  e_INPUT_STATUS  STATUS_REGISTER_1

#define WAIT_VBL() asm{ \
mov dx, 3DAh; \
@@waitnosync: \
in al, dx; \
and al, 1000b; \
jnz @@waitnosync; \
@@waitsync:; \
in al, dx; \
and al, 1000b; \
jz @@waitsync; \
}
//J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_VL.H
//#define VGAMAPMASK(x) asm{cli;mov dx,SC_INDEX;mov al,SC_MAPMASK;mov ah,x;out dx,ax;sti;}
#define VGAMAPMASK() { \
    asm { \
        mov  dx, e_SC_INDEX; \
        mov  al, e_SC_MAPMASK; \
        out  dx, al; \
    } \
}
//#define VGAMAPMASK() { \
//    asm mov dx, e_SC_INDEX; \
//    asm mov al, e_SC_MAPMASK; \
//    asm out dx, al; \
//}
//#define VGAMAPMASK() asm{mov dx, e_SC_INDEX;mov al, e_SC_MAPMASK;out dx, al;}

//#define VGAWRITEPLANE() { \
//    asm { \
//        mov dx, e_SC_DATA;   /* 0x03C5 */ \
//        mov al, ah;          /* VGA_WriteMapMask[3] = {0001,0010,0100,1000} */ \
//        out dx, al; \
//    } \
//}


#define e_GC_INDEX   0x03CE
#define e_GC_DATA    0x03CF
#define e_GC_READMAP   0x04
#define e_SC_INDEX   0x03C4
#define e_SC_DATA    0x03C5
#define e_SC_MAPMASK   0x02

#define MINX      0
#define MINY      0
#define MIDX    158
#define MIDY    100
#define MAXX    320
#define MAXY    200
#define PIXX    320
#define PIXY    200
#define LINE_STRIDE  80

#define IBM_PC_VGA_GRAPHICS_MODE_VIDEO_RAM  0xA000
#define VRAM_BASE                           0xA000



extern int g_VGA_Min_X;                             // dseg:41B2
extern int g_VGA_Min_Y;                             // dseg:41B4
extern int g_VGA_Max_X;                             // dseg:41B6
extern int g_VGA_Max_Y;                             // dseg:41B8
extern char DEFAULT_FONTS_FILE[];                   // dseg:41BA
extern unsigned int gsa_DSP_Addr;                   // dseg:41C4
extern int g_RSP_Idx;                               // dseg:41C6
extern unsigned char g_VGA_LeftBits[];              // dseg:41C8
extern unsigned char g_VGA_RightBits[];             // dseg:41CC
extern unsigned char g_VGA_WriteMapMasks[];         // dseg:41D0
// dseg:41D4 00 00 00 00 00 00 00 00 00 00+UU_Wave_Colors db 40h dup(0)
// dseg:4214 29 24 3F 05 62 6D 57 2F 53 11+UU_RNG_7bit_Lookup db  29h, 24h, 3Fh,   5, 62h, 6Dh, 57h, 2Fh, 53h, 11h, 4Ah,2 dup( 72h), 3Ch, 6Ah, 6Ch, 33h, 27h, 5Ch, 3Dh,   8, 0Dh, 3Fh, 1Ah, 25h, 5Fh, 0Eh, 1Dh,   7, 38h, 48h, 5Fh, 33h, 13h, 4Eh, 49h, 44h, 3Ch, 0Ch, 27h, 20h,   4, 5Bh, 7Eh, 0Ah, 39h, 26h, 20h, 5Dh, 55h, 4Ch, 7Dh, 17h, 76h, 46h, 3Ch, 14h, 0Eh, 0Ah, 0Bh, 1Dh, 5Ch, 2Fh, 33h, 20h, 1Bh, 51h, 6Fh, 41h, 79h, 37h, 7Eh, 13h, 4Ah, 33h, 77h, 1Fh, 7Eh, 4Ah, 5Dh, 2Dh, 50h, 15h, 73h, 45h, 41h, 67h, 51h, 6Ch, 45h, 31h, 38h, 33h, 3Ch, 22h, 23h, 76h, 23h, 12h, 1Eh, 62h, 0Ch, 20h, 5Bh, 31h, 4Bh, 1Ah,   3, 3Ah, 73h, 1Eh, 4Ah, 2Ch,   1, 7Fh, 46h, 1Ah, 56h, 6Ah,   0, 33h, 6Bh, 4Ah, 4Dh, 54h, 40h, 68h, 57h, 3Fh, 15h, 57h, 7Fh, 2Eh, 5Dh, 0Fh, 67h,   4, 70h, 58h, 4Ah, 62h, 7Fh, 6Ah, 10h, 61h, 4Eh, 52h, 1Fh, 1Eh, 1Dh, 17h,2 dup( 73h), 67h, 1Eh, 71h,   5, 50h, 4Bh, 78h,   2, 58h, 69h, 3Ah, 2Dh, 54h, 4Ch, 4Ah, 13h, 1Fh, 34h, 75h, 1Fh, 0Dh, 75h, 56h, 54h, 20h, 55h, 25h, 5Ah, 7Fh, 36h, 50h, 33h, 23h, 75h, 4Dh, 50h, 54h, 11h, 2Eh, 48h, 54h, 10h, 76h, 67h, 5Ah, 1Eh; 0
// dseg:4214 4A 72 72 3C 6A 6C 33 27 5C 3D+db  2Bh, 66h, 41h, 78h, 2Ch, 79h,   2,   8, 45h, 0Eh, 60h, 51h,   1, 55h, 62h, 0Eh, 3Fh, 7Ch,   6, 16h,   8, 3Ch, 34h,   3, 20h, 18h, 71h, 13h, 5Bh, 65h, 55h, 4Fh, 32h,   6, 3Fh, 6Ah, 16h, 79h, 47h, 6Bh,   5, 16h, 74h, 0Fh, 5Ah, 17h, 30h, 68h, 69h, 55h, 78h,2 dup( 4Bh), 51h, 58h, 69h, 77h; 199
// dseg:4314 00 00                         VGA_Outline_Color dw 0
// dseg:4316 00 00                         VGA_Skip_AA_Pixels dw 0
extern unsigned int UU_g_VGA_TextDraw_Initd;    // dseg:4318
// dseg:431A 00 00                         VGA_FltBlock_Count dw 0
// dseg:431C FF FF                         VGA_BlinkState dw 0FFFFh                ; 0 - normal, 1 - reverse, -1 - disabled

// \/\/\/ ? VGA, GUI, etc ? \/\/\/
// dseg:436C VGA_WriteMapMasks2 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4370 GUI_DS_Save dw 0
extern unsigned char g_VGA_WriteMapMasks3[]; // dseg:4372  db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4376 VGA_WriteMapMasks4 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437A VGA_WriteMapMasks5 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437E dw 0
// dseg:4380 VGA_WriteMapMasks6 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4384 dw 0
// dseg:4386 VGA_WriteMapMasks7 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// /\/\/\ ? VGA, GUI, etc ? /\/\/\

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
//dseg:A7F6 VGA_AAColor_Array db 16 dup (0)
//dseg:A806 VGA_Current_AA_Color db 0
//dseg:A807 VGA_AA_Color_1 db 0
//dseg:A808 VGA_AA_Color_2 db 0
//dseg:A809 VGA_AA_Color_3 db 0
extern unsigned int VGA_Font_ColorIndex3;                       // dseg:A80A
extern unsigned int VGA_Font_ColorIndex2;                       // dseg:A80C
extern unsigned int VGA_Font_ColorIndex1;                       // dseg:A80E
extern unsigned int VGA_Font_Index;                             // dseg:A810
//VGA_TextDraw_Y; // dseg:A812
//VGA_TextDraw_X; // dseg:A814
extern unsigned int gsa_FontStyleData;          // dseg:A816    alloc & load in VGA_DAC_Init
extern unsigned int gsa_BorderStyleData;        // dseg:A818    alloc & load in VGA_DAC_Init
extern unsigned int gsa_Cursor_Array;    // dseg:A81A  ; 500h into the palette entry
extern unsigned int UU_gsa_Palette_Data;        // dseg:A81C  ; 400h into the palette entry
extern unsigned int gsa_Palette_Font_Colors;    // dseg:A81E  ; 300h into the palette entry, 16 arrays of 16 colors
extern void * gfp_VGA_TextLine_Starts;    // dseg:A820
extern void * gfp_VGA_TextLine_Tops;      // dseg:A824
extern void * gfp_VGA_TextLine_Rights;    // dseg:A828
extern void * gfp_VGA_TextLine_Lefts;     // dseg:A82C
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
    seg016
*/
// _s16p01
void VGA_DrawFilledRect(int x1, int y1, int x2, int y2, char Color);

/*
    seg019  seg019.H
*/
// _s19p11c.c
// int VGA_GetFontHeight(void);
// _s19p14c.c
// void VGA_TextDraw_Init(void);

/*
    seg020  seg020.H
*/
//_s20p01
// void VGA_LoadPalette(int Palette_Index, int First_Color, int Last_Color);
//_s20p05
// void VGA_SetDACChanged(int First_Color, int Last_Color);

/*
    seg021  seg021.H
*/
// _s21p01
// void VGA_DAC_Write(void);
// _s21p07
// void FLIC_LoadPalette(unsigned int FlicHdr_SgmtAddr, int Frame_Index);

/*
    seg026
*/
//_s26p01
void VGA_SetDirectDraw(void);
//_s26p02
void VGA_Set_DSP_Addr(void);
//_s26p03
int VGA_IsDefaultDrawScreenPage(void);
//_s26p04
void VGA_PageFlip(void);
// _s26p05
void VGA_Copy_RSP_DSP(void);
// _s26p06
void VGA_Copy_RSP_DSP_YM(void);
// _s26p07
void VGA_Copy_RSP_DSP_NM(void);


#ifdef __cplusplus
}
#endif

#endif /* ST_VGA_H */
