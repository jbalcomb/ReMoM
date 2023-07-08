


SEEALSO: C:\STU-DASM\drake178\__WZD_main.asm



¿ code after Load_SAVE_GAM(8) is equivalent to Loaded_Game_Update() ?




WIZARDS.EXE
start
_main

                                          ; source: borlandc\lib\startup\c0.asm
push    [__C0environ]
push    [__C0argv]                        ; c0argv
push    [__C0argc]                        ; c0argc
call    _main
