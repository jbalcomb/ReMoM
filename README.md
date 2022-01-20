# Master of Magic - Reassembly Project


The goal of this project is to recreate a functional source code for Master of Magic, to allow the community to improve and expand the Simtex Universe to their hearts content.  

The source code in question is from the assembly code produced by the IDA Pro and Sourcerer disassemblers.  
It is then translated to Turbo Assembly, with MASM compatibility, and then rebuilt using Borland C++, Turbo Assembler, and Turbo Linker.  


To get involved, or even just hang out with other MoM fans, join us on the the "Master of Magic - Fans" Discord Server.  
This particular project is hosted by the [#mom-reverse-engineering Channel](https://discord.gg/kSNA8bE6Wt), on that server.  


---

NOTE: MoM game files required:  
    CONFIG.MOM, MAGIC.SET;  
    BACKGRND.LBX, FONTS.LBX, MAINSCRN.LBX, SPELLDAT.LBX, TERRSTAT.LBX, VORTEX.LBX, WIZARDS.LBX  

---

MRM - MoM Rasm - MAGIC.EXE  

Files List:  

Disassembly files produced from IDA  
/STU-MoM_Rasm/MoM_Rasm/_Dasm_Mom/IDA55_MoM131/  

Per Segment, the same disassembly files produced from IDA  
/STU-MoM_Rasm/MoM_Rasm/_Dasm_Mom/  

Per Segment, the refactored disassembly files produced from IDA  
/STU-MoM_Rasm/MoM_Rasm/<segment-label>/_<segment-label>.ASM  

/STU-MoM_Rasm/MoM_Rasm/  
MRM.CPP   main C++ source  
MRM.H     main C++ header  
MRM_A.ASM   main Assembly source  
MRM_A.INC   main Assembly include  
MRM_SEG.INC   Assembly include defining all segments  

MRM.RSP     MAKE/MAKER Response File  
TLINK.CFG   TLINK configuration file  

MAKE1ASM.BAT  Batch Script file to test compile one .ASM with TASM  
MAKE_MRM.BAT  Batch Scrip file to compile all of MRM, using the   MAKER Response File  

MRM.ASM   TASM Assembly output for MRM.CPP  
MRM.MAP   TASM Map File output for MRM.CPP  

MRM.OBJ       TASM Object File output for MRM.CPP  
MRM_A.OBJ     TASM Object File output for MRM_A.ASM  
<segment-label>.OBJ TASM Object File output for each <segment-label>.ASM  
F<ssnnmm>.Object    TASM Object File output for each f<ssnnmm>.ASM  [see below for detail]  
MRM.EXE       TLINK Executable File output  

---

ASM_Segment_Template.ASM    Template for Assembly Code/Data Segments  


---
---

Glossary of Terms:  
BCC: Borland C++ Compiler  
Casm: C++/C front-end for Assembly  
Dasm: dis-assembly  
MGC: MAGIC.EXE  
MoM: Master of Magic  
MRM: (M)oM (R)asm - (M)agic.Exe  
Rasm: re-assembly  
Simtex: Simtex Software (Simulation-Texas)  
TASM: Borland Turbo Assembler  
TLINK : Borland Turbo Linker  
WZD: WIZARDS.EXE  

---

f<ssnnmm>.ASM / F<ssnnmm>.OBJ  
'f'=Function, ss=Segment ID, nn=Function Number, mm=Function Count  
e.g., f010109.ASM is function #1, of 9, in 'seg001'  
