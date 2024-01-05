






OVL_Path_Results.Reach_Costs
    sets 2400 values to -1
    at movepath_cost_map[2400]

OVL_Path_Results.Reach_From
    sets 2400 values to {0,1,2,3,...}
    at movepath_cost_map[4800]

sets movepath_cost_map[2400 + wx] to 0




What is it ierating over with CH, CL, and SI?

CL is just an iter cnt for the loop over the middle map squares
CH seems to just be a flag that means one of the tested map squares had a value other than *impassible*
SI doubles as the counter for the outer loop and the inner loop



Two major blocks, where it sets up SI, BX, and DI?

Seconds block resets CH to 0 on every iteration?

incs BX and DI

subs SI, BX, DI
    next row up?

checks CH and SI

loops back to second major block

else, if value changed, loops back to first major block




DI is for setting the world map square index

SI is for getting the cost from the move path cost map

BX is for getting the 'Reach Costs' ... what dat is? where it come from?



Test - The First

movepath_cost_map.moves2[(src_wy * WORLD_WIDTH)]
if not *impassible*
    gets 'Reach Cost' for wms_idx - 1
        this'd be -1 in the array
            ...we're at the start of a world map row?
            so, the last square on the previous row?
            if we weren't, it'd just be like x-1 - left map sqaure to the left / west
            but, first, it's y-1,x=59 ... (60 - (x - 1))?

flip it around?
    ...at some r,c / x,y
    check Up-Left / North-West
    special-case: at c/x = 0, NW is -1
    every where else, NW is -61

if the first test in @@MajorBlock_1_Outer always the special case of y = 0?
No? Because, then it goes into another loop, that goes into another loop?
the next loop goes until CH == 0 or SI >= 2320
CH will be 0 if each check was *impassible*
    otherwise, CH gets inc'd at every test
SI gets incremented at each LODSB, so SI will be GTE 2320 when the count of bytes read, from the row offset, has happened some number of times.
    if src_wy had been 39, this would have already been the case
        so, we should be doing <60 reads?
¿ why only one test before going into the third loop ?
the third loop sets CL to 29 and goes until it's dec'd to 0
goes into the loop regardless of whether the previous was *impassible*
does two LODSB's and tests
so, 1 + 29 * 2 ... tests 59 ...
if src_wy was 38, it'd be 2280 + 59 = 2339? so, major block 2 must do another 60?

so, ...
first iter, y = 0
3 LODSB's
second iter, y = 1 + (29 * 2) = 59
so, ~ iterating over world map height? in 3's?
and, the first test is special
maybe, starting each loop at the row start is just to pin down the special case?





## Check & Update
if the current map square is not *impassible*

Test 1:

Test 2:
if 'curr moves2 cost' + 'adj reach cost' < 'curr reach cost'

Cases?
    'curr reach cost' == 0
        MUST NOT update
    'adj reach cost' == 0
        MUST update

When?
    'curr reach cost' == 0
        when evaluating the source map grid cell
    'adj reach cost' == 0
        when evaluating cells adjacent to the source map grid cell

Debug?
    adjacent_reach_cost
        if 0, then adjacent is source map grid cell
    current_reach_cost
        if 0, then current is source map grid cell
So, ...
    if adjacent_reach_cost == 0
        new_reach_cost = adjacent_reach_cost + move_cost;   0 + {0,1,2,3,4,5,6, ...}
            if new_reach_cost >= 0
                current_reach_cost = -1
                    if new_reach_cost < current_reach_cost
                        !(0 < -1)
                        !(0 < 0)
    if current_reach_cost == 0
        new_reach_cost = adjacent_reach_cost + move_cost;  -1 + {0,1,2,3,4,5,6, ...}
            if new_reach_cost >= 0
                current_reach_cost = 0
                    if new_reach_cost < current_reach_cost
                        {0, ...} < 0
OON Problem?
    if new_reach_cost < current_reach_cost fails on the -1 initialized value for cells adjacent to the source map grid cell
Solutions?
    cast -1 to unsigned so compare is against 255
        janky?
    treat initialization value as 255?
        what happens to test #1?
            always >= 0
        something with switching test to subtraction?
        something with testing ranges of values?
            carry flag set if adding   128 + 128
            carry flag set if adding  -128 + 
Meh. FTW.
    just test for -1
    






┼─────┼─────┼─────┼
│     │     │     │
┼─────┼─────┼─────┼
│ r-1 │   r │     │
┼─────┼─────┼─────┼
│     │     │     │
┼─────┼─────┼─────┼

╟─────┼─ ... ─┼─────╢
║     │  ...  │ r-1 ║
╟─────┼─ ... ─┼─────╢
║   r │  ...  │     ║
╟─────┼─ ... ─┼─────╢
║     │  ...  │     ║
╟─────┼─ ... ─┼─────╢

... ─┼─────╢ ╟─────┼─ ... 
...  │ r-1 ║ ║     │  ... 
... ─┼─────╢ ╟─────┼─ ... 
...  │     ║ ║   r │  ... 
... ─┼─────╢ ╟─────┼─ ... 
...  │     ║ ║     │  ... 
... ─┼─────╢ ╟─────┼─ ... 
e.g.,
r = (17 * 60) = 1020  x =  0, y = 17
r - 1         = 1019  x = 59, y = 16


╔═════╤═════╤═════╗
║ -61 │ -60 │ -59 ║
╟─────┼─────┼─────╢
║ - 1 │   0 │ + 1 ║
╟─────┼─────┼─────╢
║ +59 │ +60 │ +61 ║
╚═════╧═════╧═════╝

╔═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╤═╗
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╟─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─┼─╢
╚═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╧═╝












// WZD o147p1  [1of1]  (OON)
// drake178: Overland_Pathfinder()
void Move_Path_Find_C(int16_t wx, int16_t wy, int8_t * movepath_cost_map)
{
00007FF6F0D1DBB0  mov         qword ptr [rsp+18h],r8  
00007FF6F0D1DBB5  mov         word ptr [rsp+10h],dx  
00007FF6F0D1DBBA  mov         word ptr [rsp+8],cx  
00007FF6F0D1DBBF  push        rbp  
00007FF6F0D1DBC0  sub         rsp,0A0h  
00007FF6F0D1DBC7  lea         rbp,[rsp+20h]  
00007FF6F0D1DBCC  lea         rcx,[__60BE2721_MovePath@C (07FF6F0DAC108h)]  
00007FF6F0D1DBD3  call        __CheckForDebuggerJustMyCode (07FF6F0CDEF41h)  
    int16_t _AX_;
    int16_t _BX_;
    int16_t _CX_;
    int16_t _DX_;
    int16_t _DS_;
    int16_t _ES_;
    int16_t _SI_;
    int16_t _DI_;
    int16_t * _DS_SI_;
    int16_t * _ES_DI_;

    // mov     ax, [bp+wy]
    // mov     bx, e_WORLD_WIDTH
    // mul     bx
    // mov     [cs:CS_Row_Start], ax
    CS_Row_Start = (wy * WORLD_WIDTH);
00007FF6F0D1DBD8  movsx       eax,word ptr [wy]  
00007FF6F0D1DBDF  imul        eax,eax,3Ch  
00007FF6F0D1DBE2  mov         word ptr [CS_Row_Start (07FF6F0D773A4h)],ax  

    // add     ax, [bp+wx]
    // mov     dx, ax                          ; dx = tile index
    _DX_ = wx;
00007FF6F0D1DBE9  movzx       eax,word ptr [wx]  
00007FF6F0D1DBF0  mov         word ptr [_DX_],ax  

    // mov     ax, [word ptr bp+MoveMap@+2]
    // mov     ds, ax
    // mov     es, ax
    _DS_ = &movepath_cost_map;
00007FF6F0D1DBF4  lea         rax,[movepath_cost_map]  
00007FF6F0D1DBFB  mov         word ptr [_DS_],ax  
    _ES_ = &movepath_cost_map;
00007FF6F0D1DBFF  lea         rax,[movepath_cost_map]  
00007FF6F0D1DC06  mov         word ptr [_ES_],ax  

    // mov     di, OVL_Path_Results.Reach_Costs
    _DI_ = 2400;
00007FF6F0D1DC0A  mov         eax,960h  
00007FF6F0D1DC0F  mov         word ptr [_DI_],ax  

    // mov     cx, e_WORLD_SIZE_DD
    // mov     ax, -1
    // rep stosw
    _CX_ = 1200;  // WORLD_SIZE_DD
00007FF6F0D1DC13  mov         eax,4B0h  
00007FF6F0D1DC18  mov         word ptr [_CX_],ax  
    _AX_ = -1;
00007FF6F0D1DC1C  mov         eax,0FFFFFFFFh  
00007FF6F0D1DC21  mov         word ptr [rbp],ax  
    _ES_DI_ = (int16_t *)(_ES_ + _DI_);
00007FF6F0D1DC25  movsx       eax,word ptr [_ES_]  
00007FF6F0D1DC29  movsx       ecx,word ptr [_DI_]  
00007FF6F0D1DC2D  add         eax,ecx  
00007FF6F0D1DC2F  cdqe  
00007FF6F0D1DC31  mov         qword ptr [_ES_DI_],rax  
    while(_CX_--)
00007FF6F0D1DC35  movsx       eax,word ptr [_CX_]  
00007FF6F0D1DC39  mov         dword ptr [rbp+70h],eax  
00007FF6F0D1DC3C  movzx       eax,word ptr [_CX_]  
00007FF6F0D1DC40  dec         ax  
00007FF6F0D1DC43  mov         word ptr [_CX_],ax  
00007FF6F0D1DC47  cmp         dword ptr [rbp+70h],0  
00007FF6F0D1DC4B  je          Move_Path_Find_C+0B6h (07FF6F0D1DC66h)  
    {
        *_ES_DI_++ = _AX_;
00007FF6F0D1DC4D  mov         rax,qword ptr [_ES_DI_]  
00007FF6F0D1DC51  movzx       ecx,word ptr [rbp]  
00007FF6F0D1DC55  mov         word ptr [rax],cx  
00007FF6F0D1DC58  mov         rax,qword ptr [_ES_DI_]  
00007FF6F0D1DC5C  add         rax,2  
00007FF6F0D1DC60  mov         qword ptr [_ES_DI_],rax  
    }
00007FF6F0D1DC64  jmp         Move_Path_Find_C+85h (07FF6F0D1DC35h)  

    // mov     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_From
    // mov     cx, e_WORLD_SIZE_DW
    // mov     ax, 0
    // stosw
    // inc     ax
    // loop    loc_D5147
    _DI_    = 4800;
00007FF6F0D1DC66  mov         eax,12C0h  
00007FF6F0D1DC6B  mov         word ptr [_DI_],ax  
    _CX_    = 2400;  // WORLD_SIZE_DW
00007FF6F0D1DC6F  mov         eax,960h  
00007FF6F0D1DC74  mov         word ptr [_CX_],ax  
    _AX_    = 0;
00007FF6F0D1DC78  xor         eax,eax  
00007FF6F0D1DC7A  mov         word ptr [rbp],ax  
    _ES_DI_ = (int16_t *)(_ES_ + _DI_);
00007FF6F0D1DC7E  movsx       eax,word ptr [_ES_]  
00007FF6F0D1DC82  movsx       ecx,word ptr [_DI_]  
00007FF6F0D1DC86  add         eax,ecx  
00007FF6F0D1DC88  cdqe  
00007FF6F0D1DC8A  mov         qword ptr [_ES_DI_],rax  
    while(_CX_--)
00007FF6F0D1DC8E  movsx       eax,word ptr [_CX_]  
00007FF6F0D1DC92  mov         dword ptr [rbp+70h],eax  
00007FF6F0D1DC95  movzx       eax,word ptr [_CX_]  
00007FF6F0D1DC99  dec         ax  
00007FF6F0D1DC9C  mov         word ptr [_CX_],ax  
00007FF6F0D1DCA0  cmp         dword ptr [rbp+70h],0  
00007FF6F0D1DCA4  je          Move_Path_Find_C+11Ah (07FF6F0D1DCCAh)  
    {
        *_ES_DI_++ = _AX_++;
00007FF6F0D1DCA6  mov         rax,qword ptr [_ES_DI_]  
00007FF6F0D1DCAA  movzx       ecx,word ptr [rbp]  
00007FF6F0D1DCAE  mov         word ptr [rax],cx  
00007FF6F0D1DCB1  mov         rax,qword ptr [_ES_DI_]  
00007FF6F0D1DCB5  add         rax,2  
00007FF6F0D1DCB9  mov         qword ptr [_ES_DI_],rax  
00007FF6F0D1DCBD  movzx       eax,word ptr [rbp]  
00007FF6F0D1DCC1  inc         ax  
00007FF6F0D1DCC4  mov         word ptr [rbp],ax  
    }
00007FF6F0D1DCC8  jmp         Move_Path_Find_C+0DEh (07FF6F0D1DC8Eh)  

    // mov     _DI_OVL_Path_Results_Reach_Costs, _DX_wx
    // add     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_Costs
    // mov     ax, 0
    // stosb
    _DI_     =  _DX_;
00007FF6F0D1DCCA  movzx       eax,word ptr [_DX_]  
00007FF6F0D1DCCE  mov         word ptr [_DI_],ax  
    _DI_     += 2400;
00007FF6F0D1DCD2  movsx       eax,word ptr [_DI_]  
00007FF6F0D1DCD6  add         eax,960h  
00007FF6F0D1DCDB  mov         word ptr [_DI_],ax  
    _AX_     =  0;
00007FF6F0D1DCDF  xor         eax,eax  
00007FF6F0D1DCE1  mov         word ptr [rbp],ax  
    _ES_DI_  =  (int16_t *)(_ES_ + _DI_);
00007FF6F0D1DCE5  movsx       eax,word ptr [_ES_]  
00007FF6F0D1DCE9  movsx       ecx,word ptr [_DI_]  
00007FF6F0D1DCED  add         eax,ecx  
00007FF6F0D1DCEF  cdqe  
00007FF6F0D1DCF1  mov         qword ptr [_ES_DI_],rax  
    *_ES_DI_ =  _AX_;
00007FF6F0D1DCF5  mov         rax,qword ptr [_ES_DI_]  
00007FF6F0D1DCF9  movzx       ecx,word ptr [rbp]  
00007FF6F0D1DCFD  mov         word ptr [rax],cx  

    // loc_D5155:
    // mov     [cs:Value_Change], e_ST_FALSE
    // mov     si, 0
    // mov     bx, OVL_Path_Results.Reach_Costs
    // mov     _DI_OVL_Path_Results_Reach_Costs, OVL_Path_Results.Reach_From
    // add     si, [cs:CS_Row_Start]
    // add     bx, [cs:CS_Row_Start]
    // add     _DI_OVL_Path_Results_Reach_Costs, [cs:CS_Row_Start]
    // add     _DI_OVL_Path_Results_Reach_Costs, [cs:CS_Row_Start]
loc_D5155:
    CS_Value_Change = ST_FALSE;
00007FF6F0D1DD00  mov         byte ptr [CS_Value_Change (07FF6F0D46B3Ch)],0  
    _SI_ = 0;
00007FF6F0D1DD07  xor         eax,eax  
00007FF6F0D1DD09  mov         word ptr [_SI_],ax  
    _BX_ = 2400;
00007FF6F0D1DD0D  mov         eax,960h  
00007FF6F0D1DD12  mov         word ptr [_BX_],ax  
    _DI_ = 4800;
00007FF6F0D1DD16  mov         eax,12C0h  
00007FF6F0D1DD1B  mov         word ptr [_DI_],ax  
    _SI_ += CS_Row_Start;
00007FF6F0D1DD1F  movsx       eax,word ptr [CS_Row_Start (07FF6F0D773A4h)]  
00007FF6F0D1DD26  movsx       ecx,word ptr [_SI_]  
00007FF6F0D1DD2A  add         ecx,eax  
00007FF6F0D1DD2C  mov         eax,ecx  
00007FF6F0D1DD2E  mov         word ptr [_SI_],ax  
    _BX_ += CS_Row_Start;
00007FF6F0D1DD32  movsx       eax,word ptr [CS_Row_Start (07FF6F0D773A4h)]  
00007FF6F0D1DD39  movsx       ecx,word ptr [_BX_]  
00007FF6F0D1DD3D  add         ecx,eax  
00007FF6F0D1DD3F  mov         eax,ecx  
00007FF6F0D1DD41  mov         word ptr [_BX_],ax  
    _DI_ += CS_Row_Start;
00007FF6F0D1DD45  movsx       eax,word ptr [CS_Row_Start (07FF6F0D773A4h)]  
00007FF6F0D1DD4C  movsx       ecx,word ptr [_DI_]  
00007FF6F0D1DD50  add         ecx,eax  
00007FF6F0D1DD52  mov         eax,ecx  
00007FF6F0D1DD54  mov         word ptr [_DI_],ax  
    _DI_ += CS_Row_Start;
00007FF6F0D1DD58  movsx       eax,word ptr [CS_Row_Start (07FF6F0D773A4h)]  
00007FF6F0D1DD5F  movsx       ecx,word ptr [_DI_]  
00007FF6F0D1DD63  add         ecx,eax  
00007FF6F0D1DD65  mov         eax,ecx  
00007FF6F0D1DD67  mov         word ptr [_DI_],ax  

    // ...
    // ...
    // ...

loc_D554A:

    // ...
    // ...
    // ...

    // loc_D58EE:
    // inc     bx
    // inc     _DI_OVL_Path_Results_Reach_Costs
    // inc     _DI_OVL_Path_Results_Reach_Costs
    // sub     si, 120                         ; next row up
    // sub     bx, 120
    // sub     _DI_OVL_Path_Results_Reach_Costs, 240
    // cmp     ch, 0
    // jz      short loc_D5908

    // cmp     si, e_WORLD_WIDTH
    // js      short loc_D5908

    // jmp     loc_D554A

    // loc_D5908:
    // cmp     [cs:CS_Value_Change], e_ST_TRUE
    // jnz     short @@Done
loc_D5908:
    if(CS_Value_Change == ST_TRUE)
00007FF6F0D1DD6B  movsx       eax,byte ptr [CS_Value_Change (07FF6F0D46B3Ch)]  
00007FF6F0D1DD72  cmp         eax,1  
00007FF6F0D1DD75  jne         $loc_D5908+0Eh (07FF6F0D1DD79h)  
    {
        goto loc_D5155;
00007FF6F0D1DD77  jmp         Move_Path_Find_C+150h (07FF6F0D1DD00h)  
    }

}
00007FF6F0D1DD79  lea         rsp,[rbp+80h]  
00007FF6F0D1DD80  pop         rbp  
00007FF6F0D1DD81  ret  
