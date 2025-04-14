



MS Windows
SDL2

1oom
chocolate-doom
OpenXcom
OpenPoP



# MWA_ReMoM

uint32_t tick_count;

// ~== INT 1A,0 - Read System Clock Timer
uint32_t Get_System_Clock_Counter(void)
{
    uint32_t dos_tick_count = 0;
    DWORD win_tick_count = 0;
    win_tick_count = GetTickCount();  // TODO  "Consider using GetTickCount64() ..."
    // dos_tick_count = win_tick_count / (uint32_t)54.92540;  // TODO(JimBalcomb,20231115): maybe identify castings that are purposeful and/or reasonable... macro? IMA_UINT32
    // dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 2.0);
    dos_tick_count = win_tick_count / (uint32_t)(54.92540 * 1.5);
    return dos_tick_count;
}

// WZD s02p03
void Release_Time(int ticks)
{
    uint32_t current_tick_count = 0;
    do
    {
        current_tick_count = Get_System_Clock_Counter();
    } while(current_tick_count < tick_count + ticks);
}



# 1oom


hw.h
extern uint32_t hw_get_time_us(void);

hwsdl.c
uint32_t hw_get_time_us(void)
{
    return SDL_GetTicks() * 1000;
}



uidelay.h
uidelay.c

hw_get_time_us()



#define DELAY_EVENT_HANDLE_LIMIT    12500
#define DELAY_MOUSE_UPDATE_LIMIT    20000

static uint32_t delay_start;

// ui_delay_prepare()
void Mark_Time(void)
{
    delay_start = hw_get_time_us();
}

// ui_delay_ticks_or_click()
// 110 calls-sites, return-value never used?  bool Release_Time(int ticks)
void Release_Time(int ticks)
{
    return Release_Time_Seconds(MOO_TICKS_TO_US(ticks));
}

// ui_delay_us_or_click()
// bool Release_Time_Seconds(uint32_t delay)
void Release_Time_Seconds(uint32_t delay)
{
    bool pressed = false, handled = false;
    int mx = pointer_x, my = pointer_y;
    uint32_t mouse_time = hw_get_time_us();
    while (1) {
        int32_t diff;
        uint32_t now;
        now = hw_get_time_us();
        diff = now - delay_start;
        if ((diff < 0) || (diff >= delay)) {
            if (!handled) {
                hw_event_handle();
            }
            return false;
        }
        if (diff < DELAY_EVENT_HANDLE_LIMIT) {
            continue;
        }
        hw_event_handle();
        handled = true;
        if (!pressed) {
            if (platform_mouse_button_status) {
                pressed = true;
            }
        } else {
            if (!platform_mouse_button_status) {
                return true;
            }
        }
        if (((mx != pointer_x) || (my != pointer_y)) && ((now - mouse_time) > DELAY_MOUSE_UPDATE_LIMIT)) {
            mouse_time = now;
            mx = pointer_x;
            my = pointer_y;
            ui_cursor_refresh(mx, my);
        }
    }
}
