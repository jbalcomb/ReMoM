
#include "sdl2_boolvec.h"

#include "sdl2_KD.h"



/* -------------------------------------------------------------------------- */

#define KBD_BUFSIZE 16
static struct {
    bool full;
    int head;
    int tail;
    uint32_t mod;
    uint32_t buf[KBD_BUFSIZE];
    BOOLVEC_DECLARE(pressed, MOX_KEY_OVERRUN);
} kbd = { 0 };

/* -------------------------------------------------------------------------- */

void kbd_clear(void)
{
    kbd.full = false;
    kbd.head = 0;
    kbd.tail = 0;
    kbd.mod = 0;
    BOOLVEC_CLEAR(kbd.pressed, MOX_KEY_OVERRUN);
}

void kbd_add_keypress(int key, uint32_t mod, char c)
{
    uint32_t value = ((uint32_t)key) | mod | (((uint32_t)c) << 8);
    if (key == MOX_KEY_OVERRUN) {
        return;
    }
    if (kbd.full) {
        // log_warning("kbd: full while inserting 0x%x\n", value);
    } else {
        kbd.buf[kbd.head] = value;
        if (++kbd.head == KBD_BUFSIZE) { kbd.head = 0; }
        if (kbd.head == kbd.tail) {
            kbd.full = true;
        }
    }
}

// kbd_have_keypress()
bool Keyboard_Status(void)
{
    return kbd.full || (kbd.head != kbd.tail);
}

// kbd_get_keypress()
uint32_t Read_Key(void)
{
    int key = MOX_KEY_UNKNOWN;
    if (Keyboard_Status()) {
        key = (int)kbd.buf[kbd.tail];
        if (++kbd.tail == KBD_BUFSIZE) { kbd.tail = 0; }
        kbd.full = false;
    }
    return key;
}

void kbd_set_pressed(int key, uint32_t mod, bool pressed)
{
    kbd.mod = mod;
    if ((key != MOX_KEY_UNKNOWN) && (key < MOX_KEY_OVERRUN)) {
        BOOLVEC_SET(kbd.pressed, key, pressed);
    }
}

bool kbd_is_pressed(int key, uint32_t modon, uint32_t modoff)
{
    return (((kbd.mod & modon) == modon) && ((kbd.mod & modoff) == 0) && BOOLVEC_IS1(kbd.pressed, key));
}
