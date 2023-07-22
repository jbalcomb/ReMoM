@ECHO OFF
REM https://yakvi.github.io/handmade-hero-notes/html/day1.html#debugginginremedybgorvisualstudio/setupvisualstudiodebugger
REM right-click on the win32_handmade in Solution Explorer → Properties to change your Working Directory to C:\devel\STU-MoM_Rasm\. 
REM Click File → Save MoM to save your solution as MoM.sln in misc folder, then modify your debug.bat file to load the solution directly.
REM run `devenv build\MoM.exe`
REM Visual studio will load a dummy solution

REM devenv build\PoC.exe
start devenv misc\PoC.sln
