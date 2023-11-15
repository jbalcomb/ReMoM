


MoO2
Module: fields
    signed integer (2 bytes) help_box_mode
        Address: 02:0017C4CE
    signed integer (2 bytes) field_box_mode
        Address: 02:0017C4CC
    signed integer (2 bytes) list_highlight_color
        Address: 02:0017C4AC
    signed integer (2 bytes) list_field_scan_flag
        Address: 02:0017C4A8
    signed integer (2 bytes) list_field_scroll_flag
        Address: 02:0017C4AA
    signed integer (2 bytes) list_field_on
        Address: 02:0017C496
    char (1 bytes) exit_the_list_field
        Address: 02:0017C474




ovr149

¿ MoO2  Message Box, Text Box, Help Box ?
Module: TEXTBOX
Module: COMBAT
    code (0 bytes) Message_Box_Titled_
    Address: 01:0003FDAA



Left-Click Info Button

set Advisor Screen
leave Main Screen
Screen_Control() |-> Advisor_Screen(-1)



Screenshot in Manual does not F-keys
added later?
explains addition of fkey_string and input_advisor_idx?

Manual says "list of advisors" and "advisor buttons"
In-Game says "Select An Advisor"
In-Game Help says "ADVISORS  This list..."




array of [26] for 10 list items
maybe was one of [260]?

8 list items
iters on 10, 0 to 9
passes 9 to dialogue function


passes `char *` to 'dialogue title?




Advisors_Screen()

-011E str0_Surveyor db 26 dup(?)
-0104 str1_Cartographer db 26 dup(?)
-00EA str2_Apprentice db 26 dup(?)
-00D0 str3_Historian db 26 dup(?)
-00B6 str4_Astrologer db 26 dup(?)
-009C str5_Chancellor db 26 dup(?)
-0082 str6_TaxCollector db 26 dup(?)
-0068 str7_GrandVizier db 26 dup(?)
-004E str8_Mirror db 26 dup(?)
-0034 str9_EoT db 26 dup(?)
-001A str_list@ dw 10 dup(?)
-0006 fkey_string db 4 dup(?)
-0002 input_advisor_idx dw ?
+0000  s db 2 dup(?)
+0002  r db 4 dup(?)
+0006 advisor_idx dw ?





SELECTBD { 5, ..., 11}
SELECTBK {12, ..., 33}
SELECTBD {34, ..., 37}

GUI_ListSelect_Dlg()
    LBX Loads...
        {5, ..., 11}
            SELECTBD    left, bottom left, top, right, bottom, bottom right, scroll
        {32, ..., 37}
            SELECTBK    top arrow, bottom arrow, scroll top, scroll bottom, scroll locked up, scroll locked down





GUI_ListSelect_Dlg()
OON XREF:
    j_GUI_ListSelect_Dlg()

j_GUI_ListSelect_Dlg()
XREF:
    Advisor_Screen+2B6        call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    sub_651C8+2D7             call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    CMB_SelectCaster+67       call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    sub_B148C+20D             call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    CMB_RaiseDead:loc_B3B9E   call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    CMB_AnimateDead:loc_B3FDA call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified
    sub_BCC70+101             call    j_GUI_ListSelect_Dlg            ; displays a list selection dialog with the specified

