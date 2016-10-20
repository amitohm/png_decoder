#ifndef _PNG_API_H_
#define _PNG_API_H_

#include "../test_app/pngdec.h"

#include "../zlib/zlib.h"

#include "png_chunk.h"
#include "png_error.h"
#include "png_pixel.h"

#define	 MIN(X,Y)	X < Y ? X : Y

#define	 MAX(X,Y)	X > Y ? X : Y

void init_data_buffers(pnginfo_t *png_info);

UINT8 png_check_signature(pnginfo_t *png_info);

UINT8 png_init_inflate(pngdata_l1_t *pngdata_l1);

UINT8 read_next_chunk(pnginfo_t *png_info);

#endif
