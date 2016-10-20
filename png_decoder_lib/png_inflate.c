
#include "png_api.h"

UINT8 png_init_inflate(pngdata_l1_t *pngdata_l1)
{
    INT32 err_val;

    pngdata_l1->zstream.zalloc = (alloc_func)0;
    pngdata_l1->zstream.zfree = (free_func)0;
    pngdata_l1->zstream.opaque = (voidpf)0;

    err_val = inflateInit(&pngdata_l1->zstream);

    switch (err_val)
    {
	case Z_OK:
	    /* Do nothing */
	    break;

	case Z_MEM_ERROR:
	case Z_STREAM_ERROR:
	    return PNG_ZLIB_MEMORY_ERROR;

	case Z_VERSION_ERROR:
	    return PNG_ZLIB_VERSION_ERROR;

	default:
	    return PNG_UNKNOWN_ZLIB_ERROR;
    }

    return PNG_SUCCESS;
}

