#include "stantion.h"
#include <iostream>
#include <windows.h>
#include <random>
#include <iomanip>
#include <math.h>

using namespace std;

unsigned Stantion::srpd = 0;

void Stantion::generateMessage(unsigned &msg)
{
    srand(clock() + GetCurrentThreadId());
    unsigned hi = rand();
    unsigned lo = rand();
    msg = 0;
    msg = hi << 16;
    msg |= lo;
}

void Stantion::transminFunc(Stantion* instance)
{
    while(instance->running)
    {
        Sleep(instance->delay_ms); // Stantion spicified transmit period
        unsigned msg;
        generateMessage(msg);
        int try_counter = 0;

        while(true)
        {
            if (!try_counter && Stantion::srpd == JAM_SIGNAL)
                break;

            Stantion::srpd |= msg;
            Sleep(COLISION_DELAY); // Collision window

            if (Stantion::srpd == msg){
                cout << "Transmitting sucsees from " << instance->name << ": " << setfill('0') << setw(8) << std::hex << msg << endl;
                Stantion::srpd = 0;
                break;
            }
            else{
                Stantion::srpd = JAM_SIGNAL;
                try_counter += 1;
                cout << "Collision detected from " << instance->name << ", try to resolve: " << try_counter << endl;

                if (try_counter == MAX_TRIES){
                    cout << "Unable to resolve, skipping" << endl;
                    break;
                }

                int time_slot = rand() % ((int)pow(2, try_counter) + 1);
                Sleep(time_slot * 100); // backoff delay
                Stantion::srpd = 0;
            }
        }
    }
}

void Stantion::beginTransmitting()
{
    transmit_thread = new thread(transminFunc, this);
    running = true;
}

void Stantion::stop()
{
    running = false;
    transmit_thread->join();
    cout << "Stantion " << name << " stopped" << endl;
}
