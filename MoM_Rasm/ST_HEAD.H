#ifndef ST_HEAD_H
#define ST_HEAD_H


#define __VSCODE__

#if defined(__VSCODE__)
#define _FAR
#else
#define _FAR far
#endif

#if defined(__VSCODE__)
#define _NEAR
#else
#define _NEAR near
#endif

#if defined(__VSCODE__)
#define _SEG
#else
#define _SEG _seg
#endif


#if defined(__DOS16__)
#include <ALLOC.H>  /* coreleft(), farcoreleft(), malloc(), farmalloc(), free(), farfree() */    
#include <CONIO.H>
// #include <CTYPE.H>
// #include <DIR.H>
#include <DOS.H>        /* FP_OFF(), FP_SEG(), MK_FP() */
// #include <ERRNO.H>
#include <FCNTL.H>
#include <IO.H>
// #include <MEM.H>
// #include <PROCESS.H>
#include <STDIO.H>      /* printf() */
#include <STDLIB.H>     /* ltoa() */
#include <STRING.H>
#include <SYS\STAT.H>
// #include <VALUES.H>
#endif

#include "ST_DBG.H"
#include "ST_DEF.H"
#include "ST_EMM.H"
#include "ST_GUI.H"
#include "ST_LBX.H"
#include "ST_TXT.H"
#include "ST_VID.H"


// J:\STU\DBWD\developc\_MPL13\SAMPCODE\C\OTHER\MEMORY\MOVEMEM.C
// #pragma pack(1)     /* Use pragma to force packing on byte boundaries. */



#if defined(__DOS16__)
typedef unsigned int sgmt_addr;
typedef unsigned int ofst_addr;
typedef void _SEG  * sgmt_ptr;
typedef void _NEAR * near_ptr;
typedef void _FAR  * far_ptr;
typedef void _SEG  * memptr;  // Wolf3d ID_MM.H J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_MM.H


// ID/Wolf3D-style
typedef unsigned char  byte;
typedef unsigned int   word;  // elsewhere, this is `short` (see: root42, brackeen, Wolf3d, Animator, AIL214)
typedef unsigned long  dword;

/*
    NOTE(JimBalcomb,20220829): moved to ST_TYPE.H
typedef unsigned char  DB;
typedef unsigned int   DW;
typedef unsigned long  DD;

typedef unsigned char  BYTE;
typedef unsigned int   WORD;
typedef unsigned long  DWORD;
*/

/*
    NOTE(JimBalcomb,20220829): these are just silly - DB,DW,DD are inherently signless
typedef unsigned char  UBYTE;
typedef unsigned int   UWORD;
typedef unsigned long  UDWORD;
typedef   signed char  SBYTE;
typedef   signed int   SWORD;
typedef   signed long  SDWORD;
*/

typedef unsigned char  UCHAR;
typedef unsigned short USHORT;
typedef unsigned int   UINT;
typedef unsigned long  ULONG;
typedef   signed char  SCHAR;
typedef   signed short SSHORT;
typedef   signed int   SINT;
typedef   signed long  SLONG;

// ? stdint.h ? limits.h ? - ? C89, C90, C99, ?
#endif

#define ST_TRUE  1
#define ST_FALSE 0
#define ST_UNDEFINED -1

#define ST_SUCCESS -1
#define ST_FAILURE  0

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

#ifdef MK_FP
#undef MK_FP                                                           
#endif                                                                  
#ifdef FP_SEG
#undef FP_SEG                                                           
#endif                                                                  
#ifdef FP_OFF
#undef FP_OFF                                                           
#endif                                                                  

// // #define MK_FP(sgmt,ofst) ( (void _SEG *)(sgmt) + (void _NEAR *)(ofst) )
#define MK_FP(seg,ofs) ( (void _seg *)(seg) + (void near *)(ofs) )
// #define MK_FP(_sgmt_,_ofst_) ( (void _FAR *) (  (unsigned long) (_sgmt_) << 16  | (_ofst_) ) )
// #define MK_FP(_sgmt_,_ofst_) ( (void  far *) ( ((unsigned long) (_sgmt_) << 16) | (_ofst_) ) )

// #define FP_SEG(fp) ( (unsigned int)(void _SEG *)(void _FAR *)(fp) )
#define FP_SEG(fp) ( (unsigned int)(void _seg *)(void far *)(fp) )
#define FP_OFF(fp) ( (unsigned int)(fp) )

// "The [segment address] ... is simply the upper 16 bits of a far pointer." - PC Mag, 19880126
// FarPtr = (char far *) ( (long) segment_address << 16 );
// https://forum.arduino.cc/t/long-to-two-ints/665091/6
// (2) Take out upper 16-bit (the high16) this way:
// (a) Shift total32 to the right by 16-bit positions and place the new value in y.
// (b) Replace upper 16-bit of the above valu by 0s. They will be automatically 0s due to shift.
// (c) Keep lower 16-bit using cast.
// ~== unsigned int SgmtAddr = (unsigned int) ( (unsigned long) (FarPtr >> 16) );
#define FP_SGMT(_fp_) ( (unsigned int) ( (unsigned long)((void _FAR *)(_fp_)) >> 16) )
#define FP_OFST(_fp_) ( (unsigned int)(_fp_) )


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


#endif /* ST_HEAD_H */
