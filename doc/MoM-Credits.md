

TBL_Credits_Texts  ==>  _credits
GAME_CredsRollTimer  ==>  _credits_timer
GAME_FirstShownCred  ==>  _credit_num
gsa_GUI_Credits_Text_IMG  ==>  _credits_pict_seg
GAME_CreditTextTop  ==>  _credits_y


MAGIC.EXE
main()
    |-> j_Init_Credits()
    |-> Menu_Screen_Control()

Main_Menu_Screen_()
    |-> Main_Menu_Screen_Draw()
        |-> j_Draw_Credits()
            |-> Draw_Credits()



main()
seg001:0344 9A 25 00 CB 31                                  call    j_Init_Credits

Menu_Screen_Control()
seg001:0500 9A 25 00 CE 31                                  call    j_HallOfFame_Screen
seg001:0505 9A 25 00 CB 31                                  call    j_Init_Credits

Main_Menu_Screen_Draw()
seg001:0B47 9A 20 00 CB 31                                  call    j_GAME_DrawCredits



280 x 122 "picture"
(280 * 122) = 34160 pixels/bytes
...Set_Window(0, 40, 319, 137)
...Draw_Credits() recreates the picture





credits strings are a 2-D array

calls Print_To_Bitmap() with credit_strings[0][]
...prints periods out to x < 102, relative to the bitmap, not the screen
prints role at x = 0
print name at x = 105



## credits timer
    set to 0
    starts rolling at 70
    capped at 30000

## Draw Credits Loop

¿ 10 * 10 ?

fade in from bottom
fade out to top
..._fade_in_colors[]..._fade_out_colors[]...
fade-in/out color is selected by which pixel it's on in the line of text, for the last and first lines, respectively


somehow, _SI_line_top is the y for Print_To_Bitmap()

somehow, (10 - line_top) gets you the array index for the fade-out color

somehow, (10 - (88 - line_top)) gets you the array index for the fade-in color


10 iterations, for 10 lines of text


## _credits_y
    ...set to 95 in Init_Credits()
        95 pixels, on the y axis
        relative to the bitmap
        ...40 + 95 = 135, the bottom edge of the last line of text
the current y coordinate
scope required to exist through multiple calls
decremented on each iteration of the main credits drawing loop
so, draws up pixel higher on each run through

on the way out, it sets it to start from the next line of credits, 10 pixels down
unless it's passed the last line of credits, then it resets everything


MoO2
_credits_column_one






MoO2
Module: MAINMENU
    data (0 bytes) _fade_in_colors
    Address: 02:0017A0D4
    data (0 bytes) _fade_out_colors
    Address: 02:0017A0DC












## Init_Credits()
allocates and prepares an image segment for composing
the rolling credits text (size 280x122) as the first
allocation in the sandbox, and resets the drawing
control values
```c
    _credit_num = 0;
    _credits_y = 95;
    _credits_pict_seg = Allocate_First_Block(_screen_seg, 3125);  // 3125 PR, 50000 B
    Create_Picture(280, 122, _credits_pict_seg);
```











MoO2
Module: MAINMENU
    data (0 bytes) _credits
    Address: 02:0019406C
    data (0 bytes) _credit_count
    Address: 02:00194074
    data (0 bytes) _credit_y
    Address: 02:00194078
    data (0 bytes) _credit_x
    Address: 02:0019407C
    data (0 bytes) _credits_font_style
    Address: 02:00194094
    data (0 bytes) _max_credits
    Address: 02:00194098
    data (0 bytes) _credits_column_one
    Address: 02:0019409A
    data (0 bytes) _credit_num
    Address: 02:0019409C
    data (0 bytes) _do_credits
    Address: 02:0019409D
