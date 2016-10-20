#ifndef _PNG_CHUNK_H_
#define _PNG_CHUNK_H_

#define		PNG_IHDR		0x49484452
#define 	PNG_IDAT		0x49444154
#define		PNG_PLTE		0x504C5445
#define		PNG_IEND		0x49454E44
#define		PNG_PHYS		0x70487973
#define		PNG_TRNS		0x74524E53
#define		PNG_GAMA		0x67414D41
#define		PNG_BKGD		0x624B4744
#define		PNG_HIST		0x68495354
#define		PNG_SRGB		0x73524742
#define		PNG_SBIT		0x73424954

#define COMPRESSION_METHOD_0		0
#define FILTER_METHOD_0				0
#define	NULL_METHOD					0
#define ADAM7						1


#define PNG_ROWBYTES(pixel_bits, width) \
    ((pixel_bits) >= 8 ? \
     ((width) * (((UINT32)(pixel_bits)) >> 3)) : \
     (( ((width) * ((UINT32)(pixel_bits))) + 7) >> 3) )


typedef struct pngdata_l1_t {

    /**********************UINT8**********************/

    //ihdr
    UINT8 ihdr_flag;
    UINT8 ihdr_bit_depth;
    UINT8 ihdr_colour_type;
    UINT8 ihdr_compression_method;
    UINT8 ihdr_filter_method;
    UINT8 ihdr_interlace_method;
    //ihdr

    //bkgd
    UINT16 bkgd_palette_index;
    //bkgd

    //sbit
    UINT8 sbit_grey;
    UINT8 sbit_red;
    UINT8 sbit_green;
    UINT8 sbit_blue;
    UINT8 sbit_alpha;
    //sbit

    //srgb
    UINT8 srgb_rendering_intent;
    //srgb

    //phys
    UINT8 phys_unit;
    //phys

    //filter
    UINT8 filter_type;
    UINT8 filter_interval;
    //filter

    //misc
    UINT8 bit_shift;
    UINT8 prev_row_val;
    UINT8 bit_depth_factor;
    UINT8 samples_per_pixel;
    UINT8 uncompressed_buf_select;
    UINT8 bit_depth_factor_tbl[5];
    UINT8 samples_per_pixel_tbl[7];
    UINT8 pass_factors_tbl[7][2];
    UINT8 shift_pass_tbl[8];
    UINT8 interlace_matrix_tbl[8][4];
    //misc

    //trns
    UINT8 trns_present;

    UINT8 trns_alpha_palette_cnt;
    UINT8 trns_alpha_palette[256];
    //trns

    //plte
    UINT8 plte_entry[256*3];
    //plte

    /**********************UINT8**********************/

    /**********************UINT16*********************/

    //bkgd
    UINT16 bkgd_grey;
    UINT16 bkgd_red;
    UINT16 bkgd_green;
    UINT16 bkgd_blue;
    //bkgd

    //trns
    UINT16 trns_grey;
    UINT16 trns_red;
    UINT16 trns_green;
    UINT16 trns_blue;
    //trns

    /**********************UINT16*********************/

    /**********************UINT32*********************/

    //ihdr
    UINT32 ihdr_cols;
    UINT32 ihdr_rows;
    //ihdr

    //idat
    UINT32 idat_length;
    //idat

    //phys
    UINT32 phys_pixel_per_unit_x;
    UINT32 phys_pixel_per_unit_y;
    //phys

    //plte
    UINT32 plte_entry_count;
    //plte

    //gama
    UINT32 gama_val;
    //gama

    //misc
    UINT32 inverse_rows;

    UINT32 chunk_type;
    UINT32 chunk_length;

    UINT8 *prev_row;
    UINT8 *cur_row;

    UINT8 *uncompressed_buf_1;
    UINT8 *uncompressed_buf_2;

    UINT32 pass_size[8];

    z_stream zstream; /* decompression stream */
    //misc

    /**********************UINT32*********************/

} pngdata_l1_t;

UINT8 png_read8(pnginfo_t *png_info);
UINT16 png_read16(pnginfo_t *png_info);
UINT32 png_read32(pnginfo_t *png_info);

void png_memset(UINT8 *dst_ptr,INT32 val,INT32 len);

void png_memcpy(UINT8 *dst_ptr,UINT8 *src_ptr,INT32 len);

UINT8 png_decode_phys(pnginfo_t *png_info);

UINT8 png_decode_bkgd(pnginfo_t *png_info);

UINT8 png_decode_plte(pnginfo_t *png_info);

UINT8 png_decode_trns(pnginfo_t *png_info);

UINT8 png_decode_srgb(pnginfo_t *png_info);

UINT8 png_decode_sbit(pnginfo_t *png_info);

UINT8 png_decode_ihdr(pnginfo_t *png_info);

void png_set_bkgd(pngdata_l1_t *pngdata_l1);

UINT8 png_skip_chunk(pnginfo_t *png_info);

UINT8 png_decode_chunks(pnginfo_t *png_info);

#endif

