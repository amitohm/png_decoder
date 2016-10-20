#include "png_api.h"

//*******************************************************************************************//

void set_pass_size(pnginfo_t *png_info)
{
    UINT32 i;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    for(i=0;i<8;i++)
    {
	pngdata_l1->pass_size[i] = 0;
	if(pngdata_l1->interlace_matrix_tbl[i][1]<=pngdata_l1->ihdr_cols)
	    pngdata_l1->pass_size[i] = ((pngdata_l1->ihdr_cols - pngdata_l1->interlace_matrix_tbl[i][1])>>pngdata_l1->shift_pass_tbl[i]) + 1;
    }
}

//*******************************************************************************************//

void pixel_ctype_0_bdepth_L8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,max_val;

    max_val = (1<<pngdata_l1->ihdr_bit_depth) - 1;

    pix = (*pngdata_l1->cur_row)>>pngdata_l1->bit_shift;
    pix &= max_val;

    if(pngdata_l1->bit_shift == 0)
    {
	pngdata_l1->bit_shift = 8;
	pngdata_l1->cur_row += 1;
    }

    pngdata_l1->bit_shift -= pngdata_l1->ihdr_bit_depth;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_grey))
    {
	pix = (UINT8)pngdata_l1->bkgd_grey;
    }
    else
    {
	pix *= pngdata_l1->bit_depth_factor;
    }
    *output++ = pix;
    *output++ = pix;
    *output++ = pix;

    return;
}

void pixel_ctype_0_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix;

    pix = *pngdata_l1->cur_row++;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_grey))
    {
	pix = (UINT8)pngdata_l1->bkgd_grey;
    }
    *output++ = pix;
    *output++ = pix;
    *output++ = pix;

    return;
}

void pixel_ctype_0_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT16 lsb,msb,pix;

    msb = *pngdata_l1->cur_row++;
    lsb = *pngdata_l1->cur_row++;

    pix = (msb<<8) + lsb;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_grey))
    {
	pix = pngdata_l1->bkgd_grey;
    }
    else
    {
	pix >>= 8;
    }
    *output++ = (UINT8)pix;
    *output++ = (UINT8)pix;
    *output++ = (UINT8)pix;

    return;
}

void pixel_ctype_2_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix;

    pix = *pngdata_l1->cur_row++;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_red))
    {
	pix = (UINT8)pngdata_l1->bkgd_red;
    }
    *output++ = pix;

    pix = *pngdata_l1->cur_row++;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_green))
    {
	pix = (UINT8)pngdata_l1->bkgd_green;
    }
    *output++ = pix;

    pix = *pngdata_l1->cur_row++;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_blue))
    {
	pix = (UINT8)pngdata_l1->bkgd_blue;
    }
    *output++ = pix;

    return;
}

void pixel_ctype_2_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT16 lsb,msb,pix;

    msb = *pngdata_l1->cur_row++;
    lsb = *pngdata_l1->cur_row++;

    pix = (msb<<8) + lsb;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_red))
    {
	pix = pngdata_l1->bkgd_red;
    }
    else
    {
	pix >>= 8;
    }
    *output++ = (UINT8)pix;

    msb = *pngdata_l1->cur_row++;
    lsb = *pngdata_l1->cur_row++;

    pix = (msb<<8) + lsb;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_green))
    {
	pix = pngdata_l1->bkgd_green;
    }
    else
    {
	pix >>= 8;
    }
    *output++ = (UINT8)pix;

    msb = *pngdata_l1->cur_row++;
    lsb = *pngdata_l1->cur_row++;

    pix = (msb<<8) + lsb;

    if((pngdata_l1->trns_present)&&(pix==pngdata_l1->trns_blue))
    {
	pix = pngdata_l1->bkgd_blue;
    }
    else
    {
	pix >>= 8;
    }
    *output++ = (UINT8)pix;

    return;
}

void pixel_ctype_3_bdepth_L8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,max_val;
    UINT16 offset_palette,alpha;
    UINT8 *temp;

    max_val = (1<<pngdata_l1->ihdr_bit_depth) - 1;

    pix = (*pngdata_l1->cur_row)>>pngdata_l1->bit_shift;
    pix &= max_val;

    if(pngdata_l1->bit_shift == 0)
    {
	pngdata_l1->bit_shift = 8;
	pngdata_l1->cur_row += 1;
    }

    pngdata_l1->bit_shift -= pngdata_l1->ihdr_bit_depth;

    alpha = 255;

    offset_palette = pix;

    temp = pngdata_l1->plte_entry + (offset_palette*3);

    if(pngdata_l1->trns_present)
    {
	if(offset_palette>pngdata_l1->trns_alpha_palette_cnt)
	    offset_palette = pngdata_l1->trns_alpha_palette_cnt;

	alpha = pngdata_l1->trns_alpha_palette[offset_palette];
    }

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_red);
    *output++ = pix;

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_green);
    *output++ = pix;

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_blue);
    *output++ = pix;

    return;
}

void pixel_ctype_3_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix;
    UINT16 offset_palette,alpha;
    UINT8 *temp;

    pix = *pngdata_l1->cur_row++;

    alpha = 255;

    offset_palette = pix;

    temp = pngdata_l1->plte_entry + (offset_palette*3);

    if(pngdata_l1->trns_present)
    {
	if(offset_palette>pngdata_l1->trns_alpha_palette_cnt)
	    offset_palette = pngdata_l1->trns_alpha_palette_cnt;

	alpha = pngdata_l1->trns_alpha_palette[offset_palette];
    }

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_red);
    *output++ = pix;

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_green);
    *output++ = pix;

    pix = *temp++;
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_blue);
    *output++ = pix;

    return;
}

void pixel_ctype_4_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,alpha;

    pix = *pngdata_l1->cur_row++;

    alpha = *pngdata_l1->cur_row++;

    png_composite(pix,pix,alpha,pngdata_l1->bkgd_grey);
    *output++ = (UINT8)pix;
    *output++ = (UINT8)pix;
    *output++ = (UINT8)pix;

    return;
}

void pixel_ctype_4_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,alpha;

    pix = *(pngdata_l1->cur_row);

    alpha = *(pngdata_l1->cur_row + 2);

    png_composite(pix,pix,alpha,pngdata_l1->bkgd_grey);	
    *output++ = pix;
    *output++ = pix;
    *output++ = pix;

    pngdata_l1->cur_row += 4;

    return;
}

void pixel_ctype_6_bdepth_8(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,alpha;

    alpha = *(pngdata_l1->cur_row + 3);

    pix = *(pngdata_l1->cur_row);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_red);
    *output++ = pix;

    pix = *(pngdata_l1->cur_row + 1);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_green);
    *output++ = pix;

    pix = *(pngdata_l1->cur_row + 2);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_blue);
    *output++ = pix;

    pngdata_l1->cur_row += 4;

    return;
}

void pixel_ctype_6_bdepth_16(pngdata_l1_t *pngdata_l1,UINT8 *output)
{
    UINT8 pix,alpha;

    alpha = *(pngdata_l1->cur_row + 6);

    pix = *(pngdata_l1->cur_row);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_red);
    *output++ = pix;

    pix = *(pngdata_l1->cur_row + 2);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_green);
    *output++ = pix;

    pix = *(pngdata_l1->cur_row + 4);
    png_composite(pix,pix,alpha,pngdata_l1->bkgd_blue);
    *output++ = pix;

    pngdata_l1->cur_row += 8;

    return;
}

//*******************************************************************************************//

UINT8 paeth(UINT8 left,UINT8 above,UINT8 upper_left)
{
    UINT32 p = left + above - upper_left;

    UINT32 pa = ABSOLUTE((INT32)(p - left));
    UINT32 pb = ABSOLUTE((INT32)(p - above));
    UINT32 pc = ABSOLUTE((INT32)(p - upper_left));

    if ((pa <= pb) & (pa <= pc))
	return(left);
    else if (pb <= pc)
	return(above);
    else
	return(upper_left);
}

void filter_data(pnginfo_t *png_info,UINT8 pass)
{
    UINT8 filter_interval;	
    UINT8 *prev_row,*cur_row,*left,*upper_left;
    UINT32 j,len;

    pngdata_l1_t *pngdata_l1;

    pngdata_l1 = (pngdata_l1_t *)(png_info->l1_buf);

    filter_interval = pngdata_l1->filter_interval;

    len = pngdata_l1->pass_size[pass]*filter_interval;

    switch(pngdata_l1->filter_type)
    {
	case 0:
	    break;

	case 1:
	    left = cur_row = pngdata_l1->cur_row;
	    cur_row += filter_interval;
	    for (j=filter_interval; j<len; j++)
	    {
		*cur_row = *cur_row + *left;
		cur_row++;left++;
	    }
	    break;

	case 2:
	    prev_row = pngdata_l1->prev_row;
	    cur_row = pngdata_l1->cur_row;
	    for(j=0;j<len;j++)
	    {
		*cur_row = *cur_row + *prev_row;
		cur_row++;prev_row += pngdata_l1->prev_row_val;
	    }
	    break;

	case 3:
	    prev_row = pngdata_l1->prev_row;
	    left = cur_row = pngdata_l1->cur_row;
	    for(j=0;j<filter_interval;j++)
	    {
		*cur_row = *cur_row + ((*prev_row)>>1);
		cur_row++;prev_row += pngdata_l1->prev_row_val;
	    }

	    for (j=filter_interval;j<len;j++)
	    {
		*cur_row = *cur_row + ((*left + *prev_row)>>1);
		cur_row++;left++;prev_row += pngdata_l1->prev_row_val;
	    }
	    break;

	case 4:
	    upper_left = prev_row = pngdata_l1->prev_row;
	    left = cur_row = pngdata_l1->cur_row;
	    for(j=0;j<filter_interval;j++)
	    {
		*cur_row = *cur_row + *prev_row;
		cur_row++;prev_row += pngdata_l1->prev_row_val;
	    }

	    for (j=filter_interval;j<len;j++)
	    {
		*cur_row = *cur_row + paeth(*left,*prev_row,*upper_left);
		prev_row += pngdata_l1->prev_row_val;cur_row++;
		upper_left += pngdata_l1->prev_row_val;left++;
	    }
	    break;
    }

    pngdata_l1->prev_row = pngdata_l1->cur_row;
    pngdata_l1->prev_row_val = 1;
}

//*******************************************************************************************//

void png_set_bkgd(pngdata_l1_t *pngdata_l1)
{
    pngdata_l1->bkgd_grey = 0;

    pngdata_l1->bkgd_red = 0;
    pngdata_l1->bkgd_green = 0;
    pngdata_l1->bkgd_blue = 0;

    return;
}

//*******************************************************************************************//

