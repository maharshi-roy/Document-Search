#include "utils.h"
#include "Params.h"
//#include "Translator.h"
#include "FileHandler.h"
#include <boost/algorithm/string/trim.hpp>
#define CHUNKSIZE 1024

// Functions from FileHandler.h
int fileHandling::readInChunks(vector<char> &buffer)
{
	fill(buffer.begin(), buffer.end(), '\0');
	if (fin.eof())
		return 0;

	fin.seekg(position, ios::beg); // Go to position position
	fin.read(buffer.data(), buffer.size());
	position = static_cast<int>(fin.tellg()); // Marks the next position to be read
	int delete_count = 0;
	for (auto it = buffer.rbegin(); it != buffer.rend() && !fin.eof(); it++)
	{
		if (is_separator(*it))
			break;
		position--; // Decrement position till separator is found
		delete_count++;
		//Add check if word size is greater than 1024
		//Remove characters from buffer
	}
	fill(buffer.end() - delete_count, buffer.end(), '\0');
	return 1;
}

// Functions from Params.h
void Params::setDirectory(string dirPath, string &error)
{
	Dir = dirPath;
	if (!check_dir(dirPath))
	{
		//cout << "Invalid Dir" << endl;
		error = "Invalid Directory";
		return;
	}

	experimental::filesystem::path dir_path(Dir);
	for (const auto & entry : experimental::filesystem::recursive_directory_iterator(dir_path))
	{
		string dir_path_str = entry.path().string();

		if (dir_path_str.substr(dir_path_str.find_last_of(".") + 1) == "txt" && !is_directory(entry.path()))
			FilesInDir.push_back(entry.path().string());
	}
}

// Functions from Params.h
//append space after searchInput
void Params::setKeywords_Ops(string searchInput, string &error)
{
	searchInput += " ";
	//trim whitespace from beginning and end

	boost::algorithm::trim_left(searchInput);

	size_t prevOccurence = searchInput.find('\"');
	if (prevOccurence != 0)
	{
		error = "Opening \" missing";
		return;
		//Translator::write(message);
		//cout << "Invalid Usage : Please add quotes to input keyword";
		//exit(0);
	}
	size_t nextOccurence,dollarOccurence = searchInput.find('$');
	bool flagKeyword = true; //Used to flag if keyword is being read
	bool flagfirst = false;
	if (searchInput[prevOccurence] == '\"' )
		prevOccurence++;

	while (prevOccurence != string::npos)
	{
		size_t deleteCount = 0;
		//Remove trailing whitespace or quote
		if (searchInput[prevOccurence] == ' ' || searchInput[prevOccurence] == '\"' && flagfirst)
			prevOccurence++;
		else if (flagfirst == false)
			flagfirst = true;
		else
		{
			error = "Invalid Operator";
			return;
		}

		// If a keyword is being read, read the next quote '\"'
		if (flagKeyword)
		{
			// Eliminate all whitespaces if a keyword is not being read
			while (prevOccurence < searchInput.size() && searchInput[prevOccurence] == ' ')
				prevOccurence++;

			//// Remove quote present before word
			//if (searchInput[prevOccurence] == '\"' && !flagfirst)
			//	prevOccurence++;
		
		
			nextOccurence = searchInput.find("\" ", prevOccurence); //Match exact pattern of '\" ' 

			if (nextOccurence == string::npos)//if no quote is found break
				break;

			while (dollarOccurence < nextOccurence) //find $ in current string and remove them appropriately
			{
				searchInput.erase(dollarOccurence,1);
				dollarOccurence = searchInput.find('$', dollarOccurence + 1);
				deleteCount++;
			}

			flagKeyword = false;
			string keyw = searchInput.substr(prevOccurence, nextOccurence - prevOccurence - deleteCount);

			transform(keyw.begin(), keyw.end(), keyw.begin(), [](unsigned char c) { return tolower(c); });
			if(keyw.size())
				Keywords.push_back(keyw);
			else
			{
				error = "Search keyword can't be empty";
				return;
			}
		}

		// If an operator is being read, read the next quote " "
		else
		{
			// Eliminate all whitespaces if a keyword is not being read
			while (prevOccurence < searchInput.size() && searchInput[prevOccurence] == ' ')
				prevOccurence++;

			nextOccurence = searchInput.find(' ', prevOccurence);

			//if no space is found break, marks end of query
			if (nextOccurence == string::npos)
				break;

			flagKeyword = true;

			string op = searchInput.substr(prevOccurence, nextOccurence - prevOccurence);

			if (strcmp(op.c_str(), "AND") == 0 || strcmp(op.c_str(), "OR") == 0)
				ops.push_back(op);

			else
			{
				error = "Invalid Operator";
				return;
			}

			while (nextOccurence < searchInput.size() && searchInput[nextOccurence] == ' ')
				nextOccurence++;
		}
		prevOccurence = nextOccurence;
	}
	if (flagKeyword) //If keyword was not completely read : input is invalid
	{
		error = "Closing \" Missing";
		return;
	}

	for (auto x : Keywords)
		cout << x << " ";
	cout << endl;
}

