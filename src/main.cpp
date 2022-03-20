#include <arduino.h>

#include <rover.h>
#include <parameters.h>
#include <statusmanager.h>
#include <remotecommands.h>

//bool logEnabled;
long lastCheck;

Rover rover(1, 2);
StatusManager sm;
RemoteCommands remote(IR_REMOTE_PIN);

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

    /*
    if (logEnabled)
    {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");
    }
    */

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
    delay(TURN_TIME);
    sm.ChangeStatus(Status::Forward);
}

void changeStatusFromRemote()
{
    int cmd = remote.lastCommand();
    Serial.println(cmd);

    if (cmd == Command::Unknown_cmd)
        return;

    if (cmd == Command::Forward_cmd)
    {
        sm.ChangeStatus(Status::Forward);
    }

    if (cmd == Command::Backward_cmd)
    {
        sm.ChangeStatus(Status::Backward);
    }

    if (cmd == Command::Right_cmd)
    {
        sm.ChangeStatus(Status::TurnRigh);
    }

    if (cmd == Command::Left_cmd)
    {
        sm.ChangeStatus(Status::TurnLeft);
    }

    if (cmd == Command::Stop_cmd)
    {
        sm.ChangeStatus(Status::Stop);
    }
}

void Initialize()
{
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);

    rover.setSpeed(MOTOR_SPEED);
    remote.enable();

    sm.SetStatusAction(Status::Stop, StopStatusAction, false);
    sm.SetStatusAction(Status::Forward, FowardStatusAction, false);
    sm.SetStatusAction(Status::Backward, BackwardStatusAction, false);
    sm.SetStatusAction(Status::TurnLeft, TurnLeftStatusAction, false);
    sm.SetStatusAction(Status::TurnRigh, TurnRightStatusAction, false);
    sm.SetStatusAction(Status::FindObstacle, FindObstacleStatusAction, true);
    sm.SetStatusAction(Status::Avoid, AvoidStatusAction, true);
}

void setup()
{
    Serial.begin(9600);
    Serial.println();

    Initialize();
    
    sm.ChangeStatus(Status::Stop);
}

void loop()
{   
    changeStatusFromRemote();
    sm.ActionExecute();
}
