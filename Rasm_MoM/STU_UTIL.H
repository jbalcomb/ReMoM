#ifndef STU_UTIL_H
#define STU_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

void debug_log(const char *fmt, ...);
void debug_print(const char *fmt, ...);

void draw_background(void);

void wait_for_any_key(void);

#ifdef __cplusplus
}
#endif

#endif /* STU_UTIL_H */
