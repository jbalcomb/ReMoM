
#include "ST_TYPE.H"    /* byte_ptr, SAMB_ptr */

// TODO(JimBalcomb,20221017): remove all usages of ST_HEAD.H
#include "ST_HEAD.H"

#include "ST_VGA.H"


int g_VGA_Min_X = MINX;                             // dseg:41B2
int g_VGA_Min_Y = MINY;                             // dseg:41B4
int g_VGA_Max_X = MAXX;                             // dseg:41B6
int g_VGA_Max_Y = MAXY;                             // dseg:41B8
char DEFAULT_FONTS_FILE[] = "FONTS.LBX";            // dseg:41BA
unsigned int gsa_DSP_Addr = 0xA000;                 // dseg:41C4
int g_RSP_Idx = 0;                                  // dseg:41C6
unsigned char g_VGA_LeftBits[4] = {15,14,12,8};     // dseg:41C8
unsigned char g_VGA_RightBits[4] = {1,3,7,15};      // dseg:41CC
unsigned char g_VGA_WriteMapMasks[4] = {1,2,4,8};   // dseg:41D0

// seg019.H  unsigned int UU_g_VGA_TextDraw_Initd = 0;       // dseg:4318

// \/\/\/ ? VGA, GUI, etc ? \/\/\/
// dseg:436C VGA_WriteMapMasks2 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4370 GUI_DS_Save dw 0
unsigned char g_VGA_WriteMapMasks3[4] = {0x01, 0x02, 0x04, 0x08}; // dseg:4372
// dseg:4376 VGA_WriteMapMasks4 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437A VGA_WriteMapMasks5 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:437E dw 0
// dseg:4380 VGA_WriteMapMasks6 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// dseg:4384 dw 0
// dseg:4386 VGA_WriteMapMasks7 db         1,      10b,     100b,    1000b; 0 ; should use dseg:41d0
// /\/\/\ ? VGA, GUI, etc ? /\/\/\

SAMB_ptr p_VGAFILEH_Header;             // SM2LM    // dseg:A7D4
// SM2LM  
SAMB_addr sa_VGAFILEH_Header;

byte_ptr p_PaletteFlags;                // SM2LM    // dseg:A7D6
// SM2LM  
sgmt_addr sa_PaletteFlags;

byte_ptr p_ShadingColors;               // SM2LM    // dseg:A7D8    5,376 bytes (1500h) into the palette entry
// SM2LM  
sgmt_addr sa_ShadingColors;

SAMB_ptr p_ReplacementColors;           // SM2LM    // dseg:A7DA
// SM2LM  
SAMB_addr sa_ReplacementColors;

SAMB_ptr UU_p_PaletteSaved;                // SM2LM    // dseg:A7DC
// SM2LM  
SAMB_addr UU_sa_PaletteSaved;

byte_ptr p_Palette;                     // SM2LM    // dseg:A7DE

char font_name[16];                                 // dseg:A7E0    set in Load_Font_File

SAMB_ptr p_IntensityScaleTable;         // SM2LM    // dseg:A7F0
// SM2LM  
SAMB_addr sa_IntensityScaleTable;

SAMB_ptr sad1_PaletteLbxEntry;          // SM2LM    // dseg:A7F2
SAMB_ptr sah1_PaletteLbxEntry;          // SM2LM    // dseg:A7F4

//dseg:A7F6 VGA_AAColor_Array db 16 dup (0)
//dseg:A806 VGA_Current_AA_Color db 0
//dseg:A807 VGA_AA_Color_1 db 0
//dseg:A808 VGA_AA_Color_2 db 0
//dseg:A809 VGA_AA_Color_3 db 0
// unsigned int VGA_Font_ColorIndex3;               // dseg:A80A
// unsigned int VGA_Font_ColorIndex2;               // dseg:A80C
// unsigned int VGA_Font_ColorIndex1;               // dseg:A80E
// unsigned int VGA_Font_Index;                     // dseg:A810
//VGA_TextDraw_Y; // dseg:A812
//VGA_TextDraw_X; // dseg:A814

SAMB_ptr p_FontStyleData;               // SM2LM    // dseg:A816
// SM2LM  
SAMB_addr sa_FontStyleData;

SAMB_ptr p_BorderStyleData;             // SM2LM    // dseg:A818
// SM2LM  
SAMB_addr sa_BorderStyleData;

byte_ptr p_Cursor_Array;                // SM2LM    // dseg:A81A  ; 500h into the palette entry
// SM2LM  
sgmt_addr sa_Cursor_Array;

byte_ptr UU_p_Palette_Data;             // SM2LM     // dseg:A81C  ; 400h into the palette entry
// SM2LM  
sgmt_addr UU_sa_Palette_Data;

byte_ptr p_Palette_Font_Colors;         // SM2LM    // dseg:A81E  ; 300h into the palette entry, 16 arrays of 16 colors
// SM2LM  
sgmt_addr sa_Palette_Font_Colors;


// seg019.H  SAMB_ptr p_VGA_TextLine_Starts;        // dseg:A820
// seg019.H  SAMB_ptr p_VGA_TextLine_Tops;          // dseg:A824
// seg019.H  SAMB_ptr p_VGA_TextLine_Rights;        // dseg:A828
// seg019.H  SAMB_ptr p_VGA_TextLine_Lefts;         // dseg:A82C
// unsigned int gsa_VGA_TextDraw_Lines;             // dseg:A830

//dseg:A832 VGA_FltBlock_Bottoms dw 4 dup(0)   ; up to 4 bottom row positions used to reserve space
//dseg:A83A VGA_FltBlock_Rights dw 4 dup(0)    ; up to 4 right column positions used to reserve space
//dseg:A842 VGA_FltBlock_Tops dw 4 dup(0)      ; up to 4 top row positions used to reserve space
//dseg:A84A VGA_FltBlock_Lefts dw 4 dup(0)     ; up to 4 left column positions used to reserve space
//dseg:A852 VGA_Blink_Speed dw 0
//dseg:A854 VGA_BlinkHue_C1 dw 0                    ; the component of C1 that holds the defining hue
//dseg:A856 ; char gVGAFILEH_LBX_Name[]
//dseg:A856 gVGAFILEH_LBX_Name db 16 dup(0)
//dseg:A866 gVGAFILEH_LBX_Entry dw 0
