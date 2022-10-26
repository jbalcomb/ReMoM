#ifndef ST_QUIT_H
#define ST_QUIT_H


#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */

/*
    seg006
*/

// s06p01
void VGA_SetTextMode(void);

// s06p02
void QuitToDOS(char * quitmsg);

// s06p04
unsigned int dos_memfree_kb(void);


#ifdef __cplusplus
}
#endif


#endif /* ST_QUIT_H */
