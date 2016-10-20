
#ifndef _PNGINFO_H_
#define _PNGINFO_H_

#include "png_chunk.h"


#define ABSOLUTE(X)		X<0 ? -X : X

#define png_composite(composite, fg, alpha, bg)                  \
{ UINT16 temp = (UINT16)((UINT16)(fg) * (UINT16)(alpha)	 \
	+ 		  (UINT16)(bg) * (UINT16)(255 -      \
	    (UINT16)(alpha)) + (UINT16)128);         \
    (composite) = (UINT8)((temp + (temp >> 8)) >> 8); }


void pixel_ctype_0_bdepth_L8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_0_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_0_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output);

void pixel_ctype_2_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_2_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output);

void pixel_ctype_3_bdepth_L8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_3_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output);

void pixel_ctype_4_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_4_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output);

void pixel_ctype_6_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output);
void pixel_ctype_6_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output);

void filter_data_1(pngdata_l1_t *pngdata_l1,UINT32 len);
void filter_data_2(pngdata_l1_t *pngdata_l1,UINT32 len);
void filter_data_3(pngdata_l1_t *pngdata_l1,UINT32 len);
void filter_data_4(pngdata_l1_t *pngdata_l1,UINT32 len);

void filter_data(pnginfo_t *png_info,UINT8 pass);

void set_pass_size(pnginfo_t *png_info);

#endif


