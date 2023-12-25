



Screen_Control()
    switch(current_screen)
        case scr_NextTurn:
            Next_Turn_Proc()
                Next_Turn_Calc()
                    CTY_ProgressTurn()
                        CTY_ProdProgress()




MoO2  Module: MSG

    data (0 bytes) _msg_colony_built_ship_fmt
        Address: 02:001A2BF0

    data (0 bytes) _msg_colony_built_building_fmt
        Address: 02:001A2C3C

    code (0 bytes) Load_Msg_Strings_
        Address: 01:000EE4A1

    code (0 bytes) Allocate_Msg_Slots_
        Address: 01:000EEB3A

    code (0 bytes) First_Msg_For_Player_
        Address: 01:000EEBF8

    code (0 bytes) N_Msgs_For_Player_
        Address: 01:000EEC02

Allocate_Msg_Slots_()
    _first_msg
    _max_msgs
    _n_msgs




## Message - Building Complete

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> MSG_Clear();

Next_Turn_Proc()
    BEGIN: Messages
    if(MSG_BuildDone_Count > 0)
