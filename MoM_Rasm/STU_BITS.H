#ifndef STU_BITS_H
#define STU_BITS_H

#include "STU_TYPE.H"
#include "ST_DEF.H"

// #include "STU_BITS.H"



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

#define FPGET_WORD(_fp_,_relofs_) ( *( (word _FAR *) MK_FP( FP_SEG(_fp_), FP_OFF(_fp)+_relofs_ ) ) )

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
