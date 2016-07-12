
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <string.h>

using namespace std;

//动态申请二维数组
template <typename T>
T** newArray2D(int row, int col)
{
    int size = sizeof(T);
    int point_size = sizeof(T*);
    //先申请内存，其中sizeof(T*) * row表示存放row个行指针
    T **arr = (T **) malloc(point_size * row + size * row * col);
    if (arr != NULL)
    {
        T *head = (T*)((long)arr + point_size * row);
        for (int i = 0; i < row; ++i)
        {
            arr[i] =  (T*)((long)head + i * col * size);
            for (int j = 0; j < col; ++j)
                new (&arr[i][j]) T;
        }
    }
    return (T**)arr;
}
//释放二维数组
template <typename T>
void deleteArray2D(T **arr, int row, int col)
{
    for (int i = 0; i < row; ++i)
        for (int j = 0; j < col; ++j)
            arr[i][j].~T();
    if (arr != NULL)
        free((void**)arr);
}

typedef struct {
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB_T;

typedef RGB_T rgb_t;

class BmpTransfer {
	public:
		BmpTransfer(char *src, char *dst, unsigned int dencity);
		void GetBmpPar(void);
	private:
		void DataChange(unsigned int &width, unsigned int &height, unsigned int &density);
		ifstream *mFSrc;
		ofstream *mFDst;
		unsigned int mBmpOffset;
		unsigned int mBmpWidth;
		unsigned int mBmpHeight;
		unsigned short mBmpBpp;
		unsigned int mPixDensity;
		int mColorNum;
		rgb_t **mRgb24;
		const int BMP_OFFSET;
		const int BMP_WIDTH;
		const int BMP_BPP;
};

BmpTransfer::BmpTransfer(char *src, char *dst, unsigned int dencity = 10)
	:BMP_OFFSET(0x0A), BMP_WIDTH(0x12), BMP_BPP(0x1c)
{
	this->mPixDensity = dencity;

	mFSrc = new ifstream(src, ios::binary);
	mFDst = new ofstream(dst, ios::binary);

	mColorNum = 16;

	GetBmpPar();

	mRgb24 = newArray2D<rgb_t>(mBmpHeight, mBmpWidth);

	mFSrc->seekg(this->mBmpOffset, ios::beg);

	mFSrc->read((char *)mRgb24[0], mBmpWidth * mBmpHeight * mBmpBpp / 8);

	DataChange(this->mBmpWidth, this->mBmpHeight, this->mPixDensity);

	mFDst->seekp(this->mBmpOffset);

	mFDst->write((char *)mRgb24[0], mBmpWidth * mBmpHeight * mBmpBpp / 8);

	deleteArray2D<rgb_t>(mRgb24, mBmpHeight, mBmpWidth);

	mFSrc->close();
	mFDst->close();
}

void BmpTransfer::GetBmpPar()
{
	short signatrue = 0;
	*mFDst << mFSrc->rdbuf();

	mFSrc->seekg(0, ios::beg);
	mFSrc->read((char *)&signatrue, sizeof(signatrue));

	cout << "BMP Signature:" << hex << signatrue << endl;
	if (signatrue != 0x4D42)
	{
		cout << "Not a Bmp file" << endl;
		return;
	}

	mFSrc->seekg(BMP_OFFSET, ios::beg);
	cout << "Offset " << mFSrc->tellg() << endl;
	mFSrc->read((char *)&this->mBmpOffset, sizeof(this->mBmpOffset));
	mFSrc->seekg(BMP_WIDTH, ios::beg);
	mFSrc->read((char *)&this->mBmpWidth, sizeof(this->mBmpWidth));
	mFSrc->read((char *)&this->mBmpHeight, sizeof(this->mBmpHeight));
	mFSrc->seekg(BMP_BPP, ios::beg);
	mFSrc->read((char *)&this->mBmpBpp, sizeof(this->mBmpBpp));

	cout << dec\
		<< "offset:" << mBmpOffset\
		<< ", width:" << mBmpWidth\
		<< ", height:" << mBmpHeight\
		<< ", bpp:" << mBmpBpp\
		<< endl;


}

void BmpTransfer::DataChange(unsigned int &width, unsigned int &height, unsigned int &density)
{
	unsigned int sumr, sumg, sumb;
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
					sumr += mRgb24[row * density + i][column * density + j].r;
					sumg += mRgb24[row * density + i][column * density + j].g;
					sumb += mRgb24[row * density + i][column * density + j].b;
				}
			}

			sumr /= (density * density);
			sumg /= (density * density);
			sumb /= (density * density);

			for (unsigned i = 0; i < density; i++)
			{
				for (unsigned int j = 0; j < density; j++)
				{
					if ((i % density == 0) || (j % density == 0))
					{
						mRgb24[row * density + i][column * density + j].b = 0;
						mRgb24[row * density + i][column * density + j].g = 0;
						mRgb24[row * density + i][column * density + j].r = 0;
					}
					else
					{
						mRgb24[row * density + i][column * density + j].b = sumb;
						mRgb24[row * density + i][column * density + j].g = sumg;
						mRgb24[row * density + i][column * density + j].r = sumr;
					}
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


