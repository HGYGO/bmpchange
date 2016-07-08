
#include <iostream>
#include <fstream>

using namespace std;

class BmpTransfer {
	public:
		BmpTransfer(string src, string dst);
		void GetBmpPar(void);
	private:
		string src;
		string dst;
		unsigned int width;
		unsigned int height;
		unsigned int pbb;
};

const unsigned int BMP_DATA_OFFSET	= 0x0A;
const unsigned int BMP_WIDTH_OFFSET	= 0x12;
const unsigned int BMP_BPP_OFFSET	= 0x1C; q

BmpTransfer::BmpTransfer(string dst, string src)
{
	this->src = src;
	this->dst = dst;

	cout<<"test"<<endl;
}

void BmpTransfer::GetBmpPar()
{
	ifstream ifs;
	ifs.open(this->src);

	ifs.failbit
}

int main(int argc, char **argv)
{
	if (argc != 4)
	{
		printf("Invalid parameter, argc %d\n", argc);
		printf("argv usage: src dst xnum\n");
		return -1;
	}

	BmpTransfer t = BmpTransfer(argv[1], argv[2]);
	return 1;
}


