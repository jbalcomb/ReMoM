



What is all this?


e.g., `X` from 'City Screen'

Clear_Screens()

Util.c
// WZD s22p04
// drake178: VGA_FlushFrames()
// MoO2  DNE
OON XREF:  City_Screen__WIP()
void Clear_Screens(void)
    Set_Page_On();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
    Set_Page_Off();
    Fill(SCREEN_XMIN, SCREEN_YMIN, SCREEN_XMAX, SCREEN_YMAX, ST_TRANSPARENT);
~== Cls_On(); Cls_Off();



// WZD s17p43
void ST_PSTRM(int16_t x, int16_t y, int16_t val, char * string);

// WZD s17p44
void Disp_PSTR_Wrapper(int16_t x, int16_t y, char * buffer);

// WZD s17p45
void PSTRS(int16_t x, int16_t y, char * string);

// WZD s17p46
void ST_PSTR(int x, int y, int val);

// WZD s17p47
void PSTRL(int16_t x, int16_t y, int32_t val);

// WZD s17p48
void PSTRU(int16_t x, int16_t y, uint32_t value);

// WZD s17p49
void Disp_PSTR(int16_t x, int16_t y, char * buffer);
