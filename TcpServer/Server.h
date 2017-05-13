#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

class ClientSocket;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent=nullptr);

private slots:
    void sendMessageToAllClient(const QByteArray &message);
    void removeSocket(quint32 id);

private:
    void incomingConnection(qintptr socketDescriptor) override;

    QList<ClientSocket*> clients;
    quint32 idKey=0;
};

#endif // SERVER_H
