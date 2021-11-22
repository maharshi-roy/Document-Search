#include <string>
#include <iostream>
#include "utils.h"
using namespace std;

void preprocessKMP(string keyword, int key_len, int* lpsuffix);
long kmpsearch(string keyword, string curr_chunk, string prev_chunk);