#include "ClientSocket.h"

#include <iostream>
#include <QHostAddress>

ClientSocket::ClientSocket(QObject *parent)
    :QTcpSocket(parent)
{
    connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));
    connect(this,&ClientSocket::disconnected,[&](){
        emit removeClient(serverId);
        std::cout<<"client disconnection address: "
                 <<peerAddress().toString().toStdString()<<" port: "
                 <<peerPort()
                 <<std::endl;
    });
}

void ClientSocket::readClient()
{
    emit incomingMessage(readAll());
}
