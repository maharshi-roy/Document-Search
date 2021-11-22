#include "Translator.h"
#include "FileHandler.h"
//#include <boost/chrono.hpp>

using std::string;
using namespace boost::asio;
using ip::tcp;

void Translator::read(string message) {
	//Pop out \n
	message.pop_back();
		
	//Get everything after pipe delimiter (|)
	string data = message.substr(3);
	//Get everything before pipe delimiter
	string instruction = message.substr(0, 2);
	string error;

	if (instruction == "DI") {//Directory Change Input
		//destroy tree
		delete ft;

		params.clearFilesInDir_Dir();

		params.setDirectory(data, error);
		if (error.length()) {
			error = "ERR|" + error;
			write(error);
			return;
		}
		
		//build tree
		ft = new TTROOT_files();
		int index = 0;

		for (auto file : params.FilesInDir) {
			fileHandling fh(file);
			vector<char> buffer(1024, '\0');

			while (fh.readInChunks(buffer)) {
				string buffer_str(buffer.data());
				insert_chunk(*ft, buffer_str, index);
			}
			index++;
		}

		//std::this_thread::sleep_for(std::chrono::milliseconds(50000));
		message = "DIC|" + data;
		write(message);
	}
	else if (instruction == "KA") {  //Keyword Autofill
		data = getautofillInput(data);

		//cout << data << endl;
		transform(data.begin(), data.end(), data.begin(), [](unsigned char c) { return tolower(c); });

		string suggestions = "KAC|";

		if (data.length()) {
			suggestions += data + ";" + autofill(aft, data, ht);
		}

		write(suggestions);
	}
	else if (instruction == "KI") 
	{ 
		//Keyword Change Input
		string collation = data.substr(0, 1);
		
		data = data.substr(2);
		//string result_v;
		params.clearKeywords_Ops();
		params.setKeywords_Ops(data, error);

		if (error.length()) {
			error = "ERR|" + error;
			write(error);
			return;
		}

		int key_index = 0;
		vector<int> printFile(params.FilesInDir.size(),0);

		map<int, vector<pair<int, int>>> vertical; // map<fileid, vector<keyid,count>>
		vector<map<int, int>> horizontal_map; // vector<map<fileid,count>> :: index = keyid

		for (auto words : params.Keywords) 
		{
			/*Update history*/
			updateHistory(ht, words);
			cout << "History Updated" << endl;
			map <int, int> horizontal; // fileid, count
			if (!is_phrase(words) && !is_pre_asterisk(words) ) { //check if-phrase
				horizontal=TTsearch(*ft, words);
			}
			else { //call KMP
				string previous(1024, '\0');
				string current(1024, ' ');
				int index_files = 0;
				for (auto file: params.FilesInDir) {
					fileHandling fh(file);
					vector<char> buffer(1024, '\0');
					int kmp_count = 0;

					while (fh.readInChunks(buffer)) {
						string buffer_str(buffer.data());
						buffer_str.resize(1024);

						if (previous[0] == '\0') {
							previous = buffer_str;
							kmp_count += kmpsearch(words, previous, current);
						}
						else {
							previous = current;
							current = buffer_str;
							kmp_count +=  kmpsearch(words, current, previous);
						}
					}
					if(kmp_count != 0)
						horizontal[index_files] = kmp_count;
					index_files++;
				}
			}
			horizontal_map.push_back(horizontal);

			
			map<int, int>::iterator it;

			//Populate vertical map
			for (it = horizontal.begin(); it != horizontal.end(); it++)
				vertical[(*it).first].push_back(make_pair(key_index, (*it).second));
			
			key_index++;
			//result_h += "$" + map_int_int(horizontal);
		}
		//int verticalIndex = 0;
		//map<int, vector<pair<int, int>>>::iterator mapit = vertical.begin();
		//for (; mapit != vertical.end(); mapit++) //get print flags
		for(auto mapit : vertical)
		{
			//vector<pair<int, int>>::iterator keywordCount;
			vector<int> val(params.Keywords.size(), 0);
			for (auto keyCount : mapit.second)
				val[keyCount.first] = 1;

			int flag = val[0];
			int valIndex = 1;
			for (auto op : params.ops)
			{
				if (op[0] == 'A')
					flag &= val[valIndex] ;
				else
					flag |= val[valIndex];

				valIndex++;
			}

			printFile[mapit.first] = flag;
			//verticalIndex++;
		}


		string result_v = "KIC|V|" ;
		int printIndex = 0;
		for (auto print : printFile)
		{
			if (print == 1) 
			{
				result_v += "$" + params.FilesInDir[printIndex];	/*.substr(params.FilesInDir[printIndex].rfind('/'))*/
				for (auto pairit : vertical[printIndex])
					result_v += "@" + params.Keywords[(pairit.first)] + ";" + to_string(pairit.second);
			}
				
			printIndex++;
		}
		string result_h = "KIC|H|";
		int keyid_h = 0;
		for (auto hmap : horizontal_map)
		{
			if(hmap.size())
				result_h += "$" + params.Keywords[keyid_h];
			for (auto pairit : hmap) // pairit :: fileid,count
				if (printFile[pairit.first] == 1)
					result_h += "@" + params.FilesInDir[pairit.first] + ";" + to_string(pairit.second);

			keyid_h++;
		}

		if (collation == "H")
			write(result_h);
		else
			write(result_v);
	}
}

void Translator::autofillWrapper(string &query) {
	//
	//find the first space or quotes from the end of the current string
}

void Translator::write(std::string &message) {
	boost::system::error_code ec;
	//cout << message << endl;

	try {
		boost::asio::write(*socket, boost::asio::buffer(message));
	}
	catch (std::exception e) {
		(*socket).close();
		//terminate = true;
	}
}
