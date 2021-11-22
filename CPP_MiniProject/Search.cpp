#include <iostream>
#include "FileHandler.h"
#include "Translator.h"


Params Translator::params;
tcp::socket *Translator::socket;
TTROOT_files *Translator::ft;
TTROOT_words Translator::aft;
TTROOT_words Translator::ht;


void read_data(tcp::socket &socket) {
	while (true) {
		try {
			boost::asio::streambuf buf;
			boost::system::error_code ec;
			boost::asio::read_until(socket, buf, '\n');

			std::string message = boost::asio::buffer_cast<const char*>(buf.data());
			Translator::read(message);
		}
		catch (std::exception e) {
			socket.close();
			exit(0);
		}
	}
}

int main()
{
	cout << "Preprocessing Autofill\n";
	fileHandling fh("../vocabulary/words.txt");
	vector<char> buffer(1024, '\0');

	while (fh.readInChunks(buffer)) {
		string buffer_str(buffer.data());
		insert_chunk(Translator::aft, buffer_str, -1);
	}
	
	//Threading & Socket Initialization
	boost::asio::io_service io_service;
	tcp::socket socket(io_service);
	tcp::acceptor acceptor(io_service, tcp::endpoint(tcp::v4(), 1234));
	
	cout << "Waiting for Connections\n";

	acceptor.accept(socket);

	cout << "Client Connected\n";

	Translator::socket = &socket;

	std::thread readThread(read_data, std::ref(socket));

	//Thread joined
	readThread.join();

	std::cout << "Program Terminated\n";

	return 0;
}