/***************************************************************************
 * File: get_bmphead.c                                                     *
 *                                                                         *
 * Desc: This program reads header part of bmp format image                *
 ***************************************************************************/
  
 
#include <fcntl.h>

typedef unsigned short int USHORT;
typedef unsigned long int  ULONG; 

struct BITMAPHEADER {
  USHORT bmpType;                       // BMP ������ ������ ��Ÿ���� �ĺ���
  ULONG  bmpSize;                       // BMP ������ ũ��(����Ʈ ����)
  USHORT bmpReserved1;                  // ����
  USHORT bmpReserved2;                  // ����
  ULONG  bmpOffset;                     // �̹��� ������ ���۵Ǵ� ��ġ(Offset)
  ULONG  bmpHeaderSize;                 // Header�� ũ��(����Ʈ ����)
  ULONG  bmpWidth;                      // ���� ũ��(�ȼ� ����)
  ULONG  bmpHeight;                     // ���� ũ��(�ȼ� ����)
  USHORT bmpPlanes;                     // Color Plane�� ��(1�� ����)
  USHORT bmpBitCount;                   // �� �ȼ��� �Ҵ�� ��Ʈ��
  ULONG  bmpCompression;                // ���� ��� ��뿩��
  ULONG  bmpBitmapSize;                 // ��Ʈ�� �̹����� ũ��
  ULONG  bmpXPelsPerMeter;              // ���� �ػ�
  ULONG  bmpYPelsPerMeter;              // ���� �ػ�
  ULONG  bmpColors;                     // ���� �����
  ULONG  bmpClrImportant;
}; 

struct BITMAPHEADER bmp_header;                // Bitmap ������ Header ����ü

void ReadBitmapHeader(char *filein, char *fileout) {
	int read_fd; // ������ �а� ���µ� ����� Descriptor
	int bmp_fd, pgm_fd;
	unsigned char *bmp_data, *pgm_data;
	unsigned long int i;

	read_fd = open(filename, O_RDONLY); // bmp������ open�Ѵ�.
	if (read_fd == -1) {
		printf("Can't open file %s\n", filename);
		exit(1);
	}

	read(read_fd, &bmp_header.bmpType, sizeof(bmp_header.bmpType));
	read(read_fd, &bmp_header.bmpSize, sizeof(bmp_header.bmpSize));
	read(read_fd, &bmp_header.bmpReserved1, sizeof(bmp_header.bmpReserved1));
	read(read_fd, &bmp_header.bmpReserved2, sizeof(bmp_header.bmpReserved2));
	read(read_fd, &bmp_header.bmpOffset, sizeof(bmp_header.bmpOffset));
	read(read_fd, &bmp_header.bmpHeaderSize, sizeof(bmp_header.bmpHeaderSize));
	read(read_fd, &bmp_header.bmpWidth, sizeof(bmp_header.bmpWidth));
	read(read_fd, &bmp_header.bmpHeight, sizeof(bmp_header.bmpHeight));
	read(read_fd, &bmp_header.bmpPlanes, sizeof(bmp_header.bmpPlanes));
	read(read_fd, &bmp_header.bmpBitCount, sizeof(bmp_header.bmpBitCount));
	read(read_fd, &bmp_header.bmpCompression, sizeof(bmp_header.bmpCompression));
	read(read_fd, &bmp_header.bmpBitmapSize, sizeof(bmp_header.bmpBitmapSize));
	read(read_fd, &bmp_header.bmpXPelsPerMeter, sizeof(bmp_header.bmpXPelsPerMeter));
	read(read_fd, &bmp_header.bmpYPelsPerMeter, sizeof(bmp_header.bmpYPelsPerMeter));
	read(read_fd, &bmp_header.bmpColors, sizeof(bmp_header.bmpColors));
	read(read_fd, &bmp_header.bmpClrImportant, sizeof(bmp_header.bmpClrImportant));

	close(read_fd);

	// bmpOffset, bmpWidth, bmpHeight ���
	printf("bmpOffset: %d\n", bmp_header.bmpOffset);
	printf("bmpWidth: %d\n", bmp_header.bmpWidth);
	printf("bmpHeight: %d\n", bmp_header.bmpHeight);

	// BMP ���� ����
	bmp_fd = open(filein, O_RDONLY);
	if (bmp_fd == -1) {
		printf("Can't open BMP file %s\n", filein);
		exit(1);
	}

	// PGM ���� ����
	pgm_fd = open(fileout, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (pgm_fd == -1) {
		printf("Can't create PGM file %s\n", fileout);
		exit(1);
	}

	// BMP ������ �б�
	bmp_data = (unsigned char *)malloc(bmp_header.bmpSize);
	lseek(bmp_fd, 0, SEEK_SET);
	read(bmp_fd, bmp_data, bmp_header.bmpSize);

	// PGM ������ ����
	pgm_data = (unsigned char *)malloc(bmp_header.bmpWidth * bmp_header.bmpHeight);
	for (i = 0; i < bmp_header.bmpWidth * bmp_header.bmpHeight; i++) {
		unsigned char r = bmp_data[bmp_header.bmpOffset + i * 3];
		unsigned char g = bmp_data[bmp_header.bmpOffset + i * 3 + 1];
		unsigned char b = bmp_data[bmp_header.bmpOffset + i * 3 + 2];
		unsigned char gray = (unsigned char)(0.3 * r + 0.59 * g + 0.11 * b);
		pgm_data[i] = gray;
	}
	write(pgm_fd, pgm_data, bmp_header.bmpWidth * bmp_header.bmpHeight);

	close(bmp_fd);
	close(pgm_fd);

	free(bmp_data);
	free(pgm_data);
}
