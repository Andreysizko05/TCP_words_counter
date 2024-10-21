#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

std::string make_answer_string()
{
    static int reqCounter = 0;
    std::cout << "server answers count = " << ++reqCounter << std::endl;
    return "Hell " + std::to_string(reqCounter);
}

int main()
{
    std::cout << "server is up" << std::endl;
    try
    {
        boost::asio::io_context io_context;

        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 5555));

        for (;;)
        {
            tcp::socket socket(io_context);
            acceptor.accept(socket);

            std::string message = make_answer_string();

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