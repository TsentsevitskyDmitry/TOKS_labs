#include <QThread>
#include "SerialAPI.h"

class ReaderThread : public QThread {
Q_OBJECT
private:
    SerialPort& com;
    bool running;

public:
    ReaderThread(SerialPort& serial): running(true), com(serial) {}
    ~ReaderThread();
    void stop();

    void run()
    {
        string data;
        while(running)
        {
            if(!com.connected())
                Sleep(100);
            else if(com.try_read(data)){
                emit dataArrived("< " + QString::fromUtf8(data.c_str()));
            }
        }
    }


signals:
    void dataArrived(QString);

};
