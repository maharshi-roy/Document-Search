#include <iostream>
#include <memory>
#include <map>
#include <string>
#include <vector>
#include <set>
#include "utils.h"
using namespace std;

template <typename T>
class TTNODE
{
public:
	unique_ptr <T> left;
	unique_ptr <T> middle;
	unique_ptr <T> right;
	char data;
protected:
	TTNODE()
	{

	}
	TTNODE(const char ch) :data(ch)
	{

	}
	~TTNODE()
	{

	}
};

class TTNODE_files :public TTNODE<TTNODE_files>
{
public:
	map <int, int> word_counts;
	TTNODE_files(char ch) :TTNODE(ch)
	{

	}
	~TTNODE_files()
	{

	}
};

class TTNODE_words :public TTNODE<TTNODE_words>
{
public:
	int isleaf;

	TTNODE_words(char ch) :isleaf(0), TTNODE(ch)
	{

	}
	~TTNODE_words()
	{

	}
};

///private cons
template <typename T>
class TTROOT
{
public:
	unique_ptr <T> root;
	TTROOT()
	{

	}
	~TTROOT()
	{

	}
	unique_ptr <T> & get_top(unique_ptr <T> & node, int & count, string & pattern);
protected:	
	virtual void insert(unique_ptr <T> & node, int index, string & word, int  filename)=0;
};

class TTROOT_files :public TTROOT<TTNODE_files>//this is a singleton class
{
	void insert(unique_ptr <TTNODE_files> & node, int index, string & word, int  filename);
public:
	void traverse(unique_ptr <TTNODE_files> & node, map<int, int> &count);

	int search(unique_ptr <TTNODE_files> & node, int index, string & word, map<int, int> & count);

	void operator()(string & str, int filename);
};

class TTROOT_words :public TTROOT<TTNODE_words>
{
	void insert(unique_ptr <TTNODE_words> & node, int index, string & word, int filename);
public:

	void traverse(unique_ptr <TTNODE_words> & node, vector<char> & buffer, set <vector<char>> & suggestions);

	void operator() (string & str, int filename);
};

string autofill(TTROOT_words & words_txt, string pattern, TTROOT_words & search_history);

map<int,int> TTsearch(TTROOT_files & obj, string & pattern);

bool is_seperator(char ch);

template<typename T>
void insert_chunk(T & root_obj, string & chunk, int filename)
{
	transform(chunk.begin(), chunk.end(), chunk.begin(), [](unsigned char c) { return tolower(c); });

	string temp;
	int curr_index = 0, prev_index = 0;
	for (curr_index = 0; curr_index < chunk.size(); curr_index++)
		if (is_seperator(chunk.at(curr_index)))
		{
			temp = chunk.substr(prev_index, curr_index - prev_index);
			prev_index = curr_index + 1;
			if (temp.size())
				root_obj(temp, filename);
		}
	temp = chunk.substr(prev_index, curr_index - prev_index);
	if (temp.size())
		root_obj(temp, filename);

	/*vector<char> buffer;
	set<vector<char>> suggestions;
	*///root_obj.traverse(root_obj.root, buffer, suggestions);
}
void updateHistory(TTROOT_words & search_history, string & pattern);