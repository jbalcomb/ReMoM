
TERRAIN.LBX

File Size: 681564

Entries:    3
Type:       5



WIZARDS.EXE
dseg:29D2 terrain_lbx_file db 'TERRAIN.LBX',0     ; DATA XREF: Terrain_Init+Bo ...
XREF:
	Terrain_Init+B  mov     ax, offset terrain_lbx_file     ; "TERRAIN.LBX"
	Terrain_Init+35 mov     ax, offset terrain_lbx_file     ; "TERRAIN.LBX"
	Terrain_Init+52 mov     ax, offset terrain_lbx_file     ; "TERRAIN.LBX"

terrain_lbx_002
	Terrain_Init+4B     mov     [word ptr terrain_lbx_002], ax  ; single-loaded full entry, contains one color index
	Draw_Reduced_Map+E5 les     bx, [terrain_lbx_002]           ; single-loaded full entry, contains one color index
	Terrain_Init+47     mov     [word ptr terrain_lbx_002+2], dx; single-loaded full entry, contains one color index
...
color map/palette indices for the terrain type on the "reduced map"/'mini-map'



40 02 00 00 
80 54 0A 00 
68 60 0A 00 
5C 66 0A 00 

000A5480 - 0000240 = A5240 676416  762 * 88
000A6068 - 00A5480 =   BE8   3048  762 * 4
000A665C - 00A6068 =   5F4   1524  762 * 2
