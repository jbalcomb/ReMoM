;   Master of X
;   SimTex
;       Master of Orion 1
;       Master of Magic
;       Master of Orion 2

; https://lallouslab.net/2016/01/11/introduction-to-writing-x64-assembly-in-visual-studio/


; By default, the x64 calling convention passes the first four arguments to a function in registers. The registers used for these arguments depend on the position and type of the argument. Remaining arguments get pushed on the stack in right-to-left order.
; Integer valued arguments in the leftmost four positions are passed in left-to-right order in RCX, RDX, R8, and R9, respectively. 


; C:\Program Files (x86)\Windows Kits\10\Include\10.0.22621.0\shared\macamd64.inc
include macamd64.inc



; define read/write data
; define the data within a read/write data segment

_DATA SEGMENT
hello_msg db "Hello world", 0
info_msg  db "Info", 0
_DATA ENDS

; define read-only and executable data
_TEXT SEGMENT

const_str db "I'm inedible", 0

; __int64 c_add_by_ref(int a, int b, __int64 *r)
; {
;   *r = a + b;
;   return *r;
; }

; __int64 add_by_ref(int a, int b, __int64 *r)
add_by_ref PROC
movsxd rax, ecx
movsxd rdx, edx
add    rax, rdx ; result in RAX
mov qword ptr [r8], rax ; store / deref
ret
add_by_ref ENDP

_TEXT ENDS

END
