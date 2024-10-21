#include <QApplication>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>
#include <iostream>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    if (argc != 2)
    {
        std::cerr << "Usage: client <host>" << std::endl;
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

    // Buffer for reading data
    QByteArray data;

    // Wait for data from the server
    while (socket.waitForReadyRead(5000))
    { // Wait for data (no more than 5 seconds)
        data.append(socket.readAll()); // Read all available data
    }

    if (data.isEmpty())
    {
        qDebug() << "No data received.";
        return 1;
    }

    // Output the received data
    qDebug() << data.constData();

    // Close the connection
    socket.disconnectFromHost();
    if (socket.state() == QAbstractSocket::ConnectedState)
        socket.waitForDisconnected();

    return 0; // For GUI Apps
    //return a.exec();
}
