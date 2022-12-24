

"draw_page_num"
?"draw" as in on-screen or off-screen?
Page_Flip() sets draw_page_num to (1 - draw_page_num) and uses this switched draw_page_num to calculate and set the address for the CRT Controller to *display* the video memory
so, ...
    "draw_page_num" is the *index* of the video page that is being *drawn* to the *screen*/*display*
    "draw" ~== "active" / "visible" / "on-screen"



main()
    Screen_Control()
        ..._Screen()
            ..._Screen_Draw()
            Toggle_Pages()
                Draw_Fields()
                Save_Mouse()
                Draw_Mouse()
                Page_Flip()
                    VBB = {vbb0,vbb1}

                Restore_Mouse()
                Copy_Mouse()



...each screen has a draw function that draws the base screen
...fields have their own draw functions to accomodate changes - up/down buttons, text inputs, etc.
...once the *final* screen is ready to be presented/displayed/rendered...
...the area of the screen where the mouse cursor image is to be drawn is saved to the mouse background buffer
...then the mouse cursor image is drawn
...finally, conclusively, the video pages are switched
...so, here, Restore_Mouse() is drawing on the *current* screen what was just saved from what was just made to be the *previous* screen



Copy_Mouse()
    copies CR_Save_DSP to CR_Save_RSP

Save_DSP(), Draw_DSP(), Restore_DSP() all use (1 - draw_page_num)


Save_DSP()
    dst: CR_Save_DSP

Draw_DSP()
    src: mouse_palette

Restore_DSP()
    src: CR_Save_RSP
    dst: (1 - draw_page_num)

Restore_RSP()
    src: CR_Save_RSP
    dst: (draw_page_num)
