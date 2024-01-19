
Game-Engine

MoX !~ MoM

MoX !~ PFL




¿ Data Types, Defines, Macros ?
Base-Level, Fundamental?
just put them all in MoX.H?
also share with STU?
and TST?

e.g., SAMB_ptr; ST_FAILURE; GET_2B_OFS()

Platform?
-minimal

MOX_BASE.H?







Allocate
Fields
Draw  (FLIC_Draw)
Fonts
Farload  (LBX_Load)
Mouse
Print
Timer
Video


Dependencies:
LBX_Load
    Allocate


Allocate
    farpeekw()
    farpokew()
    Update_MemFreeWorst_KB()
    Check_Release_Version()
    Exit_With_Message()

Allocation_Error() |-> Check_Release_Version()




Per Segment:

NOT  seg001


seg002  timer

...file exists, file length, ..., ?drive space, delete file, file dat time, current drive, current directory...
seg003  LOF
seg004  DIR

seg005  MoO2 "exit"  exit with size, message, value;  + exe swap; worst free; 
seg006  ¿ set vga text mode; dos print quit; dos print ?
seg007  ¿ check allocation magic; far peek/poke ?
seg008  MoO2 "allocate"
seg009  MoO2 "farload"
seg010  MoO2 "farload"

seg011  File Open, Read, Write, Seek, Close

seg012  EMS/EMM
seg013  EMS/EMM - Startup, ..., UU EMM LBX, UU EMM VGA, EMMDATAH, EMM Error

seg014  ¿ MoO2  init ?
        MoO2  graphics  Set_Window(), ...

seg015  OON  Set Video Mode - VGA 'Mode Y'

seg016  ¿  Fill(), Line(), Tint_Fill(), Gray_Scale_Fill(), etc.

seg017  Fonts  Fonts, Print, ...
seg018  ¿ Fonts - ASM ?
seg019  Paragraph  ¿ MoO2  paragrph  ...  ¿ FMTPARA ? ?
seg020  Palette
seg021  ¿ MoO2 palstore ?

¿ misc, random, strings, struct ?
seg022  Randomize(), .., Swap_Short(), ..., Test_Bit_Field()

seg023  ¿ MoO2  special ? Delta_XY_With_Wrap(), ..., ¿ Interpolate_Line(), Absolute_Interpolate_Line(), Range(), Path(), Path_Wrap(), isqrt() ?

seg024  Debug

seg025  ¿ VGAFILEH Draw/Remap File Animation Frame ?

Video - Page_Flip(), Set_Page_On(), Set_Page_Off(), etc.
seg026  
seg027  
seg028  

Draw, Animate, Bitmap, Remap, 
    ¿ all XREF's to current_video_page ?
seg029
seg030
seg031
seg032
seg033
seg034

seg035  Mouse, Keyboard, Mouse Buffer

seg036  Fields

seg037  _AIL_
seg038  SOUND





¿ Data Segment ?
