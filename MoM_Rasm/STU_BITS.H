#ifndef STU_BITS_H
#define STU_BITS_H

//#include "STU_TYPE.H"
#include "ST_HEAD.H"

/*
void FLIC_Draw_EMM_C(int ScreenPage_X, int ScreenPage_Y, unsigned int SAMB_data_FLIC_HDR, int Frame_Index) { ... }
...
byte _FAR * fp_FlicHeader;
fp_FlicHeader = (byte _FAR *)MK_FP(SAMB_data_FLIC_HDR, 0);
GET_WORD(fp_FliHead)
    fh_Width = fp_FlicHeader[0];                  // FlicHdr_Width                MAINSCRN_LBX_000,0: 320
    fh_EmmHandleNumber = fp_FlicHeader[10];       // FlicHdr_EmmHandleNumber      MAINSCRN_LBX_000,0: 6
    fh_EmmLogicalPageIndex = fp_FlicHeader[11];   // FlicHdr_EmmLogicalPageIndex  MAINSCRN_LBX_000,0: 0
    fh_EmmLogicalPageOffset = fp_FlicHeader[6];   // FlicHdr_EmmLogicalPageOffset MAINSCRN_LBX_000,0: 0x02C0  C0 02 00 00

?
#define lbxgfx_get_paloffs(_data_) GET_LE_16(&((_data_)[0x0e]))
J:\STU\DBWD\developc\1oom-master\src\bits.h
#define GET_LE_16(_p_) (*((uint16_t const *)(_p_)))
#define GET_LE_32(_p_) (*((uint32_t const *)(_p_)))

?
INCLUDE/DOS.H:#define peek( a,b )( *( (int  far* )MK_FP( (a ),( b )) ))
INCLUDE/DOS.H:#define peekb( a,b )( *( (char far* )MK_FP( (a ),( b )) ))
?
*/
// #define MK_FP(_sgmt_,_ofst_) ( (void _FAR *) ( (unsigned long) (_sgmt_) << 16 | (_ofst_) ) )
//#define FGET_WORD(_fp_) ( *((word const *)MK_FP(FP_SEG(_fp_),FP_OFF(_fp_))) )
//    fh_Width = *( (word  far* )MK_FP( SAMB_data_FLIC_HDR, 0x00 ));
//    fh_EmmHandleNumber = *( (byte  far* )MK_FP( SAMB_data_FLIC_HDR, 0x0A ));

#define FPGET_WORD(_fp_,_relofs_) ( *( (word _FAR *)MK_FP( FP_SEG(_fp_), FP_OFF(_fp)+_relofs_ ) ) )

#define FPEEKB( _sgmt_,_ofst_) ( *(( byte _FAR *)MK_FP((_sgmt_),(_ofst_))) )
#define FPEEKW( _sgmt_,_ofst_) ( *(( word _FAR *)MK_FP((_sgmt_),(_ofst_))) )
#define FPEEKDW(_sgmt_,_ofst_) ( *((dword _FAR *)MK_FP((_sgmt_),(_ofst_))) )

#define FPOKEB( _sgmt_,_ofst_,_val_) ( (*(( byte _FAR *)MK_FP((_sgmt_),(_ofst_)))) = (_val_) )
#define FPOKEW( _sgmt_,_ofst_,_val_) ( (*(( word _FAR *)MK_FP((_sgmt_),(_ofst_)))) = (_val_) )
#define FPOKEDW(_sgmt_,_ofst_,_val_) ( (*((dword _FAR *)MK_FP((_sgmt_),(_ofst_)))) = (_val_) )

#define GET_BYTE(_p_)  ( *((byte  const *)(_p_)) )
#define GET_WORD(_p_)  ( *((word  const *)(_p_)) )
#define GET_DWORD(_p_) ( *((dword const *)(_p_)) )

#define SET_BYTE(_p_,_v_)  ( (*((byte  *)(_p_))) = (_v_) )
#define SET_WORD(_p_,_v_)  ( (*((word  *)(_p_))) = (_v_) )
#define SET_DWORD(_p_,_v_) ( (*((dword *)(_p_))) = (_v_) )


#endif /* STU_BITS_H */
