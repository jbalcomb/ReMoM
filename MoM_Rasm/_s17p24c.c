// _s17p24c.c VGA_DrawCentered
// ST_TXT.H

void VGA_DrawCentered(int Center, int Top, char *String)
{
    // int Return_Value;
    int String_Width;

    String_Width = VGA_GetStringWidth(String);

    VGA_DrawTextLine(((String_Width / 2) + Center), Top, String);

    // IDGI.
    // mov  [bp+Return_Value], ax
    // mov  ax, [bp+Return_Value]
    // jmp  short $+2
}
