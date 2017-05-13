#include <QCoreApplication>
#include <iostream>

#include "Server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server(nullptr);

    if(!server.listen(QHostAddress::Any,2333))
    {
        std::cout<<"port open fail"<<std::endl;
    }
    else
    {
        std::cout<<"port open success"<<std::endl;
    }

    return a.exec();
}
