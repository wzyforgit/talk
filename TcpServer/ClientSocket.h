#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QTcpSocket>

class ClientSocket : public QTcpSocket
{
    Q_OBJECT

public:
    ClientSocket(QObject *parent=nullptr);

    void setId(quint32 newId){ serverId=newId; }
    quint32 getId(){ return serverId; }

signals:
    void incomingMessage(const QByteArray &message);
    void removeClient(quint32 Id);

private slots:
    void readClient();

private:
    quint32 serverId;
};

#endif // CLIENTSOCKET_H
