
#include "MoX_TYPE.H"
#include "ST_DEF.H"

#include "STU_BMP.H"

#include <STDIO.H>      /* FILE; */


// J:\Development\STU-LBX_Library\LibLBX\lib_lbx_process_lbx.c
//        BMP_FILE * bmp;
//        bmp = malloc(sizeof(BMP_FILE));
//        // liblbx_convert_flic_frame_to_bmp(bmp, lbx);
//        /* 0: MAIN000, MAINSCRN - main screen, 320x200, 1 frame; 1: MAIN001, MAINBUTN - game button, 39x12, 2 frames */
//        liblbx_convert_flic_frame_to_bmp(bmp, lbx, 0, 0);
//        liblbx_convert_flic_frame_to_bmp(bmp, lbx, 1, 0);

// J:\Development\STU-LBX_Library\LibLBX\lib_lbx_file_type_bmp.h

/*
 * MS BMP / Windows DIB (Device-Independent Bitmap)
 *
 */

/*
 * FWIW, MS Paint has the following default values:
 *   Bitmap File Header:
 *     42 4D            0 00   Type
 *     00 00            6 06   Reserved1
 *     00 00            8 08   Reserved2
 *     36 04 00 00     10 0A   OffBits
 *   Bitmap Info Header:
 *                     14 0E   Size
 *                     18 12   Width
 *                     22 16   Height
 *     01 00           26 1A   Planes
 *     08 00           38 1C   BitCount
 *     00 00 00 00     30 1E   Compression
 *                     34 22   SizeImage
 *     00 00 00 00     38 26   XPelsPerMeter
 *     00 00 00 00     42 2A   YPelsPerMeter
 *     00 00 00 00     46 2E   ClrUsed
 *     00 00 00 00     50 32   ClrImportant
 *
 */

    /*
    * Fields to Populate:
    *   BMP_BITMAP_FILE_HEADER ->
    *     File Type Identifier
    *     File Size, in bytes
    *     Reserved Field 1
    *     Reserved Field 2
    *     Offset to Bitmap Pixel image_data_buffer
    *  BMP_BITMAP_INFO_HEADER ->
    *    Size of Info Header, in bytes
    *    Width, in pixels
    *    Height, in pixels
    *    Number of Planes
    *    Bits Per Pixel
    *    Compression Scheme
    *    Size of Bitmap, in bytes
    *    Horz. Resolution, in Pixels/Meter
    *    Vert. Resolution, in Pixels/Meter
    *    Number of Colors in Color-Table
    *    Number of Important Colors
    */


#define VBB_BMP_FILE_SIZE  (14 + 40 + (320 * 200))
#define VBB_BMP_IMAGE_SIZE (320 * 200)

// ? #pragma pack(push,1)
struct s_BMP_BITMAP_FILE_HEADER VBB_BMPINFOHEADER = 
{
    BMP_MAGIC,                      /* Specifies the type of file. It must be BM. */
    VBB_BMP_FILE_SIZE,              /* Specifies the size in DWORDs of the file.  */
    0,                              /* Is reserved and must be set to zero. */
    0,                              /* Is reserved and must be set to zero. */
    54                              /* Specifies, in bytes, the offset from the beginning of the file (BOF) to the image data (bitmap) */
};
// ? #pragma pack(pop)
// bmp->bmp_bitmap_file_header->bfOffBits = 14 + 40 + 1024;

// ? #pragma pack(push,1)
struct s_BMP_BITMAP_INFO_HEADER VBB_BMPINFOHEADERFILE =
{
    BMP_BITMAP_INFO_HEADER_LENGTH,  /* Specifies the number of bytes required by the BITMAPINFOHEADER structure. */
    320,                            /* Specifies the width of the bitmap in pixels. */
    -200,                           /* Specifies the height of the bitmap in pixels. */
    1,                              /* Specifies the number of planes for the target device and must be set to 1. */
    8,                              /* Specifies the number of bits per pixel. This value must be 1, 4, 8, or 24. */
    0,                              /* Specifies the type of compression for a compressed bitmap. */
    VBB_BMP_IMAGE_SIZE,             /* Specifies the size in bytes of the image. */
    BMP_XPELS_PER_METER_DEFAULT,    /* */
    BMP_YPELS_PER_METER_DEFAULT,    /* */
    256,                            /* */
    256                             /* */
};
// ? #pragma pack(pop)
// bmp->bmp_bitmap_info_header->biSizeImage = element_count; /* (4294967296 - 1) - (14 + 40) - (256 * 4);  / * 2^32 - Headers - Palette * / */


void STU_Dump_BMP(void)
{

}

void STU_BMP_Video_Back_Buffer(void)
{
    char filename[13] = "VBB.BMP";
    FILE * fileptr;

#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] BEGIN: STU_BMP_Video_Back_Buffer()\n", __FILE__, __LINE__);
#endif


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfType: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfType);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfSize: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved1: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved1);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfReserved2: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfReserved2);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADER.bfOffBits: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADER.bfOffBits);

    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSize: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSize);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biWidth: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biWidth);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biHeight: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biHeight);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biPlanes: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biPlanes);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biBitCount: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biBitCount);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biCompression: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biCompression);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biSizeImage: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biSizeImage);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biXPelsPerMeter: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biXPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biYPelsPerMeter: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biYPelsPerMeter);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrUsed: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrUsed);
    dbg_prn("DEBUG: [%s, %d] VBB_BMPINFOHEADERFILE.biClrImportant: %08X\n", __FILE__, __LINE__, VBB_BMPINFOHEADERFILE.biClrImportant);

#endif

    int itr;
    unsigned long int bmp_image_size;
    byte color_table_index;

    fileptr = fopen(filename, "wb");
    // if(fileptr == NULL) { }

    fwrite(&VBB_BMPINFOHEADER, sizeof(VBB_BMPINFOHEADER), 1, fileptr);
    fwrite(&VBB_BMPINFOHEADERFILE, sizeof(VBB_BMPINFOHEADERFILE), 1,fileptr);

    bmp_image_size = VBB_BMPINFOHEADERFILE.biWidth * -VBB_BMPINFOHEADERFILE.biHeight;

    color_table_index = MAGENTA;
    for(itr = 0; itr < bmp_image_size; itr++)
    {
        fwrite(&color_table_index, sizeof(color_table_index), 1, fileptr);
    }

    fclose(fileptr);


//    FILE *output_file;
//    output_file = fopen("generated_img.bmp", "wb");
//    if(output_file==NULL)
//    {
//        perror("Error while opening the file ");
//        exit(1);
//    }
//    fwrite(&BMPHEADERFILE, sizeof(BMPHEADERFILE), 1, output_file);
//    fwrite(&BMPINFOHEADERFILE, sizeof(BMPINFOHEADERFILE), 1,output_file);
//    size_t bmp_size = BMPINFOHEADERFILE.width*BMPINFOHEADERFILE.height;
//    for(int i=0;i<bmp_size;i++)
//    {
//        pxl.lower = rand()%17;
//        pxl.higher = rand()%17;
//
//        fwrite(&pxl, sizeof(pxl),1,output_file);
//    }
//    fclose(output_file);


#ifdef STU_DEBUG
    dbg_prn("DEBUG: [%s, %d] END: STU_BMP_Video_Back_Buffer()\n", __FILE__, __LINE__);
#endif

}





// 
//     /* ************************* *
//      *                           *
//      *   BEGIN: MS BMP/DIB 6bpp  *
//      *                           *
//      * ************************* */
// 
//     /* Image image_data_buffer Buffer Size, General Equation: (Image Width * Image Height * Bytes Per Pixel) */



// 
//     /* ******************* *
//      *                     *
//      *   END: MS BMP/DIB   *
//      *                     *
//      * ******************* */
// 
// 
//     if (itr_entry_index == 0 && itr_frame_index == 0)
//     {
//         /* char * export_file_path; */
//         char export_file_path[] = "C:\\Development\\STU-LBX_Library\\docs\\MAIN000_FF0.BMP";
//         FILE * ptr_stream_file_out;
//         ptr_stream_file_out = fopen(export_file_path, "wb");
// 
//         char tmp_bmp_hdr_export_file_path[] = "C:\\Development\\STU-LBX_Library\\docs\\MAIN000_FF0_thdr.BIN";
//         FILE * tmp_bmp_hdr_ptr_stream_file_out;
//         tmp_bmp_hdr_ptr_stream_file_out = fopen(tmp_bmp_hdr_export_file_path, "wb");
// 
//         char tmp_bmp_pal_export_file_path[] = "C:\\Development\\STU-LBX_Library\\docs\\MAIN000_FF0_tpal.BIN";
//         FILE * tmp_bmp_pal_ptr_stream_file_out;
//         tmp_bmp_pal_ptr_stream_file_out = fopen(tmp_bmp_pal_export_file_path, "wb");
// 
// 
//         if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(&(bmp->bmp_bitmap_file_header->), sizeof(bmp->bmp_bitmap_file_header->), 1, tmp_bmp_hdr_ptr_stream_file_out);\n");
// 
//         fwrite(&(bmp->bmp_bitmap_file_header->bfType), sizeof(bmp->bmp_bitmap_file_header->bfType), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfSize), sizeof(bmp->bmp_bitmap_file_header->bfSize), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfReserved1), sizeof(bmp->bmp_bitmap_file_header->bfReserved1), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfReserved2), sizeof(bmp->bmp_bitmap_file_header->bfReserved2), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfOffBits), sizeof(bmp->bmp_bitmap_file_header->bfOffBits), 1, tmp_bmp_hdr_ptr_stream_file_out);
// 
//         fwrite(&(bmp->bmp_bitmap_info_header->biSize), sizeof(bmp->bmp_bitmap_info_header->biSize), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biWidth), sizeof(bmp->bmp_bitmap_info_header->biWidth), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biHeight), sizeof(bmp->bmp_bitmap_info_header->biHeight), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biPlanes), sizeof(bmp->bmp_bitmap_info_header->biPlanes), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biBitCount), sizeof(bmp->bmp_bitmap_info_header->biBitCount), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biCompression), sizeof(bmp->bmp_bitmap_info_header->biCompression), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biSizeImage), sizeof(bmp->bmp_bitmap_info_header->biSizeImage), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biXPelsPerMeter), sizeof(bmp->bmp_bitmap_info_header->biXPelsPerMeter), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biYPelsPerMeter), sizeof(bmp->bmp_bitmap_info_header->biYPelsPerMeter), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biClrUsed), sizeof(bmp->bmp_bitmap_info_header->biClrUsed), 1, tmp_bmp_hdr_ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biClrImportant), sizeof(bmp->bmp_bitmap_info_header->biClrImportant), 1, tmp_bmp_hdr_ptr_stream_file_out);
// 
//         if (STU_DEBUG) printf("DEBUG: END: fwrite(&(bmp->bmp_bitmap_file_header->), sizeof(bmp->bmp_bitmap_file_header->), 1, tmp_bmp_hdr_ptr_stream_file_out);\n");
// 
//         if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(&(bmp->bmp_bitmap_file_header->), sizeof(bmp->bmp_bitmap_file_header->), 1, ptr_stream_file_out);\n");
// 
//         fwrite(&(bmp->bmp_bitmap_file_header->bfType), sizeof(bmp->bmp_bitmap_file_header->bfType), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfSize), sizeof(bmp->bmp_bitmap_file_header->bfSize), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfReserved1), sizeof(bmp->bmp_bitmap_file_header->bfReserved1), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfReserved2), sizeof(bmp->bmp_bitmap_file_header->bfReserved2), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_file_header->bfOffBits), sizeof(bmp->bmp_bitmap_file_header->bfOffBits), 1, ptr_stream_file_out);
// 
//         fwrite(&(bmp->bmp_bitmap_info_header->biSize), sizeof(bmp->bmp_bitmap_info_header->biSize), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biWidth), sizeof(bmp->bmp_bitmap_info_header->biWidth), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biHeight), sizeof(bmp->bmp_bitmap_info_header->biHeight), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biPlanes), sizeof(bmp->bmp_bitmap_info_header->biPlanes), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biBitCount), sizeof(bmp->bmp_bitmap_info_header->biBitCount), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biCompression), sizeof(bmp->bmp_bitmap_info_header->biCompression), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biSizeImage), sizeof(bmp->bmp_bitmap_info_header->biSizeImage), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biXPelsPerMeter), sizeof(bmp->bmp_bitmap_info_header->biXPelsPerMeter), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biYPelsPerMeter), sizeof(bmp->bmp_bitmap_info_header->biYPelsPerMeter), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biClrUsed), sizeof(bmp->bmp_bitmap_info_header->biClrUsed), 1, ptr_stream_file_out);
//         fwrite(&(bmp->bmp_bitmap_info_header->biClrImportant), sizeof(bmp->bmp_bitmap_info_header->biClrImportant), 1, ptr_stream_file_out);
// 
//         if (STU_DEBUG) printf("DEBUG: END: fwrite(&(bmp->bmp_bitmap_file_header->), sizeof(bmp->bmp_bitmap_file_header->), 1, ptr_stream_file_out);\n");
// 
// 
//         if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, tmp_bmp_pal_ptr_stream_file_out);\n");
//         fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, tmp_bmp_pal_ptr_stream_file_out);
//         if (STU_DEBUG) printf("DEBUG: END: fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, tmp_bmp_pal_ptr_stream_file_out);\n");
// 
//         if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, ptr_stream_file_out);\n");
//         fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, ptr_stream_file_out);
//         if (STU_DEBUG) printf("DEBUG: END: fwrite(bmp_palette_buffer, sizeof(unsigned char), BMP_PALETTE_SIZE, ptr_stream_file_out);\n");
// 
// 
//         // if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(bmp_image_buffer, sizeof(unsigned char), bmp_image_buffer_size, ptr_stream_file_out);\n");
//         // fwrite(bmp_image_buffer, sizeof(unsigned char), bmp_image_buffer_size, ptr_stream_file_out);
//         // if (STU_DEBUG) printf("DEBUG: END: fwrite(bmp_image_buffer, sizeof(unsigned char), bmp_image_buffer_size, ptr_stream_file_out);\n");
// 
//         if (STU_DEBUG) printf("DEBUG: BEGIN: fwrite(flipped_transposed_decoded_frame_buffer, sizeof(unsigned char), element_count, ptr_stream_file_out);\n");
//         fwrite(flipped_transposed_decoded_frame_buffer, sizeof(unsigned char), element_count, ptr_stream_file_out);
//         if (STU_DEBUG) printf("DEBUG: END: fwrite(flipped_transposed_decoded_frame_buffer, sizeof(unsigned char), element_count, ptr_stream_file_out);\n");
// 
//         fclose(ptr_stream_file_out);
//     }
