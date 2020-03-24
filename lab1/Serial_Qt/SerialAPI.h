#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

class SerialPort
{
private:
	static const int TIMEOUT = 100;

	HANDLE serialHandle;
	int currentBaud;

public:
	SerialPort() : currentBaud(0), serialHandle(0) {}
	~SerialPort();
    bool open(string port, int baud);
    void close();
    bool connected();
    int getBaud();
    bool write(string& data);
    bool try_read(string& buff);
};

