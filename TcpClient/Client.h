#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client : public QTcpSocket
{
    Q_OBJECT

public:
    struct messageFormat
    {
        QString id;
        QString message;
    };

    Client(QObject *parent=nullptr);
    QList<messageFormat> readMessage();

public slots:
    void setId(const QString &newId);
    QString getId();
    void sendMessage(const QString &message);

private slots:
    void readClient();

private:
    QString id;
    QList<messageFormat> messages;
};

#endif // CLIENT_H
