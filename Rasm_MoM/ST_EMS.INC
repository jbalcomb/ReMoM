
EMS_Record struc          ; (sizeof=0xC, standard type)
    EmsHndlNm   db 9 dup(?)             ; ~= char EmmHndlNm[EMM_HANDLE_NAME_SIZE_MAX] + '\0'
    Rsrvd       db ?                    ; ~= {0,1}
    EmsHndlNbr  dw ?                    ; ~= FAR PTR WORD / unsigned int
EMS_Record ends

EMS_L2P_Map_Record struc  ; (sizeof=0x4, standard type)
    LogicalPageNbr  dw ?                ; {0, ..., 2048}  32 MB / 16 KB = 2048 logical pages
    PhysicalPageNbr dw ?                ; {0, 1, 2, 3}
EMS_L2P_Map_Record ends


;EXTRN EMS_Open_Handles:WORD
;EXTRN EMS_Reserved_Left:WORD
;EXTRN EMS_Table:EMS_Record
;;;dseg:A5F2
;;EMS_Table EMS_Record 28h dup(<0>)       ; 40
;EXTRN Temp_String:BYTE
;EXTRN Tmp_Conv_Str_1:BYTE
;EXTRN Tmp_Conv_Str_2:BYTE
;EXTRN Tmp_Conv_Str_3:BYTE
;EXTRN cnst_EMMErr_ResOut1:BYTE
;EXTRN cnst_EMMErr_ResOut2:BYTE
;EXTRN cnst_EMMErr_ResOut3:BYTE
;EXTRN cnst_EMMErr_Reload1:BYTE
;EXTRN cnst_EMMErr_Space:BYTE
;
;
;EXTRN itoa:PROC
;EXTRN strcat:PROC
;EXTRN strcpy:PROC
;EXTRN stricmp:PROC
;EXTRN EMS_MakeNamedHandle:PROC ; _s12p05.asm
;EXTRN EMS_GetPageCount:PROC ; _s12p04.asm
;EXTRN GAME_QuitProgram:PROC ; _s05p02.asm
