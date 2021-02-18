# Lin: `cp`
# ? Mac: `cp` ?
# Win: `copy`

# sys.platform
#   Lin: "linux"
#   Mac: "darwin"
#   Win: "win32"
#   Wondows 7 Ultimate SP1, C:\Program Files\Git\git-bash.exe: "cygwin"

# platform.system()
#   Lin:
#   Mac:
#   Win:
#   Wondows 7 Ultimate SP1, C:\Program Files\Git\git-bash.exe: "CYGWIN_NT-6.1-7601"

import os
import shutil
import sys
import platform

sys_platform = sys.platform
platform_system = platform.system()

if platform_system == "win32":
    os.system('copy file1.txt file7.txt')

elif platform_system == "CYGWIN_NT-6.1-7601":
    shutil.copy2(src_file, dest_file, *, follow_symlinks=True)
    os.system('cp file1.txt file7.txt')
else
    print("FAILURE: Unidentified Platform.")
