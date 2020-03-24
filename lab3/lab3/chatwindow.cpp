#include "chatwindow.h"
#include <bitset>
#include <sstream>

QString ChatWindow::GetBitString(string &data)
{
    stringstream bits;
    for(int i = 0; i < data.length(); ++i){
        bitset<8> x(data[i]);
        bits << x;
    }

    return QString::fromStdString(bits.str());
}

ChatWindow::ChatWindow(QTextBrowser* textBrowser)
{
    this->textBrowser = textBrowser;
}

void ChatWindow::appendSendedCRC(string origin, string crc)
{
    textBrowser->setTextColor(Qt::gray);
    textBrowser->append("Sended: " + QString::fromStdString(origin));
    textBrowser->append("CRC: " + GetBitString(crc));
    textBrowser->setTextColor(Qt::black); 
}

void ChatWindow::appendRecievedCRC(bool isValid, string origin, string crc)
{
    textBrowser->append("Recieve: " + QString::fromStdString(origin));
    QString valid = isValid ? "Yes" : "No";
    textBrowser->append("Data valid: " + valid);
    textBrowser->append("CRC: " + GetBitString(crc));

}
