


MoX vs. MoM




How to organize these headers?
...MoO1, MoM, MoO2...
...MoM Demo, MoM v1.00, ..., MoM v1.30, MoM v1.31...
...`MoX` vs. `MoX for MoM`...

ReMoM solution
MoX project
MoM project
...\MoX\src
...\MoM\src
...
...ReMoM\src
...ReMoM\src\MoX
...ReMoM\src\MoX\MoM_v131
...ReMoM\src\MoX\MoM_v131\...defs, enums, structs, etc.
vs. #ifdef MOO1100
    #ifdef MOM131
    #ifdef MOO2100
probably, really should just use code-gen


e.g.,
MGC
dseg:6900 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00+_players s_WIZARD 6 dup(<0>)            ; DATA XREF: Save_SAVE_GAM+1BFo ...
Load_...(), Save_...(), MoM_Init_Tables()

dseg:2AD0 4D 65 72 6C 69 6E 00 00 00 00 05 00 00 00 05 00+TBL_Default_Wizards  = WIZARD_Preset ptr $


MoX_MoM.C/.H
MoM_MoX.C/.H
~== MoM <===> MoM_MoX <===> MoX_MoM <===> MoX


MoO2
FILEDEF & INITGAME
..."Saved_Game"..."Settings"...





¿ For Sure ?
MoX
    MoX.C/.H <-> MoX_Lib.C/.H
    Allocate
    (FLIC_)Draw
    EMM
    Exit
    Fields
    file_ani
    Fonts
    GENDRAW
    Graphics
    Input
    IO_Buffer
    (LBX)Load
    Mouse
    MoX_DBG
    paragrph
    SOUND
    TEXTBOX
    Video
    Video2
    ...
    types
    defines
    enums?
    structs?

~== MoX, but specifically/only for MoM?
ALLOC
..., defines, enums, structs, ...
...record sizes...
The settings and config would seem to be prime examples of this conundrum.
    ...structures...filenames...

Progression?
rename MoX_Data.C/.H
rename MoX.C/.H - ...MoX_MoM...MoX_Lib...

¿ weirdly add MoX_STRU, MoX_ENUM, etc. ? just used MoX_DEF? move to MoX_Lib?
...something I saw somewhere just had a massive <lib name>.H with *everything* in there...


eliminate the declarations in MoX.H for the MoX functions
add all MoX headers to MoX_Lib.H
move all MoM includes to MoM.H from MoX.H
...requires moving the modules to the MoX project


GENDRAW *knows* enum e_SPELL_BOOK_REALM, which is currently in MoM_Data.H
