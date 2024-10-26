#ifndef SOCKETHANDLER
#define SOCKETHANDLER

#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>
#include <nlohmann/json.hpp>

using boost::asio::ip::tcp;
using json = nlohmann::json;

class SocketHandler
{
public:
	SocketHandler();
	~SocketHandler();

	std::string makeAnswerString();
	std::string readUntilDelimiter(tcp::socket& socket, char delimiter);
	void receiveFile(tcp::socket& socket);

private:
	int wordCount = 0;
	std::vector<std::string> uniWordsList;
	uint16_t UniInRowCnt = 0;

};
#endif // SOCKETHANDLER