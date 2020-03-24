#ifndef TOCKENRING_H
#define TOCKENRING_H

#include <QThread>
#include <iostream>
#include <string>
#include "serialapi.h"

#define DELAY 1000

#define TOKEN_HEADER char(0x7E)
#define MSG_HEADER char(0x7A)

#define POCKET_MARK_NONE "0"
#define POCKET_MARK_TRUE "1"

Q_DECLARE_METATYPE(std::string);

class TockenRing : public QThread {
Q_OBJECT

private:
    int self, next;
    std::string name;
    SerialPort listenPort;
    SerialPort writePort;
    bool running;
    std::string data_to_write;
    std::string dest_to_write;
    bool token_owner;
    bool master_node;

    void write(int type, int dest, std::string data);
    std::string read();

public:
    TockenRing(): self(0), next(0), name(""), running(true), data_to_write(""), dest_to_write(""), token_owner(false), master_node(false) {}
    ~TockenRing(){}

    void reconnect(std::string listen, std::string write, std::string name);
    void post_write(std::string dest, std::string data);
    void process();
    void stop();
    void set_master(bool state);

    void run()
    {
        process();
    }


signals:
    void dataArrived(std::string);
    void messageArrived(std::string);

};


#endif // TOCKENRING_H
