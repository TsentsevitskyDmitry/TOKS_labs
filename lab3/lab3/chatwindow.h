#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QObject>
#include <QWidget>
#include <QTextBrowser>
#include <string>

using namespace std;

class ChatWindow
{
private:
    QTextBrowser* textBrowser;
    QString GetBitString(string& data);

public:
    ChatWindow(QTextBrowser* textBrowser);
    void appendSendedCRC(string origin, string crc);
    void appendRecievedCRC(bool isValid, string origin, string crc);
};

#endif // CHATWINDOW_H
