#ifndef MOUSE_H
#define MOUSE_H

#include "MoX_TYPE.H"



struct s_mouse_list {
    int16_t image_num;
    int16_t center_offset;
    int16_t x1;
    int16_t y1;
    int16_t x2;
    int16_t y2;
};


extern int16_t current_pointer_offset;



#ifdef __cplusplus
extern "C" {
#endif
    /* C and Asm Function Prototypes */


/*
    MGC  seg033
*/


// MGC s33p01
// WZD s35p01
void Set_Mouse_List(int16_t count, struct s_mouse_list * list);

// MGC s33p02
void Check_Mouse_Shape(int16_t x, int16_t y);

// MGC s33p03
int16_t Get_Pointer_Image_Number(void);

// MGC s33p04
int16_t Get_Pointer_Offset(void);


// MGC s33p07
int16_t Init_Mouse_Driver(void);


// MGC s33p13
void MD_Draw_Disable(void);

// MGC s33p14
void MD_Draw_Restore(void);

// MGC s33p18
int16_t Pointer_X(void);

// MGC s33p19
int16_t Pointer_Y(void);



// MGC s33p30
// MGC s33p31
// MGC s33p32
void Save_Mouse(int16_t x, int16_t y);

// MGC s33p34
// MGC s33p35
// MGC s33p36
void Restore_Mouse(void);

// MGC s33p37
// MGC s33p38
// MGC s33p39
// MGC s33p40
void Draw_Mouse(int16_t x, int16_t y);


#ifdef __cplusplus
}
#endif


#endif  /* MOUSE_H */
