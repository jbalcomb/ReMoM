
; J:\STU-EduMat\Casm\avrbeginners_30_Accessing_C_Structs_in_Assembler_1.0.1.pdf
; https://www.avrfreaks.net/sites/default/files/avrbeginners_30_Accessing_C_Structs_in_Assembler_1.0.1.pdf

ST_Alloc_Space_Header_STRUC struc ; (sizeof=0x10, standard type)
safety_malloc_bsize dw ?
safety_malloc_prevreal dw ?
MemSig1	dw ?
MemSig2	dw ?
Size_Paras dw ?
Used_Paras dw ?
unused_0C dw ?
Marked_Paras dw	?
ST_Alloc_Space_Header_STRUC ends
