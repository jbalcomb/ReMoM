
// Source - https://stackoverflow.com/a/66523216
// Posted by Jcsq6, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-14, License - CC BY-SA 4.0

https://stackoverflow.com/questions/25704602/trying-to-statically-link-sdl-in-visual-studio

I've worked on this for a while, and I finally managed to statically link SDL2 on Windows. You have the option of using Cmake or Visual Studio's IDE. I prefer Cmake, but it is a bit less complicated with Visual Studio.

Prerequisites
* CMake (Required)
* Visual Studio 2019 (Not Required, but easier)

Step 1: Build SDL2
Download the SDL2 source from here. If you don't want to keep it in your downloads folder, I'd recommend unzipping it in your documents folder or something besides downloads. After you unzip the file, open a cmd prompt inside the folder. Make sure you go inside the folder inside the unzipped folder. (You should see a list of folders and file. If you only see one folder, then go inside that folder) Your directory should be something like ~\Documents\SDL2-2.0.14\SDL2-2.0.14. After you've gotten here, open a cmd prompt in this directory. Execute the command mkdir build && cd build. Next, execute the command cmake ... Assuming you have the default Windows 10 generator that is Visual Studio 16 2019, solutions will be created inside your build folder. If you are using a different generator, then build SDL2 as you would with that generator. You can close the command prompt now. Navigate inside your build folder, and open the ALL_BUILD.vcxproj file. This will open Visual Studio. Make sure to change your configuration to Release. Now that you have opened this, go into the properties of the SDL2 solution (right click on SDL2, then click properties at the bottom of the dialog). Now that you have the properties panel open, make sure you're under General in Configuration Properties. This should be where you open to automatically. Change the Configuration Type property from Dynamic Library (.dll) to Static Library (.lib). After that, go to Librarian, and then General. Change the end of Output File from $(TargetExt) to .lib. Now we have told Visual Studio to make a static library. Click Apply, and then press Ok. Before you build, we need to add a define in the source to let SDL2 we want to use C library functions. If you don't do this, then nothing will be able to compile. You need to find and open SDL_config.h. IMPORTANT: Make sure that you are editing the correct SDL_config.h! There are two! Make sure you are editing the one that is inside the build folder. If the SDL_config.h that you have opened only has 56 lines, then you are in the wrong one. Once you have found the right SDL_config.h, add the line #define HAVE_LIBC 1 somwehere above the line #if HAVE_LIBC. Make sure you save the file. Now right-click on the SDL2 solution, and click build. After that is finished, right-click on SDL2main, and click build. This will produce the files SDL2.lib and SDL2main.lib in ~\SDL2-2.0.14\SDL2-2.0.14\build\Release. Now that you have the static libraries, you can include them in your project.

Step 2 Option 1: Link Against Static Libs Using Visual Studio IDE
This method is a lot simpler, and for anyone reading not familiar with cmake, I'd recommend this. Note that you can use any c/c++ IDE by following the same instructions, but it will be a bit different. Open your Visual Studio solution that you want to statically link. Now go to the solution properties like we did earlier. Go to Linker->General->Additional Library Directories. Edit this list by adding the following libraries IN ORDER: SDL2main.lib SDL2.lib winmm.lib version.lib Imm32.lib Setupapi.lib libcmt.lib libucrtd.lib Note: if you don't use the editing input method provided by visual studio (clicking the down arrow to the far right of Linker->Input->Additional Dependencies), then you need to directly input these libraries with semicolons in-between. You may or may not need the libs libucrtd.lib or libcmt.lib, it will depend on your version. Make sure that you are including the SDL2 header files. After this is done, click apply, then ok. IMPORTANT: make sure that your main function has the signature int main(int, char**), otherwise your program will not compile.

Step 2 Option 2: Link Against Static Libs Using Cmake
Now we're going to be using Cmake to statically link SDL2. I'm going to be using msvc, since the runtime binaries are more easily accessible to msvc than to MinGW. For msvc we'll be using Cmake through Visual Studio. It is not necessary to use Visual Studio. All you really need is notepad and a compiler. If you were to use MinGW instead of msvc, then you would need to find the runtime binaries in Windows SDK and tell MinGW where to find them.
First, make a new Cmake project in Visual Studio. If you are presented with an introductory screen, feel free to press the button to create a CMakeSettings.json file. All this does is help configure different build types for cmake. It is not neccesary, but it can be useful. If you want a CMakeSettings.json file, but did not get the option to make one, then you can right-click a blank space in your solution explorer, and select Cmake Settings for <ProjectName>. Go into your source directory, and open the CmakeLists.txt file. Make sure that this is in the parent directory-- there are two CmakeLists.txt files, but right now we are concerned with the parent. Copy and paste the following code into that CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

project("tut")

add_subdirectory("src")
```

In the lines


```cmake
// Source - https://stackoverflow.com/a/66523216
// Posted by Jcsq6, modified by community. See post 'Timeline' for change history
// Retrieved 2026-03-14, License - CC BY-SA 4.0

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")

```

we are telling CMake to use static runtime libraries. ---If you want your program to be smaller, then you could use "/MD". Note that it would need the runtime dlls to run, and all that that implies.--- EDIT: YOU NEED TO USE /MD AND /MDd. SDL2 requires it. You can still statically link though.

```cmake
add_subdirectory("src")
```

This tells CMake that there is more information to be found in the subdirectory "src". If you have your src folder named anything else, make sure to replace "src" with your folders name. Now go into the subdirectory "src" (or whatever yours is called) and open that CMakeLists.txt. Copy and paste the following code into that CMakeLists.txt:

```cmake
cmake_minimum_required (VERSION 3.8)

add_executable(tut2 WIN32 "tut2.cpp")

target_include_directories(tut2 PRIVATE "${CMAKE_SOURCE_DIR}/include")

target_compile_definitions(tut2 PRIVATE "$<$<CONFIG:Debug>:" "_DEBUG" ">" "$<$<CONFIG:Release>:" "NDEBUG" ">" "WIN32;" "_CONSOLE;" "UNICODE;" "_UNICODE")
target_compile_options(tut2 PRIVATE /Oi; /Gy; /permissive-; /sdl; /W3; $<$<CONFIG:Debug>:/Z7>; ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}; ${DEFAULT_CXX_EXCEPTION_HANDLING})

target_link_options(tut2 PRIVATE $<$<CONFIG:Debug>: /INCREMENTAL> $<$<CONFIG:Release>: /DEBUG; /OPT:REF; /OPT:ICF; /INCREMENTAL:NO> /NODEFAULTLIB:LIBCMT)

target_link_libraries(tut2 PRIVATE SDL2main SDL2 winmm version Imm32 Setupapi)

target_link_directories(tut2 PRIVATE "${CMAKE_SOURCE_DIR}/lib")
```
In the lines

```cmake
target_include_directories(tut2 PRIVATE "${CMAKE_SOURCE_DIR}/include")
```

we are telling CMake where to look for our include files. In this example the include directory is in the source dir of our project. (The same place where our original CMakeLists.txt is located). If you feel the need to hard code in the directories, then you can just type in the absolute path to your files. Be warned, however, that this will make your code hard for others to build.

```cmake
target_compile_definitions(tut2 PRIVATE "$<$<CONFIG:Debug>:" "_DEBUG" ">" "$<$<CONFIG:Release>:" "NDEBUG" ">" "WIN32;" "_CONSOLE;" "UNICODE;" "_UNICODE")
target_compile_options(tut2 PRIVATE /Oi; /Gy; /permissive-; /sdl; /W3; $<$<CONFIG:Debug>:/Z7>; ${DEFAULT_CXX_DEBUG_INFORMATION_FORMAT}; ${DEFAULT_CXX_EXCEPTION_HANDLING})
```

In these lines we are telling cmake all the flags we want it to compile with. If you want to know what each flag does, then this is where you should look.

```cmake
target_link_options(tut2 PRIVATE $<$<CONFIG:Debug>: /INCREMENTAL> $<$<CONFIG:Release>: /DEBUG; /OPT:REF; /OPT:ICF; /INCREMENTAL:NO> /NODEFAULTLIB:LIBCMT)
```

In these lines we tell cmake all the flags we want to link our libraries with. If you want to know what each of these flags do, then this is where you should look.

```cmake
target_link_libraries(tut2 PRIVATE SDL2main SDL2 winmm version Imm32 Setupapi)
target_link_directories(tut2 PRIVATE "${CMAKE_SOURCE_DIR}/lib")
```

In these lines we tell Cmake all the libraries we need to link against, and a place to look to find those libraries. Again, ${CMAKE_SOURCE_DIR}/lib is a relative path pointing to where I have put SDL2.lib and SDL2main.lib, but you can put any relative or absolute path to replace it. You can now build the project, and, if you did everything right, it should compile successfully. If you get an error saying "cannot open projectName.exe", then just re-configure your CMakeList.txt files.

If this seems way harder than it should, that's because it is. I recently had to switch back to Windows 10, and have spent a good week on what took me 2 minutes on Linux. If you have the option, I'd highly recommend giving Linux a try. All you have to do there is pass --static-libs to sdl2-config, and voila! Your program is statically linked. If you have any questions, or if there's anything I forgot to cover, let me know in the comments.



The problem your running into is that you've configured the compiler to use static C runtime library LIBCMT.lib but some object file that you're linking with has requested that it be linked a default library. Specfically msvcrt.lib, the dynamic C runtime library. Because the two libraries define the same set of symbols you get multiply defined errors.

A possible solution is suggested in the warning message at the start of your list of errors: use /NODEFAULTLIB:library. The documentation for /NODEFAULTLIB explains how to set this option in the Visual Studio 2013 IDE:

Open the project's Property Pages dialog box. [...]
Click the Linker folder.
Click the Input property page.
Select the Ignore All Default Libraries property or specify a list of the libraries you want to ignore in the Ignore Specific Library property. The Command Line property page will show the effect of the changes you make to these properties.
The name of the default library you want to exclude is msvcrt.lib.





https://stackoverflow.com/questions/76799062/how-do-i-statically-link-sdl2-in-visual-studio-2022
