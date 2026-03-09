; dismiss_dll_popups.au3
; Automatically clicks OK on DLL-not-found error MessageBox popups,
; and copies the missing DLLs from bin/ to the test output directories.
; Run this in the background while running tests.
; Press Ctrl+Shift+Q to exit.

HotKeySet("^+q", "_Exit")

; Source directory for DLLs
Global Const $SRC_DIR = @ScriptDir & "\..\bin"

; DLL files to stage
Global Const $DLL_FILES[4] = ["SDL2.dll", "SDL2d.dll", "SDL2_mixer.dll", "gmock.dll"]

; Test output directories where executables live
Global Const $TEST_DIRS[7] = [ _
    @ScriptDir & "\..\out\build\MSVC-debug\MoM\tests\Debug", _
    @ScriptDir & "\..\out\build\MSVC-debug\MoX\tests\Debug", _
    @ScriptDir & "\..\out\build\MSVC-debug\tests\Debug", _
    @ScriptDir & "\..\out\build\MSVC-debug\src\Debug", _
    @ScriptDir & "\..\out\build\MSVC-nosound-debug\MoM\tests\Debug", _
    @ScriptDir & "\..\out\build\MSVC-nosound-debug\MoX\tests\Debug", _
    @ScriptDir & "\..\out\build\MSVC-nosound-debug\tests\Debug" _
]

; Stage all DLLs to all test directories on startup
_StageAllDLLs()

While 1
    ; Look for system error dialogs by title
    For $f = 0 To UBound($DLL_FILES) - 1
        ; Windows titles the dialog with the exe name, but the static text has the DLL name.
        ; Try matching by known DLL-error window titles.
        Local $handle = WinList()
        For $i = 1 To $handle[0][0]
            Local $title = $handle[$i][0]
            Local $hWnd = $handle[$i][1]
            If $title <> "" And BitAND(WinGetState($hWnd), 2) Then
                ; Read the static text control inside the MessageBox
                Local $sMsg = ControlGetText($hWnd, "", "[CLASS:Static; INSTANCE:1]")
                If StringInStr($sMsg, ".dll") And (StringInStr($sMsg, "was not found") Or StringInStr($sMsg, "not designed to run") Or StringInStr($title, "System Error")) Then
                    ConsoleWrite("Caught popup: " & $title & " - " & $sMsg & @CRLF)
                    _CopyMissingDLL($sMsg)
                    ControlClick($hWnd, "", "[CLASS:Button; TEXT:OK]")
                EndIf
            EndIf
        Next
    Next
    Sleep(250)
WEnd

Func _StageAllDLLs()
    For $d = 0 To UBound($TEST_DIRS) - 1
        If FileExists($TEST_DIRS[$d]) Or FileExists(StringReplace($TEST_DIRS[$d], "\Debug", "")) Then
            DirCreate($TEST_DIRS[$d])
            For $f = 0 To UBound($DLL_FILES) - 1
                Local $src = $SRC_DIR & "\" & $DLL_FILES[$f]
                Local $dst = $TEST_DIRS[$d] & "\" & $DLL_FILES[$f]
                If FileExists($src) And Not FileExists($dst) Then
                    FileCopy($src, $dst)
                    ConsoleWrite("Staged: " & $DLL_FILES[$f] & " -> " & $TEST_DIRS[$d] & @CRLF)
                EndIf
            Next
        EndIf
    Next
EndFunc

Func _CopyMissingDLL($sMsg)
    For $f = 0 To UBound($DLL_FILES) - 1
        If StringInStr($sMsg, $DLL_FILES[$f]) Then
            Local $src = $SRC_DIR & "\" & $DLL_FILES[$f]
            If Not FileExists($src) Then
                ConsoleWrite("WARNING: Source DLL not found: " & $src & @CRLF)
                ContinueLoop
            EndIf
            For $d = 0 To UBound($TEST_DIRS) - 1
                Local $dst = $TEST_DIRS[$d] & "\" & $DLL_FILES[$f]
                If Not FileExists($dst) Then
                    DirCreate($TEST_DIRS[$d])
                    FileCopy($src, $dst)
                    ConsoleWrite("Copied: " & $DLL_FILES[$f] & " -> " & $TEST_DIRS[$d] & @CRLF)
                EndIf
            Next
        EndIf
    Next
EndFunc

Func _Exit()
    Exit
EndFunc
