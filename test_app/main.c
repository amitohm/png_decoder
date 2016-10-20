#include <stdio.h>
#include <stdlib.h>


// SAMPLE TEST_APP
// illustrating usage of the jpeg decoder
#include "pngdec.h"

UINT8 *png_input_buf;
UINT32 png_input_buf_size = 10;

void png_get_next_buffer(pnginfo_t *png_info)
{
    FILE *fp;

    fp = (FILE *)png_info->user_data;

    png_info->png_input_buf_size = fread(png_input_buf,sizeof(UINT8),png_input_buf_size,fp);
    if(png_info->png_input_buf_size==0)
    {
	png_info->png_terminate = PNG_TERMINATE_DECODE;
    }

    png_info->png_input_buf = png_input_buf;

    return;
}

// sample wrapper function for testing png_decoder
INT32 main(INT32 argc,INT8 *argv[])
{
    FILE *fp;
    INT32 i,j;
    INT32 val;

    // structs
    static pnginfo_t png_info;

    UINT8 *rgb_output,*png_uncompressed_buf;
    UINT32 bytes_written;

    INT8 in_filename[50],out_filename[50];

    UINT32 *temp;

    ///////////////////////////////////////////////////////////////////////////////////////////////

    //This part of the code is used to extract name of the png image from the complete file path//printf("Input filename: %s\n", filename);
    if (argc!=2) {
	printf("Incorrect arguments!!!\n");
	exit(-1);
    }

    if ((fp = fopen(argv[1], "rb"))==NULL)
    {
	printf("Error accessing %s\n", argv[1]);
	return -1;
    }
    png_info.user_data = (void *)fp;

    png_input_buf = (UINT8 *)malloc(sizeof(UINT8)*png_input_buf_size);
    if(png_input_buf==NULL)
    {
	printf("Error allocating memory for png_input_buf.\n");
	exit(0);
    }

    png_info.png_input_buf = NULL;
    png_info.png_input_buf_size = 0;

    i = j = 0;
    while(argv[1][i]!='\0')
    {
	if(argv[1][i]=='/')
	    j = i + 1;
	i++;
    }

    i = j;
    j = 0;
    while(argv[1][i]!='.')
    {
	in_filename[j] = argv[1][i];
	i++;
	j++;
    }
    in_filename[j] = '\0';

    ////////////////////////////////////////////////////////////////////////////
    //																		  //
    //		PNG Decode starts here											  //
    //																		  //
    ////////////////////////////////////////////////////////////////////////////

    //used to get buffer sizes
    png_init_decode(&png_info);

    //buffer mallocs
    temp = (UINT32 *)malloc(png_info.l1_buf_size);
    if(temp==NULL)
    {
	printf("Error allocating memory for l1_buf.\n");
	return -1;
    }
    png_info.l1_buf = (void *)temp;
    printf("png_info.l1_buf = %p\n",temp);

    png_info.usrdef_bkgd = 0;
    if(png_info.usrdef_bkgd)
    {
	png_info.usrdef_red = 255;
	png_info.usrdef_green = 255;
	png_info.usrdef_blue = 255;
    }

    //////////////////////////////////////////////////////////////////////////
    //		png_read_headers() fills all the necessary fields in png_info	//
    //		like cols, rows etc to enable user to allocate					//
    //		enough memmory for the output buffer							//
    //////////////////////////////////////////////////////////////////////////
    val = png_read_headers(&png_info);

    printf("val after pre decode: %d\n",val);

    if(val==PNG_READ_HEADERS_SUCCESS)
    {
	rgb_output = (UINT8 *)malloc(sizeof(UINT8)*png_info.png_output_buf_size);
	if (rgb_output == NULL)
	{
	    printf("Cannot alloc memory for rgb_output.\n");
	    return -1;
	}
	printf("rgb_output = %p\n",rgb_output);			
	png_info.png_output_buf = rgb_output;

	png_uncompressed_buf = (UINT8 *)malloc(sizeof(UINT8)*png_info.png_uncompressed_buf_size);
	if (png_uncompressed_buf == NULL)
	{
	    printf("Cannot alloc memory for png_uncompressed_buf.\n");
	    return -1;
	}
	printf("png_uncompressed_buf = %p\n",png_uncompressed_buf);
	png_info.png_uncompressed_buf = png_uncompressed_buf;

	/////////////////////////////////////////////////////////////////////////
	//	png_decode() decodes the compressed png data and gives rgb output  //
	/////////////////////////////////////////////////////////////////////////
	val = png_decode(&png_info);
	//if image is decoded successfully, val = 0

	printf("val after png_decode: %d\n",val);

	///////////////////////////////////////////////////////////
	//		png decode ends here.
	///////////////////////////////////////////////////////////

	sprintf(out_filename,"%s.ppm",in_filename);
	if ((fp = fopen(out_filename,"wb"))==NULL)
	{
	    printf("Error creating %s\n",out_filename);
	    return -1;
	}
	fprintf (fp,"%s\n%d %d\n","P6",png_info.cols,png_info.rows);
	fprintf (fp,"%d\n",255);

	bytes_written = fwrite(rgb_output,sizeof(UINT8),png_info.png_output_buf_size,fp);

	printf("bytes_written = %d\n",bytes_written);

	fclose(fp);

	free(png_uncompressed_buf);

	free(rgb_output);			
    }

    printf("\n====================================================================\n\n");

    free(png_info.l1_buf);

    free(png_input_buf);

    fclose(fp);

    return 0;
}
