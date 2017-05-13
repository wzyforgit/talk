#include "Server.h"
#include "ClientSocket.h"

#include <iostream>
#include <algorithm>

Server::Server(QObject *parent)
    :QTcpServer(parent)
{
}

void Server::sendMessageToAllClient(const QByteArray &message)
{
    for(auto &eachSocket:clients)
    {
        eachSocket->write(message);
    }
}

void Server::removeSocket(quint32 id)
{
    std::remove_if(clients.begin(),clients.end(),[id](ClientSocket *eachClient){
        return eachClient->getId()==id;
    });
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    auto socket=new ClientSocket(this);
    socket->setSocketDescriptor(socketDescriptor);

    std::cout<<"incoming connection address: "
             <<socket->peerAddress().toString().toStdString()<<" port: "
             <<socket->peerPort()
             <<std::endl;

    connect(socket,SIGNAL(incomingMessage(QByteArray)),this,SLOT(sendMessageToAllClient(QByteArray)));
    connect(socket,SIGNAL(removeClient(quint32)),this,SLOT(removeSocket(quint32)));

    socket->setId(idKey++);
    clients.push_back(socket);
}
