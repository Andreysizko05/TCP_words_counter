#include "SocketHandler.h"

SocketHandler::SocketHandler()
{
}

SocketHandler::~SocketHandler()
{
}

std::string SocketHandler::makeAnswerString()
{
	static int reqCounter = 0;
	std::cout << "server answers count = " << ++reqCounter << std::endl;
	return "Hell " + std::to_string(reqCounter);
}

std::string SocketHandler::readUntilDelimiter(tcp::socket& socket, char delimiter)
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

void SocketHandler::receiveFile(tcp::socket& socket)
{
	try {
		// Step 1: Read file size until the newline character
		std::string fileSizeStr = readUntilDelimiter(socket, '\n');
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

		uint8_t inWord = 0;
		uint8_t twoInRow = 0;
		uint16_t currUniInRowCnt = 0;

		std::string currentWord;
		std::vector<std::string> uniWordsListInRow;
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
						}
					}
					else
					{
						inWord = 0;
						twoInRow = 0;
					}

					if (!inWord) // word was recieved
					{
						//unique check//
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
						//unique check//

						//unique in a row check//
						bool isUniqueInRow = true;
						uint16_t duplIndex; // duplicate index
						for (duplIndex = 0; duplIndex < uniWordsListInRow.size(); duplIndex++)
						{
							if (uniWordsListInRow[duplIndex] == currentWord)
							{
								isUniqueInRow = false;
								break;
							}
						}
						if (isUniqueInRow)
						{
							uniWordsListInRow.push_back(currentWord);
							currUniInRowCnt++;
						}
						else
						{
							uniWordsListInRow.erase(uniWordsListInRow.begin(), uniWordsListInRow.begin() + duplIndex);
							currUniInRowCnt = uniWordsListInRow.size();
						}
						if (currUniInRowCnt > UniInRowCnt)
							UniInRowCnt = currUniInRowCnt;
						//unique in a row check//

						currentWord.clear();
						++wordCount;
					}
				}
			}
			else
			{
				if (!inWord) // If the character is a letter, check if we're entering a new word
				{
					inWord = 1;
					twoInRow = 0;
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
		std::cout << "uniWordCount = " << uniWordsList.size() << std::endl;
		std::cout << "UniInRowCnt = " << UniInRowCnt << std::endl;
	}
	catch (std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}