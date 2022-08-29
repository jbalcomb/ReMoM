// _s07p05c.c  farpeekb()
// ST_SA.H
#include "ST_HEAD.H" 


// J:\STU\DBWD\BORLANDC\CRTL\CLIB\PEEK.C
// int peek(unsigned segment, unsigned offset)
// {
//         _ES = segment;
//         return(* (int _es *) offset);
// }
// 
// char peekb(unsigned segment, unsigned offset)
// {
//         _ES = segment;
//         return(* (unsigned char _es *) offset);
// }
// 
// J:\STU\DBWD\BORLANDC\CRTL\CLIB\POKE.C
// 
// void poke(unsigned segment, unsigned offset, int value)
// {
//         _ES = segment;
//         * (int _es *) offset = value;
// }
// 
// void pokeb(unsigned segment, unsigned offset, char value)
// {
//         _ES = segment;
//         * (char _es *) offset = value;
// }
// 
// J:\STU\DBWD\BORLANDC\INCLUDE\DOS.H
// 
// #ifdef __cplusplus
// 
// int  inline _Cdecl peek( unsigned __segment, unsigned __offset )   { return( *( (int  far* )MK_FP( __segment, __offset )) ); }
// char inline _Cdecl peekb( unsigned __segment, unsigned __offset )  { return( *( (char far* )MK_FP( __segment, __offset )) ); }
// 
// void inline _Cdecl poke( unsigned __segment, unsigned __offset, int __value )    { ( *( (int  far* )MK_FP( __segment, __offset )) = __value ); }
// void inline _Cdecl pokeb( unsigned __segment, unsigned __offset, char __value )  { ( *( (char far* )MK_FP( __segment, __offset )) = __value ); }
// 
// #else
// 
// #define peek( a,b )  ( *( (int  far* )MK_FP( (a ),( b )) ))
// #define peekb( a,b ) ( *( (char far* )MK_FP( (a ),( b )) ))
// 
// #define poke( a,b,c )  ( *( (int  far* )MK_FP( (a ),( b )) ) =( int  )( c ))
// #define pokeb( a,b,c ) ( *( (char far* )MK_FP( (a ),( b )) ) =( char )( c ))
// 
// #endif  /* __cplusplus */



// FarPtr = (char far *) (   (long) fp_segment_address << 16 );
// FarPtr = (char far *) ( ( (long) fp_segment_address << 16 ) | fp_offset );

unsigned char farpeekb(unsigned int sgmt, unsigned int ofst)
{
    /*
    mov si, [bp+Ofst]
    mov ax, [bp+Sgmt]
    mov ds, ax
    lodsb
    */
    
    return(*( (unsigned char _FAR * )MK_FP(sgmt, ofst)));
}
