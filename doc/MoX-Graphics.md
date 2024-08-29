





## Multi_Colored_Line()

XREF:
    Clipped_Line_Base()
    HLP_DrawExpandLines__TODO()



## Clipped_Multi_Colored_Line()

just calls Clipped_Line_Base(), with `Patterened = ST_TRUE` and `color = 0`

¿ why wouldn't they have just called it directly ?


## Clipped_Line_Base()

calls Line() or Multi_Colored_Line()

XREF:
    VGA_WndDrawLine()
    Clipped_Multi_Colored_Line()

handles clipping
handles reversed lines, including color array position

1oom
ui_draw_line_limit_do()
XREF:
    ui_draw_line_limit()
    ui_draw_line_limit_ctbl()


MoO2
Module: line
Clip_Line()
    Line()
    Multi_Colored_Line()
    Anti_Alias_Line()
    Plasma_Line()



MoO2

Module: MOX
    data (0 bytes) _multi_colored_line_start
        Address: 02:0019195E
    data (0 bytes) _multi_colored_box_color_start
        Address: 02:00191972
Module: HAROLD
    code (0 bytes) Draw_Multi_Colored_Box_
        Address: 01:0007A5F7
Module: SHIPS
    code (0 bytes) Draw_Directional_Multi_Colored_Line_
        Address: 01:000A11C0
    code (0 bytes) Draw_Ship_Destination_Line_
        Address: 01:000A1299
Module: line
    function (0 bytes) Multi_Colored_Line
    Address: 01:00135870
        Num params: 4
        Return type: void (1 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        signed integer (2 bytes) 
        pointer (4 bytes) 
        signed integer (2 bytes) 
        Locals:
            signed integer (2 bytes) x1
            signed integer (2 bytes) y1
            signed integer (2 bytes) x2
            signed integer (2 bytes) y2
            pointer (4 bytes) color_array
            signed integer (2 bytes) rotation
            signed integer (2 bytes) temp
            signed integer (2 bytes) adj_up
            signed integer (2 bytes) adj_down
            signed integer (2 bytes) error_term
            signed integer (2 bytes) x_advance
            signed integer (2 bytes) x_delta
            signed integer (2 bytes) y_delta
            signed integer (2 bytes) whole_step
            signed integer (2 bytes) initial_pixel_count
            signed integer (2 bytes) final_pixel_count
            signed integer (2 bytes) i
            signed integer (2 bytes) j
            signed integer (2 bytes) run_length
            pointer (4 bytes) long_current_video_page

Draw_Ship_Destination_Line_()
    |-> Draw_Directional_Multi_Colored_Line_()
        |-> Multi_Colored_Line()


C:\STU\developp\1oom\src\ui\classic\uidraw.c
    void ui_draw_line_ctbl(int x0, int y0, int x1, int y1, const uint8_t *colortbl, int colornum, int pos, int scale)

    void ui_draw_line_limit(int x0, int y0, int x1, int y1, uint8_t color, int scale)
        ui_draw_line_limit_do(x0, y0, x1, y1, color, NULL, 0, 0, scale);

    void ui_draw_line_limit_ctbl(int x0, int y0, int x1, int y1, const uint8_t *colortbl, int colornum, int pos, int scale)
        ui_draw_line_limit_do(x0, y0, x1, y1, 0, colortbl, colornum, pos, scale);

static void ui_draw_line_limit_do(int x0, int y0, int x1, int y1, uint8_t color, const uint8_t *colortbl, int colornum, int colorpos, int scale)
    |-> ui_draw_line_ctbl(x0, y0, x1, y1, colortbl, colornum, colorpos, scale);
    OR
    |-> Line(x0, y0, x1, y1, color, scale);

