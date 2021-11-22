#include <iostream>
#include <boost/asio.hpp>
#include "Params.h"
#include "TST.h"
#include "KMP.h"

using namespace boost::asio;
using ip::tcp;

class Translator {
public:
	static Params params;
	static TTROOT_words aft, ht;  //autofill-tree, history-tree
	static TTROOT_files *ft; //files_tree
	static tcp::socket *socket;
	static void read(string message);
	static void autofillWrapper(string &data);
	static void write(std::string &message);
};