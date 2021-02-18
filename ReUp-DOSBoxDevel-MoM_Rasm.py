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

if (platform_system == "win32"):
    #os.system("copy file1.txt file7.txt")
    pass

elif (platform_system == "CYGWIN_NT-6.1-7601"):
    #shutil.copy2(src_file, dest_file, *, follow_symlinks=True)
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/_Dasm_Mom/IDA55_MoM131")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/dseg")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg000")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg001")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg002")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg003")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg004")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg005")
    os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg006")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg007")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg008")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg009")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg010")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg011")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg012")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg013")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg014")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg015")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg016")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg017")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg018")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg019")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg020")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg021")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg022")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg023")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg024")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg025")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg026")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg027")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg028")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg029")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg030")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg031")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg032")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg033")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg034")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg035")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg036")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg037")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg038")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/seg054")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub048")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub049")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub050")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub051")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub052")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub053")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub054")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub055")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub056")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub057")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub058")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub059")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub060")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/stub061")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr048")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr049")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr050")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr051")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr052")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr053")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr054")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr055")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr056")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr057")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr058")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr059")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr060")
    # os.system("mkdir --parents /c/Development/STU-MoM_Rasm/MoM_Rasm/ovr061")

    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/_Dasm_Mom/IDA55_MoM131/* /c/Development/STU-MoM_Rasm/MoM_Rasm/_Dasm_Mom/IDA55_MoM131/")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/_Dasm_Mom/*.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/_Dasm_Mom/")

    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/dseg/_dseglst.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/dseg/_dseglst.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/seg001/f010109.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/seg001/f010109.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/seg004/f040101.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/seg004/f040101.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/seg005/f050207.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/seg005/f050207.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/seg006/f060206.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/seg006/f060206.ASM")

    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/seg002/_seg002.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/seg002/_seg002.ASM")

    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/_CONFIG.MOM /c/Development/STU-MoM_Rasm/MoM_Rasm/_CONFIG.MOM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/_DSEGLST.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/_DSEGLST.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/_SEG002.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/_SEG002.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/ASM_Segment_Template.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/ASM_Segment_Template.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/F050207.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/F050207.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/F060206.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/F060206.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MAKE_MRM.BAT /c/Development/STU-MoM_Rasm/MoM_Rasm/MAKE_MRM.BAT")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MAKE1ASM.BAT /c/Development/STU-MoM_Rasm/MoM_Rasm/MAKE1ASM.BAT")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.CPP /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.CPP")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.EXE /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.EXE")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.H /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.H")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.MAP /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.MAP")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM.RSP /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM.RSP")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM_A.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM_A.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM_A.INC /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM_A.INC")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM_A.OBJ /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM_A.OBJ")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM_DSEG.ASM /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM_DSEG.ASM")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/MRM_SEG.INC /c/Development/STU-MoM_Rasm/MoM_Rasm/MRM_SEG.INC")
    os.system("cp /j/STU/DBWD/devel/Rasm_MoM/TLINK.CFG /c/Development/STU-MoM_Rasm/MoM_Rasm/TLINK.CFG")

else:
    print("FAILURE: Unidentified Platform.")
