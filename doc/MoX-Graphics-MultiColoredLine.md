


Draw_Road_Build_Path_Line()
    |-> Clipped_Multi_Colored_Line()
        |-> Clipped_Line_Base()
            |-> Multi_Colored_Line()




void ui_draw_line_limit(int x0, int y0, int x1, int y1, uint8_t color, int scale)
{
    ui_draw_line_limit_do(x0, y0, x1, y1, color, NULL, 0, 0, scale);
}

void ui_draw_line_limit_ctbl(int x0, int y0, int x1, int y1, const uint8_t *colortbl, int colornum, int pos, int scale)
{
    ui_draw_line_limit_do(x0, y0, x1, y1, 0, colortbl, colornum, pos, scale);
}


ui_draw_line_limit_do(x0, y0, x1, y1,  color,     NULL,        0,   0,  scale);
ui_draw_line_limit_do(x0, y0, x1, y1,      0, colortbl, colornum, pos,  scale);

¿ ~== Clipped_Line ?
but, why not just Clipped_Line() |-> Line()?


ui_draw_line_limit_do()

Parameter
    color
        just gets passed throuh to Line(), from VGA_WndDrawLine()

