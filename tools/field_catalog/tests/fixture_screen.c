/*
 * fixture_screen.c — synthetic input for the field_catalog unit test.
 * NOT compiled into any target; read only by test_catalog.py. Each call below
 * exercises one resolution path the catalog must get right.
 */
#define BTN_X 100
#define BTN_Y  50

int Fixture_Screen(void)
{
    /* literal rect — folds fully */
    lit_field = Add_Hidden_Field(10, 20, 30, 40, 'Q', ST_UNDEFINED);

    /* macro-based rect — folds via the constant table */
    macro_field = Add_Hidden_Field(BTN_X, BTN_Y, BTN_X + 19, BTN_Y + 9, 0, ST_UNDEFINED);

    /* button — sprite extent, top-left only, char hotkey */
    ok_btn = Add_Button_Field(200, 176, "", ok_button_seg, 'O', ST_UNDEFINED);

    /* bare hotkey — key-addressable, no geometry */
    esc_key = Add_Hot_Key(27);

    /* grid — inclusive last-pixel extent */
    grid_fld = Add_Grid_Field(0, 0, 20, 18, 12, 10, &gx, &gy, ST_UNDEFINED);

    if (some_state > 0)
    {
        for (itr = 0; itr < 5; itr++)
        {
            /* guarded loop-var coord — x folds, y does not -> runtime=1 */
            loop_field = Add_Hidden_Field(50, 60 + itr * 10, 90, 70 + itr * 10, 0, ST_UNDEFINED);
        }
    }

    /* commented-out call — must be ignored */
    // dead_field = Add_Hidden_Field(1, 2, 3, 4, 0, ST_UNDEFINED);

    return 0;
}
