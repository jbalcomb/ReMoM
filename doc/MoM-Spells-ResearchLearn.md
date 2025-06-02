


NEXTTURN.C
Players_Check_Spell_Research()
    |-> WIZ_LearnSpell__WIP()


Where's the accumulation for _players[itr_players].research_cost_remaining?




## Spell Researching

MoO2
Next_Turn_Calc()
    |-> Apply_All_Player_Changes_()
        |-> Check_For_Research_Breakthrough_()
Module: TECH
    Tech_Select_()

1oom
game_turn_process()
    |-> game_tech_research()
        |-> game_tech_get_new()
        |-> game_tech_finish_new()  ... ¿ same as populating the wizard's research spell list ?
        |-> ui_newtech()

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Update_Magic_Power()
        |-> Players_Apply_Magic_Power()
        |-> Players_Check_Spell_Research()

Players_Apply_Magic_Power() updates _players[].research_cost_remaining

Next_Turn_Proc()
    |-> Next_Turn_Calc()
        |-> Players_Check_Spell_Research()
            |-> WIZ_LearnSpell__WIP()
            |-> Spell_Research_Select()
            |-> AI_Research_Picker__STUB()

