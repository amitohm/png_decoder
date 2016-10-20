#ifndef _PNGDEC_H_
#define _PNGDEC_H_

#include <string.h>

#ifndef NULL
#define NULL (void*)0
#endif

typedef char			INT8;
typedef unsigned char	UINT8;

typedef short			INT16;
typedef unsigned short	UINT16;

typedef int				INT32;
typedef unsigned int	UINT32;

#define		PNG_READ_HEADERS_SUCCESS		100
#define 	PNG_SUCCESS					0

#define		PNG_TERMINATE_DECODE		1
#define		PNG_TERMINATED				105

typedef struct pnginfo_t
{
	UINT8 png_terminate;
	UINT8 usrdef_bkgd;
	UINT8 usrdef_red;
	UINT8 usrdef_green;
	UINT8 usrdef_blue;
	
	UINT16 cols;
	UINT16 rows;
	
	UINT32 png_progress_indicator;
	
	UINT8 *png_input_buf;
	UINT32 png_input_buf_size;
	
	UINT8 *png_uncompressed_buf;
	UINT32 png_uncompressed_buf_size;
	
	UINT8 *png_output_buf;
	UINT32 png_output_buf_size;
	
	UINT32 l1_buf_size;
	void *l1_buf;
	
	void *user_data;
	
} pnginfo_t;

void png_init_decode(pnginfo_t *png_info);

UINT8 png_read_headers(pnginfo_t *png_info);

UINT8 png_decode(pnginfo_t *png_info);

void png_get_next_buffer(pnginfo_t *png_info);

#endif
