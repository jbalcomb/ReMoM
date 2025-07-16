#ifndef SDL2_PFL_H
#define SDL2_PFL_H

#include "MOX/MOX_TYPE.h"

#include <inttypes.h>
#include <stdbool.h>

#include <string.h>

#include <SDL.h>



// ¿ C++ Name Mangled  extern SDL_Window * sdl2_window; ?
extern SDL_Window * sdl2_window;
extern SDL_Renderer * sdl2_renderer;
extern SDL_Surface * sdl2_surface_RGB666;
extern SDL_Surface * sdl2_surface_ARGB8888;
extern SDL_Texture * sdl2_texture;
extern SDL_Texture * sdl2_texture_upscaled;

extern char sdl2_window_title[];

extern SDL_Rect sdl2_blit_rect;

extern uint64_t sdl2_ticks_startup;
extern uint64_t sdl2_frame_count;
extern uint64_t sdl2_ticks_mark_time;
extern uint64_t sdl2_ticks_release_time;

extern uint64_t sdl2_performance_counter;

/*
    Frames Per Second (FPS)
    Millisconds Per Frame

DELAY_EVENT_HANDLE_LIMIT
DELAY_MOUSE_UPDATE_LIMIT

*/
#define MILLISECONDS_PER_SECOND 1000
#define MILLISECONDS_PER_FRAME 55  // ~ IBM-PC - 55 ms per BIOS timer tick
// #define FRAMES_PER_SECOND 20
#define FRAMES_PER_SECOND (MILLISECONDS_PER_SECOND / MILLISECONDS_PER_FRAME)  // ~== (100 0 / 55) = 18.18___


// MOM_PFL.H  extern SDL_Color platform_palette_buffer[256];
// extern bool sdl2_1oom_palette_to_set;

extern int sdl2_window_width;
extern int sdl2_window_height;



// MOM_PFL.H  extern int platform_mouse_button_status;



extern int kilgore_option_mouse_speed;  // ./src/hw/sdl/hwsdl_opt.h



/*
    BEGIN:  ./src/boolvec.h
*/

#define BOOLVEC_DECLARE(_name_, _fnum_) uint8_t _name_[((_fnum_) + 7) / 8]
#define BOOLVEC_CLEAR(_name_, _fnum_) do { if ((_fnum_) <= 8) { (_name_)[0] = 0; } else { memset(&(_name_), 0, sizeof(_name_)); } } while (0)
// #define BOOLVEC_COPY(_named_, _names_, _fnum_) do { if ((_fnum_) <= 8) { (_named_)[0] = (_names_)[0]; } else { memcpy(&(_named_), &(_names_), sizeof(_named_)); } } while (0)
// #define BOOLVEC_COMP(_name1_, _name2_, _fnum_) (((_fnum_) <= 8) ? ((_name1_)[0] == (_name2_)[0]) : (memcmp((_name1_), (_name2_), ((_fnum_) + 7) / 8) == 0))
// #define BOOLVEC_PTRPARAMI(_name_) uint8_t *_name_
// #define BOOLVEC_SET0(_name_, _i_) do { int ti = (_i_); _name_[ti / 8] &= ~(1 << (ti & 7)); } while (0)
// #define BOOLVEC_SET1(_name_, _i_) do { int ti = (_i_); _name_[ti / 8] |=  (1 << (ti & 7)); } while (0)
#define BOOLVEC_SET(_name_, _i_, _v_) do { int ti = (_i_); uint8_t *tp = &(_name_[ti / 8]); if ((_v_)) { *tp |= (1 << (ti & 7)); } else { *tp &= ~(1 << (ti & 7)); } } while (0)
// #define BOOLVEC_TOGGLE(_name_, _i_) do { int ti = (_i_); _name_[ti / 8] ^=  (1 << (ti & 7)); } while (0)
// #define BOOLVEC_IS0(_name_, _i_) ((_name_[(_i_) / 8] & (1 << ((_i_) & 7))) == 0)
#define BOOLVEC_IS1(_name_, _i_) ((_name_[(_i_) / 8] & (1 << ((_i_) & 7))) != 0)
// #define BOOLVEC_IS_CLEAR(_name_, _fnum_) (((_fnum_) <= 8) ? ((_name_)[0] == 0) : (/*FIXME*/0))
// #define BOOLVEC_ONLY1(_name_, _fnum_) (((_fnum_) <= 8) ? (((_name_)[0] != 0) && (((_name_)[0] & ((_name_)[0] - 1)) == 0)) : (/*FIXME*/0))
// 
// #define BOOLVEC_TBL_DECLARE(_name_, _tnum_, _fnum_) uint8_t _name_[(_tnum_)][((_fnum_) + 7) / 8]
// #define BOOLVEC_TBL_PTRPARAMM(_name_, _tnum_) &(_name_[(_tnum_)][0])
// #define BOOLVEC_TBL_CLEAR(_name_, _tnum_, _fnum_) memset(&(_name_), 0, sizeof(_name_))
// #define BOOLVEC_TBL_COPY1(_named_, _names_, _tnum_, _fnum_) BOOLVEC_COPY(_named_[_tnum_], _names_, _fnum_)
// #define BOOLVEC_TBL_SET0(_name_, _tnum_, _i_) BOOLVEC_SET0(_name_[_tnum_], _i_)
// #define BOOLVEC_TBL_SET1(_name_, _tnum_, _i_) BOOLVEC_SET1(_name_[_tnum_], _i_)
// #define BOOLVEC_TBL_SET(_name_, _tnum_, _i_) BOOLVEC_SET(_name_[_tnum_], _i_)
// #define BOOLVEC_TBL_TOGGLE(_name_, _tnum_, _i_) BOOLVEC_TOGGLE(_name_[_tnum_], _i_)
// #define BOOLVEC_TBL_IS0(_name_, _tnum_, _i_) BOOLVEC_IS0(_name_[_tnum_], _i_)
// #define BOOLVEC_TBL_IS1(_name_, _tnum_, _i_) BOOLVEC_IS1(_name_[_tnum_], _i_)

/*
    END:  ./src/boolvec.h
*/



/*
    BEGIN:  ./src/comp.h
*/

// #ifndef MIN
// #define MIN(a, b) (((a) <= (b)) ? (a) : (b))
// #endif
// #ifndef MAX
// #define MAX(a, b) (((a) >= (b)) ? (a) : (b))
// #endif
// #ifndef SETMIN
// #define SETMIN(a, b) do { if ((b) < (a)) { (a) = (b); }} while (0)
// #endif
// #ifndef SETMAX
// #define SETMAX(a, b) do { if ((b) > (a)) { (a) = (b); }} while (0)
// #endif
#ifndef SETRANGE
#define SETRANGE(a, b, c) do { if (((c) <= (b)) || ((b) > (a))) { (a) = (b); } else if ((c) < (a)) { (a) = (c); } } while (0)
#endif
// #define ADDSATT(_v_, _n_, _top_) do { int _t_; _t_ = (_v_) + (_n_); SETMIN(_t_, (_top_)); (_v_) = _t_; } while (0)
// #define SUBSATT(_v_, _n_, _bot_) do { int _t_; _t_ = (_v_) - (_n_); SETMAX(_t_, (_bot_)); (_v_) = _t_; } while (0)
// #define SUBSAT0(_v_, _n_) SUBSATT(_v_, _n_, 0)
// #define TBLLEN(_t_) (sizeof((_t_)) / sizeof((_t_)[0]))
// 
// #define SWAP(x, y) (x ^= y ^= x ^= y)

/*
    END:  ./src/comp.h
*/

#ifdef __cplusplus
extern "C" {
#endif



uint64_t Platform_Get_Millies(void);
void Platform_Sleep_Millies(uint64_t millies);

void Platform_Get_Mouse_Position_XY(int * mx, int * my);

void Platform_Maybe_Move_Mouse(void);



void Platform_Event_Handler(void);

void Platform_Window_Event(SDL_WindowEvent *sdl2_window_event);



/*
    sdl2_KD.C
*/
void hw_textinput_start(void);
void hw_textinput_stop(void);



#ifdef __cplusplus
}
#endif

#endif  /* #define SDL2_PFL_H */
