#include "utils.h"

class fileHandling
{
public:
	ifstream fin;
	int position;
	int size;

	fileHandling(string s) :fin(s, ios_base::in), position(0)
	{

	}

	~fileHandling()
	{
		fin.close();
	}

	int readInChunks(vector<char> &buffer);
};
