/***************************************************************************
 * File: arithlut.c                                                        *
 *                                                                         *
 * Desc: This program performs arithmetic point operations via LUTs        *
 ***************************************************************************/


#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "ip.h"

#define operation(VALUE)        ((float) VALUE * 1.9)

extern void write_pnm(image_ptr ptr, char filein[], int rows,
		      int cols, int magic_number);
extern image_ptr read_pnm(char *filename, int *rows, int *cols,
		      int *type);

int main(int argc, char *argv[])
    {
    char filein[100];                   /* name of input file */
    char fileout[100];                  /* name of output file */
    int rows, cols;                     /* image rows and columns */
    unsigned long i;                    /* counting index */
    unsigned long bytes_per_pixel;      /* number of bytes per image pixel */
    unsigned char LUT[256];             /* array for Look-up table */
    image_ptr buffer;                   /* pointer to image buffer */
    unsigned long number_of_pixels;     /* total number of pixels in image */
    int temp;                           /* temporary variable */
    int type;                           /* what type of image data */
	int x_scale, y_scale;

    /* set input filename and output file name */
    if(argc == 3)
	{
	strcpy(filein, argv[1]);
	strcpy(fileout, argv[2]);
	}
    else
	{
	printf("Input 파일의 이름을 입력해주세요: \n");
	gets(filein);
	printf("\nOutput 파일의 이름을 입력해주세요 : \n");
	gets(fileout);
	printf("\n");
	}

    buffer = read_pnm(filein, &rows, &cols, &type);

    /* determine bytes_per_pixel, 3 for color, 1 for gray-scale */
    if(type == PPM)
	bytes_per_pixel = 3;
    else
	bytes_per_pixel = 1;

    number_of_pixels = bytes_per_pixel * rows * cols;

	//이미지 원하는 만큼 늘릴 계수 (상수)
	x_scale = 2;
	y_scale = 2;

	//NNinterpolation(buffer, fileout, rows, cols, x_scale, y_scale, type);

	//biInterpolation(buffer, fileout, rows, cols, x_scale, y_scale, type);

	cubicConvInterpolation(buffer, fileout, rows, cols, x_scale, y_scale, type);

    IP_FREE(buffer);
    return 0;
    }
