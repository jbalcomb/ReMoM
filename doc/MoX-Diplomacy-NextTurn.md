


Next_Turn_Proc()
    |-> Next_Turn_Calc()
        ...
        |-> Diplomacy_Growth_For_Enchantments__WIP();
        ...
        /*
            BEGIN:  NPC Diplomacy
        */
        Diplomacy_Growth();
        // call    j_DIPL_ContactProgress
        // call    j_DIPL_AI_To_AI                 ; many BUGs and INCONSISTENCIES inside ; RE-EXPLORE in more context!
        // call    j_IDK_Dipl_s7373B
        Resolve_Delayed_Diplomacy_Orders();
        // TODO  End_Of_Turn_Diplomacy_Adjustments_
        // TODO  Modifier_Diplomacy_Adjustments()
        /*
            END:  NPC Diplomacy
        */



MoO2
Module: DIPLOMAC
    code (0 bytes) End_Of_Turn_Diplomacy_Adjustments_
        Address: 01:0004DAB2
    code (0 bytes) Diplomacy_Growth_
        Address: 01:0004DD6B
