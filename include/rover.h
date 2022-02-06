#ifndef rover_h
#define rover_h

#include <AFMotor.h>

class Rover
{
private:
AF_DCMotor *rMotor;
AF_DCMotor *lMotor;
uint8_t motorSpeed;

void setMotorSpeed()
{
    rMotor->setSpeed(motorSpeed);
    lMotor->setSpeed(motorSpeed);
}

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
    rMotor = new AF_DCMotor(rMotorNum);
    lMotor = new AF_DCMotor(lMotorNum);
}

Rover::~Rover()
{
}

void Rover::setSpeed(uint8_t speed)
{
    motorSpeed = speed;
}

void Rover::goForward()
{
    this->setMotorSpeed();
    rMotor->run(FORWARD);
    lMotor->run(FORWARD);
}

void Rover::goBackward()
{
    this->setMotorSpeed();
    rMotor->run(BACKWARD);
    lMotor->run(BACKWARD);
}

void Rover::turnRight()
{
    this->setMotorSpeed();
    rMotor->run(BACKWARD);
    lMotor->run(FORWARD);
}

void Rover::turnLeft()
{
    this->setMotorSpeed();
    rMotor->run(FORWARD);
    lMotor->run(BACKWARD);
}

void Rover::stop()
{
    rMotor->run(RELEASE);
    lMotor->run(RELEASE);
}

#endif