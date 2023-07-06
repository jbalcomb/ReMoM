
#include "Mox_TYPE.H"

#include "Windows.h"
#include "sysinfoapi.h"

/*
https://learn.microsoft.com/en-us/windows/win32/api/sysinfoapi/nf-sysinfoapi-gettickcount

The return value is the number of milliseconds that have elapsed since the system was started.

The resolution of the GetTickCount function is limited to the resolution of the system timer, which is typically in the range of 10 milliseconds to 16 milliseconds.

The elapsed time is stored as a DWORD value. Therefore, the time will wrap around to zero if the system is run continuously for 49.7 days. To avoid this problem, use the GetTickCount64 function. Otherwise, check for an overflow condition when comparing times.

If you need a higher resolution timer, use a multimedia timer or a high-resolution timer.


1/70 = 0.01428571428571428571428571428571

~DOS - System Clock Counter - incremented approximately 18.206 times per second
resolution of the standard DOS clock, approximately 54.925 milliseconds.


// https://www.freebasic.net/forum/viewtopic.php?t=12985
The BIOS timer tick normally counts 65536 cycles of the 1193182 Hz PIT clock.
65535/60/60 times every second, or about 18.204167 times
1193182 / 65536 = 18.206512451171875


https://github.com/ya-mouse/dos-utils/blob/master/watt32/src/timer.c
lots of cleverness...
hi = PEEKL (0, BIOS_CLK);


https://randomascii.wordpress.com/2013/05/09/timegettime-versus-gettickcount/


https://stackoverflow.com/questions/76149260/synchronizing-a-game-loop-to-a-desired-number-of-ticks-per-second-with-the-win32

*/



uint32_t Get_System_Clock_Counter(void)
{
    uint32_t dos_tick_count;
    DWORD win_tick_count;
    win_tick_count = GetTickCount();
    dos_tick_count = win_tick_count / 1000 / 18;
    return dos_tick_count;
}