




Hrrrmm... coulda sworn I'd a done bunch of work getting the main menu button highlight code working with the mouse...
but, I don't see nothing about it...
Nop. I did. Found some.
C:\devel\STU-MoM_Rasm\MoM_Rasm\_s34p01c.c
Also, in the STU-MoM_Rasm-NotGit mess, ST_Mouse.md
Looks like I have figured I'd come back for this code after I got the move to ./src and WinAPI done, but I neglected to make a note and then I fergetted.
















Save, Restore, Draw
~== Rectangle, Clipped
    but, rectangle to rectangle, with origin assumed {0,0} and clipped only on bottom and right
    so, src & dst - x1,y1,x2,y2 and src stride and dst stride (e.g., screen stride & cursor stride)
    notably not a masked bitmap style operation
    also, with transparency  (colormap index 0 is transparent index/color)


what I have as _RSP uses draw_page_num
what I have as _DSP uses 1 - draw_page_num
Toggle_Pages() uses _DSP
...
The only XREF to CRL_Save_DSP() is from Toggle_Pages()





? MoO2 "mouse_active" ?





MoM
g_CRSR_Save_DSP
ST_GUI.C:unsigned int g_CRSR_Save_DSP[600];
ST_GUI.H:extern unsigned int g_CRSR_Save_DSP[];

dseg:43B2 00 00 00 00 00 00 00 00+    CR_Save_RSP db 1200 dup(  0)
dseg:4862 00 00 00 00 00 00 00 00+    CR_Save_DSP db 1200 dup(  0)
?
16 * 16 = 256
?
CRL_Copy_DSP2RSP()
    mov     di, offset CR_Save_RSP
    mov     si, offset CR_Save_DSP
    mov     cx, 600                 ; 600 words, 1200 bytes
    rep movsw                       ; Move Byte(s) from String to String

? 75 paragraphs ?




Restore_Mouse_Off_Page_  ~== CRL_Restore_DSP
XREF  Toggle_Pages(), VGA_Copy_RSP_DSP_YM() <-| Copy_On_To_Off_Page()

Copy_On_To_Off_Page  ~==  VGA_Copy_RSP_DSP()()

?
Copy_Off_To_Back()
?
VGA_Copy_RSP_DSP_NM
Copy_On_To_Off_Page - Nay Mouse

VGA_Copy_RSP_DSP_YM

VGA_SaveDraw_Frame3









CR_Save_RSP
XREF  mouse_background_buffer
    Init_Mouse_Driver+45 mov     mouse_background_buffer_pointer, offset mouse_background_buffer; 624 bytes (24 * 26)

Cursor_Save_Bkup
CR_Save_DSP
XREF mouse_off_page_buffer
    Init_Mouse_Driver+4F           mov     mouse_off_page_buffer_pointer, offset mouse_off_page_buffer; 624 bytes (24 * 26)
    Restore_Mouse_From_Off_Page+DB mov     edx, offset mouse_off_page_buffer; 624 bytes (24 * 26)                          
    Save_Mouse_From_Off_Page+78    mov     eax, offset mouse_off_page_buffer; 624 bytes (24 * 26)                          

XREF mouse_background_buffer_pointer
    Init_Mouse_Driver+45       mov     mouse_background_buffer_pointer, offset mouse_background_buffer; 624 bytes (24 * 26)
    Restore_Mouse_Hi_Res_+19C  mov     esi, mouse_background_buffer_pointer                                                
    Restore_Mouse_Off_Page_+AF mov     esi, mouse_background_buffer_pointer                                                
    Save_Mouse_Hi_Res_+1C6     mov     edi, mouse_background_buffer_pointer                                                
    Save_Mouse_Off_Page_+DD    mov     edi, mouse_background_buffer_pointer                                                

XREF mouse_off_page_buffer_pointer
    Init_Mouse_Driver+4F mov     mouse_off_page_buffer_pointer, offset mouse_off_page_buffer; 624 bytes (24 * 26)







? mouse_off_page_buffer ?


Save_Mouse_From_Off_Page()


Save_Mouse_Hi_Res_()
    last_on_page_mouse_x = mouse_x
    last_on_page_mouse_y = mouse_y
    ...
    dst = mouse_background_buffer_pointer


MoO2 module video

dseg02:001B1E38 mouse_background_buffer db 624 dup(?)                                                                 ; 0 ; 624 bytes (24 * 26)
dseg02:001B20A8 mouse_off_page_buffer   db 624 dup(?)                                                                 ; 0 ; 624 bytes (24 * 26)
dseg02:001B38B0 mouse_background_buffer_pointer dd ?
dseg02:001B38B8 mouse_off_page_buffer_pointer   dd ?









MoO2 does not have variables for 'Mouse Driver Installed' or 'Mouse Driver Initialized'
it only has "mouse_enabled", which is the variable (flag) that controls the mouse cursor image drawing routines.
~== MoM g_MD_CursorDraw

MoM:
MD_Init() sets ?  AKA g_MouseDriverInstalled
MD_Init() sets mouse_enabled    AKA g_MD_CDraw
IN_Init() sets mouse_installed  AKA g_MD_Hardware

MoO2 Mouse_Button_Handler() does not have a short-circuit on a flag for mouse/mouse driver enabled/installed/initialized


? g_MouseDriverInstalled is effectively ineffectual ?
? g_MD_Hardware is only used in Mouse Emulation code ?




Up   r MD_GetButtonStatus+7   cmp     [g_MouseDriverInstalled], 0; Compare Two Operands         
Down r MD_INT_SetMvOnly+9     cmp     [g_MouseDriverInstalled], 0; Compare Two Operands         
     w MD_Init+2C             mov     [g_MouseDriverInstalled], e_RE_TRUE                       
Down r MD_MoveCursor+15       cmp     [g_MouseDriverInstalled], 0; Compare Two Operands         
Down r MD_Reset+9             cmp     [g_MouseDriverInstalled], e_RE_FALSE; Compare Two Operands
Down w MD_Reset:@@Done        mov     [g_MouseDriverInstalled], e_RE_FALSE                      
Down r Mouse_Button_Handler+9 cmp     [g_MouseDriverInstalled], 0; Compare Two Operands         
Down r UU_MOUSE_GetHwOrEmu+6  mov     ax, [g_MouseDriverInstalled]                              


Eh? "mouse_installed"

Up   r GUI_EditBoxControl+3F7                  cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_EditBoxControl+D                    cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_EditBoxControl:loc_2829E            cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_MouseEmuMove+B                      cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_ProcessDirKey:@@CheckMouseInstalled cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_TextEditDialog+12                   cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_TextEditDialog+381                  cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r GUI_TextEditDialog:loc_28681            cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r G_GUI_ClearInput+19                     cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   r G_GUI_PressAnyKey:loc_27CDF             cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Down r Get_Input:@@CheckMouseInstalled         cmp     [g_MD_Hardware], 0      ; Compare Two Operands     
Up   w IN_Init+33                              mov     [g_MD_Hardware], 0                                 
Up   w IN_Init+6A                              mov     [g_MD_Hardware], ax                                
     r IN_Init+6D                              cmp     [g_MD_Hardware], e_RE_FAILURE; Compare Two Operands
Down w IN_Init:@@SuccessMouseInit              mov     [g_MD_Hardware], e_ST_TRUE                         
Down r Push_Field_Down:@@CheckMouseHardware    cmp     [g_MD_Hardware], e_ST_FALSE; Compare Two Operands  


Down w MD_Draw_Disable+11            mov     [g_MD_CDraw], bx                              
Down r MD_Draw_Disable+7             mov     bx, [g_MD_CDraw]                              
Down w MD_Draw_Restore+B             mov     [g_MD_CDraw], bx                              
Down r MD_INT_Handler+3E             cmp     [g_MD_CDraw], 0         ; Compare Two Operands
Down w MD_INT_Handler+C6             mov     [g_MD_CDraw], 1                               
Down w MD_INT_Handler:@@DoDrawCursor mov     [g_MD_CDraw], 0                               
Down w MD_INT_SetMvOnly+24           mov     [g_MD_CDraw], 1                               
     w MD_Init+78                    mov     [g_MD_CDraw], 0                               
Down w MD_Reset+14                   mov     [g_MD_CDraw], 0                               
Down w Mouse_Button_Handler+24       mov     [g_MD_CDraw], 1                               


Down w Disable_Mouse+18          mov     word ptr mouse_enabled+2, 0         
Down w Enable_Mouse+18           mov     word ptr mouse_enabled+2, 1         
Up   w Init_Mouse_Driver+1B1     mov     word ptr mouse_enabled+2, e_ST_FALSE
     w Init_Mouse_Driver+227     mov     word ptr mouse_enabled+2, e_ST_TRUE 
Down w Mouse_Button_Handler+6B   mov     word ptr mouse_enabled+2, 1         
Down w Mouse_Movement_Handler+6B mov     word ptr mouse_enabled+2, 1         
Down w Restore_Mouse_State+1E    mov     word ptr mouse_enabled+2, ax        
Down r Save_Mouse_State+18       mov     ax, word ptr mouse_enabled+2        
Down w Save_Mouse_State+24       mov     word ptr mouse_enabled+2, 0         
Down r User_Mouse_Handler+D0     mov     eax, dword ptr mouse_enabled        






MoO2 Save_Mouse_State()
    mouse_state = mouse_enabled+2
    mouse_enabled+2 = ST_FALSE
MoO2 Restore_Mouse_State()
    mouse_enabled+2 = mouse_state

*** ONLY XREF by Toggle_Pages() ***
MoO2 Disable_Mouse()
    mouse_enabled+2 = ST_FALSE
MoO2 Enable_Mouse()
    mouse_enabled+2 = ST_TRUE



# Mouse Cursor Image Management

1. Mouse Move
2. Screen Change

## Mouse Move

Restore Previous
Save Current
Draw Current

## Initialization
{x,y}, image[]

## ¿ Where, What, When ?
Where to draw?
What to draw?
When draw?

Frame 0:  (AKA Initial/First Frame)
Load Cursor Image
Set Cursor Image Number
Initialize {x,y}  (e.g., mid-point/center)
Draw Off/Back
Save Cursor Area    ≜ Previous
Draw Cursor Image   ≜ Current
Copy Off to On
Render/Present

## Toggle_Pages

Save, Draw, Flip, Restore, Copy





current_pointer_image_number  AKA GUI_CurrentCursor
CRH_Draw_RSP()  AKA GUI_DisplayCursor()
CRL_Draw_RSP()  AKA VGA_DisplayCursor()
...
    if !none,  draw
? ~== MoO2 Draw_Mouse_To_Off_Page() ?
    short-circuit in MoO2 is on cursor type == 1
    writes to "__off_page_buffer"
    if old_mouse_x + 24 > screen_pixel_width2 { width = screen_pixel_width2 - old_mouse_x } else { width = 24 }
    if old_mouse_y + 24 > screen_pixel_height2 { height = screen_pixel_height2 - old_mouse_y } else { height = 24 }
Toggle_Pages()
    |-> Draw_Mouse_Off_Page()
        |-> Draw_Mouse_To_Off_Page()


MoO2
mouse_image           =  mouse_palette         +  ( (current_mouse_shape          - 1) * (24 * 24) )
MoM
Cursor_Image_Segment  =  Palette_Cursor_Data@  +  ( (GUI_CurrentCursor            - 1) >> 4 )
sa_Cursor             =  gsa_Cursor_Array      +  ( (current_pointer_image_number - 1) >> 4 )

Clear_Mouse_Image+2C      mov     ecx, mouse_palette              
Draw_Bitmap_To_Mouse+12C  mov     eax, mouse_palette              
Draw_Mouse_Hi_Res+27      mov     edx, mouse_palette              
Draw_Mouse_Off_Page+27    mov     edx, mouse_palette              
Draw_Mouse_To_Off_Page+38 mov     edx, mouse_palette              
Load_Palette+70           mov     mouse_palette, eax              
Load_System_Font+18       mov     mouse_palette, offset unk_1B8E7C
Load_System_Mouse+128     mov     edx, mouse_palette              
Load_System_Mouse+140     mov     edx, mouse_palette              
Load_System_Mouse+EC      mov     eax, mouse_palette              

Load_Palette
    ...
    font_ptr = Far_Reload(file_name = font_name, entry_num = (entry + 1), bse_seg = palette_block);
    ...
    mouse_palette = (font_ptr + 1024)



Draw_Mouse_Off_Page
Address: 01:0012435C
Num params: 0
Return type: void (1 bytes) 
Locals:

Draw_Mouse_To_Off_Page
Address: 01:001243DE
Num params: 0
Return type: void (1 bytes) 
Locals:
    signed integer (4 bytes) y
    signed integer (4 bytes) vpos
    signed integer (4 bytes) line_add
    signed integer (4 bytes) current_bank
    signed integer (4 bytes) x
    signed integer (4 bytes) pos
    signed integer (4 bytes) max_horizontal
    signed integer (4 bytes) horizontal_add
    signed integer (4 bytes) max_vertical
    pointer (4 bytes) mouse_image
    char (1 bytes) pixel










¿ mouse list is per screen ?
    i.e., ...
        ¿ there is a 1:1 relationship between a screen layout definition and a mouse list ?
    Given the location of calls to Set_Mouse_List() this does appear to be the case.
Straight away,
    the first call to Set_Mouse_List() is in the 'Initialize Input' code
    then it is (subsequently, immediatel;y) recalled in Intro_Screen() and/or Main_Menu_Screen()



MGC  seg033
MGC s33p01 Set_Mouse_List()
MGC s33p02 Check_Mouse_Shape()
MGC s33p03 Get_Pointer_Image_Number()
MGC s33p04 Get_Pointer_Offset()

current_mouse_list_count
current_mouse_list
current_pointer_offset
current_pointer_image_number


/*
    Initialized Data
*/

current_pointer_image_number dw 0
previous_pointer_image_number dw 0
g_MD_CDraw dw 0
g_MouseDriverInstalled dw 0

mouse_x dw 158
mouse_y dw 100

current_mouse_list_count dw 1
current_pointer_offset dw 0

g_MOUSE_ClickX dw 0
g_MOUSE_ClickY dw 0
g_MOUSE_ClickBtns dw 0

g_MD_INT_InProcess dw 0
g_CR_HaveSave dw 0

g_MOUSE_InitX dw 158
g_MOUSE_InitY dw 100

g_MD_CDraw_Save dw 0

current_mouse_list dw 0

CR_Save_RSP db 4B0h dup(   0)
CR_Save_DSP db 4B0h dup(   0)

g_SCRN_RedrawFn_Present dw 0
g_GUI_Last_Key_Control dw 0
g_MD_Hardware dw 0

down_mouse_button dw -1




MD_Init()
    g_MouseDriverInstalled = ST_TRUE
    g_MD_INT_InProcess = ST_FALSE
    mouse_x = g_MOUSE_InitX
    mouse_y = g_MOUSE_InitY
    g_MD_CDraw  = ST_FALSE
    g_MOUSE_ClickRec1 = 0  /* ¿ ST_FALSE ? */
    g_MOUSE_ClickRec2 = 0  /* ¿ ST_FALSE ? */
    return ST__SUCCESS



Down P GAME_New_Screen_1+AB  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P GAME_New_Screen_2+6D  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P GAME_New_Screen_3+3B  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P GAME_New_Screen_4+E8  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P GAME_New_Screen_6+F2  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P GAME_New_Screen_7+8E  call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P GUI_DisplayHelp+15    call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P IN_Init+5E            call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P Load_Screen+152       call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Up   P Main_Menu_Screen+390  call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P NEWG_CreateWorld+35   call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P SCREEN_HallOfFame+14  call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P SCREEN_HallOfFame+196 call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P SCREEN_IntroAnim+60   call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P SCREEN_NewGame_0+1CD  call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P SCREEN_Settings+11A   call    Set_Mouse_List          ; sets the passed array as the current window matrix,

Down P UU_GAME_AutoSave+3D   call    Set_Mouse_List          ; sets the passed array as the current window matrix,
Down P UU_GAME_AutoSave+56   call    Set_Mouse_List          ; sets the passed array as the current window matrix,


Down P GUI_MouseEmuMove+BD              call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P GUI_MoveDlgHighlight+39B         call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P GUI_ProcessDirKey+254            call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P GUI_SimplePageFlip+1E            call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P Interpret_Keyboard_Input+280     call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P Interpret_Mouse_Input+79D        call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down p MD_INT_Handler+86                call    near ptr Check_Mouse_Shape; saves the currently set cursor index, then finds the
Down P Scan_Field+20                    call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down P Scan_Input+1F                    call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Down p UU_GUI_ProcBufferClick:loc_24D91 call    near ptr Check_Mouse_Shape; saves the currently set cursor index, then finds the
Up   P VGA_Copy_RSP_DSP_YM+85           call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  
Up   P VGA_Copy_RSP_DSP_YM+94           call    Check_Mouse_Shape       ; saves the currently set cursor index, then finds the  


Down P CRP_GUI_KeyInputOnly+3D call    Get_Pointer_Image_Number; returns the currently active cursor's index
Up   P Copy_On_To_Off_Page+4   call    Get_Pointer_Image_Number; returns the currently active cursor's index


Down P CRP_GUI_KeyInputOnly+44      call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P GUI_MouseEmuMove+C4          call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P GUI_MoveDlgHighlight+3A2     call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P GUI_ProcessDirKey+25B        call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Interpret_Keyboard_Input+287 call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Interpret_Mouse_Input+2E     call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Interpret_Mouse_Input+7A4    call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Interpret_Mouse_Input+B54    call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Scan_Field+27                call    Get_Pointer_Offset      ; returns the currently active cursor's offset from
Down P Scan_Input+26                call    Get_Pointer_Offset      ; returns the currently active cursor's offset from













MGC
    main()
        Hardware_Init()
            IN_Init()
                GUI_SetWindows(1, g_GUI_VirtualScreen);  // MGC s33p01  ST_GUI.H
                g_MD_Hardware = MD_Init();  // MGC s33p07  int MOUSE_Init(void)  ST_GUI.H
                IN_CRL_Save_RSP(158, 100);



CRL_Save_RSP
CRL_Save_DSP
// _s33p32a.c
void IN_CRL_Save_RSP(int X_Pos, int Y_Pos);

ST_CRSR.C / .H
CR_Store_C
CR_Restore_C
CR_Draw_C





MoO2:
User_Mouser_Handler()
    ...
    `== if(mouse enable and mouse cursor image != none)
        Restore_Mouse()
        Save_Mouse()
        Draw_Mouse()
    ...



### Input vs. Mouse



current_mouse_list_count
current_mouse_list
current_pointer_offset
current_pointer_image_number



    signed int Get_Pointer_Image_Number()
        return current_mouse_list_ptr[current_mouse_list]->image_num



Get_Pointer_Image_Number()  AKA GUI_GetCursorIndex()



MGC s33p01
Set_Mouse_List()  AKA GUI_SetWindows()
    sets
        current_mouse_list_count
        current_mouse_list
        current_pointer_offset
        current_pointer_image_number

Set_Mouse_List(count, list)
    current_mouse_list_count = count
    current_mouse_list = list;
    ...
    current_pointer_offset = s_MOUSE_LIST.center_offset
    current_pointer_image_number = s_MOUSE_LIST.image_num
    ...



Check_Mouse_Shape(x,y)
    previous_pointer_image_number = current_pointer_image_number
    ...
    ... current_mouse_list_count
    ... current_mouse_list[itr_mouse_list_count] ...
    ...
    current_pointer_offset = current_mouse_list[itr_mouse_list_count]->center_offset
    current_pointer_image_number = current_mouse_list[itr_mouse_list_count]->image_num





_s33p30         CRL_Save_RSP                        GUI_SaveCursorArea_RSP          void GUI_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       GUI_SaveCursorArea          
_s33p31         CRL_Save_DSP                        GUI_SaveCursorArea_DSP          void GUI_SaveCursorArea_DSP(int X_Pos, int Y_Pos)       GUI_SaveDrawCrsrArea          

_s33p32         IN_CRL_Save_RSP                     VGA_SaveCursorArea_RSP          void VGA_SaveCursorArea_RSP(int X_Pos, int Y_Pos)       VGA_SaveCursorArea

_s33p33         CRL_Copy_DSP2RSP                    GUI_CursorCopy_Bkup2Main        void GUI_CursorCopy_Bkup2Main(void)

_s33p34         CRL_Restore_RSP                     GUI_RestoreCursorArea_RSP       VGA_RestoreCursrArea
_s33p35         CRL_Restore_DSP                     GUI_RestoreCursorArea_DSP       VGA_RestoreDrawCArea

// UU _s33p36      UU_CRL_Restore_SP3                                                     UU_VGA_RestoreCursorArea_SP3

_s33p37         CRH_Draw_RSP |-> CRL_Draw_RSP       GUI_DrawCursor_RSP              GUI_DisplayCursor
_s33p38         CRH_Draw_DSP |-> CRL_Draw_DSP       GUI_DrawCursor_DSP              GUI_DrawCursor

_s33p39         CRL_Draw_RSP                        VGA_DrawCursor_RSP              VGA_DisplayCursor
_s33p40         CRL_Draw_DSP                        VGA_DrawCursor_DSP              VGA_DrawCursor

'

Down P CRP_GUI_KeyInputOnly+21          call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P CRP_GUI_KeyInputOnly+A2          call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P GUI_MouseEmuMove+FC              call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P GUI_MoveDlgHighlight+3CB         call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P GUI_ProcessDirKey+284            call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P G_GUI_PressAnyKey+10A            call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P G_GUI_PressAnyKey+89             call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Interpret_Keyboard_Input+2BF     call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Interpret_Mouse_Input+8B         call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Interpret_Mouse_Input+BFC        call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Up   p MD_INT_Handler+8F                call    near ptr CRL_Save_RSP   ; Render Screen-Page (RSP)
Up   P Main_Menu_Screen+3A3             call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Push_Field_Down+16E              call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Push_Field_Down+1BA              call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down P Push_Field_Down+88               call    CRL_Save_RSP            ; Render Screen-Page (RSP)
Down p UU_GUI_ProcBufferClick:loc_24D99 call    near ptr CRL_Save_RSP   ; Render Screen-Page (RSP)
Up   P VGA_Copy_RSP_DSP_YM+8F           call    CRL_Save_RSP            ; Render Screen-Page (RSP)



### Restore_Mouse()
    GUI_SimplePageFlip      call    CRL_Restore_DSP
    VGA_Copy_RSP_DSP_YM     call    CRL_Restore_DSP
    VGA_Copy_RSP_DSP_YM     call    CRL_Restore_DSP

### Save_Mouse()
    GUI_SimplePageFlip      call    CRL_Save_DSP

### Draw()
    CRH_Draw_DSP            call    CRL_Draw_DSP
        just a wrapper around if(cursor != none)
    GUI_SimplePageFlip      call    CRH_Draw_DSP


? MoO2  "current_mouse_shape" vs. "current_pointer_image_number" ?


Toggle_Pages()
|-> Draw_Mouse_Off_Page
|-> Draw_Mouse_Off_Page_Buffer_()

Set_Page_Off()
    current_page_flag = 1
    current_video_page = __off_page_buffer
    copy_to_on_page_flag = 0






### Toggle_Pages()  AKA GUI_SimplePageFlip()
    MD_Draw_Disable()
    Pointer_X()
    Pointer_Y()
    ? Draw_Visible_Fields  ?  AKA GUI_DrawControls()
    Check_Mouse_Shape()
    CRL_Save_DSP()
    CRH_Draw_DSP
    Page_Flip()
        draw_page_num = (1 - draw_page_num); out; WaitSync(); Set_Page_Off();
    CRL_Restore_DSP()
    CRL_Copy_DSP2RSP()
    MD_Draw_Restore()
So, ...
    Eh?
    updates the screen for any changes to fields
    saves the cursor area on the off-screen buffer
    draws the cursor on the off-screen buffer
    switches the off-screen and on-screen buffers
        now rendering what was just finished being drawn
    restores what was previously saved for the cursor area
    copies the off-screen buffer to the on-screen buffer

? MoO2  Switch_Video_Pages ?






MoO2
cseg01:001250BB video_card_00:                          ; DATA XREF: Assign_Video_Function_Pointers:off_12507Fo
cseg01:001250BB mov     Full_Screen_Dump, offset Full_Screen_Dump_Lo_Res
cseg01:001250C5 mov     Partial_Screen_Dump, offset Partial_Screen_Dump_Lo_Res
cseg01:001250CF mov     Masked_Screen_Dump, offset Masked_Screen_Dump_Lo_Res
cseg01:001250D9 mov     Copy_Off_To_On_Page, offset Copy_Off_To_On_Lo_Res
cseg01:001250E3 mov     Save_Mouse, offset Save_Mouse_Lo_Res
cseg01:001250ED mov     Restore_Mouse, offset Restore_Mouse_Lo_Res
cseg01:001250F7 mov     Draw_Mouse, offset Draw_Mouse_Lo_Res
cseg01:00125101 mov     Save_Video_State, offset Save_Video_State_Lo_Res
cseg01:0012510B mov     Restore_Video_State, offset Restore_Video_State_Lo_Res
cseg01:00125115 jmp     video_card_04

