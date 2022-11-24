


// struc CreditsPairs ; (sizeof=0x4)
// Role dw ?                               ; offset (00031D10)
// Name dw ?                               ; offset (00031D10)
// ends CreditsPairs
struct s_CreditsPairs //; (sizeof=0x4)
{
    char *Role;
    char *Name;
};


/*
    Initialized Data
*/

// MAINMENU CREDITS  extern int GAME_FirstShownCred;             // dseg:38D8
// MAINMENU CREDITS  extern int GAME_CreditTextTop;              // dseg:38DA
// MAINMENU CREDITS  extern int GAME_CredsRollTimer;             // dseg:38DC

// MAINMENU CREDITS  extern struct s_CreditsPairs TBL_Credits_Texts[];
// dseg:38DE TBL_Credits_Texts CreditsPairs <offset cnst_Credits_00, offset cnst_Credits_01>; 0 ; could use dseg:2947 or the other way around
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 1
// dseg:38DE CreditsPairs <offset cnst_Credits_20, offset cnst_Credits_21>; 2
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_31>; 3
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_01>; 4
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 5
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 6
// dseg:38DE CreditsPairs <offset cnst_Credits_80, offset cnst_Credits_81>; 8
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 9
// dseg:38DE CreditsPairs <offset cnst_Credits_100, offset cnst_Credits_101>; 10
// dseg:38DE CreditsPairs <offset cnst_Credits_110, offset cnst_Credits_111>; 11
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_121>; 12
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_131>; 13
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_141>; 14
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_151>; 15
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>; 16
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 17
// dseg:38DE CreditsPairs <offset cnst_Credits_180, offset cnst_Credits_181>; 18
// dseg:38DE CreditsPairs <offset cnst_Credits_190, offset cnst_Credits_191>; 19
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 20
// dseg:38DE CreditsPairs <offset cnst_Credits_210, offset cnst_Credits_211>; 21
// dseg:38DE CreditsPairs <offset cnst_Credits_220, offset cnst_Credits_221>; 22
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_231>; 23
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_241>; 24
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_251>; 25
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_261>; 26
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_271>; 27
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_281>; 28
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_291>; 29
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_301>; 30
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_311>; 31
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_321>; 32
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_331>; 33
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_341>; 34
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_351>; 35
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_361>; 36
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 37
// dseg:38DE CreditsPairs <offset cnst_Credits_380, offset cnst_Credits_381>; 38
// dseg:38DE CreditsPairs <offset cnst_Credits_390, offset cnst_Credits_391>; 39
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_401>; 40
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_411>; 41
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 42
// dseg:38DE CreditsPairs <offset cnst_Credits_430, offset cnst_Credits_431>; 43
// dseg:38DE CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>); 44
// dseg:38DE CreditsPairs <offset cnst_Credits_460, offset cnst_Credits_461>; 46
// dseg:38DE CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>; 47

// dseg:399E cnst_Credits_00 db 'Game Designer'
// dseg:39AB cnst_ZeroString_6 db 0
// dseg:39AC cnst_Credits_01 db 'Steve Barcia',0
// dseg:39B9 cnst_Credits_20 db 'Programmers',0
// dseg:39C5 cnst_Credits_21 db 'Jim Cowlishaw',0
// dseg:39D3 cnst_Credits_31 db 'Ken Burd',0
// dseg:39DC cnst_Credits_51 db 'Grissel Barcia',0
// dseg:39EB cnst_Credits_80 db 'Producer',0
// dseg:39F4 cnst_Credits_81 db 'Doug Caspian-Kaufman',0
// dseg:3A09 cnst_Credits_100 db 'Art Director',0
// dseg:3A16 cnst_Credits_101 db 'Jeff Dee',0
// dseg:3A1F cnst_Credits_110 db 'Artists',0
// dseg:3A27 cnst_Credits_111 db 'Shelly Hollen',0
// dseg:3A35 cnst_Credits_121 db 'Amanda Dee',0
// dseg:3A40 cnst_Credits_131 db 'Steve Austin',0
// dseg:3A4D cnst_Credits_141 db 'George Purdy',0
// dseg:3A5A cnst_Credits_151 db 'Patrick Owens',0
// dseg:3A68 cnst_Credits_180 db 'Music Producer',0
// dseg:3A77 cnst_Credits_181 db 'The Fat Man',0
// dseg:3A83 cnst_Credits_190 db 'Composer',0
// dseg:3A8C cnst_Credits_191 db 'Dave Govett',0
// dseg:3A98 cnst_Credits_210 db 'QA Lead',0
// dseg:3AA0 cnst_Credits_211 db 'Destin Strader',0
// dseg:3AAF cnst_Credits_220 db 'Play Test',0
// dseg:3AB9 cnst_Credits_221 db 'Mike Balogh',0
// dseg:3AC5 cnst_Credits_231 db 'Damon Harris',0
// dseg:3AD2 cnst_Credits_241 db 'Geoff Gessner',0
// dseg:3AE0 cnst_Credits_251 db 'Tammy Talbott',0
// dseg:3AEE cnst_Credits_261 db 'Mick Uhl',0
// dseg:3AF7 cnst_Credits_271 db 'Jim Hendry',0
// dseg:3B02 cnst_Credits_281 db 'Frank Brown',0
// dseg:3B0E cnst_Credits_291 db 'Jim Tricario',0
// dseg:3B1B cnst_Credits_301 db 'Jen MacLean',0
// dseg:3B27 cnst_Credits_311 db 'Brian Wilson',0
// dseg:3B34 cnst_Credits_321 db 'Brian Helleson',0
// dseg:3B43 cnst_Credits_331 db 'Jeff Dinger',0
// dseg:3B4F cnst_Credits_341 db 'Chris Bowling',0
// dseg:3B5D cnst_Credits_351 db 'Charles Brubaker',0
// dseg:3B6E cnst_Credits_361 db 'Tom Hughes',0
// dseg:3B79 cnst_Credits_380 db 'Sound Effects',0   ; could use dseg:2947 or the other way around
// dseg:3B87 cnst_Credits_381 db 'Midian',0
// dseg:3B8E cnst_Credits_390 db 'Speech',0
// dseg:3B95 cnst_Credits_391 db 'Mark Reis',0
// dseg:3B9F cnst_Credits_401 db 'Peter Woods',0
// dseg:3BAB cnst_Credits_411 db 'David Ellis',0
// dseg:3BB7 cnst_Credits_430 db 'Manual',0
// dseg:3BBE cnst_Credits_431 db 'Petra Schlunk',0
// dseg:3BCC cnst_Credits_460 db 'Special Thanks',0
// dseg:3BDB cnst_Credits_461 db 'Jenny Cowlishaw',0

// MAINMENU CREDITS  extern char cnst_Credits_DOT; // dseg:3BEB  db '.',0               ; should use dseg:30c9
// dseg:3BED align 2




/*
    Uninitialized Data
*/

// MAINMENU CREDITS  extern unsigned int gsa_GUI_Credits_Text_IMG; // dseg:948C
