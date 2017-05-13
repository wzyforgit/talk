#include "dialog.h"
#include "Client.h"
#include "tools.h"

#include <QRegExp>
#include <QRegExpValidator>

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QMessageBox>
#include <QTimer>
#include <QDateTime>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent),
      all(new QHBoxLayout),

      settingGroup(new QGroupBox(tr("connect settings"))),
      idLabel(new QLabel("your ID:")),
      idInputBox(new QLineEdit),
      ipInputLabel(new QLabel(tr("IP address:"))),
      ipInputBox(new QLineEdit),
      portInputLabel(new QLabel(tr("port:"))),
      portInputBox(new QLineEdit),
      connectButton(new QPushButton(tr("connect"))),

      messageGroup(new QGroupBox(tr("message box"))),
      messageBox(new QTextEdit),

      sendinggroup(new QGroupBox(tr("send box"))),
      sendingBox(new QTextEdit),
      sendingButton(new QPushButton(tr("send"))),

      client(new Client(this))
{
    createSettingBox();
    createMessageBox();
    createSendingBox();

    auto rightLayout=new QVBoxLayout;
    rightLayout->addWidget(messageGroup);
    rightLayout->addWidget(sendinggroup);

    all->addWidget(settingGroup);
    all->addLayout(rightLayout);

    setLayout(all);
    setFixedSize(755,610);
}

Dialog::~Dialog()
{
}

void Dialog::enableSetting(bool enabled)
{
    if(enabled)
    {
        sendingButton->setDisabled(true);
        ipInputBox->setEnabled(true);
        portInputBox->setEnabled(true);
        idInputBox->setEnabled(true);
        connectButton->setText("connect");
    }
    else
    {
        sendingButton->setDisabled(false);
        ipInputBox->setEnabled(false);
        portInputBox->setEnabled(false);
        idInputBox->setEnabled(false);
        connectButton->setText("disconnect");
    }
}

void Dialog::createSettingBox()
{
    QRegExp regexp("((([0-9])|([1-9][0-9])|([1][0-9][0-9])|([2][0-5][0-5]))\\.){3}"
                   "(([0-9])|([1-9][0-9])|([1][0-9][0-9])|([2][0-5][0-5]))");
    ipInputBox->setValidator(new QRegExpValidator(regexp));
    portInputBox->setValidator(new QRegExpValidator(QRegExp("[0-9]{0,5}")));
    idInputBox->setValidator(new QRegExpValidator(QRegExp(".{1,16}")));

    connect(connectButton,&QPushButton::clicked,[=](){
        if(sendingButton->isEnabled())
        {
            client->disconnectFromHost();
        }
        else
        {
            if(idInputBox->text().isEmpty())
            {
                QMessageBox::critical(this,tr("error"),tr("please set your id"));
                return;
            }

            auto ipAddress=ipInputBox->text();
            auto port=portInputBox->text().toUShort();
            if(regexp.exactMatch(ipAddress)&&port<=60000&&port!=0)
            {
                client->setId(idInputBox->text());
                client->connectToHost(ipAddress,portInputBox->text().toUShort());
                addTextToTextEdit(messageBox,"trying to connect to the server\n");
            }
            else
            {
                QMessageBox::critical(this,tr("error"),tr("IP address format or port number incorrect"));
            }
        }
    });

    connect(client,static_cast<void(QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error),
            [=](QAbstractSocket::SocketError socketError){
        QString prompt;
        switch(socketError)
        {
        default:break;
        case QAbstractSocket::ConnectionRefusedError:
            prompt="connect fail:";
            break;
        case QAbstractSocket::OperationError:
            addTextToTextEdit(messageBox,"busy:"+client->errorString()+"\n");
            return;
        }
        addTextToTextEdit(messageBox,prompt+client->errorString()+"\n");
        enableSetting(true);
    });

    connect(client,&Client::connected,[=](){
        addTextToTextEdit(messageBox,"connection succeeded\n");
        enableSetting(false);
    });

    connect(client,&Client::disconnected,[=](){
        addTextToTextEdit(messageBox,"client is already offline\n");
        enableSetting(true);
    });

    auto settingBox=new QVBoxLayout;
    settingBox->addWidget(idLabel);
    settingBox->addWidget(idInputBox);
    settingBox->addWidget(ipInputLabel);
    settingBox->addWidget(ipInputBox);
    settingBox->addWidget(portInputLabel);
    settingBox->addWidget(portInputBox);
    settingBox->addWidget(connectButton);
    settingBox->addStretch();

    settingGroup->setLayout(settingBox);
    settingGroup->setMaximumWidth(120);
}

void Dialog::createMessageBox()
{
    messageBox->setReadOnly(true);
    auto readMessageTimer=new QTimer(this);

    connect(readMessageTimer,&QTimer::timeout,[=](){
        auto messageList=client->readMessage();
        if(!messageList.isEmpty())
        {
            QString result;
            for(auto &eachMessage:messageList)
            {
                QString temp;
                temp+=eachMessage.id+" ";
                temp+=QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss\n");
                temp+=eachMessage.message+"\n";
                result+=temp;
            }
            addTextToTextEdit(messageBox,result);
        }
    });
    readMessageTimer->start(10);

    auto messageLayout=new QHBoxLayout;
    messageLayout->addWidget(messageBox);
    messageGroup->setLayout(messageLayout);
}

void Dialog::createSendingBox()
{
    sendingButton->setDisabled(true);

    connect(sendingButton,&QPushButton::clicked,[=](){
        client->sendMessage(sendingBox->toPlainText());
    });

    auto sendingLayout=new QVBoxLayout;
    sendingLayout->addWidget(sendingBox);
    sendingLayout->addWidget(sendingButton,0,Qt::AlignRight);
    sendinggroup->setLayout(sendingLayout);
    sendinggroup->setMaximumHeight(200);
}
