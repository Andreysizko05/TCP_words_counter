#include <QApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <QFile>
#include <QDataStream>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <filepath>" << std::endl;
        return 1;
    }
    QString host = argv[1]; // Get the host from arguments

    // Create the socket
    QTcpSocket socket;

    // Connect to the server on port 5555 (custom port)
    socket.connectToHost(QHostAddress(host), 5555);

    // Wait for the connection (no more than 5 seconds)
    if (!socket.waitForConnected(5000))
    {
        qDebug() << "Connection error:" << socket.errorString();
        return 1;
    }
    qDebug() << "Connection established";

    std::string input(argv[2]);
    QString fullpath;
    std::string toRemove = "programBinDir";  // Substring to remove
    size_t firstSlashPos = input.find('/');

    if (firstSlashPos != std::string::npos && input.substr(0, firstSlashPos) == toRemove)
    {
        input.erase(0, firstSlashPos);
        fullpath = QCoreApplication::applicationDirPath() + QString::fromStdString(input);
        qDebug() << "string: \"programBinDir\" has been found";
        qDebug() << "Resulting path: " << fullpath;
    }
    else
    {
        fullpath = QString::fromStdString(input);
        qDebug() << "Resulting path: " << fullpath;
    }

    QFile file(fullpath);
    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Could not open file";
        return 1;
    }

    QDataStream stream(&socket);
    QByteArray fileData = file.readAll();
    qint64  fileSize = file.size();

    // Send file size
    QByteArray fileSizeData = QString::number(file.size()).toUtf8();
    socket.write(fileSizeData);
    socket.write("\n");

    // Send file content
    qDebug() << "File content:\n" << fileData;
    stream.writeRawData(fileData.data(), fileData.size());  // Send the file content

    socket.flush();
    socket.waitForBytesWritten(3000);
    qDebug() << "File sent successfully.";

    // Wait for response from server
    QByteArray responseData;
    while (socket.waitForReadyRead(5000))
    {
        responseData.append(socket.readAll());
    }

    if (responseData.isEmpty()) {
        qDebug() << "No data received.";
        return 1;
    }

    // Output the received data
    qDebug() << "Received from server: " << responseData.constData();

    // Close the connection
    socket.disconnectFromHost();
    if (socket.state() == QAbstractSocket::ConnectedState)
        socket.waitForDisconnected();

    return 0; // For GUI Apps
	//return a.exec();
}
