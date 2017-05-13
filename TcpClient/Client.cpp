#include "Client.h"
#include <QtDebug>
#include <utility>
#include <QNetworkProxy>

//byte0: messageLen,byte1: idlen,byte2: id,byte3: message

Client::Client(QObject *parent)
    :QTcpSocket(parent)
{
    setProxy(QNetworkProxy::NoProxy);
    connect(this,SIGNAL(readyRead()),this,SLOT(readClient()));
}

QList<Client::messageFormat> Client::readMessage()
{
    QList<messageFormat> temp;
    temp.swap(messages);
    return std::move(temp);
}

void Client::setId(const QString &newId)
{
    id=newId;
}

QString Client::getId()
{
    return id;
}

void Client::sendMessage(const QString &message)
{
    quint8 idLen(id.size());
    auto messageToSend(message.toUtf8());
    quint32 messageLen(6+idLen+messageToSend.size());

    QByteArray dataPack;
    dataPack+=QString().sprintf("%4d%2d",messageLen,idLen).toUtf8();
    dataPack+=id;
    dataPack+=messageToSend;

    write(dataPack);
}

void Client::readClient()
{
    while(bytesAvailable())
    {
        quint32 messageLen(read(4).toInt());
        quint8 idLen(read(2).toInt());
        QByteArray id(read(idLen));
        QByteArray message(read(messageLen-6-idLen));

        messageFormat singleMessage;
        singleMessage.id=id.toStdString().c_str();
        singleMessage.message=message.toStdString().c_str();
        messages.push_back(singleMessage);
    }
}
