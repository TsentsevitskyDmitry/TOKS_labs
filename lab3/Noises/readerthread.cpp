#include "readerthread.h"

ReaderThread::~ReaderThread()
{
    stop();
}


void ReaderThread::stop()
{
    this->running = false;
}
