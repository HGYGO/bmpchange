
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
//#include <stdlib.h>

using namespace std;

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB24_T;

class BmpTransfer {
	public:
		BmpTransfer(char *src, char *dst, unsigned int dencity);
		void GetBmpPar(void);
	private:
		void DataChange(unsigned int &width, unsigned int &height, unsigned int &density);
		void CopyFileTest(void);
		ifstream *fsrc;
		ofstream *fdst;
		unsigned int bmpOffset;
		unsigned int bmpWidth;
		unsigned int bmpHeight;
		unsigned short bmpBpp;
		unsigned int pixDensity;
		RGB24_T **pixData;
};

const int BMP_DATA_OFFSET	= 0x0A;
const int BMP_WIDTH			= 0x12;
const int BMP_BPP			= 0x1C;

BmpTransfer::BmpTransfer(char *src, char *dst, unsigned int dencity = 10)
{
	this->pixDensity = dencity;
	short signatrue = 0;

	fsrc = new ifstream(src, ios::binary);
	fdst = new ofstream(dst, ios::binary);

	cout << "Copy file " << src << " to " << dst << endl;
	*fdst << fsrc->rdbuf();

	fsrc->seekg(0, ios::beg);
	fsrc->read((char *)&signatrue, sizeof(signatrue));

	cout << "BMP Signature:" << hex << signatrue << endl;
	if (signatrue != 0x4D42)
	{
		cout << "Not a Bmp file" << endl;
		return;
	}

	fsrc->seekg(BMP_DATA_OFFSET, ios::beg);
	cout << "Offset " << fsrc->tellg() << endl;
	fsrc->read((char *)&this->bmpOffset, sizeof(this->bmpOffset));
	fsrc->seekg(BMP_WIDTH, ios::beg);
	fsrc->read((char *)&this->bmpWidth, sizeof(this->bmpWidth));
	fsrc->read((char *)&this->bmpHeight, sizeof(this->bmpHeight));
	fsrc->seekg(BMP_BPP, ios::beg);
	fsrc->read((char *)&this->bmpBpp, sizeof(this->bmpBpp));

	cout << dec\
		<< "offset:" << bmpOffset\
		<< ", width:" << bmpWidth\
		<< ", height:" << bmpHeight\
		<< ", bpp:" << bmpBpp\
		<< endl;

	pixData = new RGB24_T*[this->bmpHeight];

	fsrc->seekg(this->bmpOffset, ios::beg);
	for (unsigned int i = 0; i < this->bmpHeight; i++)
	{
		pixData[i] = new RGB24_T[this->bmpWidth];
		fsrc->read((char *)pixData[i], this->bmpWidth * this->bmpBpp / 8);
	}

	DataChange(this->bmpWidth, this->bmpHeight, this->pixDensity);

	fdst->seekp(this->bmpOffset);

	for (unsigned int i = 0; i < this->bmpHeight; i++)
	{
		fdst->write((char *)pixData[i], this->bmpWidth * this->bmpBpp / 8);
		delete pixData[i];
	}

	delete pixData;

	fsrc->close();
	fdst->close();
}

void BmpTransfer::GetBmpPar()
{
}

void BmpTransfer::CopyFileTest(void)
{
	int writeCount = 0;
	char *bmpHead = new char[this->bmpOffset];
	ofstream testFile("copy_test.bmp", ios::binary);

	int fsrcPos = fsrc->tellg();

	fsrc->seekg(0, ios::beg);
	fsrc->read(bmpHead, this->bmpOffset);
	testFile.write(bmpHead, this->bmpOffset);
	writeCount += this->bmpBpp;
	cout << "count " << writeCount << endl;

	for(unsigned int i = 0; i < this->bmpHeight; i++)
	{
		testFile.write((char *)pixData[i], this->bmpWidth * this->bmpBpp / 8);
	}

	if (testFile.is_open())
	{
		testFile.close();
	}
	fsrc->seekg(fsrcPos);
}

void BmpTransfer::DataChange(unsigned int &width, unsigned int &height, unsigned int &density)
{
	unsigned int sumr = 0;
	unsigned int sumg = 0;
	unsigned int sumb = 0;

	cout << "width: " << width << " height: " << height << " density: " << density << endl;

	for (unsigned int row = 0; row < height / density; row++)
	{
		for (unsigned int column = 0; column < width / density; column++)
		{
			sumr = 0; sumg = 0; sumb = 0;
			for (unsigned i = 0; i < density; i++)
			{
				for (unsigned int j = 0; j < density; j++)
				{
					sumr += pixData[row * density + i][column * density + j].r;
					sumg += pixData[row * density + i][column * density + j].g;
					sumb += pixData[row * density + i][column * density + j].b;
				}
			}

			sumr /= density * density;
			sumg /= density * density;
			sumb /= density * density;

			for (unsigned i = 0; i < density; i++)
			{
				for (unsigned int j = 0; j < density; j++)
				{
					pixData[row * density + i][column * density + j].r = sumr;
					pixData[row * density + i][column * density + j].g = sumg;
					pixData[row * density + i][column * density + j].b = sumb;
				}
			}

		}
	}
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Invalid parameter, argc %d\n", argc);
		printf("argv usage: src dst xnum\n");
		return -1;
	}

	BmpTransfer t(argv[1], argv[2], atoi(argv[3]));

	printf("finished \n");
	return 1;
}


