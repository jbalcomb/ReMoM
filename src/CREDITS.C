/*
    MAGIC.EXE ovr060
*/

#include "CREDITS.H"

#include "MOX/Allocate.H"
#include "MOX/LBX_Load.H"
#include "MOX/MOX_DAT.H"
#include "MOX/MOX_DEF.H"
#include "MOX/MOX_TYPE.H"
#include "MOX/SOUND.H"

#include "MGC_DATA.H"



#define CREDITS_COUNT 48
#define CREDITS_LINE_COUNT 10
#define CREDITS_FONT_HEIGHT 10
#define CREDITS_FONT_WIDTH 4
#define CREDITS_COLUMN_1_X 0
#define CREDITS_COLUMN_2_X 105



// MGC dseg:38D8                                                 BEGIN:  MGC seg060 - Initialized Data  (Credits)

// MGC dseg:38D8
int16_t _credit_num = 0;
// MGC dseg:38DA
int16_t _credits_y = 0;
// MGC dseg:38DC
int16_t _credits_timer = 0;
// MGC dseg:38DE
// _credits
// CreditsPairs <offset cnst_Credits_00, offset cnst_Credits_01>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_20, offset cnst_Credits_21>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_31>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_01>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>);
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>);
// CreditsPairs <offset cnst_Credits_80, offset cnst_Credits_81>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_100, offset cnst_Credits_101>; 
// CreditsPairs <offset cnst_Credits_110, offset cnst_Credits_111>; 
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_121>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_131>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_141>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_151>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_51>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_180, offset cnst_Credits_181>;
// CreditsPairs <offset cnst_Credits_190, offset cnst_Credits_191>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_210, offset cnst_Credits_211>;
// CreditsPairs <offset cnst_Credits_220, offset cnst_Credits_221>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_231>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_241>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_251>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_261>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_271>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_281>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_291>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_301>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_311>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_321>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_331>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_341>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_351>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_361>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_380, offset cnst_Credits_381>;
// CreditsPairs <offset cnst_Credits_390, offset cnst_Credits_391>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_401>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_Credits_411>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// CreditsPairs <offset cnst_Credits_430, offset cnst_Credits_431>;
// CreditsPairs 2 dup(<offset cnst_ZeroString_6, offset cnst_ZeroString_6>);
// CreditsPairs <offset cnst_Credits_460, offset cnst_Credits_461>;
// CreditsPairs <offset cnst_ZeroString_6, offset cnst_ZeroString_6>;
// MGC dseg:399E 47 61 6D 65 20 44 65 73 69 67 6E 65 72          cnst_Credits_00 db 'Game Designer'      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39AB 00                                              cnst_ZeroString_6 db 0                  ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39AC 53 74 65 76 65 20 42 61 72 63 69 61 00          cnst_Credits_01 db 'Steve Barcia',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39B9 50 72 6F 67 72 61 6D 6D 65 72 73 00             cnst_Credits_20 db 'Programmers',0      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39C5 4A 69 6D 20 43 6F 77 6C 69 73 68 61 77 00       cnst_Credits_21 db 'Jim Cowlishaw',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39D3 4B 65 6E 20 42 75 72 64 00                      cnst_Credits_31 db 'Ken Burd',0         ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39DC 47 72 69 73 73 65 6C 20 42 61 72 63 69 61 00    cnst_Credits_51 db 'Grissel Barcia',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39EB 50 72 6F 64 75 63 65 72 00                      cnst_Credits_80 db 'Producer',0         ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:39F4 44 6F 75 67 20 43 61 73 70 69 61 6E 2D 4B 61 75+cnst_Credits_81 db 'Doug Caspian-Kaufman',0
// MGC dseg:39F4 66 6D 61 6E 00                                                                          ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A09 41 72 74 20 44 69 72 65 63 74 6F 72 00          cnst_Credits_100 db 'Art Director',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A16 4A 65 66 66 20 44 65 65 00                      cnst_Credits_101 db 'Jeff Dee',0        ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A1F 41 72 74 69 73 74 73 00                         cnst_Credits_110 db 'Artists',0         ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A27 53 68 65 6C 6C 79 20 48 6F 6C 6C 65 6E 00       cnst_Credits_111 db 'Shelly Hollen',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A35 41 6D 61 6E 64 61 20 44 65 65 00                cnst_Credits_121 db 'Amanda Dee',0      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A40 53 74 65 76 65 20 41 75 73 74 69 6E 00          cnst_Credits_131 db 'Steve Austin',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A4D 47 65 6F 72 67 65 20 50 75 72 64 79 00          cnst_Credits_141 db 'George Purdy',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A5A 50 61 74 72 69 63 6B 20 4F 77 65 6E 73 00       cnst_Credits_151 db 'Patrick Owens',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A68 4D 75 73 69 63 20 50 72 6F 64 75 63 65 72 00    cnst_Credits_180 db 'Music Producer',0  ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A77 54 68 65 20 46 61 74 20 4D 61 6E 00             cnst_Credits_181 db 'The Fat Man',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A83 43 6F 6D 70 6F 73 65 72 00                      cnst_Credits_190 db 'Composer',0        ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A8C 44 61 76 65 20 47 6F 76 65 74 74 00             cnst_Credits_191 db 'Dave Govett',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3A98 51 41 20 4C 65 61 64 00                         cnst_Credits_210 db 'QA Lead',0         ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AA0 44 65 73 74 69 6E 20 53 74 72 61 64 65 72 00    cnst_Credits_211 db 'Destin Strader',0  ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AAF 50 6C 61 79 20 54 65 73 74 00                   cnst_Credits_220 db 'Play Test',0       ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AB9 4D 69 6B 65 20 42 61 6C 6F 67 68 00             cnst_Credits_221 db 'Mike Balogh',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AC5 44 61 6D 6F 6E 20 48 61 72 72 69 73 00          cnst_Credits_231 db 'Damon Harris',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AD2 47 65 6F 66 66 20 47 65 73 73 6E 65 72 00       cnst_Credits_241 db 'Geoff Gessner',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AE0 54 61 6D 6D 79 20 54 61 6C 62 6F 74 74 00       cnst_Credits_251 db 'Tammy Talbott',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AEE 4D 69 63 6B 20 55 68 6C 00                      cnst_Credits_261 db 'Mick Uhl',0        ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3AF7 4A 69 6D 20 48 65 6E 64 72 79 00                cnst_Credits_271 db 'Jim Hendry',0      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B02 46 72 61 6E 6B 20 42 72 6F 77 6E 00             cnst_Credits_281 db 'Frank Brown',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B0E 4A 69 6D 20 54 72 69 63 61 72 69 6F 00          cnst_Credits_291 db 'Jim Tricario',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B1B 4A 65 6E 20 4D 61 63 4C 65 61 6E 00             cnst_Credits_301 db 'Jen MacLean',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B27 42 72 69 61 6E 20 57 69 6C 73 6F 6E 00          cnst_Credits_311 db 'Brian Wilson',0    ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B34 42 72 69 61 6E 20 48 65 6C 6C 65 73 6F 6E 00    cnst_Credits_321 db 'Brian Helleson',0  ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B43 4A 65 66 66 20 44 69 6E 67 65 72 00             cnst_Credits_331 db 'Jeff Dinger',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B4F 43 68 72 69 73 20 42 6F 77 6C 69 6E 67 00       cnst_Credits_341 db 'Chris Bowling',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B5D 43 68 61 72 6C 65 73 20 42 72 75 62 61 6B 65 72+cnst_Credits_351 db 'Charles Brubaker',0
// MGC dseg:3B5D 00                                                                                      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B6E 54 6F 6D 20 48 75 67 68 65 73 00                cnst_Credits_361 db 'Tom Hughes',0      ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B79 53 6F 75 6E 64 20 45 66 66 65 63 74 73 00       cnst_Credits_380 db 'Sound Effects',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B79                                                                                         ; could use dseg:2947 or the other way around
// MGC dseg:3B87 4D 69 64 69 61 6E 00                            cnst_Credits_381 db 'Midian',0          ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B8E 53 70 65 65 63 68 00                            cnst_Credits_390 db 'Speech',0          ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B95 4D 61 72 6B 20 52 65 69 73 00                   cnst_Credits_391 db 'Mark Reis',0       ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3B9F 50 65 74 65 72 20 57 6F 6F 64 73 00             cnst_Credits_401 db 'Peter Woods',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3BAB 44 61 76 69 64 20 45 6C 6C 69 73 00             cnst_Credits_411 db 'David Ellis',0     ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3BB7 4D 61 6E 75 61 6C 00                            cnst_Credits_430 db 'Manual',0          ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3BBE 50 65 74 72 61 20 53 63 68 6C 75 6E 6B 00       cnst_Credits_431 db 'Petra Schlunk',0   ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3BCC 53 70 65 63 69 61 6C 20 54 68 61 6E 6B 73 00    cnst_Credits_460 db 'Special Thanks',0  ; DATA XREF: dseg:TBL_Credits_Textso
// MGC dseg:3BDB 4A 65 6E 6E 79 20 43 6F 77 6C 69 73 68 61 77 00 cnst_Credits_461 db 'Jenny Cowlishaw',0 ; DATA XREF: dseg:TBL_Credits_Textso

// MGC dseg:3BEB 2E 00                                           
char cnst_Credits_DOT[] = ".";

static char * credit_strings[CREDITS_COUNT][2] = {
    {"Game Designer", "Steve Barcia"},
    {"", ""},
    {"Programmers", "Jim Cowlishaw"},
    {"", "Ken Burd"},
    {"", "Steve Barcia"},
    {"", "Grissel Barcia"},
    {"", ""},
    {"Producer", "Doug Caspian-Kaufman"},
    { "", "" },
    { "Art Director", "Jeff Dee" },
    { "Artists", "Shelly Hollen" },
    { "", "Amanda Dee" },
    { "", "Steve Austin" },
    { "", "George Purdy" },
    { "", "Patrick Owens" },
    { "", "Grissel Barcia" },
    { "", "" },
    { "Music Producer", "The Fat Man" },
    { "Composer", "Dave Govett" },
    { "", "" },
    { "QA Lead", "Destin Strader" },
    { "Play Test", "Mike Balogh" },
    { "", "Damon Harris" },
    { "", "Geoff Gessner" },
    { "", "Tammy Talbott" },
    { "", "Mick Uhl" },
    { "", "Jim Hendry" },
    { "", "Frank Brown" },
    { "", "Jim Tricario" },
    { "", "Jen MacLean" },
    { "", "Brian Wilson" },
    { "", "Brian Helleson" },
    { "", "Jeff Dinger" },
    { "", "Chris Bowling" },
    { "", "Charles Brubaker" },
    { "", "Tom Hughes" },
    { "", "" },
    { "Sound Effects", "Midian" },
    { "Speech", "Mark Reis" },
    { "", "Peter Woods" },
    { "", "David Ellis" },
    { "", "" },
    { "Manual", "Petra Schlunk" },
    { "", "" },
    { "", "" },
    { "Special Thanks", "Jenny Cowlishaw" },
    { "", "" },
};

// IDGI  struct s_CREDIT_PAIR TBL_Credits_Texts[] = {
// IDGI      {"Game Designer", "Steve Barcia"},
// IDGI      {"", ""},
// IDGI      {"Programmers", "Jim Cowlishaw"}
// IDGI  };

// static char *emm_error_strings[] = {
//     "no error",
//     "EMM software malfunction",
//     "EMM hardware malfunction",
//     "RESERVED",
//     "Invalid EMM handle",
//     "Invalid EMM function code",
//     "All EMM handles being used",
//     "Save/restore page mapping context error",
//     "Not enough expanded memory pages",
//     "Not enough unallocated pages",
//     "Can not allocate zero pages",
//     "Logical page out of range",
//     "Physical page out of range",
//     "Page mapping hardware state save area full",
//     "Page mapping hardware state save area already has handle",
//     "No handle associated with the page mapping hardware state save area",
//     "Invalid subfunction"
//    };

// MGC dseg:3BED 00                                              align 2

// MGC dseg:3BEB                                                 END:  MGC seg060 - Initialized Data  (Credits)



// MGC dseg:948C                                                 BEGIN:  MGC seg060 - Uninitialized Data  (Credits)

// MGC dseg:948C
// ¿ MoO2  Module: MAINMENU  data (0 bytes) _credits  Address: 02:0019406C ?
SAMB_ptr _credits_pict_seg;

// MGC dseg:948C                                                 END:  MGC seg060 - Uninitialized Data  (Credits)




// MGC o60p1
// drake178: ¿ ?
// MoO2  Module: MAINMENU  Draw_Credits_()
// 1oom: DNE
/*
increases the credit roll timer and, if it is above
70, composes and overlays a corresponding credits
text image into the current draw frame
MGC_DEF.H
*/
/*

*/
void Draw_Credits(void)
{
    int16_t string_width = 0;
    int16_t itr = 0;
    uint8_t _fade_in_colors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t _fade_out_colors[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t colors[4] = {0, 0, 0, 0};
    int16_t line_top = 0;  // _SI_
    int16_t itr_string_width = 0;  // _DI_

    _credits_timer++;

    if(_credits_timer > 30000)
    {
        _credits_timer = 70;
    }

    if(_credits_timer < 70)
    {
        return;
    }

    colors[0] = 68;

    for(itr = 0; itr < CREDITS_LINE_COUNT; itr++)
    {
        _fade_out_colors[(itr)] = (224 + itr);
    }

    for(itr = 0; itr < CREDITS_LINE_COUNT; itr++)
    {
        _fade_in_colors[(itr)] = (233 - itr);
    }

    _fade_in_colors[0] = 232;

    Create_Picture(280, 122, _credits_pict_seg);

    Set_Window(0, 40, 319, 137);

    line_top = _credits_y;

    for(itr = 0; itr < CREDITS_LINE_COUNT; itr++)
    {

        if(
            (line_top >= 99)
            ||
            ((_credit_num + itr) >= CREDITS_COUNT)
        )
        {
             continue;
        }

        colors[1] = 224;

        if(line_top < 10)
        {

            colors[1] = _fade_out_colors[(CREDITS_LINE_COUNT - line_top)];
            
        }

        if(line_top > 88)
        {

            colors[1] = _fade_in_colors[(CREDITS_LINE_COUNT - (line_top - 88))];

        }

        Set_Font_Colors_15(2, &colors[0]);

        Print_To_Bitmap(CREDITS_COLUMN_1_X, line_top, credit_strings[(_credit_num + itr)][0], _credits_pict_seg);

        string_width = Get_String_Width(credit_strings[(_credit_num + itr)][0]);

        if(string_width > 1)
        {
            for(itr_string_width = (string_width + 1); itr_string_width < 102; itr_string_width += CREDITS_FONT_WIDTH)
            {
                Print_To_Bitmap(itr_string_width, line_top, cnst_Credits_DOT, _credits_pict_seg);
            }
        }

        Print_To_Bitmap(CREDITS_COLUMN_2_X, line_top, credit_strings[(_credit_num + itr)][1], _credits_pict_seg);

        line_top += CREDITS_FONT_HEIGHT; // ¿ font height ?

    }

    _credits_y--;

    if(_credits_y < 1)
    {

        _credit_num++;

        _credits_y += 10;  // ¿ font height ?

        if(_credit_num > CREDITS_COUNT)
        {
            _credits_timer = 0;
            _credit_num = 0;
            _credits_y = 95;
        }
        
    }

    Draw_Picture_Windowed(85, 35, _credits_pict_seg);

    Reset_Window();

}


// MGC o60p2
// drake178: ¿ ?
// MoO2  Module: MAINMENU  ()
// 1oom: DNE
/*
allocates and prepares an image segment for composing
the rolling credits text (size 280x122) as the first
allocation in the sandbox, and resets the drawing
control values
*/
/*

*/
void Init_Credits(void)
{
    _credit_num = 0;
    _credits_y = 95;
    _credits_pict_seg = Allocate_First_Block(_screen_seg, 3125);  // 3125 PR, 50000 B
    Create_Picture(280, 122, _credits_pict_seg);
}
