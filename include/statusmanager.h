#ifndef statusmanager_h
#define statusmanager_h

enum Status 
{
    Stop = 0,
    Forward = 1,
    Backward = 2,
    TurnRigh = 3,
    TurnLeft = 4,
    FindObstacle = 5,
    Avoid = 6
};

struct StatusAction
{
    bool always;
    Status status;
    void (*action)();

    void Execute()
    {
        action();
    }
};


class StatusManager
{
private:
    Status currentStatus;
    bool statusChanged;
    StatusAction actions[7];

    StatusAction createAction(Status status, void (*actionToExecute)(), bool always)
    {
        StatusAction action;
        action.status = status;
        action.action = actionToExecute;
        action.always = always;

        return action;
    }
    
public:
    StatusManager();
    ~StatusManager();
    void ChangeStatus(Status newStatus);
    void SetStatusAction(Status status, void (*actionToExecute)(), bool always);
    void ActionExecute();
};

StatusManager::StatusManager()
{ 
}

StatusManager::~StatusManager()
{ }

void StatusManager::ChangeStatus(Status newStatus)
{
    //if (newStatus == currentStatus)
    //    return;
    
    Serial.println(currentStatus);
    Serial.println(newStatus);

    currentStatus = newStatus;
    statusChanged = true;
}

void StatusManager::SetStatusAction(Status status, void (*actionToExecute)(), bool always)
{
    actions[status] = this->createAction(status, actionToExecute, always);
}

void StatusManager::ActionExecute()
{
    if (!actions[currentStatus].action)
    {
        Serial.println("no action");
        return;
    }

    if (actions[currentStatus].always
        || statusChanged)
    {
        Serial.print("Start ");
        Serial.println(actions[currentStatus].status);
        statusChanged = false;
        actions[currentStatus].Execute();
        return;
    }

    //Serial.println("not changed");
}

#endif