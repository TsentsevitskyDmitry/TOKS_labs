#include <iostream>
#include <string>
#include "SerialAPI.h"

using namespace std;

void sender();
void reciever();

int main()
{
	string mode;
	cin >> mode;
	if (mode == "s")
		sender();
	else
		reciever();
}

void sender()
{
	SerialPort com;
	com.open("COM2", 9600);
	string data;

	cout << "sender" << endl;
	while (1)
	{
		cout << "Input: ";
		cin >> data;
		com.write(data);
	}
}

void reciever()
{
	SerialPort com;
	com.open("COM3", 9600);
	string data;

	cout << "reader" << endl;
	while (1)
	{
		if (com.try_read(data))
			cout << "Recieve: " << data;

		//Sleep(100);
	}
}
