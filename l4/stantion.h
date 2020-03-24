#ifndef STANTION_H
#define STANTION_H

#include <cstdlib>
#include <thread>
#include <string >

#define JAM_SIGNAL 0xFFFFFFFF
#define MAX_TRIES 10
#define COLISION_DELAY 10

class Stantion
{
private:
    static unsigned srpd;
    std::thread* transmit_thread;
    bool running;
    int delay_ms;
    std::string name;
    static void transminFunc(Stantion* instance);
    static void generateMessage(unsigned& msg);

public:
    Stantion(std::string name, int delay_ms) : running(false)
    {
        this->name = name;
        this->delay_ms = delay_ms;
    }

    void beginTransmitting();
    void stop();
};

#endif // STANTION_H
