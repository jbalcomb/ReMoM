






## s_UNIT


### Move_Failed


### Rd_Constr_Left

ovr061:100C 8B 46 10                                        mov     ax, [bp+unit_idx]
ovr061:100F B1 05                                           mov     cl, 5
ovr061:1011 D3 E0                                           shl     ax, cl
ovr061:1013 C4 1E C2 9E                                     les     bx, [_UNITS]
ovr061:1017 03 D8                                           add     bx, ax
ovr061:1019 26 8A 47 1C                                     mov     al, [es:bx+s_UNIT.Rd_Constr_Left]
ovr061:101D 98                                              cbw
ovr061:101E 89 46 E6                                        mov     [bp+unit_roadbuild_count], ax

0000001C Rd_Constr_Left db ?                     ; 1-byte, signed

cmp     [es:bx+s_UNIT.Rd_Constr_Left], 99

ovr095:0696 26 80 7F 1C 63                                  cmp     [es:bx+s_UNIT.Rd_Constr_Left], 99
63h == 99d




    