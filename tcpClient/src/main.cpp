#include "MainWindow.h"
#include <QApplication>

#include <qdebug.h>
#include <iostream>
#include <boost/asio.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
