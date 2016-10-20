
#include "png_api.h"


void png_init_decode(pnginfo_t *png_info)
{
    png_info->l1_buf_size = sizeof(pngdata_l1_t);
}

void init_data_buffers(pnginfo_t *png_info)
{
    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    //INIT
    png_memset(png_info->l1_buf,0,png_info->l1_buf_size);


    //UINT8 bit_depth_factor_tbl[5] = {0,255,85,0,17};
    pngdata_l1->bit_depth_factor_tbl[0] = 0;
    pngdata_l1->bit_depth_factor_tbl[1] = 255;
    pngdata_l1->bit_depth_factor_tbl[2] = 85;
    pngdata_l1->bit_depth_factor_tbl[3] = 0;
    pngdata_l1->bit_depth_factor_tbl[4] = 17;

    //UINT8 samples_per_pixel_tbl[7] = {1,0,3,1,2,0,4}; //multiply with bit depth
    pngdata_l1->samples_per_pixel_tbl[0] = 1;
    pngdata_l1->samples_per_pixel_tbl[1] = 0;
    pngdata_l1->samples_per_pixel_tbl[2] = 3;
    pngdata_l1->samples_per_pixel_tbl[3] = 1;
    pngdata_l1->samples_per_pixel_tbl[4] = 2;
    pngdata_l1->samples_per_pixel_tbl[5] = 0;
    pngdata_l1->samples_per_pixel_tbl[6] = 4;

    //UINT8 pass_factors_tbl[7][2] = {{1,1},{1,1},{2,1},{2,2},{4,2},{4,4},{8,4}};
    pngdata_l1->pass_factors_tbl[0][0] = 1; pngdata_l1->pass_factors_tbl[0][1] = 1;
    pngdata_l1->pass_factors_tbl[1][0] = 1; pngdata_l1->pass_factors_tbl[1][1] = 1;
    pngdata_l1->pass_factors_tbl[2][0] = 2; pngdata_l1->pass_factors_tbl[2][1] = 1;
    pngdata_l1->pass_factors_tbl[3][0] = 2; pngdata_l1->pass_factors_tbl[3][1] = 2;
    pngdata_l1->pass_factors_tbl[4][0] = 4; pngdata_l1->pass_factors_tbl[4][1] = 2;
    pngdata_l1->pass_factors_tbl[5][0] = 4; pngdata_l1->pass_factors_tbl[5][1] = 4;
    pngdata_l1->pass_factors_tbl[6][0] = 8; pngdata_l1->pass_factors_tbl[6][1] = 4;

    //UINT8 shift_pass_tbl[8] = {3,3,2,2,1,1,0,0};
    pngdata_l1->shift_pass_tbl[0] = 3;
    pngdata_l1->shift_pass_tbl[1] = 3;
    pngdata_l1->shift_pass_tbl[2] = 2;
    pngdata_l1->shift_pass_tbl[3] = 2;
    pngdata_l1->shift_pass_tbl[4] = 1;
    pngdata_l1->shift_pass_tbl[5] = 1;
    pngdata_l1->shift_pass_tbl[6] = 0;
    pngdata_l1->shift_pass_tbl[7] = 0;

    //UINT8 interlace_matrix_tbl[8][4] = {{1,1,8,8},{1,5,8,8},{5,1,8,4},{1,3,4,4},{3,1,4,2},{1,2,2,2},{2,1,2,1},{1,1,1,1}};
    pngdata_l1->interlace_matrix_tbl[0][0] = 1; pngdata_l1->interlace_matrix_tbl[0][1] = 1;
    pngdata_l1->interlace_matrix_tbl[0][2] = 8; pngdata_l1->interlace_matrix_tbl[0][3] = 8;

    pngdata_l1->interlace_matrix_tbl[1][0] = 1; pngdata_l1->interlace_matrix_tbl[1][1] = 5;
    pngdata_l1->interlace_matrix_tbl[1][2] = 8; pngdata_l1->interlace_matrix_tbl[1][3] = 8;

    pngdata_l1->interlace_matrix_tbl[2][0] = 5; pngdata_l1->interlace_matrix_tbl[2][1] = 1;
    pngdata_l1->interlace_matrix_tbl[2][2] = 8; pngdata_l1->interlace_matrix_tbl[2][3] = 4;

    pngdata_l1->interlace_matrix_tbl[3][0] = 1; pngdata_l1->interlace_matrix_tbl[3][1] = 3;
    pngdata_l1->interlace_matrix_tbl[3][2] = 4; pngdata_l1->interlace_matrix_tbl[3][3] = 4;

    pngdata_l1->interlace_matrix_tbl[4][0] = 3; pngdata_l1->interlace_matrix_tbl[4][1] = 1;
    pngdata_l1->interlace_matrix_tbl[4][2] = 4; pngdata_l1->interlace_matrix_tbl[4][3] = 2;

    pngdata_l1->interlace_matrix_tbl[5][0] = 1; pngdata_l1->interlace_matrix_tbl[5][1] = 2;
    pngdata_l1->interlace_matrix_tbl[5][2] = 2; pngdata_l1->interlace_matrix_tbl[5][3] = 2;

    pngdata_l1->interlace_matrix_tbl[6][0] = 2; pngdata_l1->interlace_matrix_tbl[6][1] = 1;
    pngdata_l1->interlace_matrix_tbl[6][2] = 2; pngdata_l1->interlace_matrix_tbl[6][3] = 1;

    pngdata_l1->interlace_matrix_tbl[7][0] = 1; pngdata_l1->interlace_matrix_tbl[7][1] = 1;
    pngdata_l1->interlace_matrix_tbl[7][2] = 1; pngdata_l1->interlace_matrix_tbl[7][3] = 1;
}

UINT8 png_read_headers(pnginfo_t *png_info)
{
    UINT8 err_val;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    init_data_buffers(png_info);

    err_val = png_init_inflate(pngdata_l1);
    if(err_val!=PNG_SUCCESS)
	return err_val;

    err_val = png_check_signature(png_info);
    if(err_val!=PNG_SUCCESS)
    {
	return err_val;
    }

    png_set_bkgd(pngdata_l1);

    pngdata_l1->trns_present = 0;
    pngdata_l1->trns_alpha_palette[0] = 255;
    pngdata_l1->trns_alpha_palette_cnt = 0;

    if(png_info->usrdef_bkgd)
    {
	pngdata_l1->bkgd_red = png_info->usrdef_red;
	pngdata_l1->bkgd_green = png_info->usrdef_green;
	pngdata_l1->bkgd_blue = png_info->usrdef_blue;
    }

    while(1)
    {
	err_val = read_next_chunk(png_info);
	if(err_val!=PNG_SUCCESS)
	    return err_val;

	err_val = png_decode_chunks(png_info);

	if(err_val==PNG_READ_HEADERS_SUCCESS)
	    break;

	if(err_val!=PNG_SUCCESS)
	    return err_val;
    }

    return err_val;
}

UINT8 png_decode(pnginfo_t *png_info)
{							
    UINT8 err_val;
    UINT8 pass,pass_a,pass_b;
    INT32 index;
    UINT32 cols,rows,pixel_bits,row_bytes,row_count,crc;
    UINT8 *rgb_output;

    void (*pixel_function)(pngdata_l1_t *pngdata_l1,UINT8 *output);

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    png_memset(png_info->png_uncompressed_buf,0,png_info->png_uncompressed_buf_size);

    pngdata_l1->uncompressed_buf_1 = png_info->png_uncompressed_buf;
    pngdata_l1->uncompressed_buf_2 = png_info->png_uncompressed_buf + (png_info->png_uncompressed_buf_size>>1);

    pngdata_l1->bit_shift = 8 - pngdata_l1->ihdr_bit_depth;

    pngdata_l1->uncompressed_buf_select = 0;

    pixel_bits = pngdata_l1->ihdr_bit_depth * pngdata_l1->samples_per_pixel;

    if (pngdata_l1->ihdr_interlace_method==1)
    {
	pass_a = 0;
	pass_b = 7;
    }
    else
    {
	pass_a = 7;
	pass_b = 8;
    }

    switch(pngdata_l1->ihdr_colour_type)
    {
	case 0:
	    switch(pngdata_l1->ihdr_bit_depth)
	    {
		case 1:
		case 2:
		case 4:
		    pixel_function = &pixel_ctype_0_bdepth_L8;
		    break;
		case 8:
		    pixel_function = &pixel_ctype_0_bdepth_8;
		    break;
		case 16:
		    pixel_function = &pixel_ctype_0_bdepth_16;
		    break;
	    }
	    break;

	case 2:
	    switch(pngdata_l1->ihdr_bit_depth)
	    {
		case 8:
		    pixel_function = &pixel_ctype_2_bdepth_8;
		    break;
		case 16:
		    pixel_function = &pixel_ctype_2_bdepth_16;
		    break;
	    }
	    break;

	case 3:
	    switch(pngdata_l1->ihdr_bit_depth)
	    {
		case 1:
		case 2:
		case 4:
		    pixel_function = &pixel_ctype_3_bdepth_L8;
		    break;
		case 8:
		    pixel_function = &pixel_ctype_3_bdepth_8;
		    break;
	    }
	    break;

	case 4:		
	    switch(pngdata_l1->ihdr_bit_depth)
	    {
		case 8:
		    pixel_function = &pixel_ctype_4_bdepth_8;
		    break;
		case 16:
		    pixel_function = &pixel_ctype_4_bdepth_16;
		    break;
	    }
	    break;

	case 6:
	    switch(pngdata_l1->ihdr_bit_depth)
	    {
		case 8:
		    pixel_function = &pixel_ctype_6_bdepth_8;
		    break;
		case 16:
		    pixel_function = &pixel_ctype_6_bdepth_16;
		    break;
	    }
	    break;
    }

    rgb_output = png_info->png_output_buf;

    row_count = 0;

    for(pass=pass_a;pass<pass_b;pass++)
    {
	row_bytes = PNG_ROWBYTES(pixel_bits,pngdata_l1->pass_size[pass]) + 1;

	rows = pngdata_l1->interlace_matrix_tbl[pass][0] - 1;
	cols = pngdata_l1->interlace_matrix_tbl[pass][1] - 1;

	if((cols < pngdata_l1->ihdr_cols)&&(rows < pngdata_l1->ihdr_rows))
	{
	    pngdata_l1->prev_row_val = 0;
	    pngdata_l1->prev_row = &(pngdata_l1->prev_row_val);

	    while (rows < pngdata_l1->ihdr_rows)
	    {
		if(pngdata_l1->uncompressed_buf_select)
		{
		    pngdata_l1->cur_row = pngdata_l1->uncompressed_buf_2;

		    pngdata_l1->uncompressed_buf_select = 0;
		}
		else
		{
		    pngdata_l1->cur_row = pngdata_l1->uncompressed_buf_1;

		    pngdata_l1->uncompressed_buf_select = 1;
		}

		pngdata_l1->zstream.next_out = pngdata_l1->cur_row;
		pngdata_l1->zstream.avail_out = row_bytes;

		do
		{
		    if (!(pngdata_l1->zstream.avail_in))
		    {
			while (!pngdata_l1->idat_length)
			{
			    crc = png_read32(png_info);//read crc data
			    if(png_info->png_terminate)
				return PNG_TERMINATED;

			    err_val = read_next_chunk(png_info);
			    if(err_val!=PNG_SUCCESS)
				return err_val;

			    if(pngdata_l1->chunk_type!=PNG_IDAT)
				return PNG_ERROR_NOT_ENOUGH_DATA;

			    pngdata_l1->idat_length = pngdata_l1->chunk_length;
			}

			if(png_info->png_input_buf_size==0)
			    png_get_next_buffer(png_info);

			if(png_info->png_terminate)
			    return PNG_TERMINATED;

			pngdata_l1->zstream.avail_in = MIN(png_info->png_input_buf_size,pngdata_l1->idat_length);
			pngdata_l1->zstream.next_in = png_info->png_input_buf;

			png_info->png_input_buf += pngdata_l1->zstream.avail_in;

			png_info->png_input_buf_size -= pngdata_l1->zstream.avail_in;

			pngdata_l1->idat_length -= pngdata_l1->zstream.avail_in;
		    }

		    err_val = inflate(&pngdata_l1->zstream, Z_PARTIAL_FLUSH);
		    if (err_val == Z_STREAM_END)
		    {
			if (pngdata_l1->zstream.avail_out || pngdata_l1->zstream.avail_in ||
				pngdata_l1->idat_length)
			    return PNG_ZLIB_EXTRA_COMPRESSED_DATA;

			break;
		    }
		    if (err_val != Z_OK)
			return PNG_ZLIB_ERROR;

		} while (pngdata_l1->zstream.avail_out);

		pngdata_l1->filter_type = *pngdata_l1->cur_row++;

		pngdata_l1->bit_shift = 8 - pngdata_l1->ihdr_bit_depth;

		filter_data(png_info,pass);

		cols = pngdata_l1->interlace_matrix_tbl[pass][1] - 1;

		while(cols < pngdata_l1->ihdr_cols)
		{
		    index = ((rows*pngdata_l1->ihdr_cols) + cols)*3;

		    pixel_function(pngdata_l1,(rgb_output + index));

		    cols += pngdata_l1->interlace_matrix_tbl[pass][3];
		}

		rows += pngdata_l1->interlace_matrix_tbl[pass][2];

		if(pass>4)
		{
		    row_count++;
		    png_info->png_progress_indicator = ((row_count*pngdata_l1->inverse_rows)>>20) + 1;
		}
	    }
	}
    }

    pngdata_l1->chunk_length = pngdata_l1->idat_length;

    err_val = png_skip_chunk(png_info);//skip remaining data in IDAT
    if(err_val!=PNG_SUCCESS)
	return err_val;

    crc = png_read32(png_info);//read crc data
    if(png_info->png_terminate)
	return PNG_TERMINATED;

    while(1)
    {
	err_val = read_next_chunk(png_info);
	if(err_val!=PNG_SUCCESS)
	    return err_val;

	if(pngdata_l1->chunk_type!=PNG_IEND)
	    err_val = png_skip_chunk(png_info);
	else
	    break;

	if(err_val!=PNG_SUCCESS)
	    return err_val;

	crc = png_read32(png_info);//read crc data
	if(png_info->png_terminate)
	    return PNG_TERMINATED;
    }

    return PNG_SUCCESS;
}

