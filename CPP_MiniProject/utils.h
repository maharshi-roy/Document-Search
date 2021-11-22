#include<string>
#include<iostream>
#include<vector>
#include <map>
#include <set>
#include<fstream>
#include<experimental/filesystem>

using namespace std;

bool check_dir(string dir);
bool is_phrase(string keyword);
bool is_pre_asterisk(string keyword);
bool is_post_asterisk(string s);
bool is_separator(char c);

string set_vec(set<vector<char>> & suggestions);
string map_int_int(map<int, int> & words_count);

string getautofillInput(string searchInput);
