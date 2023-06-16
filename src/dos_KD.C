
#include "MoX_DEF.H"

#include "Keyboard.H"

#include "Input.H"


// WZD s35p14
int16_t Keyboard_Status(void)
{
    int16_t keyboard_status;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Keyboard_Status()\n", __FILE__, __LINE__);
#endif

// mov     ah, 11h
// int     16h                             ; KEYBOARD - CHECK ENHANCED KEYSTROKE (AT model 339,XT2,XT286,PS)
//                                         ; Return: ZF clear if keystroke available
//                                         ; AH = scan code \ meaningless if ZF = 1
//                                         ; AL = character /
//                                         ; ZF set if kbd buffer empty

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Keyboard_Status() { keyboard_status = %d }\n", __FILE__, __LINE__, keyboard_status);
#endif

    return keyboard_status;
}


// WZD s35p15
uint8_t Read_Key(void)
{
    int16_t scancode_character;
    uint8_t return_key;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: BEGIN: Read_Key()\n", __FILE__, __LINE__);
#endif

// mov     ah, 10h
// int     16h                             ; KEYBOARD - GET ENHANCED KEYSTROKE (AT model 339,XT2,XT286,PS)
//                                         ; Return: AH = scan code, AL = character

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d]: END: Read_Key() { return_key = %d }\n", __FILE__, __LINE__, return_key);
#endif

    return return_key;
}
