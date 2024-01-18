
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
FLIC_Draw
Fonts
LBX_Load
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

MGC
seg002
seg003
seg004
seg005  MoO2 "exit"  exit with size, message, value;  + exe swap; worst free; 
seg006  ¿ set vga text mode; dos print quit; dos print ?
seg007  ¿ check allocation magic; far peek/poke ?
seg008  MoO2 "allocate"
seg009  MoO2 "farload"
seg010  MoO2 "farload"

¿ Data Segment ?
