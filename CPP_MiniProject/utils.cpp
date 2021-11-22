#include "utils.h"
#include <stack>
// Functions from utils.h


string getautofillInput(string inp)
{
	stack<char> st;
	bool open = false;
	for (int i = 0; i < inp.length(); i++) {
		if (inp[i] == '"') {
			if (open)
				while (!st.empty()) st.pop();
			open = !open;
		}
		else {
			if (open) {
				if (inp[i] == '$') { //next character must be a quote
					if (i + 1 < inp.length() && inp[i + 1] == '"')   //if next character is a quote, skip $
						st.push(inp[++i]);
					else
						st.push(inp[i]);   //if next character is not a quote, then this is a valid $
				}
				else
					st.push(inp[i]);
			}
		}
	}

	string tmp;
	while (!st.empty()) {
		if (is_separator(st.top()))
			break;
		else {
			tmp += st.top();
			st.pop();
		}
	}

	reverse(tmp.begin(), tmp.end());

	return tmp;
}

string set_vec(set<vector<char>> & suggestions)
{
	string str;
	for (auto i: suggestions)
	{
		for (auto ch : i) {
			if (ch == '"')   //if a quote is present, prepend $ to designate escape in suggestions
				str += '$';
			str += ch;
		}
		str += ":";
	}

	if (str.length())
		str.pop_back();
	
	return str;
}

string map_int_int(map<int, int> & words_count)
{
	string str;
	for (auto i : words_count)
		str.append( to_string(i.first) + ";" + to_string(i.second)+ ":");

	return str;
}

bool is_separator(char c)
{
	switch (c) {
	case ' ':
		return true;
	case ',':
		return true;
	case '.':
		return true;
	case '\0':
		return true;
	case '\n':
		return true;
	case '\t':
		return true;
	default:
		return false;
	}
}

bool is_post_asterisk(string s)
{
	size_t occurence_position = s.find_first_of("*");
	if (occurence_position == strlen(s.c_str()))
		return true;
	return false;
}
bool is_pre_asterisk(string s)
{
	size_t occurence_position = s.find_first_of("*");
	if (occurence_position == 0)
		return true;
	return false;
}
bool is_phrase(string s)
{
	// Assumed separators = " ,;.?"
	size_t occurence_position = s.find_first_of(" ,;.?!");
	if (occurence_position != string::npos)
		return true;
	return false;
}

bool check_dir(string dir)
{
	experimental::filesystem::path dir_path(dir);
	return is_directory(dir_path);
}
