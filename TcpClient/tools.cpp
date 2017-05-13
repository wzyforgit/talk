#include "tools.h"

#include <QTextCodec>

QByteArray transcoding(QByteArray str,const char *srcCode,const char *dstCode)
{
    QTextCodec *codec=QTextCodec::codecForName(srcCode);
    QString temp=codec->toUnicode(str);

    QTextCodec *decodec=QTextCodec::codecForName(dstCode);
    return (decodec->fromUnicode(temp));
}

void addTextToTextEdit(QTextEdit *textEdit,const QString &text)
{
    textEdit->moveCursor(QTextCursor::End);
    textEdit->insertPlainText(text.toUtf8());
    textEdit->moveCursor(QTextCursor::End);
}
