// _s13p20c.c EMM_GetHandle
// ST_EMM.H

#include "ST_HEAD.H"

/*

NOTE:
    _SI = EmmHandleName
    _DI = varEmmHndlNbr_FoundOrCreated

if EmmHandleName == NULL then return 0

varExistingEmmHndlNbr = -1
varItr = 0

while varItr < g_EMM_Open_Handles
    gEMM_Table[varItr].EmmHndlNm
    if strcmp(EmmHandleName, gEMM_Table[varItr].EmmHndlNm) != 0
        Continue
    else
        varExistingEmmHndlNbr = varItr
        Continue

if varExistingEmmHndlNbr != -1
    _AX = EMM_GetHandlePageCount(gEMM_Table[varExistingEmmHndlNbr].EmmHndlNbr)
    if _AX < EmmLogicalPageCount
        itoa(); itoa(); itoa(); strcpy(); strcat(); strcat(); strcat(); strcat(); strcat(); strcat();
        ...Temp_String...cnst_EMMErr_Reload1...' reloaded into EMM, diff size ='
        GAME_QuitProgram(Temp_String)
    else
        _AX = _DI
        return _AX

strcpy(gEMM_Table[g_EMM_Open_Handles].EmmHndlNm, EmmHandleName)

if EmmRsvdFlag != 1
    ? g_EMM_Open_Handles = (
        _BX = g_EMM_Open_Handles * sizeof EMM_Record
        _AL = gEMM_Table[].EmmHndlNm + _BX
        NEG _AL
        gEMM_Table[g_EMM_Open_Handles].EmmHndlNm)
    ?

_DI = EMM_MakeNamedHandle(EmmLogicalPageCount, gEMM_Table[g_EMM_Open_Handles])

if _DI == 0
    _AX = _DI
    return _AX

strcpy(gEMM_Table[g_EMM_Open_Handles].EmmHndlNm, EmmHandleName)
gEMM_Table[g_EMM_Open_Handles].Rsvd = EmmRsvdFlag
gEMM_Table[g_EMM_Open_Handles].EmmHndlNbr = _DI
g_EMM_Open_Handles++

if EmmRsvdFlag != 1
    gEMM_Pages_Reserved = gEMM_Pages_Reserved - EmmLogicalPageCount

    // asm{cmp [gEMM_Pages_Reserved],0; jge @@NewJmpJmpJmpDone}
    // Jump If (0 - gEMM_Pages_Reserved) ???
if !(gEMM_Pages_Reserved >= 0)
    'EMM reserved exceeded by ' gEMM_Pages_Reserved ' blocks [' EmmHandleName '.LBX]'
    GAME_QuitProgram()

_AX = _DI
return _AX

*/

unsigned int EMM_GetHandle(unsigned int EmmLogicalPageCount, char *EmmHandleName, int EmmRsvdFlag)
{
    char varTmpStrEmmRsvdErr[6] = {0};
    int varExistingEmmHndlNbr;
    unsigned int varItrOurOpenHandleCount;
    unsigned int varOurOpenHandlePageCount;
    unsigned int varEmmHndlNbr;
    char tmp_EmmHndlNm_byte0;

    // printf("DEBUG: [%s, %d]: BEGIN: EMM_GetHandle(EmmLogicalPageCount = %u, EmmHandleName = %s, EmmRsvdFlag = %d)\n", __FILE__, __LINE__, EmmLogicalPageCount, EmmHandleName, EmmRsvdFlag);

    if (EmmHandleName == NULL)
    {
        return 0;
    }

    /* Loop through our EMM Table, Check for existing handle by name */
    varExistingEmmHndlNbr = -1;
    varItrOurOpenHandleCount = 0;

    while (varItrOurOpenHandleCount < g_EMM_Open_Handles)
    {
        if (strcmp(EmmHandleName, g_EMM_Table[varItrOurOpenHandleCount].eEmmHndlNm) == 0)
        {
            varExistingEmmHndlNbr = varItrOurOpenHandleCount;
        }
        varItrOurOpenHandleCount++;
    }

    /* If we had already created an entry with this name... */
    if (varExistingEmmHndlNbr != -1)
    {
        /* If the reuqested page count is not the same as the existing page count...*/
        varEmmHndlNbr = EMM_GetHandlePageCount(g_EMM_Table[varExistingEmmHndlNbr].eEmmHndlNbr);
        if (varOurOpenHandlePageCount < EmmLogicalPageCount)
        {
            itoa(g_EMM_Table[varExistingEmmHndlNbr].eEmmHndlNbr, Tmp_Conv_Str_1, 10);
            itoa(varEmmHndlNbr, Tmp_Conv_Str_2, 10);
            itoa(EmmLogicalPageCount, Tmp_Conv_Str_3, 10);
            strcpy(Temp_String, EmmHandleName);
            strcat(Temp_String, cnst_EmmErr_Reload1);  //' reloaded into EMM, diff size ='
            strcat(Temp_String, Tmp_Conv_Str_1);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_2);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_3);
            Quit(Temp_String);
        }
        else
        {
            return varEmmHndlNbr;
        }
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, EmmHandleName);

// TODO(JimBalcomb): figure out what this business is with neg(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0])
//     if ( EmmRsvdFlag == 1 )
//     {
//         /*
//             cmp  [bp+EmmRsvdFlag], 1
//             jnz  short @@NotReserved

//             @@Reserved:
//             mov  ax, [g_EMM_Open_Handles]
//             mov  dx, e_sizeof_EMM_RECORD
//             imul dx
//             mov  bx, ax
//             mov  al, [byte ptr EMM_Table.EmmHndlNm+bx]
//             neg  al
//             push ax
//             mov  ax, [g_EMM_Open_Handles]
//             mov  dx, e_sizeof_EMM_RECORD
//             imul dx
//             mov  bx, ax
//             pop  ax
//             mov  [byte ptr EMM_Table.EmmHndlNm+bx], al
//         */
//         /*
//         ? g_EMM_Open_Handles = (
//             _BX = g_EMM_Open_Handles * sizeof EMM_Record
//             _AL = gEMM_Table[].EmmHndlNm + _BX
//             NEG _AL
//             gEMM_Table[g_EMM_Open_Handles].EmmHndlNm)
//         ?
//         */
//         printf("DEBUG: [%s, %d]: g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm: %s\n", __FILE__, __LINE__, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);
//         tmp_EmmHndlNm_byte0 = g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0];
//         printf("DEBUG: [%s, %d]: tmp_EmmHndlNm_byte0: %c)\n", __FILE__, __LINE__, tmp_EmmHndlNm_byte0);
//         tmp_EmmHndlNm_byte0 = -tmp_EmmHndlNm_byte0;
//         printf("DEBUG: [%s, %d]: tmp_EmmHndlNm_byte0: %c)\n", __FILE__, __LINE__, tmp_EmmHndlNm_byte0);
//         g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm[0] = tmp_EmmHndlNm_byte0;
//         printf("DEBUG: [%s, %d]: g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm: %s\n", __FILE__, __LINE__, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);
// getch();
//     }

    varEmmHndlNbr = EMM_MakeNamedHandle(EmmLogicalPageCount, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);

    if (varEmmHndlNbr == 0)
    {
        return 0;  // ~= return varEmmHndlNbr
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, EmmHandleName);
    g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = EmmRsvdFlag;
    g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = varEmmHndlNbr;
    g_EMM_Open_Handles++;

    if (EmmRsvdFlag == 1)
    {
        // printf("DEBUG: [%s, %d]: g_EMM_Pages_Reserved: %u\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
        g_EMM_Pages_Reserved = g_EMM_Pages_Reserved - EmmLogicalPageCount;
        // printf("DEBUG: [%s, %d]: g_EMM_Pages_Reserved: %u\n", __FILE__, __LINE__, g_EMM_Pages_Reserved);
// getch();
    }

    if ( g_EMM_Pages_Reserved < 0 )
    {
        // 'EMM reserved exceeded by ' gEMM_Pages_Reserved ' blocks [' EmmHandleName '.LBX]'
        //asm mov ax, [gEMM_Pages_Reserved]
        //asm neg ax
        itoa((g_EMM_Pages_Reserved * -1), varTmpStrEmmRsvdErr, 10);
        strcpy(Temp_String, cnst_EmmErr_ResOut1);
        strcat(Temp_String, varTmpStrEmmRsvdErr);
        strcat(Temp_String, cnst_EmmErr_ResOut2);
        strcat(Temp_String, EmmHandleName);
        strcat(Temp_String, cnst_EmmErr_ResOut3);
        Quit(Temp_String);
    }

    // printf("DEBUG: [%s, %d]: END: EMM_GetHandle(EmmLogicalPageCount = %u, EmmHandleName = %s, EmmRsvdFlag = %d) { varEmmHndlNbr = %u }\n", __FILE__, __LINE__, EmmLogicalPageCount, EmmHandleName, EmmRsvdFlag, varEmmHndlNbr);
    return varEmmHndlNbr;
}
