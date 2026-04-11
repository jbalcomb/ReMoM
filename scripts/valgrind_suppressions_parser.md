
valgrind_suppressions_parser.md
valgrind_suppressions_parser.py

Pilfered from:
https://wiki.wxwidgets.org/Valgrind_Suppression_File_Howto
|-> https://wiki.wxwidgets.org/Parse_valgrind_suppressions.sh
Asked Gemini3Pro to translate it to python

https://valgrind.org/docs/manual/manual-core.html
https://valgrind.org/docs/manual/manual-core.html#manual-core.options
https://valgrind.org/docs/manual/mc-manual.html#mc-manual.suppfiles

https://www.jedsharpsoftware.com/c++/2022/08/15/FindingMemoryLeaksValgrind.html

Below the first line is a stack trace telling you where the problem occurred. Stack traces can get quite large, and be confusing, especially if you are using the C++ STL. Reading them from the bottom up can help. If the stack trace is not big enough, use the --num-callers option to make it bigger.
The code addresses (eg. 0x804838F) are usually unimportant, but occasionally crucial for tracking down weirder bugs.


there is a  - Conditional jump or move depends on uninitialised value(s)
caused by   - Uninitialised value was created by a stack allocation

==509881== Conditional jump or move depends on uninitialised value(s)
==509881==    at 0x4C80893: getenv (getenv.c:55)
==509881==    by 0xCB90AFB: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xCC1A0E2: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xD22BEC9: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xD16B7CE: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xD1739EF: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xCB7F208: ??? (in /usr/lib/x86_64-linux-gnu/libnvidia-glcore.so.580.126.09)
==509881==    by 0xC001330: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_nvidia.so.580.126.09)
==509881==    by 0xC001467: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_nvidia.so.580.126.09)
==509881==    by 0xC02AE33: ??? (in /usr/lib/x86_64-linux-gnu/libGLX_nvidia.so.580.126.09)
==509881==    by 0xC01F23D: glXChooseVisual (in /usr/lib/x86_64-linux-gnu/libGLX_nvidia.so.580.126.09)
==509881==    by 0x4B3161B: ??? (in /usr/lib/x86_64-linux-gnu/libSDL2-2.0.so.0.3200.4)
==509881==  Uninitialised value was created by a stack allocation
==509881==    at 0x414A1A0: Startup_Platform (platform/sdl2/sdl2_Init.c:43)
