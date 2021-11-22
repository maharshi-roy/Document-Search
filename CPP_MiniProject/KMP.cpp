#include "KMP.h"
#define MAXSIZE 2048

void preprocessKMP(string keyword, int key_len, int* lpsuffix)
{
	int len = 0;
	lpsuffix[0] = 0;
	int i = 1;
	while (i < key_len)
	{
		if (keyword.at(i) == keyword.at(len))
		{
			len++;
			lpsuffix[i] = len;
			i++;
		}
		else
		{
			if (len != 0)
			{
				len = lpsuffix[len - 1];
			}
			else
			{
				lpsuffix[i] = 0;
				i++;
			}
		}
	}
}

long kmpsearch(string keyword, string curr_chunk, string prev_chunk)
{
	int key_len = keyword.length();
	int cmp_flag;
	long count = 0;
	string tempstr;
	if (keyword.at(0) == '*' && keyword.at(key_len - 1) == '*')
	{
		cmp_flag = 0;
		key_len -= 2;
		tempstr = keyword.substr(1, key_len);
	}
	else if (keyword.at(0) != '*' && keyword.at(key_len - 1) == '*')
	{
		cmp_flag = 1;
		key_len -= 1;
		tempstr = keyword.substr(0, key_len);
	}
	else if (keyword.at(0) == '*' && keyword.at(key_len - 1) != '*')
	{
		cmp_flag = 2;
		key_len -= 1;
		tempstr = keyword.substr(1, key_len);
	}
	else
	{
		cmp_flag = 3;
		tempstr = keyword.substr(0, key_len);
	}
	keyword = tempstr;
	long chunk_len = curr_chunk.length();
	string new_chunk;
	if (!prev_chunk.empty())
		new_chunk = prev_chunk.substr(chunk_len - key_len - 1, key_len + 1) + curr_chunk;
	else
		new_chunk = curr_chunk;
	long new_chunk_len = new_chunk.length();
	int lpsuffix[MAXSIZE] = { 0 };

	preprocessKMP(keyword, key_len, lpsuffix);

	int i = 0;
	int j = 0;
	while (i < new_chunk_len)
	{
		if (tolower(keyword.at(j)) == tolower(new_chunk.at(i)))
		{
			j++;
			i++;
		}

		if (j == key_len && i < new_chunk_len - 1)
		{
			if (cmp_flag == 3)
			{
				if (is_separator(new_chunk.at(i - j - 1)) && is_separator(new_chunk.at(i - j + key_len)))
				{
					count++;
				}
			}
			else if (cmp_flag == 2)
			{
				if (is_separator(new_chunk.at(i - j + key_len)))
				{
					count++;
				}
			}
			else if (cmp_flag == 1)
			{
				if (is_separator(new_chunk.at(i - j - 1)))
				{
					count++;
				}
			}
			else
			{
				count++;
			}
			j = lpsuffix[j - 1];
		}
		else if (i < new_chunk_len && tolower(keyword.at(j)) != tolower(new_chunk.at(i)))
		{
			if (j != 0)
				j = lpsuffix[j - 1];
			else
				i = i + 1;
		}
	}
	return count;
}