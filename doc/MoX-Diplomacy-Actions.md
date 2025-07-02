


Somewhere, ...
    //OSG  "If you also off me [] spell I would accept."  Agree  Forget It
...counter-offer...proposal...





m_IDK_diplomatic_order

set from _players[].Dipl.Dipl_Action[]
but, updated

passed to
    Get_Diplomacy_Statement()
     IDK_Diplo_Response()
    


m_IDK_diplomatic_order vs. m_IDK_diplomatic_flag


IDK_Dipl_s72690() ~== Diplomacy_Screen__WIP()

    if(
        (m_IDK_diplomatic_flag == 0)
        ||
        (m_IDK_diplomatic_flag == 1)
        ||
        (m_IDK_diplomatic_flag == 2)
    )
        full_screen_field = Add_Hidden_Field(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, (int16_t)str_empty_string__ovr086[0], ST_UNDEFINED);
    ...means there's no other fields to interact with? so, just displaying something



IDK_Diplo_Response()
    m_IDK_diplomatic_order = diplomatic_order;

Diplomacy_Screen__WIP()
    else if(m_IDK_diplomatic_flag == 6)
        IDK_Diplo_Response(m_IDK_diplomatic_order, 3);
IDK_Dipl_s72690()
    else if(m_IDK_diplomatic_flag == 6)
        IDK_Diplo_Response(m_IDK_diplomatic_order, 3);

Down P IDK_Diplo_Scrn:loc_6EE67 call    j_Find_Worst_Modifier
Down P IDK_Dipl_s7373B+A8       call    j_Find_Worst_Modifier


IDK_Dipl_s72690()
    if(m_IDK_diplomatic_order == 47)
        Declare_War(HUMAN_PLAYER_IDX, word_42E86);
    if(
        (m_IDK_diplomatic_order == 45)
        ||
        (m_IDK_diplomatic_order == 46)
        ||
        (m_IDK_diplomatic_order == 49)
    )
        Player_Add_Gold(_human_player_idx, _players[HUMAN_PLAYER_IDX].Dipl.Tribute_Gold[m_diplomac_player_idx]);
        if(_players[HUMAN_PLAYER_IDX].Dipl.Tribute_Spell[m_diplomac_player_idx] != spl_NONE)
            WIZ_DIPL_TeachSpell__STUB(HUMAN_PLAYER_IDX, _players[HUMAN_PLAYER_IDX].Dipl.Tribute_Spell[m_diplomac_player_idx]);



45
46
updates treaty interest
    45 calls sign treaty type 1
    46 calls sign treaty type 2

47
calls declare war
calls break treaties

48
updates trade interest
uses spell index

49
updates peace interest
calls sign peace treaty

45
46
49
uses tribute gold
uses tribute spell
