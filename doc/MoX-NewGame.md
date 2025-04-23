
MAGIC.EXE
ovr050
ovr051
ovr056

MoO2
MAINMENU
NEWGAME
MAPGEN
HOMEGEN



main()
Main_Menu_Control()
Newgame_Control()
Init_New_Game()




current_menu_screen AKA auto_input_field_idx



OG-MOM
Main Menu - New Game
0: Game Options
    Difficulty, Opponents, Land Size, Magic
    Cancel, OK
1: Select Wizard
    Merlin, ..., Kali
    Custom
2:
3:
4:
5:
6:
7:

Prefab Wizard vs. Custom
    Prefab:
        Wizard's Name
        Select Race
    Custom:
        Select Picture
        Wizard's Name
        picks
        Select ... Spells
        Select ... Spells
        Select ... Spells
        Select ... Spells
        Select ... Spells
        Select Race







## Newgame_Screen_0_Draw()


-0092 Opponents_Texts_Len8 db 32 dup(?)
-0072 Magic_Texts_Len9 db 27 dup(?)
-0057 db ? ; undefined
-0056 LandSize_Texts_Len7 db 21 dup(?)
-0041 db ? ; undefined
-0040 Difficulty_Texts_Len11 db 55 dup(?)
-0009 db ? ; undefined

lea     ax, [bp+Difficulty_Texts_Len11]
push    ss
push    ax                                ; Dest_Struct
mov     ax, offset cnst_Diff0_Text_11b    ; "Intro"
push    ds
push    ax                                ; Src_Struct
mov     cx, 55                            ; 5 * 11
call    SCOPY@

lea     ax, [bp+LandSize_Texts_Len7]
push    ss
push    ax                                ; Dest_Struct
mov     ax, offset cnst_LandS0_Text_7b    ; "Small"
push    ds
push    ax                                ; Src_Struct
mov     cx, 21                            ; 3 * 7
call    SCOPY@

lea     ax, [bp+Magic_Texts_Len9]
push    ss
push    ax                                ; Dest_Struct
mov     ax, offset cnst_Magic0_Text_9b    ; "Weak"
push    ds
push    ax                                ; Src_Struct
mov     cx, 27                            ; 3 * 9
call    SCOPY@

lea     ax, [bp+Opponents_Texts_Len8]
push    ss
push    ax                                ; Dest_Struct
mov     ax, offset cnst_Opnts0_Text_8b    ; "One"
push    ds
push    ax                                ; Src_Struct
mov     cx, 32                            ; 4 * 8
call    SCOPY@

auto_input_variable
MGC dseg:4D3A 00 00                                           auto_input_variable dw 0                ; DATA XREF: Interpret_Mouse_Input+18w ...
    auto_input_field_idx = Auto_Input();
    if(auto_input_field_idx != newgame_difficulty_button_field)
    if(auto_input_field_idx != newgame_opponents_button_field)
    if(auto_input_field_idx != newgame_landsize_button_field)
    if(auto_input_field_idx != newgame_magic_button_field)




MoO2
Module: NEWGAME
Newgame_Screen_()





Newgame_Control()




## TBL_Bookshelf_Books

GAME_RandBookBinders__WIP()


     w GAME_RandBookBinders__WIP+AA mov     [TBL_Bookshelf_Books+bx], ax      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr7+104        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr7+12D        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr7+156        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr7+17F        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr7+1A8        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr6+176        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr6+1A2        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr6+1CE        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr6+1FA        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr6+226        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r NEWG_DrawDefShelf+9E         add     ax, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r NEWG_DrawDefShelf+D1         add     ax, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr4+373        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr4+3C3        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr4+413        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr4+463        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r GAME_Draw_NewScr4+4B3        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r SCRN_Draw_NewScr5+FD         mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r SCRN_Draw_NewScr5+126        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r SCRN_Draw_NewScr5+14F        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r SCRN_Draw_NewScr5+178        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2
Down r SCRN_Draw_NewScr5+1A1        mov     bx, [TBL_Bookshelf_Books+bx]      ; initialized to random values of 0, 1, or 2

Down w GAME_RandBookBinders__WIP+82 mov     [TBL_Realm0_Books+bx], ax
Down r GAME_Draw_NewScr4+39F        mov     bx, [TBL_Realm0_Books+bx]

Down w GAME_RandBookBinders__WIP+96 mov     [TBL_Realm1_Books+bx], ax
Down r GAME_Draw_NewScr4+34F        mov     bx, [TBL_Realm1_Books+bx]

Down w GAME_RandBookBinders__WIP+6E mov     [TBL_Realm2_Books+bx], ax
Down r GAME_Draw_NewScr4+43F        mov     bx, [TBL_Realm2_Books+bx]

Down w GAME_RandBookBinders__WIP+5A mov     [TBL_Realm3_Books+bx], ax
Down r GAME_Draw_NewScr4+48F        mov     bx, [TBL_Realm3_Books+bx]

Down w GAME_RandBookBinders__WIP+46 mov     [TBL_Realm4_Books+bx], ax
Down r GAME_Draw_NewScr4+3EF        mov     bx, [TBL_Realm4_Books+bx]
