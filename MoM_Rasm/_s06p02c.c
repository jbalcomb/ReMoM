// _s06p02c.c QuitToDOS
// in MOM_DEF.H

#include "ST_HEAD.H"


void QuitToDOS(char *quitmsg)
{
    char *ptr_quitmsg;

//    dlvfprintf("DEBUG: BEGIN: QuitToDOS()\n");

    //char msg_tail[] = {'\r','\n','\r','\n','$'}
    // printf("DEBUG: BEGIN: DOS_QuitWithMsg()\n");
    // //wait_for_any_key();
    
    // printf("DEBUG: quitmsg:  [%p]  %s\n", quitmsg, quitmsg);
    // //wait_for_any_key();
    
    // TODO(JimBalcomb): figure out how to pass a (near) char* to DOS Print String
// // asm mov ax, _S06P02C_TEXT
// // asm mov ds, ax
// //     _DX = quitmsg[0];
// //     _AH = PRINT_STRING;
// //     geninterrupt(DOS_INT);
// //     wait_for_any_key();
// //     ptr_quitmsg = quitmsg;
// //     _DX = *ptr_quitmsg;
// //     _AH = PRINT_STRING;
// //     geninterrupt(DOS_INT);
// //     wait_for_any_key();

// // @@FindNull:
// //      lodsb
// //      cmp al, 0
// //      jnz short @@FindNull
// //      dec si
// //      mov byte ptr [si], 0Dh
// //      inc si
// //      mov byte ptr [si], 0Ah
// //      inc si
// //      mov byte ptr [si], 0Dh
// //      inc si
// //      mov byte ptr [si], 0Ah
// //      inc si
// //      mov byte ptr [si], '$'
// //      inc si

//     //; ~=                            https://www.quora.com/How-do-I-receive-string-as-parameter-in-C
//     //;   while(*i - '\0') 
//     //;   { 
//     //;       if(*i==' ') {*i = '_';} 
//     //;       i++; 
//     //;   } 

//     //; ~=                            https://dev-notes.eu/2019/07/Passing-Strings-in-C/
//     //;   *(*s + length) = 'x';
// 	//;   *(*s + length + 1) = 'y';
// 	//;   // Add a null terminator
// 	//;   *(*s + length + 2) = 0;
//     //;   ...
//     //;   // Remove newline - maybe realloc memory after this.
// 	//;   s[strcspn(s, "\n")] = 0;
//     //; ~=                            https://overiq.com/c-programming-101/character-array-and-character-pointer-in-c/
//     //;   arr[0] = 'R';
//     //;   arr[1] = 'e';
//     //;   arr[2] = 'd';
//     //;   arr[3] = ' ';

//     // *(*quitmsg + 2) = '\0';          https://www.quora.com/How-do-I-receive-string-as-parameter-in-C
//     // quitmsg[2] = '\0';               https://overiq.com/c-programming-101/character-array-and-character-pointer-in-c/
//     // while(*quitmsg - '\0') 
//     // { 
//     //     if(*quitmsg == '\0') {*quitmsg = '_';} 
//     //     quitmsg++; 
//     // } 

//     //for (ptr_quitmsg = quitmsg; *ptr_quitmsg && *ptr_quitmsg == quitmsg; ptr_quitmsg++, quitmsg++);
//     for (ptr_quitmsg = quitmsg; *ptr_quitmsg != '\0'; ptr_quitmsg++);
//     ptr_quitmsg--;
//     *ptr_quitmsg = '\r';
//     ptr_quitmsg++;
//     *ptr_quitmsg = '\n';
//     ptr_quitmsg++;
//     *ptr_quitmsg = '\r';
//     ptr_quitmsg++;
//     *ptr_quitmsg = '\n';
//     ptr_quitmsg++;
//     *ptr_quitmsg = '$';
//     ptr_quitmsg++;
//     *ptr_quitmsg = '\0';


//     printf("DEBUG: *ptr_quitmsg:  [%p]  \n", *ptr_quitmsg);
//     wait_for_any_key();

//     printf("DEBUG: quitmsg:  [%p]  %s\n", quitmsg, quitmsg);
//     wait_for_any_key();

//     //printf("DEBUG: *ptr_quitmsg:  [%p]  \n", **ptr_quitmsg);  // Error: Invalid indirection
//     //wait_for_any_key();

//     // _DX = quitmsg;  // Error Nonportable pointer conversion
//     _DX = quitmsg[0];
//     _AH = PRINT_CHAR;
//     geninterrupt(DOS_INT);

    printf("%s\n\n", quitmsg);

//    dlvfprintf("DEBUG: END: QuitToDOS()\n");
    
    Debug_Log_Shutdown();
    
    _AH = 0x4C;
    geninterrupt(DOS_INT);
}
