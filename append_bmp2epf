#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include<string.h>

#pragma pack(push, 1)  

int t = 0;
int t2 = 0;
int t5 = 0;
int t6 = 0;
unsigned int numArr[400000];
unsigned int numArr2[400000];

typedef struct _BITMAPFILEHEADER
{
	unsigned short bfType;
	unsigned int   bfSize;
	unsigned short bfReserved1;
	unsigned short bfReserved2;
	unsigned int   bfOffBits;
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER
{
	unsigned int   biSize;
	int            biWidth;
	int            biHeight;
	unsigned short biPlanes;
	unsigned short biBitCount;
	unsigned int   biCompression;
	unsigned int   biSizeImage;
	int            biXPelsPerMeter;
	int            biYPelsPerMeter;
	unsigned int   biClrUsed;
	unsigned int   biClrImportant;
} BITMAPINFOHEADER;



#pragma pack(pop)

#define PIXEL_SIZE   1   
#define PIXEL_ALIGN  4   
int main()
{
	int iter;
	char epf[20];
	char epf2[300];
	printf("\n [ BMP 뒤에 좌표 값 기록하는 프로그램 ]\n");
	printf("\n 2019.07.06 \n");
	printf("\n 구버전 개발자 무이스 만듬. \n");
	printf("\n 사진의 개수(n)를 입력하세요: 00000.bmp ~ n.bmp ");
	scanf("%d", &iter);
	printf("\n");

	printf(" 기존 epf파일 이름을 적어주세요. xxxxx.epf ( 안적으면 픽셀 값 + 스텐실 값만 출력 )\n\n ");
	scanf("%s", &epf);
	sprintf(epf2, "%s.epf", epf);

	printf("\n 처리중.. 프로그램이 꺼질 때 까지 기다려주세요. \n ");

	for (int i = 0; i <= iter; i++)
	{
		FILE *fpBmp;
		FILE *fpTxt;
		//FILE *fpTxt;
		FILE *fpEpf;
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;
		unsigned char *buffer;

		unsigned char *image;
		int size;
		int width, height;
		int padding;


		char filename[20];
		char filename2[20];
		sprintf(filename, "%05d.bmp", i);
		sprintf(filename2, "%05d.txt", i);
		fpBmp = fopen(filename, "rb");
		if (fpBmp == NULL)
			return 0;

		(fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBmp));


		if (fileHeader.bfType != 'MB')
		{
			fclose(fpBmp);
			return 0;
		}

		(fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBmp));


		if (infoHeader.biBitCount != 8)
		{
			fclose(fpBmp);
			return 0;
		}


		size = infoHeader.biSizeImage;
		width = infoHeader.biWidth;
		height = infoHeader.biHeight;

		padding = (PIXEL_ALIGN - ((width * PIXEL_SIZE) % PIXEL_ALIGN)) % PIXEL_ALIGN;

		if (size == 0)
		{
			size = (width * PIXEL_SIZE + padding) * height;
		}

		image = malloc(size);

		fseek(fpBmp, fileHeader.bfOffBits, SEEK_SET);


		(fread(image, size, 1, fpBmp));


		fclose(fpBmp);

		fpTxt = fopen(filename, "ab+");
		if (fpTxt == NULL)
		{
			free(image);
			return 0;
		}

		int c = 0;
		int d = 128;


		fpEpf = fopen(epf2, "rb");
		if (fpEpf == NULL)
		{
			puts(" Cannot file open ! ");
			return 1;
		}
		//fprintf(fpTxt, "\n ");
		//fprintf(fpTxt, "%d", size);
		//fprintf(fpTxt, "\n ");

		buffer = malloc(16);

		fseek(fpEpf, -16 * (iter + 1) + 16 * i, SEEK_END);

		fread(buffer, 16, 1, fpEpf);

		fclose(fpEpf);


		fwrite(buffer, 8, 1, fpTxt);


		fclose(fpTxt);
	}
}
