
#include <malloc.h>
#include <stdio.h>          /* FILE; fclose(), fopen(), fread(), fwrite(), printf(); */
#include <stdint.h>



/*
~==
Load_Font_File(char * font_file)
	strcpy(font_name, font_file);
	font_style_data = LBX_Load(font_file, 0);
	border_style_data = LBX_Load(font_file, 1);
	font_header = (struct s_FONT_HEADER *)font_style_data;
	palette_block          = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes
	// MoO2  current_palette
	p_Palette              = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
	// 1oom: lbxpal_palette = p_Palette;

	p_PaletteFlags         = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
// DELETE      p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32  4 bytes per pixel * 256 colors / 16 bytes per paragraph

	// TODO  UU_DAC_Save_Seg = Allocate_Space(48);

	// Replacement_Colors = Allocate_Space(384);  // 384 paragraphs = 384 * 16 = 6,144 bytes  (24 * 256  ~'remap color tables')
	remap_color_palettes = Allocate_Space(384);

	// TODO  gsa_VGAFILEH_Header = Allocate_Space(2);

	Intensity_Scale_Tbl = Allocate_Space(96);  // 96 paragraphs = 96 * 16 = 1,536 bytes

	// TODO  VGA_TextDraw_Init();


*/

/*
seek to 19232   0x4B20
read 768 bytes  0x300

*/

#define GET_4B_OFS(_ptr_,_ofs_) (                              \
  ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 0) )       ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 1) ) <<  8 ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 2) ) << 16 ) \
| ( (uint32_t) ( (uint8_t) *( (_ptr_) + (_ofs_) + 3) ) << 24 ) \
)



#define SZ_LBX_HEADER     512 
#define SZ_PARAGRAPH       16
#define SZ_32KB			32768



uint8_t lbx_header[SZ_LBX_HEADER];




strcpy(font_name, font_file);

font_style_data = LBX_Load(font_file, 0);
border_style_data = LBX_Load(font_file, 1);

font_header = (struct s_FONT_HEADER*)font_style_data;

palette_block = Allocate_Space(348);    // 348 paragraphs = 386 * 16 bytes = 5568 bytes

// MoO2  current_palette
p_Palette = Allocate_Space(64);     //  64 paragraphs =  64 * 16 bytes = 1024 bytes
// 1oom: lbxpal_palette = p_Palette;

p_PaletteFlags = p_Palette + (48 * 16);  // ~== p_PaletteFlags = &p_Palette[768];
// DELETE      p_Palette_XBGR         = Allocate_Space(64);     // STU/Win32  4 bytes per pixel * 256 colors / 16 bytes per paragraph

	// TODO  UU_DAC_Save_Seg = Allocate_Space(48);

	// Replacement_Colors = Allocate_Space(384);  // 384 paragraphs = 384 * 16 = 6,144 bytes  (24 * 256  ~'remap color tables')
remap_color_palettes = Allocate_Space(384);

// TODO  gsa_VGAFILEH_Header = Allocate_Space(2);

Intensity_Scale_Tbl = Allocate_Space(96);  // 96 paragraphs = 96 * 16 = 1,536 bytes  

// TODO  VGA_TextDraw_Init();




for (itr = 0; itr < 768; itr++)
{
	*(p_Palette + itr) = 0;
}
for (itr = 0; itr < 256; itr++)
{
	*(p_PaletteFlags + itr) = 1;
}



void STU_Load_Font(void)
{
	FILE * fileptr;
	int16_t entry_num;
	int32_t entry_start;
	int32_t entry_end;
	int32_t entry_length;
	uint32_t num_blocks;
	uint32_t read_size;
	uint8_t * data;
	uint8_t * rvr_data;

	entry_num = 0;

	fileptr = fopen("FONTS.LBX", "rb");

	fread(lbx_header, 1, SZ_LBX_HEADER, fileptr);

	entry_start = GET_4B_OFS(lbx_header, (8 + (entry_num * 4) + 0));
	entry_end   = GET_4B_OFS(lbx_header, (8 + (entry_num * 4) + 4));
	entry_length = entry_end - entry_start;

	fseek(fileptr, entry_start, 0);

	num_blocks = 1 + (entry_length / SZ_PARAGRAPH);  // "blocks" ~== "paragraphs"

	data = malloc(num_blocks * SZ_PARAGRAPH);

	rvr_data = data;

	read_size = SZ_32KB;
	while (entry_length >= read_size)
	{
		entry_length -= read_size;
#pragma warning(suppress : 6386)
		fread(rvr_data, read_size, 1, fileptr);  // C6386: Buffer overrun while writing to 'rvr_data': the writable size is 'num_blocks*16', but '32768' bytes might be written.
		rvr_data += read_size;
	}
	if (entry_length > 0)
	{
		read_size = entry_length;
#pragma warning(suppress : 6387)
		fread(rvr_data, read_size, 1, fileptr);  // C6387: 'rvr_data' could be '0': this does not adhere to the specification for the function 'fread'.
	}


	fclose(fileptr);

}
