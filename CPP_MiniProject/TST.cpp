#include "TST.h"

template <typename T>
unique_ptr <T> & TTROOT<T>::get_top(unique_ptr <T> & node, int & count, string & pattern)
{
	if (node == NULL)
		return node;
	if (count == pattern.size())
		return node;
	if (pattern[count] < node->data)
		return get_top(node->left, count, pattern);
	else if (pattern[count] > node->data)
		return get_top(node->right, count, pattern);
	else
	{
		count++;
		return get_top(node->middle, count, pattern);
	}
}

void TTROOT_files::insert(unique_ptr <TTNODE_files> & node, int index, string & word, int  filename)
{
	if (node == NULL)
	{
		unique_ptr <TTNODE_files> temp = make_unique<TTNODE_files>(word[index]);
		node = move(temp);
	}

	if (word[index] < node->data)
		insert(node->left, index, word, filename);

	else if (word[index] > node->data)
		insert(node->right, index, word, filename);

	else
	{
		if (word[index + 1] == '\0')
			node->word_counts[filename]++;
		else
			insert(node->middle, index + 1, word, filename);
	}
}

void TTROOT_files::traverse(unique_ptr <TTNODE_files> & node, map<int, int> &count)
{
	if (node != NULL)
	{
		traverse(node->left, count);
		map<int, int>::iterator i;
		for (i = (node->word_counts).begin(); i != (node->word_counts).end(); i++)
			count[i->first] += i->second;
		traverse(node->middle, count);
		traverse(node->right, count);
	}
}

int TTROOT_files::search(unique_ptr <TTNODE_files> & node, int index, string & word, map<int, int> & count)
{
	if (node == NULL)
		return 0;

	if (word[index] < node->data)
		return search(node->left, index, word, count);

	else if (word[index] > node->data)
		return search(node->right, index, word, count);

	else
	{
		if (index + 1 == word.size())
		{
			for (auto i : node->word_counts)
				count[i.first] = i.second;
			return 1;
		}
		else
			return search(node->middle, index + 1, word, count);
	}

}

void TTROOT_files::operator()(string & str, int filename)
{
	this->insert(root, 0, str, filename);
}

void TTROOT_words::insert(unique_ptr <TTNODE_words> & node, int index, string & word, int filename)
{
	if (node == NULL)
	{
		unique_ptr <TTNODE_words> temp = make_unique<TTNODE_words>(word[index]);
		node = move(temp);
	}

	if (word[index] < node->data)
		insert(node->left, index, word, filename);

	else if (word[index] > node->data)
		insert(node->right, index, word, filename);

	else
	{
		if (index + 1 == word.size())
			node->isleaf = 1;
		else
			insert(node->middle, index + 1, word, filename);
	}
}

void TTROOT_words::traverse(unique_ptr <TTNODE_words> & node, vector<char> & buffer, set <vector<char>> & suggestions)
{
	if (suggestions.size() == 10)
		return;
	if (node != NULL)
	{
		traverse(node->left, buffer, suggestions);
		buffer.push_back(node->data);
		if (node->isleaf == 1)
		{
			suggestions.insert(buffer);
			if (suggestions.size() == 10)
				return;
		}
		traverse(node->middle, buffer, suggestions);
		buffer.pop_back();
		traverse(node->right, buffer, suggestions);
	}
	return;
}

void TTROOT_words::operator() (string & str, int filename)
{
	this->insert(root, 0, str,filename);
}

string autofill(TTROOT_words & words_txt, string pattern, TTROOT_words & search_history)
{
	int count = 0;
	set <vector<char>> suggestions;
	vector<char> buffer, buffer2;

	unique_ptr <TTNODE_words> & new_root = search_history.get_top(search_history.root, count, pattern);
	search_history.traverse(new_root, buffer, suggestions);

	count = 0;

	unique_ptr <TTNODE_words> & new_root2 = words_txt.get_top(words_txt.root, count, pattern);
	words_txt.traverse(new_root2, buffer2, suggestions);

	return set_vec(suggestions);
}

map<int,int> TTsearch(TTROOT_files & obj, string & pattern)
{
	int co = 0;
	map<int, int> count;
	if (pattern[pattern.size() - 1] == '*')
	{
		string temp = pattern.substr(0, pattern.size() - 1);
		obj.search(obj.root, 0, temp, count);
		unique_ptr <TTNODE_files> & new_root = obj.get_top(obj.root, co, temp);
		obj.traverse(new_root, count);
	}
	else
		obj.search(obj.root, 0, pattern, count);
	return count;
}

bool is_seperator(char ch)
{
	if (ch == '.' || ch == ',' || ch == ';' || ch == '\n' || ch == '\t' || ch == ' ' || ch == '\0')
		return true;
	else
		return false;
}
void updateHistory(TTROOT_words & search_history, string & chunk) // chunk = searchPattern
{
	string temp;
	int curr_index = 0, prev_index = 0;
	for (curr_index = 0; curr_index < chunk.size(); curr_index++)
		if (is_seperator(chunk.at(curr_index)))
		{
			temp = chunk.substr(prev_index, curr_index - prev_index);
			prev_index = curr_index + 1;
			if (temp.size())
				search_history(temp,-1);
				//search_history.insert(search_history.root, 0, temp,-1);
		}
	temp = chunk.substr(prev_index, curr_index - prev_index);
	if (temp.size())
		search_history(temp, -1);
		//search_history.insert(search_history.root, 0, temp,-1);
}
