
#include "MoX_MoM.H"


int RAM_Min_KB = RAM_MIN_KB;                    // MGC  dseg:A5C4  WZD  dseg:E5CE

char _init_game_message[] = "";
char _leave_game_message[] = "Thank you for playing Master of Magic!";


// s10p17
void RAM_SetMinKB(int Min_KB)
{
    RAM_Min_KB = Min_KB;
}
