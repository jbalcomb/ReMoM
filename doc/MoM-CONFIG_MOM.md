


18 bytes, 9 words

NOTE(JimBalcomb,20221120):
    can't find my notes
    did a mass of reserach on INSTALL.EXE and comparing this to MoO1
NOTE(JimBalcomb,20221120):
    grep'd the entire STU dir, found this
        J:\STU\DBWD\MoM131_I\MoM131-Install-Notes.txt
    Per MoO1,
        "CONFIG.MOO"
        "Keyboard Only"
        "Microsoft Compatible Mouse"



MoO2
Module: sound
AIL_dig_driver
    pointer (4 bytes)
    Address: 02:0017C388
AIL_mdi_driver
    pointer (4 bytes)
    Address: 02:0017C38C



INSTALL.EXE


INSTALL.DAT
Does not run without INSTALL.DAT present,
  prints "Install.dat not found!" and exits.


INSTALL.GIF
Runs without INSTALL.GIF present,
  just does not display image.
				 

MAGIC.EXE
Runs without MAGIC.EXE present,
  but does not show "Configuration" button.

CONFIG.MOM
Runs without CONFIG.MOM present,
  no apparent impact.

If you run it without CONFIG.MOM present and simply accept the defaults, the bytes are:
20 02 20 00 07 00 20 02 20 00 07 00 01 00 00 00 00 00 00 00

If you run it without CONFIG.MOM present and select "No Sound" , the bytes are:
00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00


PS.
There is a mild oddity in that it seems the INSTALL.EXE program writes 20 bytes,
 but in MAGIC.EXE the code to read CONFIF.MOM only reads 18 bytes.



00000000 struc CONFIG_DATA ; (sizeof=0x12, standard type)
00000000 MIDI_IO dw ?
00000002 MIDI_HW dw ?
00000004 MIDI_IRQ dw ?
00000006 SFX_IO dw ?
00000008 SFX_HW dw ?
0000000A SFX_IRQ dw ?
0000000C SFX_DMA dw ?
0000000E Unused_0E dw ?
00000010 Unused_10 dw ?
00000012 ends CONFIG_DATA

CONFIG_DATA struc ; (sizeof=0x12, standard type)
MIDI_IO	dw ?
MIDI_HW	dw ?
MIDI_IRQ dw ?
SFX_IO dw ?
SFX_HW dw ?
SFX_IRQ	dw ?
SFX_DMA	dw ?
Unused_0E dw ?
Unused_10 dw ?
CONFIG_DATA ends





INSTALL.EXE

Configuration

    Music Card Selection
        No Sound
        Adlib or compatible
        Not Applicable
        SoundBlaster (orig.)
        SoundBlaster Pro (early)
        SoundBlaster Pro (later)
        Pro Audio Spectrum
        Pro Audio Spectrum Plus/16
        Not Applicable
        Roland MT-32
        Roland LAPC-1/CM-32L
        General Midi


    Digitized Effects Card Selection
        No Sound
        Not Applicable
        SoundBlaster (orig.)
        SoundBlaster Pro (early)
        SoundBlaster Pro (later)
        Pro Audio Spectrum
        Pro Audio Spectrum Plus/16
        Not Applicable


    Music Card Selection
        No Sound
            N/A
        Adlib or compatible
            N/A
        Not Applicable
            Configure Card
                Sound Card Address
        SoundBlaster (orig.)
            Configure Card
                Sound Card Address
        SoundBlaster Pro (early)
            Configure Card
                Sound Card Address
        SoundBlaster Pro (later)
            Configure Card
                Sound Card Address
        Pro Audio Spectrum
            N/A
        Pro Audio Spectrum Plus/16
            N/A
        Not Applicable
            Configure Card
                Sound Card Address
        Roland MT-32
            Configure Card
                Music Card Address
                Music IRQ
        Roland LAPC-1/CM-32L
            Configure Card
                Music Card Address
                Music IRQ
        General Midi
            Configure Card
                Music Card Address
                Music IRQ


    Digitized Effects Card Selection
        No Sound
            N/A
        Not Applicable
            Configure Card
                Digital Card Address
                Digital IRQ
                Digital DRQ
        SoundBlaster (orig.)
            Configure Card
                Digital Card Address
                Digital IRQ
                Digital DRQ
        SoundBlaster Pro (early)
            Configure Card
                Digital Card Address
                Digital IRQ
                Digital DRQ
        SoundBlaster Pro (later)
            Configure Card
                Digital Card Address
                Digital IRQ
                Digital DRQ
        Pro Audio Spectrum
            Configure Card
                Digital IRQ
                Digital DRQ
        Pro Audio Spectrum Plus/16
            Configure Card
                Digital IRQ
                Digital DRQ
        Not Applicable
            Configure Card
                Digital Card Address
                Digital IRQ
                Digital DRQ



Sound Card Address
    210 Hex
    220 Hex
    230 Hex
    240 Hex
    250 Hex
    260 Hex
    280 Hex
    2c0 Hex


Roland LAPC-1/CM-32L
    Music Card Address
    Music IRQ

Music Card Address
    300 Hex
    330 Hex
    332 Hex
    334 Hex
    336 Hex

Music IRQ
    IRQ 2
    IRQ 3
    IRQ 4
    IRQ 5
    IRQ 6
    IRQ 7

Digital Card Address
    210 Hex
    220 Hex
    230 Hex
    240 Hex
    250 Hex
    260 Hex
    280 Hex
    2c0 Hex

Digital IRQ
    IRQ 2
    IRQ 3
    IRQ 4
    IRQ 5
    IRQ 6
    IRQ 8
    IRQ 9
    IRQ 10
    IRQ 11
    IRQ 12
    IRQ 13
    IRQ 14
    IRQ 15

Digital DRQ
    DRQ 1
    DRQ 2
    DRQ 3


Music Card Selection
    {Sound Card Address} {Music Card Address,Music IRQ}

Digitized Effects Card Selection
    {Digital Card Address, Digital IRQ, Digital DRQ}


e.g.,
    SoundBlaster Pro (later):  Sound Card Address    210
    Not Applicable (2):        Digital Card Address  220
                               Digital IRQ             2
                               Digital DRQ             1
10 02 20 00 02 00 20 02 12 00 02 00 01 00 00 00 00 00 00 00
    SoundBlaster Pro (later):  Sound Card Address    230
    Not Applicable (2):        Digital Card Address  240
                               Digital IRQ             4
                               Digital DRQ             3
30 02 20 00 02 00 40 02 12 00 04 00 03 00 00 00 00 00 00 00

 10 02 20 00 02 00 20 02 12 00 02 00 01 00 00 00 00 00 00 00
 30 02 20 00 02 00 40 02 12 00 04 00 03 00 00 00 00 00 00 00
|-----|           |-----|     |--|  |--|
|-----|           |-----|     |--|  Digital DRQ
|-----|           |-----|     Digital IRQ
|-----|           Digital Card Address
Sound Card Address


    SoundBlaster Pro (later):  Sound Card Address    230
    Not Applicable (2):        Digital Card Address  240
                               Digital IRQ             4
                               Digital DRQ             3
30 02 20 00 02 00 40 02 12 00 04 00 03 00 00 00 00 00 00 00

    Roland MT-32:          Music Card Address  336
                            
    SoundBlaster (orig.):  Digital Card Address  2c0
                           Digital IRQ  15
                           Digital DRQ  2
36 03 30 00 07 00 C0 02 12 00 0F 00 02 00 00 00 00 00 00 00

 30 02 20 00 02 00 40 02 12 00 04 00 03 00 00 00 00 00 00 00
 36 03 30 00 07 00 C0 02 12 00 0F 00 02 00 00 00 00 00 00 00
|-----|     |-----|     |-----|     |-----|     |-----|     |
|-----|     |-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     |-----|     |?????
|-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     Digital DRQ
|-----|     |-----|     |-----|Digital IRQ
|-----|     |-----|     |?????
|-----|     |-----|Digital Card Address
|-----|     |?????
|-----|?????
Sound Card Address
Music Card Address

 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
 36 03 30 00 07 00 C0 02 12 00 0F 00 02 00 00 00 00 00 00 00
|-----|     |-----|     |-----|     |-----|     |-----|     |
|-----|     |-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     |-----|     |?????
|-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     Digital DRQ
|-----|     |-----|     |-----|Digital IRQ
|-----|     |-----|     |?????
|-----|     |-----|Digital Card Address
|-----|     |?????
|-----|?????
Sound Card Address
Music Card Address

SoundBlaster Pro (early)
Roland LAPC-1/CM-32L

 36 03 30 00 07 00 C0 02 12 00 0F 00 02 00 00 00 00 00 00 00
 36 03 31 00 07 00 C0 02 18 00 0F 00 02 00 00 00 00 00 00 00
|-----|     |-----|     |-----|     |-----|     |-----|     |
|-----|     |-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     |-----|     |?????
|-----|     |-----|     |-----|     |-----|?????
|-----|     |-----|     |-----|     Digital DRQ
|-----|     |-----|     |-----|Digital IRQ
|-----|     |-----|     |?Card ID?
|-----|     |-----|Digital Card Address
|-----|     |Music IRQ
|-----|?Card ID?
Sound Card Address
Music Card Address

Sound Card Address / Music Card Address
ID - Sound Card / Music Card
Music IRQ
Digital Card Address
ID - Digital Card
Digital IRQ
Digital DRQ




36 03 31 00 06 00 C0 02 18 00 0F 00 02 00 00 00 00 00 00 00
     |-----|           |-----|

36 03 31 00 06 00 C0 02 20 00 0F 00 02 00 00 00 00 00 00 00
     |-----|           |-----|

30 03 38 00 02 00 20 02 12 00 03 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

20 02 12 00 03 00 20 02 12 00 03 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

Adlib or compatible
Not Applicable (1)
00 00 10 00 00 00 20 02 12 00 03 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

Not Applicable (1)
SoundBlaster (orig.)
20 02 12 00 03 00 20 02 12 00 03 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

SoundBlaster (orig.)
Pro Audio Spectrum
20 02 12 00 03 00 88 03 19 00 07 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

SoundBlaster Pro (early)
Pro Audio Spectrum Plus/16
20 02 18 00 07 00 88 03 21 00 05 00 03 00 00 00 00 00 00 00
     |-----|           |-----|

SoundBlaster Pro (later)
Not Applicable (2)
20 02 20 00 07 00 20 02 12 00 03 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

Pro Audio Spectrum
Pro Audio Spectrum
00 00 19 00 00 00 88 03 19 00 07 00 01 00 00 00 00 00 00 00
     |-----|           |-----|

Pro Audio Spectrum Plus/16
Pro Audio Spectrum Plus/16
00 00 21 00 00 00 88 03 21 00 05 00 03 00 00 00 00 00 00 00
     |-----|           |-----|

Music Card Selection
00 00   No Sound
10 00   Adlib or compatible
12 00   Not Applicable
12 00   SoundBlaster (orig.)
18 00   SoundBlaster Pro (early)
20 00   SoundBlaster Pro (later)
19 00   Pro Audio Spectrum
21 00   Pro Audio Spectrum Plus/16
12 00   Not Applicable
30 00   Roland MT-32
31 00   Roland LAPC-1/CM-32L
38 00   General Midi


Digitized Effects Card Selection
00 00   No Sound
12 00   Not Applicable
12 00   SoundBlaster (orig.)
18 00   SoundBlaster Pro (early)
20 00   SoundBlaster Pro (later)
19 00   Pro Audio Spectrum
21 00   Pro Audio Spectrum Plus/16
12 00   Not Applicable

ID - Card Selection
    00 00   No Sound
    10 00   Adlib or compatible
    12 00   SoundBlaster (orig.)
            Not Applicable (1)
            Not Applicable (2)
    18 00   SoundBlaster Pro (early)
    19 00   Pro Audio Spectrum
    20 00   SoundBlaster Pro (later)
    21 00   Pro Audio Spectrum Plus/16
    30 00   Roland MT-32
    31 00   Roland LAPC-1/CM-32L
    38 00   General Midi





J:\STU\DBWD\developc\AIL214\AIL.H

#define XMIDI_DRVR 3        /* equates for drvr_desc.drvr_type values    */
#define DSP_DRVR 2

typedef int HDRIVER;        /* handle to driver                          */

typedef struct
{
   unsigned min_API_version;
   unsigned drvr_type;
   char data_suffix[4];
   void far *dev_name_table;
   int default_IO;
   int default_IRQ;
   int default_DMA;
   int default_DRQ;
   int service_rate;
   unsigned display_size;
}  
drvr_desc;

drvr_desc far * cdecl far AIL_describe_driver(HDRIVER driver);



J:\STU\DBWD\developc\AIL214\AIL.ASM

PUBLIC AIL_describe_driver

;*****************************************************************************
AIL_describe_driver PROC HDrvr

                push SEG AIL_interrupt_divisor
                push OFFSET AIL_interrupt_divisor
                push [HDrvr]
                mov ax,AIL_DESC_DRVR
                call call_driver
                add sp,6
                ret
                ENDP


;*****************************************************************************
call_driver     PROC                    ;Call function AX in specified driver
                                        ;(Warning: re-entrant procedure!)
                mov bx,sp
                mov bx,ss:[bx+4]        ;get handle

                call find_proc

                cmp ax,0
                jne __do_call
                cmp dx,0
                je __invalid_call

__do_call:      push dx                 ;call driver function via stack 
                push ax
                retf                    

__invalid_call: ret                     ;return DX:AX = 0 if call failed

                ENDP


;*****************************************************************************
;*                                                                           *
;* Internal procedures                                                       *
;*                                                                           *
;*****************************************************************************

find_proc       PROC                    ;Return addr of function AX in driver
                                        ;BX (Note: reentrant!)
                cmp bx,16
                jae __bad_handle        ;exit sanely if handle invalid
                shl bx,1                ;(a legitimate action for apps)
                shl bx,1
                les bx,index_base[bx]   ;ES:BX -> driver procedure table
                mov cx,es
                or cx,bx
                jz __bad_handle         ;handle -> unreg'd driver, exit

__find_proc:    mov cx,es:[bx]          ;search for requested function in 
                cmp cx,ax               ;driver procedure table
                je __found
                add bx,4
                cmp cx,-1               
                jne __find_proc

                mov ax,0                ;return 0: function not available
                mov dx,0
                ret

__bad_handle:   mov ax,0                ;return 0: invalid driver handle
                mov dx,0
                ret                                                     

__found:        mov ax,es:[bx+2]        ;get offset from start of driver
                mov dx,es               ;get segment of driver (org = 0)
                ret

                ENDP

...
index_base      dd 16 dup (?)           ;Driver table base addresses

...
AIL_startup     PROC                    ;Initialize AIL API
                USES ds,si,di

                pushf
                cli

                mov active_timers,0     ;# of registered timers
                mov timer_busy,0        ;timer re-entrancy protection

                IF DIGPAK
                mov DP_registered,0
                ENDIF

                cld
                mov ax,cs
                mov es,ax
                lea di,index_base
                mov cx,16*2
                mov ax,0
                rep stosw
