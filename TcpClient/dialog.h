#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class QGroupBox;
class QLineEdit;
class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QGridLayout;
class QTextEdit;
class Client;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void enableSetting(bool enabled);

private:
    QHBoxLayout *all;

    void createSettingBox();
    QGroupBox *settingGroup;
    QLabel *idLabel;
    QLineEdit *idInputBox;
    QLabel *ipInputLabel;
    QLineEdit *ipInputBox;
    QLabel *portInputLabel;
    QLineEdit *portInputBox;
    QPushButton *connectButton;

    void createMessageBox();
    QGroupBox *messageGroup;
    QTextEdit *messageBox;

    void createSendingBox();
    QGroupBox *sendinggroup;
    QTextEdit *sendingBox;
    QPushButton *sendingButton;

    Client *client;
};

#endif // DIALOG_H
