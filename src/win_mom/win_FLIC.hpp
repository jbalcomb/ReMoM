#ifndef WIN32_FLIC_HPP
#define WIN32_FLIC_HPP

#include "win_TYPE.hpp"     /* byte_ptr */
#include "win_BITS.hpp"     /* GET_1B_OFS(), GET_2B_OFS(), GET_4B_OFS() */


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
#define FLIC_HDR_POS_REMAP                      0x11
#define FLIC_HDR_POS_FRAME_OFFSET_TABLE         0x12    // 18 bytes from BoF; 4-byte values for offsets from BoF to the FLIC Frame Data

#define FLIC_PAL_HDR_POS_PALETTE_DATA_OFFSET    0x00
#define FLIC_PAL_HDR_POS_COLOR_START            0x02
#define FLIC_PAL_HDR_POS_COLOR_COUNT            0x04
#define FLIC_PAL_HDR_POS_FRAME_PALETTES         0x06

#define FLIC_FRM_PAL_HDR_POS_PALETTE_DATA_OFFSET  0x00
#define FLIC_FRM_PAL_HDR_POS_COLOR_START        0x02
#define FLIC_FRM_PAL_HDR_POS_COLOR_COUNT        0x03

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
#define FLIC_GET_REMAP(_ptr_)                    ( GET_1B_OFS( (_ptr_), FLIC_HDR_POS_REMAP)                   )
// #define FLIC_GET_FRAME_OFFSET_TABLE(_ptr_)       ( GET_2B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_OFFSET_TABLE)      )
#define FLIC_GET_FRAME_OFFSET(_ptr_,_idx_)       ( GET_4B_OFS( (_ptr_), FLIC_HDR_POS_FRAME_OFFSET_TABLE + (4 * (_idx_)) )      )

#define FLIC_SET_WIDTH(_ptr_,_width_)                ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_WIDTH,         (_width_) ) )
#define FLIC_SET_HEIGHT(_ptr_,_height_)              ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_HEIGHT,        (_height_) ) )
#define FLIC_SET_CURRENT_FRAME(_ptr_,_frame_index_)  ( SET_2B_OFS( (_ptr_), FLIC_HDR_POS_CURRENT_FRAME, (_frame_index_) ) )
// #define FLIC_SET_FRAME_COUNT(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_FRAME_COUNT) )
// #define FLIC_SET_LOOP_FRAME(_ptr_) ( GET_2B_OFS(_ptr_,FLIC_HDR_POS_LOOP_FRAME) )
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



#endif /* WIN32_FLIC_HPP */
