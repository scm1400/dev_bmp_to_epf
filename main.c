#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include<string.h>

#pragma pack(push, 1)  

int t = 0;
int t2 = 0;
int numArr[50000];
int numArr2[10000];

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

typedef struct _RGBTRIPLE         
{
	unsigned char rgbtBlue;       
	unsigned char rgbtGreen;        
	unsigned char rgbtRed;           
} RGBTRIPLE;


#pragma pack(pop)

#define PIXEL_SIZE   1   
#define PIXEL_ALIGN  4   

int main()
{
	int iter;
	printf("사진의 개수(n)를 입력하세요: 00000.bmp ~ n.bmp ");
	scanf("%d", &iter);
	for (int i = 0; i <= iter; i++)
	{
		FILE *fpBmp;                   
		FILE *fpTxt;                   
		BITMAPFILEHEADER fileHeader;    
		BITMAPINFOHEADER infoHeader;   

		unsigned char *image;    
		int size;              
		int width, height;      
		int padding;            


		char filename[20];
		sprintf(filename, "%05d.bmp", i);
		fpBmp = fopen(filename, "rb");
		if (fpBmp == NULL)
			return 0;

		if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

		if (fileHeader.bfType != 'MB')
		{
			fclose(fpBmp);
			return 0;
		}

		if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

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

		if (fread(image, size, 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

		fclose(fpBmp); 

		fpTxt = fopen("stencil.txt", "at+");  
		if (fpTxt == NULL)    
		{
			free(image);     
			return 0;       
		}

		int c = 0;
		int d = 128;

		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{

				int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);


				int pixel = image[index];

				fprintf(fpTxt, "%02x ", pixel);
			}

			//fprintf(fpTxt, "\n");
		}

		//각 줄의 스텐실 작성 픽셀 값이 연속으로 0이면 k개 만큼 0k 로 개수를 표시하고
		//0보다 큰 픽셀 값이 연속으로 나오면 128 + 개수를 표시
		//남은 픽셀 값이 0 으로 마무리되면 0개수에 상관없이 00(줄마침)표시
		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{
				int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);

				int pixel = image[index];

				if (pixel == 0)
				{
					c++;
					if (d > 128)
					{
						fprintf(fpTxt, "%02x ", d);
						d = 128;
					}
					if ( ((index - padding * y) % width == width - 1 ) && c > 0)
					{	
						if(c==width)
						{
							c = width;
							fprintf(fpTxt, "%02x ", c);
							fprintf(fpTxt, "00 ");
						}
						else
						{
							c = 0;
							fprintf(fpTxt, "%02x ", c);
						}

					}

				}
				else if (pixel > 0)
				{
					d++;
					if (c > 0)
					{
						fprintf(fpTxt, "%02x ", c);
						c = 0;
					}
					
					if (((index - padding * y) % width == width - 1) && c == 0)
					{
						fprintf(fpTxt, "%02x ", d);
						fprintf(fpTxt, "%02x ", c);
					}
					
				}
			}
			c = 0;
			d = 128;
			//fprintf(fpTxt, "\n");  
		}

		fclose(fpTxt);  

		free(image);     
	}
	// 모든 파일 다시 반복해서 시작위치와 끝나는 위치 기록
	for (int i = 0; i <= iter; i++)
	{
		FILE *fpBmp;
		FILE *fpTxt;
		BITMAPFILEHEADER fileHeader;
		BITMAPINFOHEADER infoHeader;

		unsigned char *image;
		int size;
		int width, height;
		int padding;


		char filename[20];
		sprintf(filename, "%05d.bmp", i);
		fpBmp = fopen(filename, "rb");
		if (fpBmp == NULL)
			return 0;

		if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

		if (fileHeader.bfType != 'MB')
		{
			fclose(fpBmp);
			return 0;
		}

		if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

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

		if (fread(image, size, 1, fpBmp) < 1)
		{
			fclose(fpBmp);
			return 0;
		}

		fclose(fpBmp);

		fpTxt = fopen("stencil.txt", "at+");
		if (fpTxt == NULL)
		{
			free(image);
			return 0;
		}

		int c = 0;
		int d = 128;

		//각 줄의 스텐실 작성 픽셀 값이 연속으로 0이면 k개 만큼 0k 로 개수를 표시하고
		//0보다 큰 픽셀 값이 연속으로 나오면 128 + 개수를 표시
		//남은 픽셀 값이 0 으로 마무리되면 0개수에 상관없이 00(줄마침)표시
		
		
		// t에 스텐실 개수 저장해서 배열에 저장함
		for (int y = height - 1; y >= 0; y--)
		{
			for (int x = 0; x < width; x++)
			{
				int index = (x * PIXEL_SIZE) + (y * (width * PIXEL_SIZE)) + (padding * y);

				int pixel = image[index];

				if (pixel == 0)
				{
					c++;
					if (d > 128)
					{
						//fprintf(fpTxt, "%02x ", d);
						d = 128;
						t++;
					}
					if (((index - padding * y) % width == width - 1) && c > 0)
					{
						if (c == width)
						{
							c = width;
							//fprintf(fpTxt, "%02x ", c);
							//fprintf(fpTxt, "00 ");
							t += 2;
						}
						else
						{
							c = 0;
							//fprintf(fpTxt, "%02x ", c);
							t++;
						}

					}

				}

				else if (pixel > 0)
				{
					d++;
					if (c > 0)
					{
						//fprintf(fpTxt, "%02x ", c);
						t++;
						c = 0;
					}

					if (((index - padding * y) % width == width - 1) && c == 0)
					{
						//fprintf(fpTxt, "%02x ", d);
						//fprintf(fpTxt, "%02x ", c);
						t += 2;
					}

				}
			}
			c = 0;
			d = 128;
			//fprintf(fpTxt, "\n"); 
		} 
		numArr[i+1] = t;
		t = 0;
		numArr2[i + 1] = width * height;
		if (i == 0)
		{
			fprintf(fpTxt, "\n");
			fprintf(fpTxt, "00 00 00 00 ");
			if (width * height < 256)
			{
				fprintf(fpTxt, "%02x 00 00 00", width * height);
			}
			else if (width * height < 65536)
			{
				fprintf(fpTxt, "%02x 00 00", width * height);
			}
			fprintf(fpTxt, "\n");
		}
		else
		{
			int p = 0;
			int t3 = 0;
			unsigned char data[4];

			int o = 1;
			while (o<=i)
			{
				p += numArr[o];
				o++;
			}

			int z = 1;
			t2 = 0;
			while (z <= i)
			{
				t2 += numArr2[z];
				z++;
			}
			int max = 0x00;
			max = t2 + p ;
		//시작위치 : 전에 있던 스텐실의 총 개수 (p) + 이전 이미지들의 넓이 합 
			memcpy(data, &max, 4);
			int f = 0;
			for (f = 0; f < 4; f++)
			{
				fprintf(fpTxt, "%02x ",data[f]);
			}
			for (f = 0; f < 4; f++)
			{
				data[f] = 0 ;
			}
		//끝위치 : 시작위치 + 넓이
			t3 = width * height ;
			int max2 = 0x00;
			max2 = (max + t3);

			memcpy(data, &max2, 4);
			for (f = 0; f < 4; f++)					
			{
				fprintf(fpTxt, "%02x ", data[f]);
			}

			fprintf(fpTxt, "\n");


		}

		fclose(fpTxt);

		free(image);
	}
	return 0;
}
