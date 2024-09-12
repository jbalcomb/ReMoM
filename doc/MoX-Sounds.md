


// WZD s01p13
Play_Left_Click()

// WZD s01p14
Play_Standard_Click()

// WZD s01p15
Play_Left_Click__DUPE()





Load_Button_Sounds()

SOUNDFX.LBX, 000    SLX19 VO    Standard Button click
SOUNDFX.LBX, 002    OKBUTTN     left button click





## Can we deduce the reason for the existence of Play_Left_Click__DUPE()?

usages:
    Main_Screen()
        Left-Click Unit Window Grid Field
        Right-Click Movement Map Grid Field
    Hire_Hero_Popup()
        Left-Click Reject Button
        Left-Click Hire / Accept Button
    Item_Screen()
        Right-Click Hero Portrait - Hero Unit Statistics Window
        Right-Click Hero Item
        Right-Click Vault Item
    Spellbook_Screen()
        Hot-Key ESCAPE
        Hot-Key Page Forward
        Hot-Key Page Backward
        

## Play_Left_Click()

XREF:  (110)
    City_Screen__WIP()
    Enemy_City_Screen()
    Production_Screen()
    Main_Screen()
    CityList_Screen()
    ArmyList_Screen()
    Road_Build_Screen()
    IDK_Spell_Casting_Screen()
    Unit_Statistics_Popup()
    UV_Handle_Arrow_Buttons()
    Magic_Screen()
    Alchemy_Popup()
    Mirror_Screen()
    Item_Screen()
    Powergraph_Screen()
    Status_Screen()
    IDK_Cartographer_Screen()
    Outpost_Screen()
    Item_View_Popup()
    Tactical_Combat__STUB()
    Surveyor_Screen()
    CMB_ShowSpellbook()
    Hire_Hero_Popup()
    Hire_Merc_Popup()
    Spellbook_Screen()
    IDK_Spell_DisjunctOrBind()
    IDK_DiploScrn()



## Play_Standard_Click()

XREF:  (2)
    CMB_CounterMessage()
    Warn()



## Play_Left_Click__DUPE()

XREF:  (17)
    Main_Screen()
    Item_Screen()
    CMB_ShowSpellbook()
    Chancellor_Screen__STUB()
    Hire_Hero_Popup()
    Hire_Merc_Popup()
    Spellbook_Screen()
    IDK_Spell_DisjunctOrBind()
