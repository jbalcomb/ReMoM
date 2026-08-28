


Windows 10
VS 2022
Python 3
...
git-bash
...
Strawberry
Conan
LibreOffice 7.4



Administrator: CMD
C:\>where cmake
C:\Program Files\CMake\bin\cmake.exe
C:\>where cmake
C:\Program Files\CMake\bin\cmake.exe
C:\>where clang
C:\Program Files\LLVM\bin\clang.exe
C:\>where gcc
C:\Strawberry\c\bin\gcc.exe
C:\>where code
C:\Users\jbalcomb\AppData\Local\Programs\Microsoft VS Code\bin\code.cmd
C:\>where cl
INFO: Could not find files for the given pattern(s).
C:\>where clang-cl
C:\Program Files\LLVM\bin\clang-cl.exe



## Visual Studio 2022


## Clang  (LLVM)
https://github.com/llvm/llvm-project/releases
https://github.com/llvm/llvm-project/releases/download/llvmorg-20.1.4/LLVM-20.1.4-win64.exe

LLVM-20.1.4-win64.exe

C:\Program Files\LLVM\bin\
clang.exe
clang++.exe


## WSL  (Windows Subsystem for Linux)

reset WSL image
 - reset                        (deletes application data, restores to default state)
 - unregister and reinstall     (removes distribution entirely) [allows for clean reinstall]

Resetting a WSL distribution (using the Settings app):
Open the Settings app.
Go to Apps > Installed apps.
Find the Linux distribution you want to reset.
Click the three-dot menu and select Advanced options.
Click the Reset button. 

Unregistering and reinstalling a WSL distribution:
Open an elevated PowerShell or Command Prompt.
Unregister the distribution using the command: wsl --unregister <DistroName> (replace <DistroName> with the name of your distribution, e.g., Ubuntu). 
Find the distribution in the Microsoft Store and reinstall it. 
Alternatively, you can disable and re-enable WSL in Windows Features and then reinstall. 

WSL shutdown:
wsl --shutdown to shut down all running WSL distributions. 

UPDATE 2023-01-06: You can use unregister on WSL v2 as well. You can also reset a distro from the add/remove programmes app.
WSL v1 is made up of 2 parts. WSL itself and the Linux distribution(s) you install.
You can reset any distribution that you have installed by using the following command (from W10 1903):
wsl --unregister <DistributionName>

You can then reinstall the distribution by finding it in the Windows store and clicking on "Launch".
Note that, prior to W10 1903, you need to use the wslconfig.exe - 1903 merges the commands from that into wsl.exe.
If you need to also reset WSL, you will need to uninstall it then reinstall. WSL is a Windows feature so you can add/remove it using the appropriate settings or control panel entry.

If you are running Windows 10 Version 1903:
wsl --unregister <DistributionName>
If you are running Windows 10 Version 1709, 1803, or 1809:
wslconfig /unregister <DistributionName>
Manage and configure Windows Subsystem for Linux
https://learn.microsoft.com/en-us/windows/wsl/wsl-config
Try this:
wsl --set-default-version 2
You can also set the version of wsl to any installed distributive:
wsl --set-version Ubuntu-20.04 1

https://learn.microsoft.com/en-gb/windows/wsl/

https://code.visualstudio.com/docs/remote/wsl


## Visual Studio Code


CMake: Select Configure Preset
CMake: Select Configure Preset lists the union of non-hidden Configure Presets defined in CMakePresets.json and CMakeUserPresets.json. Select a Configure Preset to make it the active Configure Preset. This is the configurePreset value that's used when CMake is invoked to generate the project build system. The active Configure Preset appears in the Project Status View on the CMake Tools sidebar.

CMake Tools uses the value of hostOS in the Microsoft Visual Studio Settings vendor map to hide Configure Presets that don't apply to your platform. For more information, see the entry for hostOS in the table under Visual Studio Settings vendor map.

CMake: Configure
To configure the project, run CMake: Configure from the command palette. This is the same as running cmake --preset <configurePreset> from the command line, where <configurePreset> is the name of the active Configure Preset.

Note
If you're a windows developer, you must open Visual Studio Code from a developer command prompt. Or, run the CMake: Scan for Compilers command before the CMake Tools extension can detect build tools (CMake, Ninja) that are installed with Visual Studio.



### VSC Settings
User
Workspace
Folder

./.vscode/
c_cpp_properties.json
cmake-tools-kits.json
launch.json
tasks.json
/c/Users/jbalcomb/.vscode/

%APPDATA%\Code\User\settings.json

VSC Settings files are located in the following locations:
Windows %APPDATA%\Code\User\settings.json
macOS $HOME/Library/Application Support/Code/User/settings.json
Linux $HOME/.config/Code/User/settings.json
VSC Extensions are located in the following locations:
Windows %USERPROFILE%\.vscode\extensions
macOS ~/.vscode/extensions
Linux ~/.vscode/extensions

As for your settings, you can open your user settings through the command palette:

Press F1
Type user settings
Press enter
Click the "sheet" icon to open the settings.json file:

For step '4.' you could also type @modified in the top-bar and press Enter - will show you just the modified settings. From there you can click each, click the cog ("Manage") and choose Reset. 



### VSC Extensions
C/C++
C/C++ Extension Pack
C/C++ Themes
Darcula Theme  (rokoroku)
EditorConfig for VS Code
Visual Studio Code WSL extension
CMake Tools
clangd
Clang-Format
Makefile Tools



## Conan

doc/__TODO-Test.md:Saving detected profile to C:\Users\jbalcomb\.conan2\profiles\default
