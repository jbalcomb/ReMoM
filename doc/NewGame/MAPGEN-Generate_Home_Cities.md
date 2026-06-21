MAPGEN-Generate_Home_Cities.md

C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Home_City.asm
C:\STU\devel\STU-Extras\Piethawn\Piethawn\out\MAGIC\ovr051\Generate_Home_City.c

Newgame_Control()
    |-> Init_New_Game();
        ...
        |-> Init_Landmasses(ARCANUS_PLANE);
        |-> Init_Landmasses(MYRROR_PLANE);
        |-> Generate_Landmasses(ARCANUS_PLANE);
        |-> Generate_Landmasses(MYRROR_PLANE);

---
