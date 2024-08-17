








https://masterofmagic.fandom.com/wiki/Objective
Base Priorities
The wizard will use the base priorities when selecting what buildings to build in their cities.
Version	None	Money	Religion	Resrch	Military	Prod.	Food	Naval
1.31	10	15	30	10	10	15	15	10




MoO2
    Module: AIBUILD
        Empire_Buy_Outright_()
            |-> Get_Weighted_Choice_Short_(&weights[0], _NUM_COLONIES)
            |-> Colony_Buy_Outright_()

Per `MoO2`, ...
    Module: AIBUILD
        Player_Colony_Autobuild_()
            |->  Assign_Colony_New_Building_()

Apply_Colony_Changes_()
    |-> Apply_Production_()
        |-> Player_Colony_Autobuild_()
            |->  Assign_Colony_New_Building_()

Assign_Buildings_()
    |-> Assign_Colony_Building_()
        |->  Assign_Colony_New_Building_()

Colony_AI_Given_Tax_()
    |-> Assign_Colony_Building_()
        |->  Assign_Colony_New_Building_()

Next_Turn_Proc_()
    |-> All_Colony_AI_()
        |-> Colony_AI_()
            |-> Assign_Buildings_()
                |-> Assign_Colony_Building_()
