/*
    WIZARDS.EXE
        seg030
        seg031
        seg032
        seg033
        seg034

seg030    
MoO2 Module: bitmap

seg033
MoO2 Module: bitmap
MoO2 Module: shear

*/

#ifndef FLIC_DRAW_H
#define FLIC_DRAW_H

#include "MoX_TYPE.H"
#include "MoX_BITS.H"

/*
"FRM"   "frame"
"HDR"   "header"
"LC"    "line compressed"  ~== Run-Length Encoded (RLE)
"PAL"   "palette"
"POS"   "position"
"SZ"    "size"
*/

struct s_FLIC_HDR
{
    /* 00 */ int16_t width;
    /* 02 */ int16_t height;
    /* 04 */ int16_t current_frame;
    /* 06 */ int16_t frame_count;
    /* 08 */ int16_t loop_frame;
    /* 0A */ int8_t emm_handle_number;
    /* 0B */ int8_t emm_logical_page_number;
    /* 0C */ uint16_t emm_logical_page_offset;
    /* 0E */ int16_t palette_header_offset;
};

#define SZ_FLIC_HDR                             16
#define SZ_FLIC_PAL_HDR                          8
#define SZ_FLIC_FRM_PAL_HDR                      4

#define FLIC_HDR_POS_WIDTH                      0x00
#define FLIC_HDR_POS_HEIGHT                     0x02
#define FLIC_HDR_POS_CURRENT_FRAME              0x04
#define FLIC_HDR_POS_FRAME_COUNT                0x06
#define FLIC_HDR_POS_LOOP_FRAME                 0x08
#define FLIC_HDR_POS_EMM_HANDLE_NUMBER          0x0A
#define FLIC_HDR_POS_EMM_LOGICAL_PAGE_NUMBER    0x0B
#define FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET    0x0C
#define FLIC_HDR_POS_PALETTE_HEADER_OFFSET      0x0E    // End of 16-byte FLIC Header / animation_header
#define FLIC_HDR_POS_FRAME_TYPE                 0x10
#define FLIC_HDR_POS_REMAP_FLAG                 0x11    // Remap Colors Flag
#define FLIC_HDR_POS_FRAME_OFFSET_TABLE         0x12    // 18 bytes from BoF; 4-byte values for offsets from BoF to the FLIC Frame Data

#define FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET    0x00
#define FLIC_PAL_HDR_POS_COLOR_START            0x02
#define FLIC_PAL_HDR_POS_COLOR_COUNT            0x04
#define FLIC_PAL_HDR_POS_FRAME_PALETTES         0x06

#define FLIC_FRM_PAL_HDR_POS_PALETTE_DATA_OFFSET    0x00
#define FLIC_FRM_PAL_HDR_POS_COLOR_START            0x02
#define FLIC_FRM_PAL_HDR_POS_COLOR_COUNT            0x03

/*
    BEGIN: GET/SET FLIC File

    FLIC_Get/Set            Offset from FLIC File / FLIC Header
    FLIC_PAL_Get/Set        Offset from FLIC File Palette Header
    FLIC_FRM_PAL_Get/Set    Offset from FLIC Frame Palette Header

    _ptr_ is (Far) Pointer to FLIC File (Header)
*/

// #define FLIC_LBX_Has_Palette(_ptr_)               ( FLIC_LBX_Get_Palette_Header_Offset(_ptr_) != 0 )
// #define FLIC_LBX_Has_Palette_PerFrame(_ptr_)      ( FLIC_LBX_Get_Palette_PerFrame_Flag(_ptr_) != 0 )

#define FLIC_GET_WIDTH(_ptr_)                    ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_WIDTH)                   )
#define FLIC_GET_HEIGHT(_ptr_)                   ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_HEIGHT)                  )
#define FLIC_GET_CURRENT_FRAME(_ptr_)            ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_CURRENT_FRAME)           )
#define FLIC_GET_FRAME_COUNT(_ptr_)              ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_COUNT)             )
#define FLIC_GET_LOOP_FRAME(_ptr_)               ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_LOOP_FRAME)              )
#define FLIC_GET_EMM_HANDLE_NUMBER(_ptr_)        ( GET_1B_OFS( (_ptr_), FLIC_HDR_POS_EMM_HANDLE_NUMBER)       )
#define FLIC_GET_EMM_LOGICAL_PAGE_NUMBER(_ptr_)  ( GET_1B_OFS( (_ptr_), FLIC_HDR_POS_EMM_LOGICAL_PAGE_NUMBER) )
#define FLIC_GET_EMM_LOGICAL_PAGE_OFFSET(_ptr_)  ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET) )
#define FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_)    ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_PALETTE_HEADER_OFFSET)   )
#define FLIC_GET_FRAME_TYPE(_ptr_)               ( GET_1B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_TYPE)              )
#define FLIC_GET_REMAP_FLAG(_ptr_)               ( GET_1B_OFS( (_ptr_), FLIC_HDR_POS_REMAP_FLAG)              )
// #define FLIC_GET_FRAME_OFFSET_TABLE(_ptr_)       ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_OFFSET_TABLE)      )
#define FLIC_GET_FRAME_OFFSET(_ptr_,_idx_)       ( GET_4B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_OFFSET_TABLE + (4 * (_idx_)) )      )

#define FLIC_SET_WIDTH(_ptr_,_width_)                ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_WIDTH,         (_width_)       ) )
#define FLIC_SET_HEIGHT(_ptr_,_height_)              ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_HEIGHT,        (_height_)      ) )
#define FLIC_SET_CURRENT_FRAME(_ptr_,_frame_index_)  ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_CURRENT_FRAME, (_frame_index_) ) )
// #define FLIC_SET_FRAME_COUNT(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_FRAME_COUNT) )
#define FLIC_SET_LOOP_FRAME(_ptr_,_frame_index_)     ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_LOOP_FRAME,    (_frame_index_) ) )
// #define FLIC_SET_EMM_HANDLE_NUMBER(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_HDR_POS_EMM_HANDLE_NUMBER) )
// #define FLIC_SET_EMM_LOGICAL_PAGE_NUMBER(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_HDR_POS_EMM_LOGICAL_PAGE_NUMBER) )
// #define FLIC_SET_EMM_LOGICAL_PAGE_OFFSET(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_EMM_LOGICAL_PAGE_OFFSET) )
// #define FLIC_SET_PALETTE_HEADER_OFFSET(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_PALETTE_HEADER_OFFSET) )
// #define FLIC_SET_FRAME_TYPE(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_HDR_POS_FRAME_TYPE) )
// #define FLIC_SET_SHADING(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_HDR_POS_REMAP) )
// #define FLIC_SET_FRAME_OFFSET_TABLE(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_FRAME_OFFSET_TABLE) )
// #define FLIC_SET_PALETTE_DATA_OFFSET(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET) )
// #define FLIC_SET_COLOR_START(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_COLOR_START) )
// #define FLIC_SET_COLOR_COUNT(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_PAL_HDR_POS_COLOR_COUNT) )
// #define FLIC_SET_FRAME_PALETTES(_ptr_) ( GET_1B_OFS(_ptr_,FLIC_PAL_HDR_POS_FRAME_PALETTES) )


#define FLIC_GET_PALETTE_DATA_OFFSET(_ptr_)   ( GET_2B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET) ) )
#define FLIC_GET_PALETTE_COLOR_START(_ptr_)   ( GET_2B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + FLIC_PAL_HDR_POS_COLOR_START)         ) )
#define FLIC_GET_PALETTE_COLOR_COUNT(_ptr_)   ( GET_2B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + FLIC_PAL_HDR_POS_COLOR_COUNT)         ) )
#define FLIC_GET_FRAME_PALETTES(_ptr_)        ( GET_2B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + FLIC_PAL_HDR_POS_FRAME_PALETTES)      ) )

// #define FLIC_PAL_GET_DATA_OFFSET(_ptr_)          ( GET_2B_OFS( (_ptr_), FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET) )
// #define FLIC_PAL_GET_COLOR_START(_ptr_)          ( GET_2B_OFS( (_ptr_), FLIC_PAL_HDR_POS_COLOR_START)         )
// #define FLIC_PAL_GET_COLOR_COUNT(_ptr_)          ( GET_2B_OFS( (_ptr_), FLIC_PAL_HDR_POS_COLOR_COUNT)         )
// #define FLIC_PAL_GET_FRAME_PALETTES(_ptr_)       ( GET_1B_OFS( (_ptr_), FLIC_PAL_HDR_POS_FRAME_PALETTES)      )

// frame_palette_header_offset = palette_header_offset + 8 + (4 * (frame_index - 1));

#define FLIC_GET_FRAME_PALETTE_DATA_OFFSET(_ptr_,_idx_)   ( GET_2B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + SZ_FLIC_PAL_HDR + (4 * ((_idx_) - 1)) + FLIC_FRM_PAL_HDR_POS_PALETTE_DATA_OFFSET) ) )
#define FLIC_GET_FRAME_PALETTE_COLOR_INDEX(_ptr_,_idx_)   ( GET_1B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + SZ_FLIC_PAL_HDR + (4 * ((_idx_) - 1)) + FLIC_FRM_PAL_HDR_POS_COLOR_START ) ) )
#define FLIC_GET_FRAME_PALETTE_COLOR_COUNT(_ptr_,_idx_)   ( GET_1B_OFS( (_ptr_), (FLIC_GET_PALETTE_HEADER_OFFSET(_ptr_) + SZ_FLIC_PAL_HDR + (4 * ((_idx_) - 1)) + FLIC_FRM_PAL_HDR_POS_COLOR_COUNT ) ) )

// #define FLIC_FRM_PAL_GET_DATA_OFFSET(_ptr_)  ( GET_2B_OFS( (_ptr_), FLIC_FRAME_PAL_HDR_POS_PALETTE_DATA_OFFSET) )
// #define FLIC_FRM_PAL_GET_COLOR_START(_ptr_)  ( GET_1B_OFS( (_ptr_), FLIC_FRAME_PAL_HDR_POS_COLOR_START)         )
// #define FLIC_FRM_PAL_GET_COLOR_COUNT(_ptr_)  ( GET_1B_OFS( (_ptr_), FLIC_FRAME_PAL_HDR_POS_COLOR_COUNT)         )


/*
    END: GET/SET FLIC File
*/



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    WZD  seg021
*/

// WZD s21p07
void FLIC_Load_Palette(SAMB_ptr p_FLIC_Header, int16_t frame_index);


/*
    WZD seg024
*/
// WZD s24p08
void Screen_Picture_Capture(int16_t x1, int16_t y1, int16_t x2, int16_t y2, SAMB_ptr pict_seg);

// WZD s24p09
void Capture_Screen_Block(SAMB_ptr pict_seg, int16_t x1, int16_t y1, int16_t x2, int16_t y2);



/*
    WZD  seg029
*/

// WZD s29p01
void FLIC_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data);

// WZD s29p02
// MoO2  Module: animate  Remap_Draw() |-> Remap_Draw_Animated_Sprite(); ... Module: remap  unsigned int picture_remap_color_list[256]  Address: 02:001B479C
void FLIC_Remap_Draw_Frame(int16_t x_start, int16_t y_start, int16_t width, byte_ptr frame_data);


/*
    WZD  seg030
*/

// WZD s30p01
// LBX_Image_Copy

// WZD s30p02
void Create_Picture(int16_t width, int16_t height, byte_ptr pict_seg);

// WZD s30p03
// LBX_IMG_VShiftRect
// WZD s30p04
// LBX_IMG_VertWarp
// WZD s30p05
// LBX_IMG_HorzWarp

// WZD s30p06
void Create_Blank_Picture(int16_t width, int16_t height, byte_ptr pict_seg, uint8_t color);

// WZD s30p07
// UU_LBX_IMG_ColorShift
// WZD s30p08
// UU_LBX_IMG_CReplaceRect

// WZD s30p09
// drake178: LBX_IMG_ClearGraphic()
// MoO2  Module: replace  Replace_Color_All()
void Replace_Color_All(SAMB_ptr picture, uint8_t replacement_color);

// WZD s30p10
// drake178: LBX_IMG_LoadPalette()
void Load_Palette_From_Animation(SAMB_ptr picture);


// WZD s30p11
void FLIC_Draw(int16_t x_start, int16_t y_start, SAMB_ptr p_FLIC_File);

// WZD s30p12
void Clipped_Draw(int16_t x, int16_t y, SAMB_ptr picture);


// WZD s30p13
// AKA LBX_IMG_BuildFrame();
void Draw_Picture_To_Bitmap(SAMB_ptr src_pict_seg, SAMB_ptr dst_pict_seg);

// WZD s30p14
void FLIC_Set_CurrentFrame(SAMB_ptr p_FLIC_Header, int16_t frame_index);

// WZD s30p15
void FLIC_Reset_CurrentFrame(SAMB_ptr p_FLIC_Header);

// WZD s30p16
int16_t FLIC_Get_CurrentFrame(SAMB_ptr p_FLIC_Header);

// WZD s30p17
int16_t FLIC_Get_FrameCount(SAMB_ptr p_FLIC_Header);

// WZD s30p18
int16_t FLIC_Get_Width(SAMB_ptr p_FLIC_Header);

// WZD s30p19
int16_t FLIC_Get_Height(SAMB_ptr p_FLIC_Header);


// WZD s30p24
void Draw_Picture(int16_t x, int16_t y, byte_ptr pict_seg);

// WZD s30p25
void Draw_Picture_Windowed(int16_t x, int16_t y, byte_ptr pict_seg);

// WZD s30p40
// MoO2  Get_Full_Store_Flag()
int16_t Get_Full_Store_Flag(SAMB_ptr p_FLIC_Header);

// WZD s30p41
// drake178: LBX_IMG_SetLoop1
void FLIC_Set_LoopFrame_1(SAMB_ptr p_FLIC_Header);

// WZD s30p42
// drake178: LBX_IMG_OutlineOvr()
// MoO2: Module: bitmap  Outline_Bitmap() |-> Outline_Bitmap_Pixels_()
// checks transparent, outline color  (NOT > 223)
void Outline_Bitmap_Pixels(SAMB_ptr pict_seg, uint8_t outline_color);

// WZD s30p43
// drake178: LBX_IMG_OutlineOvr_R()
// MoO2: Module: bitmap  Outline_Bitmap() |-> Outline_Bitmap_Pixels_No_Glass_()
// checks transparent, outline color, and > 223
void Outline_Bitmap_Pixels_No_Glass(SAMB_ptr pict_seg, uint8_t outline_color);

// WZD s30p44
// drake178: LBX_IMG_ColorScrmblr
// MoO2: Bitmap_Aura() |-> Bitmap_Aura_Pixels_()
void Bitmap_Aura_Pixels(SAMB_ptr pict_seg, uint8_t color, uint8_t * color_list);

// WZD s30p45
// drake178: UU_LBX_IMG_DrawRect()

// WZD s30p46
// drake178: LBX_IMG_GetGFXSize()
void Get_Bitmap_Actual_Size(SAMB_ptr bitmap_addr, int16_t * x1, int16_t * y1, int16_t * width, int16_t * height);



/*
    WIZARDS.EXE  seg031
*/

// WZD s31p01
// drake178: VGA_DrawPartialImage()
void Clipped_Draw_Frame(int16_t x1, int16_t y1, int16_t width, int16_t height, int16_t skip_x, int16_t skip_y, byte_ptr frame_data);

// WZD s31p02
// drake178: VGA_DrawPartImage_R()
// MoO2  Module:   Remap_Clipped_Draw_Animated_Sprite()
void Clipped_Remap_Draw_Frame(int16_t x1, int16_t y1, int16_t width, int16_t height, int16_t skip_x, int16_t skip_y, byte_ptr frame_data);


/*
    WIZARDS.EXE  seg032
*/

// WZD s32p01
// drake178: VGA_DrawPartEmsImg()

// WZD s32p02
// drake178: EMM_MapNextIMGPages()

// WZD s32p03
// drake178: VGA_DrawPartEmsImg_R()



/*
    WIZARDS.EXE  seg033
*/

// WZD s33p01
// AKA LBX_IMG_DecPrepare()
void Create_Multi_Frame_Blank_Bitmap(SAMB_ptr src_pict_seg, SAMB_ptr dst_pict_seg, int16_t frame_idx);

// WZD s33p02
// AKA LBX_IMB_LoadFrame()
void Add_Picture_To_Bitmap(byte_ptr source_picture, byte_ptr destination_bitmap);

// WZD s33p03

// WZD s33p04

// WZD s33p05
void Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x);

// WZD s33p06
void Remap_Draw_Picture_ASM(int16_t x_start, int16_t y_start, int16_t ofst, byte_ptr pict_seg, int16_t width, int16_t height, int16_t skip_x);

// WZD s33p07

// WZD s33p08

// WZD s33p09
// AKA FLIC_Remap_Color()
// drake178: LBX_IMG_ColorReplace
// MoO2:  Replace_Color()
void Replace_Color(SAMB_ptr pict_seg, uint8_t color_to_replace, uint8_t replacement_color);

// WZD s33p10
// WZD s33p11
// WZD s33p12
// WZD s33p13
// WZD s33p14

// WZD s33p15
// drak178: LBX_IMG_RandomDelete()
// MoO2  Module: bitmap  Vanish_Bitmap()
// MoO2  Moudle: shear   Vanish_Bitmap_Pixels_()
void Vanish_Bitmap(SAMB_ptr bitmap, int16_t percent);


// WZD s33p16
// drake178: LBX_IMG_Grayscale()
// MoO2: Gray_Scale_Bitmap()
void Gray_Scale_Bitmap(SAMB_ptr pict_seg, int16_t color_start);

// WZD s33p17

// WZD s33p18

// WZD s33p19
/*
drake178:
    a secondary linear feedback shift register using the
    same feedback polynomial as RNG_Random, also shifted
    9 states per call, but actually returning the lowest
    order 10 bits of the resulting state (1-1023) rather
    than the shifted out return value
*/
int16_t RNG_GFX_Random(int16_t max);




#ifdef __cplusplus
}
#endif



#endif /* FLIC_DRAW_H */
