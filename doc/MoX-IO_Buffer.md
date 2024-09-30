



EMM
IO Buffer
File Animation



MGC/WZD seg012 & seg013
    all EMM
    VGAFILEH ~== file_animation_buffer

MoO2
    Create_IO_Buffers() in Module: init
    file_animation_buffer is in Module: file_ani

1oom
    gfx_anim_hdr_seg        ~== file_animation_buffer
    ehandle_vgafileh        ~== _VGAFILEH_seg
    emm_copy_from_handle()  ~== EMM_MapnRead()

C:\STU\developp\1oom\src\ui\classic\uielection.c

struct election_data_s {
    struct election_s *el;
    uint8_t *gfx_cylinder;
    uint8_t *gfx_racem[4];
    uint8_t *gfx_race[PLAYER_NUM];
    bool flag_countdown;
    int count;
};

static void election_load_data(struct election_data_s *d)
    d->gfx_cylinder = lbxfile_item_get(LBXFILE_COUNCIL, 1);
static void ui_election_draw_cb(void *vptr)
    lbxgfx_draw_frame(0, 0, d->gfx_cylinder, UI_SCREEN_W, ui_scale);


MoO1 seg038 ~== MoM seg029
...vs. MoM seg025  Draw_File_Animation_Frame()



; changes the EMM page mapping such that the previous
; second page now becomes the first one
; preserves ax and bx, reduces si by $4000 (1 page)

; int VGA_MapNextEMMPage(void)
proc VGA_MapNextEMMPage near
push    bx


mov     cx, ax


inc     [cs:CS_file_animation_emm_logical_page]


mov     ax, 4400h
mov     bx, [cs:CS_file_animation_emm_logical_page]
mov     dx, [cs:CS_file_animation_emm_handle]
int     67h                             ;  - LIM EMS - MAP MEMORY
                                        ; AL = physical page number (0-3)
                                        ; BX = logical page number, DX = handle
                                        ; Return: AH = status
mov     ax, 4401h
mov     bx, [cs:CS_file_animation_emm_logical_page]
inc     bx
mov     dx, [cs:CS_file_animation_emm_handle]
int     67h                             ;  - LIM EMS - MAP MEMORY
                                        ; AL = physical page number (0-3)
                                        ; BX = logical page number, DX = handle
                                        ; Return: AH = status
sub     si, 16384


mov     ax, cx



; changes the EMM page mapping such that the previous
; last page now becomes the first one
; preserves ax and bx, reduces si by $C000 (3 pages)

; int VGA_MapNextEMMPages(void)
proc VGA_MapNextEMMPages near
push    bx
mov     cx, ax


mov     bx, [cs:Log_Page_seg029]
inc     bx
inc     bx
inc     bx
mov     [cs:Log_Page_seg029], bx
mov     dx, [cs:EMM_Handle_seg029]
push    dx                              ; EMM_Handle
push    bx                              ; First_Log_Page
call    EMM_Map4Pages                   ; maps in four consecutive logical pages from the
                                        ; passed handle, starting with the one specified
                                        ; uses a different EMM function than seg012:0255
                                        ; preserves all register values
add     sp, 4


sub     si, 49152




ALLOC.C
    Allocate_Data_Space()
Allocate.C
    Allocate_Space()
    Allocate_First_Block()
    Allocate_Next_Block()





// https://open-watcom.github.io/open-watcom-v2-wikidocs/cpguide.html

// union REGS regs;
// struct SREGS sregs;

#define DPMI_INT        0x31

struct meminfo
{
	unsigned LargestBlockAvail;
	unsigned MaxUnlockedPage;
	unsigned LargestLockablePage;
	unsigned LinAddrSpace;
	unsigned NumFreePagesAvail;
	unsigned NumPhysicalPagesFree;
	unsigned TotalPhysicalPages;
	unsigned FreeLinAddrSpace;
	unsigned SizeOfPageFile;
	unsigned Reserved[3];
} MemInfo;



struct inregs
{
	struct x
	{
		unsigned long integer eax;
		unsigned long integer ebx;
		unsigned long integer ecx;
		unsigned long integer edx;
		unsigned long integer esi;
		unsigned long integer edi;
		unsigned long integer cflag;
	};
	struct w
	{
		unsigned short integer ax;
		unsigned short integer _1;
		unsigned short integer bx;
		unsigned short integer _2;
		unsigned short integer cx;
		unsigned short integer _3;
		unsigned short integer dx;
		unsigned short integer _4;
		unsigned short integer si;
		unsigned short integer _5;
		unsigned short integer di;
		unsigned short integer _6;
		unsigned long integer cflag;
	};
	struct h
	{
		signed char al;
		signed char ah;
		unsigned short integer _1;
		signed char bl;
		signed char bh;
		unsigned short integer _2;
		signed char cl;
		signed char ch;
		unsigned short integer _3;
		signed char dl;
		signed char dh;
		unsigned short integer _4;
	};
};

struct outregs
{
	struct x
	{
		unsigned long integer eax;
		unsigned long integer ebx;
		unsigned long integer ecx;
		unsigned long integer edx;
		unsigned long integer esi;
		unsigned long integer edi;
		unsigned long integer cflag;
	};
	struct w
	{
		unsigned short integer ax;
		unsigned short integer _1;
		unsigned short integer bx;
		unsigned short integer _2;
		unsigned short integer cx;
		unsigned short integer _3;
		unsigned short integer dx;
		unsigned short integer _4;
		unsigned short integer si;
		unsigned short integer _5;
		unsigned short integer di;
		unsigned short integer _6;
		unsigned long integer cflag;
	};
	struct h
	{
		signed char al;
		signed char ah;
		unsigned short integer _1;
		signed char bl;
		signed char bh;
		unsigned short integer _2;
		signed char cl;
		signed char ch;
		unsigned short integer _3;
		signed char dl;
		signed char dh;
		unsigned short integer _4;
		}
	};

struct header
{
	signed char id;
	signed char version;
	signed char format;
	signed char bitspixelplane;
	signed short integer xmin;
	signed short integer ymin;
	signed short integer xmax;
	signed short integer ymax;
	signed short integer hdpi;
	signed short integer vdpi;
	// array (48 bytes) egapalette
	// Num elements:   48, Type:                        unsigned long integer 
	unsigned long integer egapalette[12];
	signed char reserved;
	signed char numberofplanes;
	signed short integer charslineplane;
	signed short integer paletteinfo;
	signed short integer hscreensize;
	signed short integer vscreensize;
	// array (54 bytes) filler
	// Num elements:   54, Type:                        unsigned long integer 
	unsigned long integer filler[14];
};



/*
Module: capture
    signed short integer release_flag
    Address: 02:0017C374

    function (0 bytes) Screen_Flic_Capture
    Address: 01:001101F0
        Num params: 0
        Return type: void (1 bytes) 
        Locals:
            pointer (4 bytes) ronnie
            struct (28 bytes) inregs
                    struct (28 bytes) x
                            unsigned long integer eax
                            unsigned long integer ebx
                            unsigned long integer ecx
                            unsigned long integer edx
                            unsigned long integer esi
                            unsigned long integer edi
                            unsigned long integer cflag
                    struct (28 bytes) w
                            unsigned short integer ax
                            unsigned short integer _1
                            unsigned short integer bx
                            unsigned short integer _2
                            unsigned short integer cx
                            unsigned short integer _3
                            unsigned short integer dx
                            unsigned short integer _4
                            unsigned short integer si
                            unsigned short integer _5
                            unsigned short integer di
                            unsigned short integer _6
                            unsigned long integer cflag
                    struct (16 bytes) h
                            char al
                            char ah
                            unsigned short integer _1
                            char bl
                            char bh
                            unsigned short integer _2
                            char cl
                            char ch
                            unsigned short integer _3
                            char dl
                            char dh
                            unsigned short integer _4
            struct (28 bytes) outregs
                    struct (28 bytes) x
                            unsigned long integer eax
                            unsigned long integer ebx
                            unsigned long integer ecx
                            unsigned long integer edx
                            unsigned long integer esi
                            unsigned long integer edi
                            unsigned long integer cflag
                    struct (28 bytes) w
                            unsigned short integer ax
                            unsigned short integer _1
                            unsigned short integer bx
                            unsigned short integer _2
                            unsigned short integer cx
                            unsigned short integer _3
                            unsigned short integer dx
                            unsigned short integer _4
                            unsigned short integer si
                            unsigned short integer _5
                            unsigned short integer di
                            unsigned short integer _6
                            unsigned long integer cflag
                    struct (16 bytes) h
                            char al
                            char ah
                            unsigned short integer _1
                            char bl
                            char bh
                            unsigned short integer _2
                            char cl
                            char ch
                            unsigned short integer _3
                            char dl
                            char dh
                            unsigned short integer _4
            signed integer (4 bytes) mode
            char color
            char old_bank
            signed integer (4 bytes) start_x
            signed integer (4 bytes) start_y
            unsigned long integer dumb_number
            unsigned long integer offset
            signed integer (4 bytes) i
            signed integer (4 bytes) new_bank
            char flag
            struct (128 bytes) header
                    char id
                    char version
                    char format
                    char bitspixelplane
                    signed short integer xmin
                    signed short integer ymin
                    signed short integer xmax
                    signed short integer ymax
                    signed short integer hdpi
                    signed short integer vdpi
                    array (48 bytes) egapalette
                    Num elements:   48, Type:                        unsigned long integer 
                    char reserved
                    char numberofplanes
                    signed short integer charslineplane
                    signed short integer paletteinfo
                    signed short integer hscreensize
                    signed short integer vscreensize
                    array (54 bytes) filler
                    Num elements:   54, Type:                        unsigned long integer 
            char count
            array (768 bytes) palette
            Num elements:  768, Type:                unsigned long integer 
            array (30 bytes) file_name
            Num elements:   30, Type:                unsigned long integer 
            array (15 bytes) file_buffer2
            Num elements:   15, Type:                unsigned long integer 
            signed integer (4 bytes) file_num
*/
