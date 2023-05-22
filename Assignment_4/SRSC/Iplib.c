/*************************************************************************** 
 * File: iplib.c                                                           *
 *                                                                         *
 * Desc: general purpose image processing routines                         * 
 ***************************************************************************/


#include <malloc.h>
#include <stdio.h>
#include <stdlib.h>
#include "ip.h"
#include "math.h"

image_ptr read_pnm(char *filename, int *rows, int *cols, int *type);
int getnum(FILE *fp);

/***************************************************************************
 * Func: read_pnm                                                          *
 *                                                                         *
 * Desc: reads a portable bitmap file                                      *
 *                                                                         *
 * Params: filename - name of image file to read                           *
 *         rows - number of rows in the image                              *
 *         cols - number of columns in the image                           *
 *         type - file type                                                *
 *                                                                         *
 * Returns: pointer to the image just read into memory                     *
 ***************************************************************************/

image_ptr read_pnm(char *filename, int *rows, int *cols, int *type)
    {
    int i;                     /* index variable */
    int row_size;              /* size of image row in bytes */
    int maxval;                /* maximum value of pixel */
    FILE *fp;                  /* input file pointer */
    int firstchar, secchar;    /* first 2 characters in the input file */
    image_ptr ptr;             /* pointer to image buffer */
    unsigned long offset;      /* offset into image buffer */
    unsigned long total_size;  /* size of image in bytes */
    unsigned long total_bytes; /* number of total bytes written to file */
    float scale;               /* number of bytes per pixel */

    /* open input file */
    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open %s for reading\n",filename);
	exit(1);
	}

    firstchar = getc(fp);
    secchar = getc(fp);

    if(firstchar != 'P')
	{
	printf("You silly goof... This is not a PPM file!\n");
	exit(1);
	}

    *cols = getnum(fp);
    *rows = getnum(fp);
    *type = secchar - '0';

    switch(secchar)
	{
	case '4':            /* PBM */
	    scale = 0.125;
	    maxval = 1;
	    break;
	case '5':            /* PGM */
	    scale = 1.0;
	    maxval = getnum(fp);
	    break;
	case '6':             /* PPM */
	    scale = 3.0;
	    maxval = getnum(fp);
	    break;
	default :             /* Error */
	    printf("read_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

    row_size = (*cols) * scale;
    total_size = (unsigned long) (*rows) * row_size;

    ptr = (image_ptr) IP_MALLOC(total_size);

    if(ptr == NULL)
	{
	printf("Unable to malloc %lu bytes\n",total_size);
	exit(1);
	}

    total_bytes=0;
    offset = 0;
    for(i=0; i<(*rows); i++)
	{
	total_bytes+=fread(ptr+offset, 1, row_size, fp);
	offset += row_size;
	}

    if(total_size != total_bytes)
	{
	printf("Failed miserably trying to read %ld bytes\nRead %ld bytes\n",
		total_size, total_bytes);
	exit(1);
	}

    fclose(fp);
    return ptr;
    }

/***************************************************************************
 * Func: getnum                                                            *
 *                                                                         *
 * Desc: reads an ASCII number from a portable bitmap file header          *
 *                                                                         *
 * Param: fp - pointer to file being read                                  *
 *                                                                         *
 * Returns: the number read                                                *
 ***************************************************************************/

int getnum(FILE *fp)
    {
    char c;               /* character read in from file */
    int i;                /* number accumulated and returned */

    do
	{
	c = getc(fp);
	}
    while((c==' ') || (c=='\t') || (c=='\n') || (c=='\r'));

    if((c<'0') || (c>'9'))
	if(c == '#')                   /* chew off comments */
	    {
	    while(c == '#')
		{
		while(c != '\n')
		    c = getc(fp);
		c = getc(fp);
		}
	    }
	else
	    {
	    printf("Garbage in ASCII fields\n");
	    exit(1);
	    }

    i=0;
    do
	{
	i=i*10+(c-'0');         /* convert ASCII to int */
	c = getc(fp);
	}
    while((c>='0') && (c<='9'));

    return i;
    }

/***************************************************************************
 * Func: write_pnm                                                         *
 *                                                                         *
 * Desc: writes out a portable bitmap file                                 *
 *                                                                         *
 * Params: ptr - pointer to image in memory                                *
 *         filename _ name of file to write image to                       *
 *         rows - number of rows in the image                              *
 *         cols - number of columns in the image                           *
 *         magic_number - number that defines what type of file it is      *
 *                                                                         *
 * Returns: nothing                                                        *
 ***************************************************************************/

void write_pnm(image_ptr ptr, char *filename, int rows, 
	       int cols, int magic_number)
    {
    FILE *fp;             /* file pointer for output file */
    long offset;          /* current offset into image buffer */
    long total_bytes;     /* number of bytes written to output file */
    long total_size;      /* size of image buffer */
    int row_size;         /* size of row in bytes */
    int i;                /* index variable */
    float scale;          /* number of bytes per image pixel */

    switch(magic_number)
	{
	case 4:            /* PBM */
	    scale = 0.125;
	    break;
	case 5:            /* PGM */
	    scale = 1.0;
	    break;
	case 6:             /* PPM */
	    scale = 3.0;
	    break;
	default :             /* Error */
	    printf("write_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

    /* open new output file */
    if((fp=fopen(filename, "wb")) == NULL)
	{
	printf("Unable to open %s for output\n",filename);
	exit(1);
	}

    /* print out the portable bitmap header */
    fprintf(fp, "P%d\n%d %d\n", magic_number, cols, rows);
    if(magic_number != 4)
	fprintf(fp, "255\n");

    row_size = cols * scale;
    total_size = (long) row_size *rows;
    offset = 0;
    total_bytes = 0;
    for(i=0; i<rows; i++)
	{
	total_bytes += fwrite(ptr+offset, 1, row_size, fp);
	offset += row_size;
	}

    if(total_bytes != total_size)
	printf("Tried to write %ld bytes...Only wrote %ld\n",
		total_size, total_bytes);

    fclose(fp);
    }

/****************************************************************************
 * Func: pnm_open                                                           *
 *                                                                          *
 * Desc: opens a pnm file and determines rows, cols, and maxval             *
 *                                                                          *
 * Params: rows- pointer to number of rows in the image                     *
 *         cols - pointer number of columns in the image                    *
 *         maxval - pointer to max value                                    *
 *         filename - name of image file                                    *
 ****************************************************************************/

FILE *pnm_open(int *rows, int *cols, int *maxval, char *filename)
    {
    int firstchar, secchar;
    float scale;
    unsigned long row_size;
    FILE *fp;

    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open %s for reading\n",filename);
	exit(1);
	}

    firstchar = getc(fp);
    secchar = getc(fp);

    if(firstchar != 'P')
	{
	printf("You silly goof... This is not a PPM file!\n");
	exit(1);
	}

    *cols = getnum(fp);
    *rows = getnum(fp);

    switch(secchar)
	{
	case '4':            /* PBM */
	    scale = 0.125;
	    *maxval = 1;
	    break;
	case '5':            /* PGM */
	    scale = 1.0;
	    *maxval = getnum(fp);
	    break;
	case '6':             /* PPM */
	    scale = 3.0;
	    *maxval = getnum(fp);
	    break;
	default :             /* Error */
	    printf("read_pnm: This is not a Portable bitmap RAWBITS file\n");
	    exit(1);
	    break;
	}

    row_size = (*cols) * scale;
    return fp;
    }


/****************************************************************************
 * Func: read_mesh                                                          *
 *                                                                          *
 * Desc: reads mesh data into a mesh structure				    *
 *                                                                          *
 * Params: filename - name of input mesh file                               *
 *                                                                          *
 * Returns: mesh structure storing width, height, x data  and y data        *
 ****************************************************************************/
mesh *read_mesh(char *filename)
    {
    FILE *fp;
    mesh *mesh_data;
    int width, height, mesh_size;

    /* open mesh file for input */
    if((fp = fopen(filename, "rb")) == NULL)
	{
	printf("Unable to open mesh file %s for reading\n", filename);
	exit(1);
	}

    mesh_data = malloc(sizeof(mesh));
    /* read dimensions of mesh */
    fread(&width, sizeof(int), 1, fp);
    fread(&height, sizeof(int), 1, fp);
    mesh_data->width = width;
    mesh_data->height = height;
    mesh_size = width * height;

    /* allocate memory for mesh data */
    mesh_data->x_data = malloc(sizeof(float) * mesh_size);
    mesh_data->y_data = malloc(sizeof(float) * mesh_size);

    fread(mesh_data->x_data, sizeof(float), mesh_size, fp);
    fread(mesh_data->y_data, sizeof(float), mesh_size, fp);

    return(mesh_data);
    }


/****************************************************************************
 * Func: scale_pnm                                                          *
 *                                                                          *
 * Desc: scale an image using nearest neighbor interpolation                *
 *                                                                          *
 * Params: buffer - pointer to image in memory                              *
 *         fileout - name of output file                                    *
 *         rows - number of rows in image                                   *
 *         cols - number of columns in image                                *
 *         x_scale - scale factor in X direction                            *
 *         y_scale - scale factor in Y direction                            *
 *         type - graphics file type (5 = PGM    6 = PPM)                   *
 ****************************************************************************/
void NNinterpolation(image_ptr buffer, char *fileout,
	int rows, int cols, int x_scale, int y_scale, int type)
{
	unsigned long x, y;          /* loop indices for columns and rows */
	unsigned long index;        /* index into line buffer */
	unsigned long source_index; /* address of source pixel in image buffer */
	unsigned char *line_buff;   /* output line buffer */
	int new_rows, new_cols;     /* values of rows and columns for new image */
	unsigned line;              /* number of pixels in one scan line */
	FILE *fp;                   /* output file pointer */
	unsigned long X_Source, Y_Source;  /* x and y address of source pixel */
	pixel_ptr color_buff;       /* pointer to a color image in memory */

	/* open new output file */
	if ((fp = fopen(fileout, "wb")) == NULL)
	{
		printf("Unable to open %s for output\n", fileout);
		exit(1);
	}

	new_cols = cols * x_scale;  // 수정된 부분

	new_rows = rows * y_scale;  // 수정된 부분

	/* print out the portable bitmap header */
	fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

	if (type == 5)        /* PGM file */
		line = new_cols;
	else                 /* PPM file */
	{
		line = new_cols * 3;
		color_buff = (pixel_ptr)buffer;
	}

	line_buff = (unsigned char *)malloc(line);

	for (y = 0; y < new_rows; y++)
	{
		index = 0;
		for (x = 0; x < new_cols; x++)
		{
			X_Source = (unsigned long)((x / x_scale) + 0.5);
			Y_Source = (unsigned long)((y / y_scale) + 0.5);
			source_index = Y_Source * cols + X_Source;
			if (type == 5)      /* PGM */
				line_buff[index++] = buffer[source_index];
			else               /* PPM */
			{
				line_buff[index++] = color_buff[source_index].r;
				line_buff[index++] = color_buff[source_index].g;
				line_buff[index++] = color_buff[source_index].b;
			}
		}
		fwrite(line_buff, 1, line, fp);
	}
	fclose(fp);
}

void biInterpolation(image_ptr buffer, char* fileout, int rows, int cols, int x_scale, int y_scale, int type)
{
	unsigned long x, y;          /* loop indices for columns and rows */
	unsigned long index;        /* index into line buffer */
	unsigned long source_index; /* address of source pixel in image buffer */
	unsigned char* line_buff;   /* output line buffer */
	unsigned line;              /* number of pixels in one scan line */
	int new_rows, new_cols;     /* values of rows and columns for new image */
	FILE* fp;                   /* output file pointer */
	unsigned long X_Source, Y_Source; /* x and y address of source pixel */
	pixel_ptr color_buff;       /* pointer to a color image in memory */

	/* open new output file */
	if ((fp = fopen(fileout, "wb")) == NULL)
	{
		printf("Unable to open %s for output\n", fileout);
		exit(1);
	}

	new_cols = cols * x_scale; // 수정된 부분
	new_rows = rows * y_scale; // 수정된 부분

	/* print out the portable bitmap header */
	fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

	if (type == 5) /* PGM file */
		line = new_cols;
	else /* PPM file */
	{
		line = new_cols * 3;
		color_buff = (pixel_ptr)buffer;
	}

	line_buff = (unsigned char*)malloc(line);

	for (y = 0; y < new_rows; y++)
	{
		index = 0;
		for (x = 0; x < new_cols; x++)
		{
			float NW, NE, SW, SE;
			float EWweight, NSweight;
			float EWtop, EWbottom;

			int X_Source = (int)x / x_scale;
			int Y_Source = (int)y / y_scale;

			NW = buffer[Y_Source * cols + X_Source];
			NE = buffer[Y_Source * cols + X_Source + 1];
			SW = buffer[(Y_Source + 1) * cols + X_Source];
			SE = buffer[(Y_Source + 1) * cols + X_Source + 1];

			EWweight = ((float)x / x_scale) - (float)X_Source;
			NSweight = ((float)y / y_scale) - (float)Y_Source;

			EWtop = NW + EWweight * (NE - NW);
			EWbottom = SW + EWweight * (SE - SW);

			float pixel = EWtop + NSweight * (EWbottom - EWtop);

			source_index = Y_Source * cols + X_Source;

			if (type == 5) /* PGM */
				line_buff[index++] = (unsigned char)pixel;
			else /* PPM */
			{
				line_buff[index++] = color_buff[source_index].r;
				line_buff[index++] = color_buff[source_index].g;
				line_buff[index++] = color_buff[source_index].b;
			}
		}
		fwrite(line_buff, 1, line, fp);
	}
	fclose(fp);
}


float cubicConvWeight(float x) {
	float absX = abs(x);
	float absX2 = absX * absX;
	float absX3 = absX2 * absX;

	float a = -1;

	if (0 <= absX && absX < 1) {
		return (((a + 2) * absX3) - ((a + 3) * absX2) + 1);
	}
	else if (1 <= absX && absX < 2) {
		return ((a * absX3) - ((5 * a) * absX2) + ((8 * a) * absX) - (4 * a));
	}
	else if(2 <= absX ) {
		return 0;
	}
}

void CCinterpolation(image_ptr buffer, char* fileout, int rows, int cols, int x_scale, int y_scale, int type) {
	unsigned long x, y;
	unsigned long index;
	unsigned long source_index;
	unsigned char* line_buff;
	unsigned long line;
	int new_rows, new_cols;
	FILE* fp;
	pixel_ptr color_buff;

	if ((fp = fopen(fileout, "wb")) == NULL) {
		printf("Unable to open %s for output\n", fileout);
		exit(1);
	}

	new_cols = cols * x_scale;
	new_rows = rows * y_scale;

	fprintf(fp, "P%d\n%d %d\n255\n", type, new_cols, new_rows);

	if (type == 5)
		line = new_cols;
	else {
		line = new_cols * 3;
		color_buff = (pixel_ptr)buffer;
	}

	line_buff = (unsigned char*)malloc(line);

	for (y = 0; y < new_rows; y++) {
		index = 0;
		for (x = 0; x < new_cols; x++) {
			float pixel = 0.0;
			float weightSum = 0.0;

			float X_Source = x / (float)x_scale;
			float Y_Source = y / (float)y_scale;

			int X_Source_int = (int)floor(X_Source);
			int Y_Source_int = (int)floor(Y_Source);

			for (int i = -2; i <= 1; i++) {
				for (int j = -2; j <= 1; j++) {
					int currX = X_Source_int + j;
					int currY = Y_Source_int + i;

					if (currX >= 0 && currX < cols && currY >= 0 && currY < rows) {
						float weight = cubicConvWeight(X_Source - currX) * cubicConvWeight(Y_Source - currY);
						pixel += buffer[currY * cols + currX] * weight;
						weightSum += weight;
					}
				}
			}

			pixel /= weightSum;

			source_index = Y_Source_int * cols + X_Source_int;

			if (type == 5)
				line_buff[index++] = (unsigned char)pixel;
			else {
				line_buff[index++] = color_buff[source_index].r;
				line_buff[index++] = color_buff[source_index].g;
				line_buff[index++] = color_buff[source_index].b;
			}
		}
		fwrite(line_buff, 1, line, fp);
	}
	fclose(fp);
}
