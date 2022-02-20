#include <arduino.h>

#include <rover.h>
#include <parameters.h>
#include <IRremote.hpp>

bool logEnabled;
long lastCheck;
long startTurn;
char status = STATUS_STOP;
boolean statusChanged = false;

Rover rover(1, 2);

void SetStatus(char newStatus)
{
    if (newStatus == status)
        return;

    if (logEnabled)
    {
        Serial.print("Set status: ");
        Serial.println(newStatus);
    }

    status = newStatus;
    statusChanged = true;
}


void GoForward()
{
    rover.goForward();

    if (logEnabled)
    {
        Serial.println("Go forward");
    }
}

void TurnRight()
{
    rover.turnRight();

    if (logEnabled)
    {
        Serial.println("Start turn Right");
    }
}

void Stop()
{
    rover.stop();

    if (logEnabled)
    {
        Serial.println("Stop");
    }
}

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
        //Serial.print("Distance: ");
        //Serial.print(distance);
        //Serial.println(" cm");
    }

    return distance;
}

void Initialize()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    rover.setSpeed(MOTOR_SPEED);

    logEnabled = true;
}

void CheckDistance(long interval)
{
    /*
    if (logEnabled)
    {
        Serial.println("Check distance");
    }
    */

    long elapsed = millis() - lastCheck;
    if (elapsed >= interval)
    {
        if (GetDistance() <= MIN_DISTANCE)
        {
            SetStatus(STATUS_TURNRIGHT);
        }
        else
        {
            SetStatus(STATUS_FORWARD);
        }

        lastCheck = millis();
    }
}


void Execute()
{
    /*
    if (logEnabled)
    {
        Serial.print("Execute: ");
        Serial.println(status);
        Serial.print("Changed: ");
        Serial.println(statusChanged);
    }
    */

    if (status == STATUS_FORWARD)
    {
        if (statusChanged)
        {
            GoForward();
            statusChanged = false;
        }

        CheckDistance(MIN_DISTANCE);
        return;
    }

    if (status == STATUS_STOP)
    {
        if (statusChanged)
        {
            Stop();
            statusChanged = false;
        }

        return;
    }

    if (status == STATUS_TURNRIGHT)
    {
        TurnRight();
        if (statusChanged)
        {
            statusChanged = false;
            startTurn = millis();
        }

        long elapsed = millis() - startTurn;
        if (elapsed >= TURN_INTERVAL)
        {
            Serial.println("End turn Right");
            SetStatus(STATUS_FORWARD);
        }

        return;
    }
}

IRrecv irrecv(A0);
decode_results results;

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Initialize();
    SetStatus(STATUS_FORWARD);
    
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
            SetStatus(STATUS_FORWARD);
        }     

        if (irrecv.decodedIRData.command == CMD_STOP)
        {
            SetStatus(STATUS_STOP);
        }     

        if (irrecv.decodedIRData.command == CMD_RIGHT)
        {
            SetStatus(STATUS_TURNRIGHT);
        }     
    }   

    //Execute();
    rover.goForward();
}
