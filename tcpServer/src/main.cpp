#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include <fstream>

using boost::asio::ip::tcp;

std::string make_answer_string()
{
    static int reqCounter = 0;
    std::cout << "server answers count = " << ++reqCounter << std::endl;
    return "Hell " + std::to_string(reqCounter);
}

std::string read_until_delimiter(tcp::socket& socket, char delimiter)
{
    std::string result;
    char buffer;

    while (true) {
        boost::asio::read(socket, boost::asio::buffer(&buffer, 1));
        if (buffer == delimiter) {
            break;
        }
        result += buffer;
    }

    return result;
}
void receive_file(tcp::socket& socket)
{
    try {
        // Step 1: Read file size until the newline character
        std::string fileSizeStr = read_until_delimiter(socket, '\n');
        std::uint64_t fileSize = std::stoull(fileSizeStr);
        std::cout << "File size: " << fileSize << " bytes" << std::endl;

        // Step 2: Read file data in chunks
        std::vector<char> fileData;
        fileData.reserve(fileSize); // Reserve space for the file

        std::size_t bytesReceived = 0;
        while (bytesReceived < fileSize)
        {
            std::vector<char> chunk(1024); // Reading in chunks of 1024 bytes
            std::size_t bytesToRead = std::min(static_cast<std::size_t>(fileSize - bytesReceived), chunk.size());

            std::size_t bytesRead = socket.read_some(boost::asio::buffer(chunk, bytesToRead));
            fileData.insert(fileData.end(), chunk.begin(), chunk.begin() + bytesRead);
            bytesReceived += bytesRead;

            std::cout << "Received " << bytesReceived << " of " << fileSize << " bytes" << std::endl;
        }

        std::cout << "File successfully received:" << std::endl;

        int wordCount = 0;
        uint8_t inWord = 0;
        uint8_t twoInRow = 0;

        std::string currentWord;
        std::vector<std::string> uniWordsList;
        for (char c : fileData)
        {
            if (!std::isalpha(static_cast<unsigned char>(c)))
            {
                if (inWord) // If the current character is not a letter, end the word if we're in one
                {
                    if (c == '\'')
                    {
                        if (twoInRow++)
                        {
                            inWord = 0;

                            twoInRow = 0;
                            bool isUnique = true;
                            for (const std::string& str : uniWordsList)
                            {
                                if (str == currentWord)
                                {
                                    isUnique = false;
                                    break;
                                }
                            }
                            if (isUnique)
                                uniWordsList.push_back(currentWord);
                        }
                    }
                    else
                    {
                        inWord = 0;

                        twoInRow = 0;
                        bool isUnique = true;
                        for (const std::string& str : uniWordsList)
                        {
                            if (str == currentWord)
                            {
                                isUnique = false;
                                break;
                            }
                        }
                        if (isUnique)
                            uniWordsList.push_back(currentWord);
                    }
                }
            }
            else
            {
                if (!inWord) // If the character is a letter, check if we're entering a new word
                {
                    inWord = 1;

                    currentWord.clear();
                    twoInRow = 0;
                    ++wordCount;
                }
                currentWord += c;
            }
            std::cout << c;
        }
        std::cout << std::endl << std::endl;

        std::cout << "uniWordsList:" << std::endl;
        for (std::string str : uniWordsList)
            std::cout << str << std::endl;
        std::cout << std::endl << "wordCount = " << wordCount << std::endl;
        std::cout <<  "uniWordCount = " << uniWordsList.size() << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
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

            receive_file(socket);

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