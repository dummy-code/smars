#include <arduino.h>

#include <rover.h>

#define echoPin 9       // servo-1 motor shield
#define trigPin 10      // servo-2 motor shield

#define MIN_DISTANCE 5  // cm
#define MOTOR_SPEED 170

#define CHECK_INTERVAL 500
#define TURN_INTERVAL 2000

#define STATUS_FORWARD 'F'
#define STATUS_STOP 'S'
#define STATUS_TURNRIGHT 'R'

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
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
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


void setup()
{
    Serial.begin(9600);
    Serial.println();

    Initialize();
    SetStatus(STATUS_FORWARD);
}

void loop()
{
    Execute();
}
