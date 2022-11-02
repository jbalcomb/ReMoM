#ifndef MOX_EXIT_H
#define MOX_EXIT_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg005
*/

// s05p01
void Exit_MemBug(void);

// s05p02
void Exit(char * argQuitMessage);
void Exit_With_Message(char * string);

// s05p05
void Update_MemFreeWorst_KB(void);


#ifdef __cplusplus
}
#endif


#endif /* MOX_EXIT_H */
