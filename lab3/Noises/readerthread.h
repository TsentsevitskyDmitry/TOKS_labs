#include <QThread>
#include "SerialAPI.h"

Q_DECLARE_METATYPE(std::string);

class ReaderThread : public QThread {
Q_OBJECT
private:
    SerialPort& com;
    bool running;

public:
    ReaderThread(SerialPort& serial): com(serial), running(true) {}
    ~ReaderThread();
    void stop();

    void run()
    {
        string data;
        while(running)
        {
            if(!com.connected())
                Sleep(100);
            else if(com.read(data)){
                emit dataArrived(data);
            }
        }
    }


signals:
    void dataArrived(std::string);

};
