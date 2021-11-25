// _s13p20c.c EMM_CreateHandle

#include "ST_HEAD.H"

/*

NOTE:
    _SI = argEmmHandleName
    _DI = varEmmHndlNbr_FoundOrCreated

if argEmmHandleName == NULL then return 0

varExistingEmmHndlNbr = -1
varItr = 0

while varItr < gEMM_Open_Handles
    gEMM_Table[varItr].EmmHndlNm
    if strcmp(argEmmHandleName, gEMM_Table[varItr].EmmHndlNm) != 0
        Continue
    else
        varExistingEmmHndlNbr = varItr
        Continue

if varExistingEmmHndlNbr != -1
    _AX = EMM_GetHandlePageCount(gEMM_Table[varExistingEmmHndlNbr].EmmHndlNbr)
    if _AX < argEmmLogicalPageCount
        itoa(); itoa(); itoa(); strcpy(); strcat(); strcat(); strcat(); strcat(); strcat(); strcat();
        ...Temp_String...cnst_EMMErr_Reload1...' reloaded into EMM, diff size ='
        GAME_QuitProgram(Temp_String)
    else
        _AX = _DI
        return _AX

strcpy(gEMM_Table[gEMM_Open_Handles].EmmHndlNm, argEmmHandleName)

if argEmmRsvdFlag != 1
    ? gEMM_Open_Handles = (
        _BX = gEMM_Open_Handles * sizeof EMM_Record
        _AL = gEMM_Table[].EmmHndlNm + _BX
        NEG _AL
        gEMM_Table[gEMM_Open_Handles].EmmHndlNm)
    ?

_DI = EMM_MakeNamedHandle(argEmmLogicalPageCount, gEMM_Table[gEMM_Open_Handles])

if _DI == 0
    _AX = _DI
    return _AX

strcpy(gEMM_Table[gEMM_Open_Handles].EmmHndlNm, argEmmHandleName)
gEMM_Table[gEMM_Open_Handles].Rsvd = argEmmRsvdFlag
gEMM_Table[gEMM_Open_Handles].EmmHndlNbr = _DI
gEMM_Open_Handles++

if argEmmRsvdFlag != 1
    gEMM_Pages_Reserved = gEMM_Pages_Reserved - argEmmLogicalPageCount

    // asm{cmp [gEMM_Pages_Reserved],0; jge @@NewJmpJmpJmpDone}
    // Jump If (0 - gEMM_Pages_Reserved) ???
if !(gEMM_Pages_Reserved >= 0)
    'EMM reserved exceeded by ' gEMM_Pages_Reserved ' blocks [' argEmmHandleName '.LBX]'
    GAME_QuitProgram()

_AX = _DI
return _AX

*/

unsigned int EMM_GetOrCreateHandle(unsigned int argEmmLogicalPageCount, char *argEmmHandleName, int argEmmRsvdFlag)
{
    char varTmpStrEmmRsvdErr[6] = {0};
    int varExistingEmmHndlNbr;
    unsigned int varItrOurOpenHandleCount;
    unsigned int varOurOpenHandlePageCount;
    unsigned int varEmmHndlNbr;

    if (argEmmHandleName == NULL)
    {
        return 0;
    }

    /* Loop through our EMM Table, Check for existing handle by name */
    varExistingEmmHndlNbr = -1;
    varItrOurOpenHandleCount = 0;

    while (varItrOurOpenHandleCount < g_EMM_Open_Handles)
    {
        if (strcmp(argEmmHandleName, g_EMM_Table[varItrOurOpenHandleCount].eEmmHndlNm) == 0)
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
        if (varOurOpenHandlePageCount < argEmmLogicalPageCount)
        {
            itoa(g_EMM_Table[varExistingEmmHndlNbr].eEmmHndlNbr, Tmp_Conv_Str_1, 10);
            itoa(varEmmHndlNbr, Tmp_Conv_Str_2, 10);
            itoa(argEmmLogicalPageCount, Tmp_Conv_Str_3, 10);
            strcpy(Temp_String, argEmmHandleName);
            strcat(Temp_String, cnst_EmmErr_Reload1);  //' reloaded into EMM, diff size ='
            strcat(Temp_String, Tmp_Conv_Str_1);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_2);
            strcat(Temp_String, cnst_EmmErr_Space);
            strcat(Temp_String, Tmp_Conv_Str_3);
            GAME_QuitProgram(Temp_String);
        }
        else
        {
            return varEmmHndlNbr;
        }
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, argEmmHandleName);

    if (argEmmRsvdFlag != 1)
    {
        /*
            cmp  [bp+argEmmRsvdFlag], 1
            jnz  short @@NotReserved

            @@Reserved:
            mov  ax, [gEMM_Open_Handles]
            mov  dx, e_sizeof_EMM_RECORD
            imul dx
            mov  bx, ax
            mov  al, [byte ptr EMM_Table.EmmHndlNm+bx]
            neg  al
            push ax
            mov  ax, [gEMM_Open_Handles]
            mov  dx, e_sizeof_EMM_RECORD
            imul dx
            mov  bx, ax
            pop  ax
            mov  [byte ptr EMM_Table.EmmHndlNm+bx], al
        */
        /*
        ? gEMM_Open_Handles = (
            _BX = gEMM_Open_Handles * sizeof EMM_Record
            _AL = gEMM_Table[].EmmHndlNm + _BX
            NEG _AL
            gEMM_Table[gEMM_Open_Handles].EmmHndlNm)
        ?
        */
    }

    varEmmHndlNbr = EMM_MakeNamedHandle(argEmmLogicalPageCount, g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm);

    if (varEmmHndlNbr == 0)
    {
        return 0;  // ~= return varEmmHndlNbr
    }

    strcpy(g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNm, argEmmHandleName);
    g_EMM_Table[g_EMM_Open_Handles].eEmmRsrvd = argEmmRsvdFlag;
    g_EMM_Table[g_EMM_Open_Handles].eEmmHndlNbr = varEmmHndlNbr;
    g_EMM_Open_Handles++;

    if (argEmmRsvdFlag != 1)
    {
        g_EMM_Pages_Reserved = g_EMM_Pages_Reserved - argEmmLogicalPageCount;
    }

    if ( g_EMM_Pages_Reserved < 0 )
    {
        // 'EMM reserved exceeded by ' gEMM_Pages_Reserved ' blocks [' argEmmHandleName '.LBX]'
        //asm mov ax, [gEMM_Pages_Reserved]
        //asm neg ax
        itoa((g_EMM_Pages_Reserved * -1), varTmpStrEmmRsvdErr, 10);
        strcpy(Temp_String, cnst_EmmErr_ResOut1);
        strcat(Temp_String, varTmpStrEmmRsvdErr);
        strcat(Temp_String, cnst_EmmErr_ResOut2);
        strcat(Temp_String, argEmmHandleName);
        strcat(Temp_String, cnst_EmmErr_ResOut3);
        GAME_QuitProgram(Temp_String);
    }

    return varEmmHndlNbr;
}
