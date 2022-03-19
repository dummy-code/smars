#include <arduino.h>

#include <rover.h>
#include <parameters.h>
#include <IRremote.hpp>
#include <statusmanager.h>

bool logEnabled;
long lastCheck;

Rover rover(1, 2);
StatusManager sm;

int GetDistance()
{
    long duration; 
    int distance;

    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.0343 / 2;

    if (logEnabled)
    {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    }

    return distance;
}

void StopStatusAction()
{
    rover.stop();
}

void FowardStatusAction()
{
    rover.goForward();
    sm.ChangeStatus(Status::FindObstacle);
}

void BackwardStatusAction()
{
    rover.goBackward();
}

void TurnLeftStatusAction()
{
    rover.turnLeft();
}

void TurnRightStatusAction()
{
    rover.turnRight();
}

void FindObstacleStatusAction()
{
    long elapsed = millis() - lastCheck;
    if (elapsed >= CHECK_INTERVAL)
    {
        if (GetDistance() <= MIN_DISTANCE)
        {
            sm.ChangeStatus(Status::Avoid);
        }

        lastCheck = millis();
    }
}

void AvoidStatusAction()
{
    rover.turnRight();
    delay(500);
    sm.ChangeStatus(Status::Forward);
}

void Initialize()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    rover.setSpeed(MOTOR_SPEED);

    logEnabled = true;

    sm.SetStatusAction(Status::Stop, StopStatusAction, false);
    sm.SetStatusAction(Status::Forward, FowardStatusAction, false);
    sm.SetStatusAction(Status::Backward, BackwardStatusAction, false);
    sm.SetStatusAction(Status::TurnLeft, TurnLeftStatusAction, false);
    sm.SetStatusAction(Status::TurnRigh, TurnRightStatusAction, false);
    sm.SetStatusAction(Status::FindObstacle, FindObstacleStatusAction, true);
    sm.SetStatusAction(Status::Avoid, AvoidStatusAction, true);
}

IRrecv irrecv(A0);
decode_results results;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Initialize();
    
    sm.ChangeStatus(Status::Stop);
    irrecv.enableIRIn();
}

#define POWER 69
#define CMD_FORWARD  9
#define CMD_BACKWARD  7
#define CMD_LEFT  68
#define CMD_RIGHT  67
#define CMD_STOP 64

void loop()
{
    if (irrecv.decode())     
    {     
        Serial.print("Command: ");     
        Serial.println(irrecv.decodedIRData.command); //prints the value a a button press     
        Serial.println(" ");     
        irrecv.resume(); // Restart the ISR state machine and Receive the next value

        if (irrecv.decodedIRData.command == CMD_FORWARD)
        {
            sm.ChangeStatus(Status::Forward);
        }     

        if (irrecv.decodedIRData.command == CMD_BACKWARD)
        {
            sm.ChangeStatus(Status::Backward);
        }     

        if (irrecv.decodedIRData.command == CMD_RIGHT)
        {
            sm.ChangeStatus(Status::TurnRigh);
        }     

        if (irrecv.decodedIRData.command == CMD_LEFT)
        {
            sm.ChangeStatus(Status::TurnLeft);
        }     

        if (irrecv.decodedIRData.command == CMD_STOP)
        {
            sm.ChangeStatus(Status::Stop);
        }     

    }   

    sm.ActionExecute();
}
