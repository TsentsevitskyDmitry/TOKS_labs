#include "SerialAPI.h"

SerialPort::~SerialPort()
{
	close();
}

int SerialPort::open(string port, int baud)
{
	close();
	currentBaud = baud;

	//открытие порта
	serialHandle = CreateFile(
		port.c_str(), //наименование порта, например COM1
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (serialHandle == (HANDLE)-1) {
		serialHandle = 0;
		throw exception();
	}
	//устанавливаем размеры входного и выходного буферов
	SetupComm(serialHandle, 1500, 1500);
	//настраиваем тайм-ауты для текущего устройства
	COMMTIMEOUTS CommTimeOuts;
	CommTimeOuts.ReadIntervalTimeout = TIMEOUT;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
	CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
	CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
	CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
	if (!SetCommTimeouts(serialHandle, &CommTimeOuts)) {
		serialHandle = 0;
		throw exception();
	}

	//настраиваем параметры работы для текущего устройства
	DCB ComDCM;
	memset(&ComDCM, 0, sizeof(ComDCM));
	ComDCM.DCBlength = sizeof(DCB);
	GetCommState(serialHandle, &ComDCM); // текущие значения
	ComDCM.BaudRate = DWORD(baud); // скорость в бодах
	ComDCM.ByteSize = 8; // размер байта
	ComDCM.Parity = NOPARITY; // паритет
	ComDCM.StopBits = ONESTOPBIT; // количество стоп бит
	ComDCM.fAbortOnError = TRUE;
	ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // сброс DTR бита

											  // автоустановка RTS бита
	ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;
	ComDCM.fBinary = TRUE; //бинарный режим всегда
	ComDCM.fParity = FALSE; //паритет
	ComDCM.fInX = ComDCM.fOutX = FALSE;
	ComDCM.XonChar = 0;
	ComDCM.XoffChar = uint8_t(0xff);
	ComDCM.fErrorChar = FALSE;
	ComDCM.fNull = FALSE;
	ComDCM.fOutxCtsFlow = FALSE;
	ComDCM.fOutxDsrFlow = FALSE;
	ComDCM.XonLim = 128;
	ComDCM.XoffLim = 128;
	//установка параметров работы для текущего устройства
	if (!SetCommState(serialHandle, &ComDCM)) {
		CloseHandle(serialHandle);
		serialHandle = 0;
		throw exception();
	}
}

void SerialPort::close()
{
	if (serialHandle != 0) {
		CloseHandle(serialHandle);
		serialHandle = 0;
	}
}

int SerialPort::getBaud()
{
	return currentBaud;
}

void SerialPort::write(string& data)
{
	data.append("\r\n");
	if (serialHandle == 0) { throw exception(); }
	DWORD feedback;
	if (!WriteFile(serialHandle, data.c_str(), data.length(), &feedback, 0) ||
		feedback != data.length()) {
		CloseHandle(serialHandle);
		serialHandle = 0;
		throw exception();
	}
}

bool SerialPort::try_read(string& data)
{
	char buff[1024];
	DWORD feedback = 0;
	ReadFile(serialHandle, buff, 1024, &feedback, NULL);
	if (feedback) {
		buff[feedback] = 0;
		data = string(buff);
		return true;
	}

	return false;
}