#ifndef MOX_MOM_H
#define MOX_MOM_H

#include <stdint.h>


#define EMM_PAGES_REQUIRED 158
#define EMM_MIN_KB 2700  // 168 pages ? 168 = 158 + (1 + 5 + 4) ?
#define RAM_MIN_KB 583

#define MOM_FONT_FILE "FONTS.LBX"
// #define MOM_FONT_FILE "FoNtS.LbX"
#define GAME_FONT_FILE MOM_FONT_FILE

extern char _init_game_message[];
extern char _leave_game_message[];

extern char mainscrn_lbx_file[];                // MGC  dseg:28A7

extern char vortex_lbx_file[];                  // MGC  dseg:28B0



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */



#ifdef __cplusplus
}
#endif

#endif /* MOX_MOM_H */
