#ifndef rover_h
#define rover_h

#include <AFMotor.h>

class Rover
{
private:
AF_DCMotor *rMotor;
AF_DCMotor *lMotor;

public:
    Rover(uint8_t rMotorNum, uint8_t lMotorNum);
    ~Rover();

    void setSpeed(uint8_t speed);
    void goForward();
    void goBackward();
    void turnRight();
    void turnLeft();
    void stop();
};

Rover::Rover(uint8_t rMotorNum, uint8_t lMotorNum)
{
    *rMotor = AF_DCMotor(rMotorNum);
    *lMotor = AF_DCMotor(lMotorNum);
}

Rover::~Rover()
{
}

void Rover::setSpeed(uint8_t speed)
{
    rMotor->setSpeed(speed);
    lMotor->setSpeed(speed);
}

void Rover::goForward()
{
    rMotor->run(FORWARD);
    lMotor->run(FORWARD);
}

void Rover::goBackward()
{
    rMotor->run(BACKWARD);
    lMotor->run(BACKWARD);
}

void Rover::turnRight()
{
    rMotor->run(BACKWARD);
    lMotor->run(FORWARD);
}

void Rover::turnLeft()
{
    rMotor->run(FORWARD);
    lMotor->run(BACKWARD);
}

void Rover::stop()
{
    rMotor->run(RELEASE);
    lMotor->run(RELEASE);
}

#endif