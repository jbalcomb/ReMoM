


seg011


DISK_OpenCreate                                seg011
DISK_ReadFile                                  seg011
DISK_WriteFile                                 seg011
DISK_FileSeek                                  seg011
DISK_FileClose                                 seg011


## DISK_OpenCreate()

XREF:
    UU_GUI_LoadClickFile+15
    UU_GUI_SaveClickFile+12
    Play_Sound__STUB+EC
    Save_SAVE_GAM+54
    Load_SAVE_GAM+6B
    GAME_ReloadWorldData+2D
    GAME_DumpWorldData+2D







dseg:7601                                                 ¿ END: Allocate.C - Initialized Data ?
dseg:7602 72 00                                           str_R__s011 db 'r',0                    ; DATA XREF: DISK_OpenCreate:loc_169A8o
dseg:7602                                                                                         ; OON XREF: DISK_OpenCreate()
dseg:7604 72 62 00                                        cstr_RB__6 db 'rb',0                    ; DATA XREF: DISK_OpenCreate:loc_169B8o
dseg:7604                                                                                         ; should use dseg:28b8
dseg:7607 77 00                                           cstr_W db 'w',0                         ; DATA XREF: DISK_OpenCreate:loc_169D5o
dseg:7609 77 62 00                                        cstr_WB__6 db 'wb',0                    ; DATA XREF: DISK_OpenCreate:loc_169E5o
dseg:7609                                                                                         ; should use dseg:28ef
dseg:760C 00 00                                           EMM_PageFrame dw 0                      ; DATA XREF: EMM_DetectDevice:loc_16AE0w ...
dseg:760C                                                                                         ; contains the segment address of the EMS page frame
dseg:760E 45 4D 4D 58 58 58 58 30 00                      EMM_DevName db 'EMMXXXX0',0             ; DATA XREF: EMM_DetectDevice:loc_16AA0o
dseg:7617 00 00 00 00 00 00 01 00 00 00 02 00 00 00 03 00 EMM_Log2Phys_Map EMM_L2P_Map_Record <0, 0>
dseg:7617                                                                                         ; DATA XREF: EMM_Map4Pages:loc_16D34w ...
dseg:7617                                                 EMM_L2P_Map_Record <0, 1>
dseg:7617                                                 EMM_L2P_Map_Record <0, 2>
dseg:7617                                                 EMM_L2P_Map_Record <0, 3>
dseg:7627 00                                              db    0
