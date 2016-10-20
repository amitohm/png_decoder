#include "png_api.h"

UINT8 png_read8(pnginfo_t *png_info)
{
    UINT8 temp = 0;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp = *png_info->png_input_buf++;	
    png_info->png_input_buf_size -= 1;

    return temp;
}

UINT16 png_read16(pnginfo_t *png_info)
{
    UINT16 temp = 0;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp = *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp <<= 8;
    temp += *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    return temp;
}

UINT32 png_read32(pnginfo_t *png_info)
{
    UINT32 temp = 0;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp = *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp <<= 8;
    temp += *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp <<= 8;
    temp += *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    if(png_info->png_input_buf_size==0)
	png_get_next_buffer(png_info);

    if(png_info->png_terminate)
	return temp;

    temp <<= 8;
    temp += *png_info->png_input_buf++;
    png_info->png_input_buf_size -= 1;

    return temp;
}

void png_memset(UINT8 *dst_ptr,INT32 val,INT32 len)
{
    memset(dst_ptr,val,len);
}

void png_memcpy(UINT8 *dst_ptr,UINT8 *src_ptr,INT32 len)
{
    memcpy(dst_ptr,src_ptr,len);
}

UINT8 read_next_chunk(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    pngdata_l1->chunk_length = png_read32(png_info);

    if(png_info->png_terminate)
	return PNG_TERMINATED;

    pngdata_l1->chunk_type = png_read32(png_info);

    if(png_info->png_terminate)
	return PNG_TERMINATED;

    return PNG_SUCCESS;
}

UINT8 png_check_signature(pnginfo_t *png_info)
{
    if((png_read32(png_info))!=0x89504e47)
    {
	if(png_info->png_terminate)
	    return PNG_TERMINATED;

	return PNG_INVALID_SIGNATURE;
    }

    if((png_read32(png_info))!=0x0d0a1a0a)
    {
	if(png_info->png_terminate)
	    return PNG_TERMINATED;

	return PNG_INVALID_SIGNATURE;
    }

    return PNG_SUCCESS;
}

UINT8 png_decode_ihdr(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    pngdata_l1->ihdr_cols = png_read32(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if(pngdata_l1->ihdr_cols==0)
	return PNG_ERROR_ZERO_COLS;

    pngdata_l1->ihdr_rows = png_read32(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if(pngdata_l1->ihdr_rows==0)
	return PNG_ERROR_ZERO_ROWS;

    pngdata_l1->ihdr_bit_depth = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if((pngdata_l1->ihdr_bit_depth!=1) && (pngdata_l1->ihdr_bit_depth!=2) && (pngdata_l1->ihdr_bit_depth!=4) && 
	    (pngdata_l1->ihdr_bit_depth!=8) && (pngdata_l1->ihdr_bit_depth!=16))
	return PNG_INVALID_BIT_DEPTH;

    pngdata_l1->ihdr_colour_type = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if((pngdata_l1->ihdr_colour_type!=0) && (pngdata_l1->ihdr_colour_type!=2) && (pngdata_l1->ihdr_colour_type!=3) && 
	    (pngdata_l1->ihdr_colour_type!=4) && (pngdata_l1->ihdr_colour_type!=6) )
	return PNG_INVALID_COLOUR_TYPE;

    pngdata_l1->ihdr_compression_method = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if(pngdata_l1->ihdr_compression_method!=COMPRESSION_METHOD_0)
	return PNG_INVALID_COMPRESSION_METHOD;

    pngdata_l1->ihdr_filter_method = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if(pngdata_l1->ihdr_filter_method!=FILTER_METHOD_0)
	return PNG_INVALID_FILTER_METHOD;

    pngdata_l1->ihdr_interlace_method = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    if((pngdata_l1->ihdr_interlace_method!=NULL_METHOD) && 
	    (pngdata_l1->ihdr_interlace_method!=ADAM7))
	return PNG_INVALID_INTERLACE_METHOD;

    return PNG_SUCCESS;
}

UINT8 png_decode_plte(pnginfo_t *png_info)
{
    UINT32 chunk_length,count;
    UINT8 *plte_entry;
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    pngdata_l1->plte_entry_count = pngdata_l1->chunk_length/3;

    chunk_length = pngdata_l1->chunk_length;

    plte_entry = pngdata_l1->plte_entry;

    while(chunk_length)
    {
	count = MIN(png_info->png_input_buf_size,chunk_length);

	png_memcpy(plte_entry,png_info->png_input_buf,count);

	png_info->png_input_buf += count;
	plte_entry += count;

	chunk_length -= count;
	png_info->png_input_buf_size -= count;

	if(png_info->png_input_buf_size==0)
	    png_get_next_buffer(png_info);

	if(png_info->png_terminate)
	    return PNG_TERMINATED;
    }

    return PNG_SUCCESS;
}

UINT8 png_decode_trns(pnginfo_t *png_info)
{
    UINT32 chunk_length,count;
    UINT8 *trns_alpha_palette;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    chunk_length = pngdata_l1->trns_alpha_palette_cnt = pngdata_l1->chunk_length;

    pngdata_l1->trns_present = 1;

    switch(pngdata_l1->ihdr_colour_type)
    {
	case 0:
	    pngdata_l1->trns_grey = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 2:
	    pngdata_l1->trns_red = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->trns_green = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->trns_blue = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 3:
	    trns_alpha_palette = pngdata_l1->trns_alpha_palette;

	    while(chunk_length)
	    {
		count = MIN(png_info->png_input_buf_size,chunk_length);

		png_memcpy(trns_alpha_palette,png_info->png_input_buf,count);

		png_info->png_input_buf += count;
		trns_alpha_palette += count;

		chunk_length -= count;
		png_info->png_input_buf_size -= count;

		if(png_info->png_input_buf_size==0)
		    png_get_next_buffer(png_info);

		if(png_info->png_terminate)
		    return PNG_TERMINATED;
	    }

	    if(pngdata_l1->trns_alpha_palette_cnt<255)
		pngdata_l1->trns_alpha_palette[pngdata_l1->trns_alpha_palette_cnt] = 255;
	    break;

	case 4:
	case 6:
	    return PNG_INVALID_TRNS_CHUNK;
    }

    return PNG_SUCCESS;
}

UINT8 png_decode_bkgd(pnginfo_t *png_info)
{
    UINT8 *temp;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    switch (pngdata_l1->ihdr_colour_type)
    {
	case 0:
	case 4:
	    pngdata_l1->bkgd_grey = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 2:
	case 6:
	    pngdata_l1->bkgd_red = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->bkgd_green = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->bkgd_blue = png_read16(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 3:
	    pngdata_l1->bkgd_palette_index = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    temp = pngdata_l1->plte_entry + (pngdata_l1->bkgd_palette_index)*3;
	    pngdata_l1->bkgd_red = *temp++;
	    pngdata_l1->bkgd_green = *temp++;
	    pngdata_l1->bkgd_blue = *temp++;
	    break;
    }

    if(pngdata_l1->ihdr_bit_depth==16)
    {
	pngdata_l1->bkgd_grey >>= 8;
	pngdata_l1->bkgd_red >>= 8;
	pngdata_l1->bkgd_green >>= 8;
	pngdata_l1->bkgd_blue >>= 8;
    }

    return PNG_SUCCESS;
}

UINT8 png_decode_phys(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    pngdata_l1->phys_pixel_per_unit_x = png_read32(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    pngdata_l1->phys_pixel_per_unit_y = png_read32(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    pngdata_l1->phys_unit = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    return PNG_SUCCESS;
}

UINT8 png_decode_sbit(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    switch(pngdata_l1->ihdr_colour_type)
    {
	case 0:
	    pngdata_l1->sbit_grey = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 2:
	case 3:
	    pngdata_l1->sbit_red = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_green = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_blue = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 4:
	    pngdata_l1->sbit_grey = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_alpha = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;

	case 6:
	    pngdata_l1->sbit_red = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_green = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_blue = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    pngdata_l1->sbit_alpha = png_read8(png_info);
	    if(png_info->png_terminate)
		return PNG_TERMINATED;

	    break;
    }

    return PNG_SUCCESS;
}

UINT8 png_decode_srgb(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    pngdata_l1->srgb_rendering_intent = png_read8(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    return PNG_SUCCESS;
}

UINT8 png_skip_chunk(pnginfo_t *png_info)
{
    UINT32 chunk_length;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    chunk_length = pngdata_l1->chunk_length;

    while(chunk_length > png_info->png_input_buf_size)
    {
	chunk_length -= png_info->png_input_buf_size;

	png_get_next_buffer(png_info);

	if(png_info->png_terminate)
	    return PNG_TERMINATED;
    }

    png_info->png_input_buf_size -= chunk_length;
    png_info->png_input_buf += chunk_length;

    return PNG_SUCCESS;
}

UINT8 png_decode_chunks(pnginfo_t *png_info)
{
    UINT8 err_val;
    UINT32 pixel_bits,temp,crc;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    switch(pngdata_l1->chunk_type)
    {
	case PNG_IHDR:
	    if(pngdata_l1->chunk_length<13)
	    {
		return PNG_INVALID_HEADER_CHUNK;
	    }

	    err_val = png_decode_ihdr(png_info);
	    if(err_val!=PNG_SUCCESS)
		return err_val;

	    pngdata_l1->ihdr_flag = 1;

	    png_info->cols = pngdata_l1->ihdr_cols;
	    png_info->rows = pngdata_l1->ihdr_rows;

	    pngdata_l1->inverse_rows = 104857600;//2^20*100
	    pngdata_l1->inverse_rows = ((pngdata_l1->inverse_rows)/(png_info->rows)) - 1;

	    png_info->png_output_buf_size = (3 * png_info->cols * png_info->rows);

	    pngdata_l1->samples_per_pixel = pngdata_l1->samples_per_pixel_tbl[pngdata_l1->ihdr_colour_type];

	    pixel_bits = pngdata_l1->ihdr_bit_depth * pngdata_l1->samples_per_pixel;

	    pngdata_l1->filter_interval = ((pixel_bits&7)==0) ? (pixel_bits>>3) : ((pixel_bits>>3) + 1);

	    set_pass_size(png_info);

	    temp = (png_info->cols + 7) & ~(7);
	    temp = PNG_ROWBYTES(pixel_bits,temp) + ((pixel_bits + 7)>>3) + 1;

	    temp += 64;

	    png_info->png_uncompressed_buf_size = 2*temp;

	    pngdata_l1->bit_depth_factor = 1;	
	    if(pngdata_l1->ihdr_bit_depth<8)
	    {
		pngdata_l1->bit_depth_factor = pngdata_l1->bit_depth_factor_tbl[pngdata_l1->ihdr_bit_depth];
	    }
	    break;

	case PNG_PLTE:
	    err_val = png_decode_plte(png_info);
	    break;

	case PNG_GAMA:
	    if(pngdata_l1->chunk_length>=4)
	    {
		pngdata_l1->gama_val = 45000;

		err_val = png_skip_chunk(png_info);
	    }
	    else
	    {
		return PNG_INVALID_GAMA_VALUE;
	    }
	    break;

	case PNG_SBIT:
	    err_val = png_decode_sbit(png_info);
	    break;

	case PNG_SRGB:
	    err_val = png_decode_srgb(png_info);
	    break;

	case PNG_BKGD:
	    if(png_info->usrdef_bkgd)
		err_val = png_skip_chunk(png_info);
	    else
		err_val = png_decode_bkgd(png_info);
	    break;

	case PNG_TRNS:
	    err_val = png_decode_trns(png_info);
	    break;

	case PNG_PHYS:
	    err_val = png_decode_phys(png_info);
	    break;

	case PNG_IDAT:
	    if(pngdata_l1->ihdr_flag==0)
		return PNG_ERROR_IDAT_DETECTED_BEFORE_IHDR;

	    pngdata_l1->idat_length = pngdata_l1->chunk_length;

	    return PNG_READ_HEADERS_SUCCESS;

	case PNG_IEND://redundant case
	    return PNG_SUCCESS;

	default:
	    err_val = png_skip_chunk(png_info);
	    break;
    }

    if(err_val!=PNG_SUCCESS)
	return err_val;

    crc = png_read32(png_info);
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    return PNG_SUCCESS;
}

