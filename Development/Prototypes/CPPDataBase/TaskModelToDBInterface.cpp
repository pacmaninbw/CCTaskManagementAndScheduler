#include <iostream>
#include "TaskModelToDBInterface.h"

TaskModelToDBInterface::TaskModelToDBInterface()
: DBInterface("Tasks", "createTask")
{

}

TaskModelToDBInterface::~TaskModelToDBInterface()
{

}

void TaskModelToDBInterface::addTaskToDatabase(TaskModel &task)
{
    // Prevent bad data from throwing an exception.
    if (!ModelObjectHasAllRequiredFields(&task))
    {
        return;
    }

    std::size_t priorityInAllTasks{1};

    startAddStmt();

    appendArgToSqlStmt(task.getCreatorID(), true);
    appendArgToSqlStmt(task.getDescription(), true);
    appendArgToSqlStmt(task.getParentTaskID(), true);
    appendArgToSqlStmt(task.getEstimatedEffort(), true);
    appendArgToSqlStmt(priorityInAllTasks, true);
    appendArgToSqlStmt(task.getDueDate(), true);
    appendArgToSqlStmt(task.getScheduledStart());

    sqlStatement += ")";

    asyncExecutionSqlStatment(sqlStatement.c_str());
}

bool TaskModelToDBInterface::ModelObjectHasAllRequiredFields(ModelBase *modelObject)
{
    TaskModel* task = dynamic_cast<TaskModel*>(modelObject);

    if (!task)
    {
        return false;
    }

    bool isValid = true;

    if (!requiredKeyHasValue(task->getCreatorID()))
    {
        std::cerr << "Creator ID not set\n";
        isValid = false;
    }

    if (!requiredFieldHaseData(task->getDescription()))
    {
        std::cerr << "Missing Task Description\n";
        isValid = false;
    }

    if (task->getEstimatedEffort() == 0)
    {
        std::cerr << "Estimated effor in hours not set\n";
        isValid = false;
    }

    if (!requiredFieldHaseData(task->getDueDate()))
    {
        std::cerr << "The due date has not been set\n";
        isValid = false;
    }

    if (!requiredFieldHaseData(task->getScheduledStart()))
    {
        std::cerr << "The scheduled start date has not been set\n";
        isValid = false;
    }

    if (!isValid)
    {
        std::cerr << task;
    }

    return isValid;
}

