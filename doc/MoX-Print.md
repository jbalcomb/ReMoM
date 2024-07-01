


move notes here from MoM-Fonts.md



Print()
Clipped_Print()

The 'Clipped Print' train ...
...different in Print()/Print_Display()
...does not support aliasing?
...missing the big block of text control codes?
...does not support full print/justified text?



Print_Integer_Right()
    |-> Print_Right()
        |-> Get_String_Width()
        |-> Print()
            |-> Print_Display()
                |-> Print_String()


Print_String()
    |-> Print_Character()
        |-> Print_Character_ASM()

Print_String()
    |-> Print_Character_No_Alias()



Clipped_Print_Centered()


Print_Clipped_Letter()
copies the bytes from the font glyph data to a temporary glyph buffer
copies the temporary glyph buffer to the video buffer
column by column, planar memory
sets all the bytes in the temporary glyph buffer to 0xFF and then skips those if they do not get overwritten by the glyph data


main/outer loop is over width

¿ needs to set and reset the screen position ?
can't know where a column ends
need to start back up top at the next column
current position is previous position plus writes and skips
next position is previous position plus one column

same for writing/reading the temporary glyph buffer?


in Print_Clipped_Character()
set-up for Print_Clipped_Letter()
read from font glyph data
write to VGA video ram / screen

DS:SI
DS = font_style_data

ES:DI


later, elsewhere
SI = offset _CS_glyph_buffer

