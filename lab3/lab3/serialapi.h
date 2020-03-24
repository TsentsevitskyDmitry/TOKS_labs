#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <QString>
#include <windows.h>

#define CRC_POLY 0x04C11DB7

using namespace std;

class SerialPort
{
private:
    static const int TIMEOUT = 100;

    HANDLE serialHandle;
    int currentBaud;

    string uncapsulateData(string data);
    string decapsulateData(string data);

public:
    SerialPort() : serialHandle(0) ,currentBaud(0) {}
    ~SerialPort();
    bool open(string port, int baud);
    void close();
    bool connected();
    int getBaud();

    bool write(string& data);
    bool read(string& buff);
    string addCRC(string& data);
    bool checkCRC(string& data);
    string cutCRC(string& data);
};

