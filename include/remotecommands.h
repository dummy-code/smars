#ifndef remotecommands_h
#define remotecommands_h

#include <IRremote.hpp>
#include <Arduino.h>

enum Command
{
    Unknown_cmd = -1,
    Forward_cmd =  9,
    Backward_cmd = 7,
    Left_cmd = 68,
    Right_cmd = 67,
    Stop_cmd = 64
};

class RemoteCommands
{
private:
    IRrecv *irrecv;

public:
    RemoteCommands(uint8_t pin);
    ~RemoteCommands();

    uint16_t lastCommand();
    void enable();
};

RemoteCommands::RemoteCommands(uint8_t pin)
{
    irrecv = new IRrecv(pin);
}

RemoteCommands::~RemoteCommands()
{ }

uint16_t RemoteCommands::lastCommand()
{
    if (irrecv->decode())     
    {        
        irrecv->resume();          
        if (irrecv->decodedIRData.command == Command::Forward_cmd
            || irrecv->decodedIRData.command == Command::Backward_cmd
            || irrecv->decodedIRData.command == Command::Right_cmd
            || irrecv->decodedIRData.command == Command::Left_cmd
            || irrecv->decodedIRData.command == Command::Stop_cmd)
        {
            return irrecv->decodedIRData.command;
        }
    }   

    return Command::Unknown_cmd;
}

void RemoteCommands::enable()
{
    irrecv->enableIRIn();
}


#endif