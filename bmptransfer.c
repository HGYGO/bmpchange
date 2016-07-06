#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int copy_file(char *dst, char *src);

typedef struct rgb_pix_t {
	unsigned char red;
	unsigned char green;
	unsigned char blue;
}rgb_pix_t;

int main(int argc, char **argv)
{
	char *buff;
	char **pmatrix;
	FILE *fsrc, *fdst;
	short signature, bpp;
	int offset, xres, yres;
	unsigned int length;
	int x, y, z;
	char *src, *dst;
	int xnum, ynum;
	rgb_pix_t *rgb_buf;
	int pix_unit_side;

	if (argc != 4)
	{
		printf("Invalid parameter, argc %d\n", argc);
		printf("argv usage: src dst xnum\n");
		return -1;
	}

	src = (char *)malloc(strlen(argv[1]));
	dst = (char *)malloc(strlen(argv[2]));

	strcpy(src, argv[1]);
	strcpy(dst, argv[2]);
	xnum = atoi(argv[3]);

	printf("input file %s, output file %s, %d\r\n", src, dst, xnum);

	copy_file(dst, src);

	/*fsrc = fopen(src, "r");*/
	fdst = fopen(src, "r+");
	fread(&signature, sizeof(short), 1, fdst);

	if (signature != 0x4D42)
	{
		printf("input file is not a bitmap file. \n");
		fclose(fdst);
		return -1;
	}
	else
	{
		fseek(fdst, 0x0A, SEEK_SET);		// bitmap data offset
		fread(&offset, sizeof(int), 1, fdst);
		fseek(fdst, 0x12, SEEK_SET);		// bitmap width
		fread(&xres, sizeof(int), 1, fdst);
		fread(&yres, sizeof(int), 1, fdst);	// bitmap height
		fseek(fdst, 0x1C, SEEK_SET);
		fread(&bpp, sizeof(short), 1, fdst);	// bitmap bpp

		printf("offset = %d, width = %d, height = %d, bpp = %d \n", offset, xres, yres, bpp);

		length = xres * yres * bpp / 8;
		buff = (char *)malloc(length);

		if ((length % sizeof(rgb_pix_t)) != 0)
		{
			printf("error: buff len is not a multiple of %d\n", (int)sizeof(rgb_pix_t));
			return -1;
		}
		rgb_buf = (rgb_pix_t *)buff;

		fseek(fdst, offset, SEEK_SET);		// seek to data start position
		fread(buff, length, 1, fdst);		// read out bitmap data
		/*fclose(fdst);*/
		pix_unit_side = xres / xnum;
		ynum  = yres / pix_unit_side;

		printf("xnum:ynum %d:%d\n", xnum, ynum);

		for (x = 0; x <= xnum; x++)	// convert XRGB to RGBX
		{
			for (y = 0; y <= ynum; y++)
			{
				/*int x_orig = 0, y_orig = 0;*/

				for (z = 0; z < pix_unit_side * pix_unit_side; z++)
				{
					buff[x * xres + y % ynum + z] = 0;
					buff[x * xres + y % ynum + z + 1] = 0;
					buff[x * xres + y % ynum + z + 2] = 0;
					buff[x * xres + y % ynum + z + 3] = 0;
				}
			}
			buff[i * 4] = buff[i * 4 + 1];
			buff[i * 4 + 1] = buff[i * 4 + 2];
			buff[i * 4 + 2] = buff[i * 4 + 3];
			buff[i * 4 + 3] = 0x00;
		}
		fseek(fdst, offset, SEEK_SET);
		/*fdst = fopen("logo.pic", "wr+");*/
		fwrite(buff, length, 1, fdst);
		fclose(fdst);
	}

	return 0;
}

#if 1
int copy_file(char *dst, char *src)
{
	FILE *fsrc, *fdst;
	char c;

	fsrc = fopen(src, "rb");
	fdst = fopen(dst, "wb");

	if (fsrc == NULL || fdst == NULL)
	{
		printf("error - Open file failed\n");
		return -1;
	}

	while ((c = fgetc(fsrc)) != EOF)
	{
		fputc(c, fdst);
	}

	fclose(fsrc);
	fclose(fdst);

	return 0;
}
#endif



void modify_pix(char *buf, unsigned int len, int sres, int yres)
{
}
