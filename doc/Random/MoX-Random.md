PRNG

same in MGC and WZD
MAGIC.EXE  seg022
WIZARDS.EXE  seg022
// WZD s22p07
void Randomize(void)
AKA RND_TimerSeed()
// WZD s22p08
int16_t Random(int16_t n)




MoO2
Module: MOX2
Set_Game_Random_Seed_()
```asm
Set_Game_Random_Seed_ proc near
call    Check_Release_Version
cmp     _cmd_line_seed, 0
jz      Randomize  ; does not return
mov     eax, _cmd_line_seed
jmp     Set_Random_Seed
Set_Game_Random_Seed_ endp
```



Randomize function
Disassembled in IDA Pro 5.5
MAGIC.EXE, 16-bit MS-DOS executable
```asm
proc Randomize far
timer_value= dword ptr -4
push    bp
mov     bp, sp
sub     sp, 4
push    es
mov     ax, 0
mov     es, ax
assume es:nothing
mov     ax, [es:46Ch]
mov     [word ptr bp+timer_value+2], ax
mov     ax, [es:46Eh]
mov     [word ptr bp+timer_value], ax
pop     es
assume es:nothing
mov     ax, [word ptr bp+timer_value+2]
mov     [word ptr random_seed], ax
mov     ax, [word ptr bp+timer_value]
mov     [word ptr random_seed+2], ax
mov     sp, bp
pop     bp
retf
endp Randomize
```



Randomize function
Disassembled in IDA Pro 5.5
MAGIC.EXE, 32-bit Windows 95 DOS mode executable
```asm
proc Randomize near
timer_value= dword ptr -4
push    20h
call    __CHK
push    ebx
push    ecx
push    edx
push    esi
push    edi
push    ebp
mov     ebp, esp
sub     esp, 4
xor     eax, eax
call    time_
mov     [ebp+timer_value], eax
mov     eax, [ebp+timer_value]
call    Set_Random_Seed
mov     esp, ebp
pop     ebp
pop     edi
pop     esi
pop     edx
pop     ecx
pop     ebx
retn
endp Randomize
```

