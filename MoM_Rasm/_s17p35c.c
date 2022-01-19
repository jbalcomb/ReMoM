// _s17p35c.c VGA_DrawTextLine
// ST_TXT.H

void VGA_DrawTextLine(int Left, int Top, char *String)
{
    VGA_DrawStyledString(Left, Top, String, 0);
}
