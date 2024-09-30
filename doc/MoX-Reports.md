
MoX

WIZARDS.EXE
ovr126

MoO2
Module: REPORT


Must_Display_Reports_()
XREF:
    Main_Screen_Report_Handler_
    Reports_Screen_
    Hotseat_Select_Player_
Reports_Screen_()
XREF:
    Screen_Control_()

Module: MAINSCR2
    code (0 bytes) Reports_Screen_
    Address: 01:0008B956






scroll postion and height

In Chancellor_Screen_Scroll_Draw__WIP(), 
Clipped_Print_Centered()
Clipped_Print()
Clipped_Print_Far()
(_scroll_start_x + _scroll_text_top + x_add)

_scroll_start_x = (2 + (99 - (_scroll_text_height / 2)))

_scroll_text_height = ((total_message_count * 7) + (scroll_text_line_count * 12))

m_report_scroll_text_height = _scroll_text_height


Combat_Results_Scroll()
    _scroll_text_height = Combat_Results_Scroll_Text()
    ...
    _scroll_text_top = ((199 - (22 + _scroll_text_height)) / 2)




## 'Next Turn'
Next_Turn_Calc()
    MSG_Clear();









Module: REPORT
    data (0 bytes) _something_interesting_happened
    Address: 02:001A3124

    data (0 bytes) _your_spy_steals_fmt
    Address: 02:001A312C

    data (0 bytes) _enemy_spy_destroys_fmt
    Address: 02:001A3130

    data (0 bytes) _really_trash_colony_base_fmt
    Address: 02:001A3134

    data (0 bytes) _colonize_preselected_planet_fmt
    Address: 02:001A3138

    data (0 bytes) _enemy_spy_steals_fmt
    Address: 02:001A313C

    data (0 bytes) _outpost_preselected_planet_fmt
    Address: 02:001A3140

    data (0 bytes) _your_spy_destroys_fmt
    Address: 02:001A3144

    data (0 bytes) _you_framed_fmt
    Address: 02:001A3148

    code (0 bytes) Load_Report_Strings_
    Address: 01:000FD5EE
        Locals:

    code (0 bytes) sprintf_report_
    Address: 01:000FD69F
        Locals:

    code (0 bytes) Client_Initialize_Reports_
    Address: 01:000FD7EA
        Locals:

    code (0 bytes) Initialize_Reports_
    Address: 01:000FD81C
        Locals:

    code (0 bytes) Add_Report_
    Address: 01:000FD846
        Locals:

    code (0 bytes) Draw_Special_
    Address: 01:000FD875
        Locals:

    code (0 bytes) Star_Has_Special_For_Player_
    Address: 01:000FD8ED
        Locals:

    code (0 bytes) Set_Initial_Tech_
    Address: 01:000FD911
        Locals:

    code (0 bytes) Explored_New_Star_
    Address: 01:000FD95A
        Locals:

    code (0 bytes) Player_Colony_Base_At_Star_
    Address: 01:000FDA3F
        Locals:

    code (0 bytes) Player_Ship_Type_At_Star_
    Address: 01:000FDAA7
        Locals:

    code (0 bytes) Colonization_
    Address: 01:000FDB01
        Locals:

    code (0 bytes) Empty_Colony_Queue_
    Address: 01:000FDF93
        Locals:

    code (0 bytes) Need_To_Set_Occupation_Policy_
    Address: 01:000FE02C
        Locals:

    code (0 bytes) Has_Msg_
    Address: 01:000FE0EA
        Locals:

    code (0 bytes) Has_Officer_For_Hire_
    Address: 01:000FE251
        Locals:

    code (0 bytes) Has_Research_Breakthrough_
    Address: 01:000FE2E8
        Locals:

    code (0 bytes) Has_Evolutionary_Upgrade_
    Address: 01:000FE31F
        Locals:

    code (0 bytes) Has_Report_
    Address: 01:000FE359
        Locals:

    code (0 bytes) Has_Diplomacy_Messages_
    Address: 01:000FE408
        Locals:

    code (0 bytes) Has_GNN_
    Address: 01:000FE514
        Locals:

    code (0 bytes) Antarans_
    Address: 01:000FE552
        Locals:

    code (0 bytes) Display_Report_Aux_
    Address: 01:000FE63E
        Locals:

    code (0 bytes) Display_Report_
    Address: 01:000FE785
        Locals:

    code (0 bytes) Redirect_Newly_Built_Ships_
    Address: 01:000FE793
        Locals:

    code (0 bytes) Must_Display_Reports_
    Address: 01:000FE86B
        Locals:

    data (0 bytes) _reports
    Address: 02:001A314C

    data (0 bytes) _n_reports
    Address: 02:001A3502

    data (0 bytes) _done_colonize_check
    Address: 02:001A3504

    data (0 bytes) _done_player_reports
    Address: 02:001A350D

    data (0 bytes) _displayed_gnn
    Address: 02:001A350E

    data (0 bytes) _displayed_msgs
    Address: 02:001A350F
