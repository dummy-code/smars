#ifndef smars_h
#define smars_h

#include <arduino.h>
#include <AFMotor.h>

class smars
{
private:
    AF_DCMotor rMotor = NULL;
    AF_DCMotor lMotor = NULL;
    uint8_t speed;

public:
    smars(uint8_t _rMotorPin, uint8_t _lMotorPin);
    ~smars();

    void stop();
    void setSpeed(uint8_t _speed);
    void goForward();
    void goBackward();
    void turnRight();
    void turnLeft();
};

smars::smars(uint8_t _rMotorPin, uint8_t _lMotorPin)
{
    rMotor = AF_DCMotor(_rMotorPin);
    lMotor = AF_DCMotor(_lMotorPin);
}

smars::~smars()
{
}


#endif