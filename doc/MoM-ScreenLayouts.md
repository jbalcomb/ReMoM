
# Screen Layouts


Main Menu Screen

x1,y1,x2,y for Fill()
? clear-screen macro ?

x,y of start position of menu items


Summary Window  

                  X,   Y
Background      240,  76

"GP"            
"MP"

gold            273, 101
"Gold"          278, 101

"Food"
"Mana"
gp
mp
food
mana



Load_Palette() |-> Set_Font(0, 0)

Main_Screen_Draw_Summary_Window()
... ALL Set_Font_Style4() ...



#### Gold

Set_Font_Colors_15(0, [198, 198, 198])
Set_Outline_Color(ST_BLACK)

gold >= 0                           Set_Font_Style4(0,  4, 0, 0)    Set_Alias_Color(184)
gold  < 0 && cycle_incomes <= -1    Set_Font_Style4(0,  2, 0, 0)    Set_Alias_Color(  0)
gold  < 0 && cycle_incomes  > -1    Set_Font_Style4(0, 15, 0, 0)    Set_Alias_Color(  0)

Print_Integer_Right(273, 101, gold);
Print(278, 101, cstr_Gold);


#### Food





if(cycle_incomes)
mov     ax, 3
push    ax                              ; step_value
xor     ax, ax
push    ax                              ; blue_max
xor     ax, ax
push    ax                              ; green_max
mov     ax, 63
push    ax                              ; red_max
xor     ax, ax
push    ax                              ; blue_min
xor     ax, ax
push    ax                              ; green_min
mov     ax, 40
push    ax                              ; red_min
mov     ax, 198
push    ax                              ; color_num
call    Cycle_Palette_Color


Hrrmm...
Set_Alias_Color()
sets the 1st color in each of the 16 color blocks in font_colors[][]
and the first color in each block of font_style_data
    s_FONT_HEADER.normalcurrent_colors_colors
    s_FONT_HEADER.normal_colors
    s_FONT_HEADER.highlight_colors
    s_FONT_HEADER.special_colors



Main_Screen_Draw_Status_Window()
Set_Outline_Color(0)
  Set_Font_Style1(0,0,0,0)  OR  Set_Font_Style1(1,0,0,0)
Set_Alias_Color(8)
Set_Font_Style1(0,0,0,0)
Set_Outline_Color(0)


                          
"Font Style"
{1,2,3,4,5}
outline_style = GET_1B_OFS(font_style_data, FONT_HDR_POS_SHADOW_FLAG);
overdraws left, right, top, bottom ... by 1 or 2 pixels
has to handle the corners
e.g., (x + 1, y + 1) will overdraw right + bottom, catches the bottom-right corner but miss the top-right and bottom-left
