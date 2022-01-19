
struct HLP_ENTRY //; (sizeof=0xA, standard type)
{
    int Entry_Index;
    int X1;
    int Y1;
    int X2;
    int Y2;
};

/*
    Uninitialized Data
*/

// 87A4 - 85B0 = 1F4 (34724 - 34224 = 500) ... ? ~= 50 HLP_ENTRY ?
// 863B - 85B0 =  8B (34363 - 34224 = 139) ... ? nothing ?
//extern struct HLP_ENTRY HLP_Entry_Table[]; // dseg:85B0
extern int HLP_IDX_0;    // dseg:85B0
extern int HLP_IDX_0_X1; // dseg:85B2
extern int HLP_IDX_0_Y1; // dseg:85B4
extern int HLP_IDX_0_X2; // dseg:85B6
extern int HLP_IDX_0_Y2; // dseg:85B8
extern int HLP_IDX_1;    // dseg:85BA
extern int HLP_IDX_1_X1; // dseg:85BC
extern int HLP_IDX_1_Y1; // dseg:85BE
extern int HLP_IDX_1_X2; // dseg:85C0
extern int HLP_IDX_1_Y2; // dseg:85C2
extern int HLP_IDX_2;    // dseg:85C4
extern int HLP_IDX_2_X1; // dseg:85C6
extern int HLP_IDX_2_Y1; // dseg:85C8
extern int HLP_IDX_2_X2; // dseg:85CA
extern int HLP_IDX_2_Y2; // dseg:85CC
extern int HLP_IDX_3;    // dseg:85CE
extern int HLP_IDX_3_X1; // dseg:85D0
extern int HLP_IDX_3_Y1; // dseg:85D2
extern int HLP_IDX_3_X2; // dseg:85D4
extern int HLP_IDX_3_Y2; // dseg:85D6
extern int HLP_IDX_4;    // dseg:85D8
extern int HLP_IDX_4_X1; // dseg:85DA
extern int HLP_IDX_4_Y1; // dseg:85DC
extern int HLP_IDX_4_X2; // dseg:85DE
extern int HLP_IDX_4_Y2; // dseg:85E0
// dseg:85E2 db    0
// ...
// dseg:863B db    0
// dseg:863C HLP_IDX_15 db    0
// dseg:863D db    0
// ...
// dseg:87A3 db    0
