#include "SerialAPI.h"
#include "tchar.h"
#include <bitset>

SerialPort::~SerialPort()
{
    close();
}

bool SerialPort::open(string port, int baud)
{
    close();
    currentBaud = baud;

    //???????? ?????
    serialHandle = CreateFileA(
        port.c_str(), //???????????? ?????, ???????? COM1
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL);
    if (serialHandle == (HANDLE)-1) {
        serialHandle = 0;
        return false;
    }
    //????????????? ??????? ???????? ? ????????? ???????
    SetupComm(serialHandle, 1500, 1500);
    //??????????? ????-???? ??? ???????? ??????????
    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout = TIMEOUT;
    CommTimeOuts.ReadTotalTimeoutMultiplier = 0;
    CommTimeOuts.ReadTotalTimeoutConstant = TIMEOUT;
    CommTimeOuts.WriteTotalTimeoutMultiplier = 0;
    CommTimeOuts.WriteTotalTimeoutConstant = TIMEOUT;
    if (!SetCommTimeouts(serialHandle, &CommTimeOuts)) {
        serialHandle = 0;
        return false;
    }

    //??????????? ????????? ?????? ??? ???????? ??????????
    DCB ComDCM;
    memset(&ComDCM, 0, sizeof(ComDCM));
    ComDCM.DCBlength = sizeof(DCB);
    GetCommState(serialHandle, &ComDCM); // ??????? ????????
    ComDCM.BaudRate = DWORD(baud); // ???????? ? ?????
    ComDCM.ByteSize = 8; // ?????? ?????
    ComDCM.Parity = NOPARITY; // ???????
    ComDCM.StopBits = ONESTOPBIT; // ?????????? ???? ???
    ComDCM.fAbortOnError = TRUE;
    ComDCM.fDtrControl = DTR_CONTROL_DISABLE; // ????? DTR ????

                                              // ????????????? RTS ????
    ComDCM.fRtsControl = RTS_CONTROL_TOGGLE;
    ComDCM.fBinary = TRUE; //???????? ????? ??????
    ComDCM.fParity = FALSE; //???????
    ComDCM.fInX = ComDCM.fOutX = FALSE;
    ComDCM.XonChar = 0;
    ComDCM.XoffChar = uint8_t(0xff);
    ComDCM.fErrorChar = FALSE;
    ComDCM.fNull = FALSE;
    ComDCM.fOutxCtsFlow = FALSE;
    ComDCM.fOutxDsrFlow = FALSE;
    ComDCM.XonLim = 128;
    ComDCM.XoffLim = 128;
    //????????? ?????????? ?????? ??? ???????? ??????????
    if (!SetCommState(serialHandle, &ComDCM)) {
        CloseHandle(serialHandle);
        serialHandle = 0;
        return false;
    }
    return true;
}

void SerialPort::close()
{
    if (serialHandle != 0) {
        CloseHandle(serialHandle);
        serialHandle = 0;
    }
}

bool SerialPort::connected()
{
    return (bool)serialHandle;
}

int SerialPort::getBaud()
{
    return currentBaud;
}

string SerialPort::uncapsulateData(string data)
{
    char* in_data = (char*)data.c_str();
    char temp = 0;
    int temp_shift = 0;
    int ones = 0;
    char out_data[512] = {};
    int position = 0;
    int dataLength = data.length() < 255 ? data.length() : 255;

    for(int i = 0; i < dataLength; ++i){
        char operand = *in_data;
        ++in_data;
        for(int i = 0; i < 8; ++i){
            //char bit = (operand << i) >> 7;
            unsigned char bit = operand << i;
            bit = bit >> 7;

            if(bit == 0){
                if(ones != 6){
                    ones = 0;
                }
                else{
                    temp = temp << 1;
                    temp = temp | 0b00000001;
                    ++temp_shift;
                    ones = 0;
                }
            }
            else{
                ++ones;
            }

            if(temp_shift == 8){
                out_data[position++] = temp;
                temp = 0;
                temp_shift = 0;
            }

            temp = temp << 1;
            temp = temp | bit;
            ++temp_shift;

            if(temp_shift == 8){
                out_data[position++] = temp;
                temp = 0;
                temp_shift = 0;
            }
        }


    }
    if(temp_shift) {
        out_data[position++] = temp << (8 - temp_shift);
    }
    return string().append(1, (char)0x7E) + string().append(1, (char)dataLength) + string(out_data);
}

string SerialPort::decapsulateData(string data)
{
    char* in_data = (char*)data.c_str();
    char temp = 0;
    int temp_shift = 0;
    int ones = 0;
    char out_data[512] = {};
    int position = 0;

    for(int i = 0; i < data.length(); ++i){
        char operand = *in_data;
        ++in_data;
        for(int i = 0; i < 8; ++i){
            //char bit = (operand << i) >> 7;
            unsigned char bit = operand << i;
            bit = bit >> 7;

            if (bit == 0)
                ones = 0;
            if (bit == 1)
                ++ones;
            if(ones == 7){
                ones = 0;
                continue;
            }

            temp = temp << 1;
            temp = temp | bit;
            ++temp_shift;

            if(temp_shift == 8){
                out_data[position++] = temp;
                temp = 0;
                temp_shift = 0;
            }
        }
    }

    return string(out_data);
}


uint32_t CRC_Calc(const uint8_t* pData, unsigned NumBytes, uint32_t crc, uint32_t Polynom) {
  int i;
  uint8_t Data;
  uint8_t Xor;

  do {
    Data = *pData++;
    i = 8;
    do {
      Xor = crc ^ Data;
      crc >>= 1;
      if (Xor & 1) {
        crc ^= Polynom;
      }
      Data >>= 1;
    } while (--i);
  } while (--NumBytes);
  return crc;
}


string SerialPort::addCRC(string& data)
{
    uint32_t crc = CRC_Calc((const uint8_t*)data.c_str(), data.length(), 0xFFFFFFFF, CRC_POLY);
    cout << "crc1 = " << std::hex << crc << endl;
    string crc_string = string((char*)&crc, 4);
    cout << "crc_str1 = " << crc_string <<  " " << crc_string.length() << endl;

    return data + crc_string;
}

bool SerialPort::checkCRC(string &data)
{
    cout << "crc_str2 = " << data <<  " " << data.length() << endl;
    uint32_t result = CRC_Calc((const uint8_t*)data.c_str(), data.length(), 0xFFFFFFFF, CRC_POLY);
    return !result;
}

string SerialPort::cutCRC(string &data)
{
    return  data.substr(0, data.length() - 4);
}

bool SerialPort::write(string& data)
{
    string newData = uncapsulateData(data);
    for(int i = 0; i < newData.length(); ++i){
        bitset<8> x(newData[i]);
        cout << x << endl;
    }
    cout << decapsulateData(string((char*)newData.c_str() + 2)) << endl;

    if (serialHandle == 0)
        return false;
    DWORD feedback;
    if (!WriteFile(serialHandle, newData.c_str(), newData.length(), &feedback, 0) ||
        feedback != newData.length()) {
        return false;
    }
    return true;
}

bool SerialPort::read(string& data)
{
    if(!serialHandle)
        return false;

    int length = 0;
    DWORD feedback = 0;
    string indata = "";

    while (serialHandle) {
        char buff[256] = {};
        ReadFile(serialHandle, buff, 256, &feedback, NULL);
        if (feedback) {
            if(buff[0] == 0x7E){
                length = (unsigned char)buff[1];
                indata = "";
            }
            indata += decapsulateData(string(buff + 2));
            if(length == indata.length())
                break;
        }
        else{
            Sleep(100);
        }
    }

    if(feedback){
        data = indata;
        cout << "readed" << data << endl;
        return true;
    }
    return false;
}
