#include <iostream>
#include "stantion.h"


using namespace std;

int main(int argc, char *argv[])
{
    Stantion st1("one", 200);
    Stantion st2("two", 220);
    Stantion st3("thre", 300);

    st1.beginTransmitting();
    st2.beginTransmitting();
    st3.beginTransmitting();

    cin.get();

    st1.stop();
    st2.stop();
    st3.stop();
    return 0;
}
