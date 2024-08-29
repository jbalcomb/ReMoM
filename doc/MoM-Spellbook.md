








OVL_ComposeBookText()
    |-> LBX_DrawTextLine()
        |-> LBX_DrawStyledString()
            |-> LBX_DrawString()
                |-> LBX_DrawGlyph()

1oom:
    |-> lbxfont_print_str_normal_do()
        |-> lbxfont_print_str_do()
            |-> lbxfont_print_char_ret_x()
                |-> lbxfont_plotchar()
                    |-> lbxfont_plotchar_1x()

MoO2: Module: fonts

    |-> Print_To_Bitmap()
        |-> Print_Display_To_Bitmap()
            |-> Print_String_To_Bitmap()
                |-> Print_Character_To_Bitmap()
                    |-> Print_Clipped_Letter_To_Bitmap()




LBX_DrawGlyph()  ~== lbxfont_plotchar() && lbxfont_plotchar_1x()


LBX_DrawJustified()  ~==  lbxfont_print_str_normal_do_w0()


print_xpos, print_ypos  ~== lbxfont_temp_x, lbxfont_temp_y


Print_Display_To_Bitmap()  vs. Print_Display()

    outline_style checks
        != 2
        != 1 && != 3
        == 3 || == 5
        > 3
        == 5

## Print_Display_To_Bitmap()
    no draw_alias_flag

## Print_Display()

















##### Naming Things Is Hard


Page 25  (PDF Page 30)

Spells





Clicking on the spells button takes you to your spellbook.
Only those spells that you know and that can be cast overland
(i.e., noncombat spells such as city enchantments, unit enchantments, summoning spells, etc.)
are visible.
At the beginning of the game, the only spell in your spellbook is likely to be magic spirit, a common arcane summoning spell described in the Spellbook supplement.
The casting cost of a spell is shown to the right of the spell’s name in your spellbook.


Dog ear: click here to turn page
Casting cost
Bookmark: click here to close book
Length of time to cast
Name of spell
Type of spell effect
