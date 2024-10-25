#include <iostream>
#include <string>
#include <boost/asio.hpp>

#include "SocketHandler.h"

int main()
{
	SocketHandler socketHandler;
	std::cout << "server is up" << std::endl;
	try
	{
		boost::asio::io_context io_context;

		tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5555));

		for (;;)
		{
			tcp::socket socket(io_context);
			acceptor.accept(socket);

			socketHandler.receiveFile(socket);

			std::string message = socketHandler.makeAnswerString();
			boost::system::error_code ignored_error;
			boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	return 0;
}