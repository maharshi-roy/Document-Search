#include "utils.h"

class Params
{
	/*
		Class to maintain params in current session
		Keywords : Stores the list of keywords to be searched in current directory
		FilesInDir : List of files in directory and subdirectories
		ops : List of logical operations to be done on keywords
		Dir : Path of directory to be searched in
	*/
public:
	Params() {}
	vector<string> Keywords;
	vector<string> FilesInDir;
	vector<string> ops;
	string Dir;
	void setDirectory(string dirPath, string &error);
	void setKeywords_Ops(string searchInput, string &error);
	void clearKeywords_Ops() { Keywords.clear(); ops.clear(); }
	void clearFilesInDir_Dir() { Dir.clear(); FilesInDir.clear(); }
};
