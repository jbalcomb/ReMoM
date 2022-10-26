#ifndef STU_BITS_H
#define STU_BITS_H

#include "STU_TYPE.H"
#include "ST_DEF.H"     /* MK_FP() */


// NOTE(JimBalcomb,20221017): SHOULD BE Not-In-Use (NIU); this has become a rag-tag pile of scraps; it's mostly in ST_DEF now, but 'See Also' BitsBites.md in the docs;







// J:\STU\DBWD\developc\_MPL13\SAMPCODE\C\OTHER\MEMORY\MOVEMEM.C
// #pragma pack(1)     /* Use pragma to force packing on byte boundaries. */



// J:\STU-EduMat\DOS\Microsoft Programmers Library 1.3 (199103)\Microsoft Programmers Library 1.3 (March 1991) (ISO)\Extracted Book Text\msc\mscsampl.txt
// ? fun for funsies ?
#define LO_NIBBLE        0x0F
#define HI_NIBBLE        0xF0



// J:\STU\DBWD\BORLANDC\CRTL\RTLINC\ASMRULES.H
/* Use these macros when you want to return a value in DX:AX and
   avoid a warning messages being generated.  Ex: return( MK_LONG ); */

#ifdef MK_LONG
#undef MK_LONG                                                           
#endif                                                                  
#ifdef MK_ULONG
#undef MK_ULONG                                                           
#endif                                                                  

#define MK_LONG  (  signed long)( (void _seg *)(_DX) + (void near *)(_AX) )
#define MK_ULONG (unsigned long)( (void _seg *)(_DX) + (void near *)(_AX) )

// // J:\STU\DBWD\BORLANDC\INCLUDE\DOS.H
// #define MK_FP(seg,ofs) ( (void _seg *)(seg) + (void near *)(ofs) )
// #define FP_SEG(fp) ( (unsigned int)(void _seg *)(void far *)(fp))
// #define FP_OFF(fp) ( (unsigned int)(fp) )

// DELETE #ifdef MK_FP
// DELETE #undef MK_FP                                                           
// DELETE #endif                                                                  
// DELETE #ifdef FP_SEG
// DELETE #undef FP_SEG                                                           
// DELETE #endif                                                                  
// DELETE #ifdef FP_OFF
// DELETE #undef FP_OFF                                                           
// DELETE #endif                                                                  

// // // #define MK_FP(sgmt,ofst) ( (void _SEG *)(sgmt) + (void _NEAR *)(ofst) )
// #define MK_FP(seg,ofs) ( (void _seg *)(seg) + (void near *)(ofs) )
// // #define MK_FP(_sgmt_,_ofst_) ( (void _FAR *) (  (unsigned long) (_sgmt_) << 16  | (_ofst_) ) )
// // #define MK_FP(_sgmt_,_ofst_) ( (void  far *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )
#define ST_MK_FP(_sgmt_,_ofst_) ( (void far *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )


// // #define FP_SEG(fp) ( (unsigned int)(void _SEG *)(void _FAR *)(fp) )
// #define FP_SEG(fp) ( (unsigned int)(void _seg *)(void far *)(fp) )
// #define FP_OFF(fp) ( (unsigned int)(fp) )

// "The [segment address] ... is simply the upper 16 bits of a far pointer." - PC Mag, 19880126
// FarPtr = (char far *) ( (long) segment_address << 16 );
// https://forum.arduino.cc/t/long-to-two-ints/665091/6
// (2) Take out upper 16-bit (the high16) this way:
// (a) Shift total32 to the right by 16-bit positions and place the new value in y.
// (b) Replace upper 16-bit of the above valu by 0s. They will be automatically 0s due to shift.
// (c) Keep lower 16-bit using cast.
// ~== unsigned int SgmtAddr = (unsigned int) ( (unsigned long) (FarPtr >> 16) );

// #define FP_SGMT(_fp_) ( (unsigned int) ( (unsigned long)((void _FAR *)(_fp_)) >> 16) )

#define ST_FP_SEG(_fp_) ( (unsigned int) ( (unsigned long)((void *)(_fp_)) >> 16) )

#define ST_FP_OFF(_fp_) ( (unsigned int) (_fp_) )


//#define MK_HP(sgmt,ofst) ( (void _seg * )( seg ) +( void near * )( ofs ))
// J:\STU\DBWD\developc\AIL214\AIL.MAC
// FAR_TO_HUGE     MACRO fp_seg,fp_off             ;normalize far pointer
// ADD_PTR         MACRO add_l,add_h,pseg,poff     ;add 32-bit dword to far ptr


// https://groups.google.com/g/comp.os.msdos.programmer/c/zUC_3ZLs_Hs
// Microsoft C provides FP_SEG and FP_OFF or _FP_SEG and _FP_OFF (new version)
//   that take a far pointer as an argument and allow the segment and offset to be set or extracted.
// I'm not sure when the leading underscore was added. C 5.1 did not have it. C 7.0 did have.
// ...
// You can also make MK_FP yourself:
// #define MK_FP (seg,ofs) ((DWORD)seg <<16 + (DWORD)ofs)
// DWORD is defined in AFX.H: it's an unsigned 32 bit integer type
// (unsigned long probably).
// ...
// A simpler, and more portable (no AFX.H file needed, whatever that is) follows:
// #if !defined(MK_FP)
// #define MK_FP(seg,ofs) ( (void far *) (((unsigned long)(seg) | (unsigned)(ofs)) )
// #endif
// As an aside, it is generally safer to use macro arguments in parens.
// If you are worried about porting to a different DOS compiler, put the above in a #ifdef _MSC_VER ... #endif block.
// ...
// #define MK_FP( s,o ) ( (void far*) ((unsigned long) (o) | ((unsigned long) (s)<<16)) )
// ...
// Still not right. Here's a better one:
// #if !defined(MK_FP)
// #define MK_FP(seg,ofs) ((void far *)(((unsigned long)(seg) << 16) | (ofs)))
// #endif



// https://books.google.com/books?id=hEaMhncO06EC&pg=PT259&lpg=PT259&dq=C+code+shift+get+segment+address+far+pointer&source=bl&ots=76nQup67Ge&sig=ACfU3U2_S5jOtmPfkpnsjIWIcDTGqu5y-A&hl=en&sa=X&ved=2ahUKEwjLpIv55oX6AhXoElkFHcACBsIQ6AF6BAgvEAM#v=onepage&q=C%20code%20shift%20get%20segment%20address%20far%20pointer&f=false
// PC Mag 19890912
//    char far * ginfo;
//    DosGetInfoSeg(&gseg, &lseg);       // get info segment selectors
//    (long) ginfo = (long) gseg << 16;  // make far pointer





// // J:\STU\DBWD\devel\Rasm_MoM\POCEMS\PCINTERN\EMMC.C
// /*== Macros ==========================================================*/
                                                                        
// /*-- MK_FP creates a FAR pointer out of segment and offset addresses -*/ 
// /*-- to on objetc                                              -------*/  
                                                                         
// #ifdef MK_FP                                  /* is MK_FP defined yet */  
//  #undef MK_FP                                                           
// #endif                                                                  
// #define MK_FP(seg, ofs) ((void far *) ((unsigned long) (seg)<<16|(ofs)))

// /*-- PAGE_ADR returns a pointer to the physical page X within the ----*/
// /*-- page frame of the EMS memory.                                ----*/
                                                                        
// #define PAGE_ADR(x) ((void *) MK_FP(ems_frame_seg() + ((x) << 10), 0))  


// J:\STU\DBWD\BORLANDC\TVISION\INCLUDE\DRAWBUF.H
// #define loByte(w)    (((uchar *)&w)[0])
// #define hiByte(w)    (((uchar *)&w)[1])








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

#define FPGET_WORD(_fp_,_relofs_) ( *( (word _FAR *) MK_FP( FP_SEG(_fp_), FP_OFF(_fp_)+_relofs_ ) ) )

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

// J:\STU\DBWD\developc\1oom-master\src\bits.h
#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
#define GET_LE_24(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)*(((uint8_t const *)(_p_)) + 2)) << 16))
#define GET_LE_32(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)GET_LE_16((_p_) + 2)) << 16))
#define SET_LE_16(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu))
#define SET_LE_32(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))




// Eh? Pointer vs. Address? 1oom uses address
// #define GET_LE_16(_ptr_) ( (_ptr_[1] <<  8) |  _ptr_[0] )
// #define GET_LE_32(_ptr_) ( (_ptr_[3] << 24) | (_ptr_[2] << 16) | (_ptr_[1] << 8) | _ptr_[0] )
// ((unsigned int)((unsigned char *)_addr_)[1]) ... cast the address to a byte pointer, index the value, cast the value to the desired data type/size, left-shift into position
// #define GET_LE_16(_addr_) ( (((unsigned int)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned int)((unsigned char *)(_addr_))[0]) )
// #define GET_LE_32(_addr_) ( (((unsigned long)((unsigned char *)(_addr_))[3]) << 24) | (((unsigned long)((unsigned char *)(_addr_))[2]) << 16) | (((unsigned long)((unsigned char *)(_addr_))[1]) <<  8) |  ((unsigned long)((unsigned char *)(_addr_))[0]) )
// // #define FPGET_WORD(_fp_,_rel_) ( *( (word _FAR *) MK_FP( FP_SEG(_fp_), (FP_OFF(_fp_) + (_rel_)) ) ) )

// //#ifdef __DOS16__
// ? Nein, danke. ? #define FPGET_WORD(_fp_) ( *((word _FAR *)MK_FP(FP_SEG(_fp_), FP_OFF(_fp_))) )

#define GET_LE_16(_ptr_) ( (((unsigned int)(((unsigned char _FAR *)(_ptr_))[1])) << 8) | ((unsigned int)(((unsigned char _FAR *)(_ptr_))[0])) )

#define GET_LE_32(_ptr_) ( (((unsigned long)((unsigned char _FAR *)(_ptr_))[3]) << 24) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[2]) << 16) | (((unsigned long)((unsigned char _FAR *)(_ptr_))[1]) <<  8) |  ((unsigned long)((unsigned char _FAR *)(_ptr_))[0]) )

// //#endif

// // J:\STU\DBWD\developc\1oom-master\src\bits.h
// #define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// #define GET_LE_24(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)*(((uint8_t const *)(_p_)) + 2)) << 16))
// #define GET_LE_32(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)GET_LE_16((_p_) + 2)) << 16))
// #define SET_LE_16(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu))
// #define SET_LE_32(_p_,_v_) (((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))


// // cast _ptr_ to (unsigned char *), index/add offset, set low byte of value with value and high byte cleared, set high byte of value with high byte shifted low and high byte cleared
// #define SET_2B(_ptr_,_val_) ( *((_ptr_) + 1) = _val_ >> 8 )
// // ( (uint8_t *) (_p_) )[0] = ((_v_) & 0xffu )
// // ( (uint8_t *) (_p_) )[1] = (((_v_) >> 8) & 0xffu )
// // `_val_ >> 8` vs. `(_val_) >> 8` :: protect _val_ from macro expansion operator precence issues
// // `_val_ >> 8` vs. `(_val_ >> 8) & 0xFF` :: explcitly clear the high byte AFTER the shift ? because sign-bit or 1's complement platforms ?


/*
    HOW-TO: Access multi-byte values through a (unsigned char far *)

    unsigned char far * src;
    unsigned char baito;
    baito = *src;
    unsigned char far * src;
    unsigned int wordo;
    wordo = (unsigned int *)src;


void lbxgfx_apply_palette(uint8_t *data)
{
    if (lbxgfx_has_palette(data)) {
        uint8_t *p = lbxgfx_get_palptr(data);
        int first = lbxgfx_get_palfirst(data);
        int num = lbxgfx_get_palnum(data);
        lbxpal_set_palette(p, first, num);
    }
}
#define lbxgfx_get_paloffs(_data_) GET_LE_16(&((_data_)[0x0e]))
#define lbxgfx_get_paldataoffs(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00]))
#define lbxgfx_get_palfirst(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x02]))
#define lbxgfx_get_palnum(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x04]))
#define lbxgfx_get_palptr(_data_) ((_data_) + lbxgfx_get_paldataoffs(_data_))
...
#define lbxgfx_get_palptr(_data_) ( (_data_) + lbxgfx_get_paldataoffs(_data_))
#define lbxgfx_get_palptr(_data_) ( (_data_) + GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00])) )
#define lbxgfx_get_palptr(_data_) ( (_data_) + GET_LE_16(&( (_data_)[ GET_LE_16(&((_data_)[0x0e])) + 0x00 ] )) )
So, ...
    lbxgfx_get_palptr takes a pointer and returns a pointer
    lbxgfx_get_paldataoffs takes a pointer and returns an unsigned integer
    GET_LE_16 takes an address and returns an unsigned integer
    GET_LE_16 takes an address, casts it to a pointer, dereferences it to get a value, and then casts that value to an unsigned integer

e.g.,
0xAD 0xFE
LBX_Header
LBX_Header[2] == 0xAD
LBX_Header[3] == 0xFE
∵ Little-Endian ∴ (unsigned int) LBX_Header[2] == 0xADFE
0xFEAD == LBX_Header[3], LBX_Header[2]
unsigned int is 2 bytes
LBX_Header[2] == 0xAD == 10101101
LBX_Header[3] == 0xFE == 11111110
unsigned int
00000000 00000000
(unsigned char) LBX_Header[2] == 10101101
(unsigned char) LBX_Header[3] == 11111110
(unsigned int)  LBX_Header[3] == 00000000 11111110
((unsigned int)  LBX_Header[3]) << 8 == 11111110 00000000
(((unsigned int)  LBX_Header[3]) << 8) | ((unsigned char) LBX_Header[2]) == 11111110 10101101
...
byte pointer to LBX_Header
get byte at index 3
explicitly cast 1-byte value to 2-byte value
left-shift the low-byte into the high-byte
get byte at index 2
OR the low-byte of the 2-byte value with the byte at index 2  ( 00000000 | 10101101 == 10101101)

unsigned int(b[1])<<8 | unsigned int(b[0])

#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
...
static int GetLE16(const uint8_t* const data) {
  return (data[0] << 0) | (data[1] << 8);
}
...
unsigned int GET_LE_16( unsigned char * fp_Data)
{
    return ( (fp_Data[1] << 8) | fp_Data[0] );
}
#define GET_LE_16(_ptr_) ( (_ptr_[1] <<  8) |  _ptr_[0] )
#define GET_LE_24(_ptr_) ( (_ptr_[2] << 16) | (_ptr_[1] <<  8) |  _ptr_[0] )
#define GET_LE_32(_ptr_) ( (_ptr_[3] << 24) | (_ptr_[2] << 16) | (_ptr_[1] << 8) | _ptr_[0] )
Eh? GTG?!?




s21p07  FLIC_LoadPalette()
fptr_Src = (unsigned int *)MK_FP(SrcSgmt, SrcOfst);
fh_palette_data_offset = fptr_Src[0];  // Offset to FLIC Palette Data

s20p01  VGA_LoadPalette()
fptr_DstByte = (unsigned char *)MK_FP(DstSgmt, DstOfst);
fptr_SrcByte = (unsigned char *)MK_FP(SrcSgmt, SrcOfst);
for(itr_Color_Count = 0; itr_Color_Count < (Color_Count * 3); itr_Color_Count++)
    *fptr_DstByte++ = *fptr_SrcByte++;

*/

// ST_SA.C
// //#define SET_LE_16(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu) )
// //#define SET_LE_32(_p_,_v_) ( ((uint8_t *)(_p_))[0] = ((_v_) & 0xffu), ((uint8_t *)(_p_))[1] = (((_v_) >> 8) & 0xffu), ((uint8_t *)(_p_))[2] = (((_v_) >> 16) & 0xffu), ((uint8_t *)(_p_))[3] = (((_v_) >> 24) & 0xffu))

// //LBX_EntryCount = farpeekw(LBX_Header, 0);
// // LBX_EntryCount = *((unsigned int *)LBX_Header + 0);
// // //#define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// // LBX_EntryCount = (((uint16_t)(((uint8_t const *)(LBX_Header))[0])) | (((uint16_t)(((uint8_t const *)(LBX_Header))[1])) << 8));
// LBX_EntryCount = ((unsigned int)*((unsigned char *)LBX_Header + 0)) | (unsigned int)((unsigned int)*((unsigned char *)LBX_Header + 1) << 8);

// #define GET_LE_16(_p_) (((uint16_t)(((uint8_t const *)(_p_))[0])) | (((uint16_t)(((uint8_t const *)(_p_))[1])) << 8))
// #define GET_LE_32(_p_) (((uint32_t)GET_LE_16((_p_))) | (((uint32_t)GET_LE_16((_p_) + 2)) << 16))
// #define lbxgfx_get_w(_data_) GET_LE_16(&((_data_)[0x00]))
// #define lbxgfx_get_h(_data_) GET_LE_16(&((_data_)[0x02]))
// #define lbxgfx_get_frame(_data_) GET_LE_16(&((_data_)[0x04]))
// #define lbxgfx_set_frame(_data_, _v_) SET_LE_16(&((_data_)[0x04]), (_v_))
// #define lbxgfx_set_frame_0(_data_) lbxgfx_set_frame((_data_), 0)
// #define lbxgfx_get_frames(_data_) GET_LE_16(&((_data_)[0x06]))
// #define lbxgfx_get_frames2(_data_) GET_LE_16(&((_data_)[0x08]))
// #define lbxgfx_get_ehandle(_data_) ((_data_)[0x0a])
// #define lbxgfx_get_epage(_data_) ((_data_)[0x0b])
// #define lbxgfx_set_epage(_data_, _v_) ((_data_)[0x0b] = (_v_))
// #define lbxgfx_get_offs0c(_data_) GET_LE_16(&((_data_)[0x0c]))
// #define lbxgfx_get_paloffs(_data_) GET_LE_16(&((_data_)[0x0e]))
// #define lbxgfx_has_palette(_data_) (lbxgfx_get_paloffs(_data_) != 0)
// #define lbxgfx_get_indep(_data_) ((_data_)[0x10])
// #define lbxgfx_get_format(_data_) ((_data_)[0x11])
// #define lbxgfx_get_paldataoffs(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x00]))
// #define lbxgfx_get_palfirst(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x02]))
// #define lbxgfx_get_palnum(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x04]))
// #define lbxgfx_get_paloffs06(_data_) GET_LE_16(&((_data_)[lbxgfx_get_paloffs(_data_)+0x06]))
// #define lbxgfx_get_palptr(_data_) ((_data_) + lbxgfx_get_paldataoffs(_data_))
// #define lbxgfx_get_frameoffs0(_data_, _frame_) (GET_LE_32(&((_data_)[0x12 + 4 * (_frame_)])))
// #define lbxgfx_get_frameoffs1(_data_, _frame_) (GET_LE_32(&((_data_)[0x16 + 4 * (_frame_)])))
// #define lbxgfx_get_frameptr(_data_, _frame_) ((_data_) + lbxgfx_get_frameoffs0((_data_), (_frame_)))
// #define lbxgfx_get_frameclearflag(_data_, _frame_) (*lbxgfx_get_frameptr((_data_), (_frame_)))









// https://chromium.googlesource.com/webm/libwebp/+/v0.6.1-rc2/examples/webpinfo.c
// / -----------------------------------------------------------------------------
// // Data reading.
// static int GetLE16(const uint8_t* const data) {
//   return (data[0] << 0) | (data[1] << 8);
// }
// static int GetLE24(const uint8_t* const data) {
//   return GetLE16(data) | (data[2] << 16);
// }
// static uint32_t GetLE32(const uint8_t* const data) {
//   return GetLE16(data) | ((uint32_t)GetLE16(data + 2) << 16);
// }
// static int ReadLE16(const uint8_t** data) {
//   const int val = GetLE16(*data);
//   *data += 2;
//   return val;
// }
// static int ReadLE24(const uint8_t** data) {
//   const int val = GetLE24(*data);
//   *data += 3;
//   return val;
// }
// static uint32_t ReadLE32(const uint8_t** data) {
//   const uint32_t val = GetLE32(*data);
//   *data += 4;
//   return val;
// }

// https://pkg.go.dev/go.fuchsia.dev/fuchsia/src/lib/thinfs/bitops
// func GetLE16 ¶
// func GetLE16(b []byte) uint16
// GetLE16 gets a 16 bit little endian number from memory.
// func GetLE32 ¶
// func GetLE32(b []byte) uint32
// GetLE32 gets a 32 bit little endian number from memory.
// func PutLE16 ¶
// func PutLE16(b []byte, v uint16)
// PutLE16 puts a 16 bit little endian number into memory.
// func PutLE32 ¶
// func PutLE32(b []byte, v uint32)
// PutLE32 puts a 32 bit little endian number into memory.
// |-> https://fuchsia.googlesource.com/fuchsia/+/220857068aaf/src/lib/thinfs/bitops/little_endian.go#8
// package bitops
// // GetLE16 gets a 16 bit little endian number from memory.
// func GetLE16(b []byte) uint16 {
// 	return uint16(b[1])<<8 | uint16(b[0])
// }
// // GetLE32 gets a 32 bit little endian number from memory.
// func GetLE32(b []byte) uint32 {
// 	return uint32(b[3])<<24 | uint32(b[2])<<16 | uint32(b[1])<<8 | uint32(b[0])
// }
// // PutLE16 puts a 16 bit little endian number into memory.
// func PutLE16(b []byte, v uint16) {
// 	b[0] = byte(v)
// 	b[1] = byte(v >> 8)
// }
// // PutLE32 puts a 32 bit little endian number into memory.
// func PutLE32(b []byte, v uint32) {
// 	b[0] = byte(v)
// 	b[1] = byte(v >> 8)
// 	b[2] = byte(v >> 16)
// 	b[3] = byte(v >> 24)
// }




#endif /* STU_BITS_H */
