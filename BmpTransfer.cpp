
#include <iostream>
#include <fstream>

using namespace std;

typedef struct _RGB24_T {
	unsigned char r;
	unsigned char g;
	unsigned char b;
}RGB24_T;

class BmpTransfer {
	public:
		BmpTransfer(char *src, char *dst);
		BmpTransfer(int a);
		void GetBmpPar(void);
	private:
		int test;
		ifstream *fsrc;
		ofstream *fdst;
		string src;
		string dst;
		unsigned int width;
		unsigned int height;
		unsigned int pbb;
};

const unsigned int BMP_DATA_OFFSET	= 0x0A;
const unsigned int BMP_WIDTH_OFFSET	= 0x12;
const unsigned int BMP_BPP_OFFSET	= 0x1C;

BmpTransfer::BmpTransfer(char *src, char *dst)
{
	short signatrue = 0;

	fsrc = new ifstream(src, ios::binary);
	fdst = new ofstream(dst, ios::binary);

	fsrc->

	*fdst << fsrc->rdbuf();

	//fsrc->close();
	//fdst->close();
}

void BmpTransfer::GetBmpPar()
{
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Invalid parameter, argc %d\n", argc);
		printf("argv usage: src dst xnum\n");
		return -1;
	}

	BmpTransfer t(argv[1], argv[2]);
	return 1;
}


