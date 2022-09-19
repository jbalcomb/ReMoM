#ifndef ST_TYPE_H
#define ST_TYPE_H


// ? stdint.h ? limits.h ? - ? C89, C90, C99, ?

// ? #if defined(__DOS16__) ?


typedef unsigned int        SAMB_addr;
typedef unsigned char  *    SAMB_ptr;

typedef SAMB_addr SAMB_head;
typedef SAMB_addr SAMB_data;


typedef unsigned char   DB;
typedef unsigned short  DW;
typedef unsigned long   DD;

typedef unsigned char   BYTE;
typedef unsigned short  WORD;
typedef unsigned long   DWORD;
// ID/Wolf3D-style
typedef unsigned char  byte;
typedef unsigned int   word;  // elsewhere, this is `short` (see: root42, brackeen, Wolf3d, Animator, AIL214)
typedef unsigned long  dword;


typedef unsigned int sgmt_addr;
typedef unsigned int ofst_addr;
//typedef void seg  * sgmt_ptr;
//typedef void near * near_ptr;
//typedef void far  * far_ptr;
// typedef void _SEG  * memptr;  // Wolf3d ID_MM.H J:\STU\DBWD\developc\wolf3d\WOLFSRC\ID_MM.H


// B8,B16,B32,B64,B128,B256,B512
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

#endif /* ST_TYPE_H */
