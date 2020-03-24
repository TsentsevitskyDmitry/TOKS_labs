#include "tockenring.h"

void TockenRing::reconnect(std::string listen, std::string write, string name)
{
    this->name = name;
    this->next = std::stoi(write);
    this->self = std::stoi(listen);
    bool s = listenPort.open("COM" + listen, 19200);
    bool n = writePort.open("COM" + write, 19200);
    emit dataArrived("connecting");

    std::cout << "connecting to self " << listen << " res: " << s << std:: endl;
    std::cout << "connecting to next " << write << " res: " << n << std:: endl;
}

void TockenRing::post_write(string dest, string data)
{
    this->data_to_write = data;
    this->dest_to_write = dest;
}

void TockenRing::process()
{
    while(running)
    {
        std::string indata;
        if(!listenPort.connected() || !writePort.connected()){
            emit dataArrived("not connected");
            Sleep(100);
            continue;
        }
        else if(listenPort.try_read(indata)){
            std::string res = "";

            if (indata[0] == TOKEN_HEADER){
                res += "Token recieved";
                token_owner = true;
            }
            else{
                res += "Message recieved \n";
                std::string mark = indata.substr(1, 1);
                res += "Mark = " + mark;
                std::string dest = indata.substr(2, 1);
                res += "To: " + dest + "\n";
                std::string content = indata.substr(3, indata.length());
                res += "Content: " + content + "\n";

                if(dest == name){
                    emit messageArrived(content);
                    indata = "";
                }

                if(master_node)
                    if(mark == "0")
                        indata[1] = '1';
                    else
                        indata = "";

            }
            emit dataArrived(res);
        }

        Sleep(DELAY);

        if(!token_owner && indata.length() != 0){
            writePort.write(indata);
            continue;
        }

        if(data_to_write.length() != 0 && token_owner){
            std::string out = "";
            out += MSG_HEADER;
            out += POCKET_MARK_NONE;
            out += dest_to_write + data_to_write;
            writePort.write(out);
            data_to_write = "";
            Sleep(DELAY);
            emit dataArrived("");
        }

        if (token_owner){
            std::string out = "";
            out += TOKEN_HEADER;
            out += POCKET_MARK_NONE;
            writePort.write(out);
            token_owner = false;
            emit dataArrived("");
        }

    }
}

void TockenRing::set_master(bool state)
{
    this->master_node = state;
    std::string out = "";
    out += TOKEN_HEADER;
    out += POCKET_MARK_NONE;
    writePort.write(out);
}

void TockenRing::stop()
{
    running = false;
}
