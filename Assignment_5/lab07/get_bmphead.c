/***************************************************************************
 * File: get_bmphead.c                                                     *
 *                                                                         *
 * Desc: This program reads header part of bmp format image                *
 ***************************************************************************/
  
 
#include <fcntl.h>

typedef unsigned short int USHORT;
typedef unsigned long int  ULONG; 

struct BITMAPHEADER {
  USHORT bmpType;                       // BMP 파일의 종류를 나타내는 식별자
  ULONG  bmpSize;                       // BMP 파일의 크기(바이트 단위)
  USHORT bmpReserved1;                  // 예약
  USHORT bmpReserved2;                  // 예약
  ULONG  bmpOffset;                     // 이미지 정보가 시작되는 위치(Offset)
  ULONG  bmpHeaderSize;                 // Header의 크기(바이트 단위)
  ULONG  bmpWidth;                      // 가로 크기(픽셀 단위)
  ULONG  bmpHeight;                     // 세로 크기(픽셀 단위)
  USHORT bmpPlanes;                     // Color Plane의 수(1로 셋팅)
  USHORT bmpBitCount;                   // 한 픽셀에 할당된 비트수
  ULONG  bmpCompression;                // 압축 기법 사용여부
  ULONG  bmpBitmapSize;                 // 비트맵 이미지의 크기
  ULONG  bmpXPelsPerMeter;              // 수평 해상도
  ULONG  bmpYPelsPerMeter;              // 수직 해상도
  ULONG  bmpColors;                     // 사용된 색상수
  ULONG  bmpClrImportant;
}; 

struct BITMAPHEADER bmp_header;                // Bitmap 파일의 Header 구조체

void ReadBitmapHeader(char *filename) {
	int read_fd; // 파일을 읽고 쓰는데 사용할 Descriptor

	read_fd = open(filename, O_RDONLY); // bmp파일을 open한다.
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

	// bmpOffset, bmpWidth, bmpHeight 출력
	printf("bmpOffset: %d\n", bmp_header.bmpOffset);
	printf("bmpWidth: %d\n", bmp_header.bmpWidth);
	printf("bmpHeight: %d\n", bmp_header.bmpHeight);
}

