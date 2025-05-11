#include <exception>
#include <iostream>
#include <stdexcept>
#include "TaskModelToDBInterface.h"

TaskModelToDBInterface::TaskModelToDBInterface()
: DBInterface("Tasks", "createTask")
{

}

TaskModelToDBInterface::~TaskModelToDBInterface()
{

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

void TaskModelToDBInterface::addDataToSqlStatement(ModelBase* modelObject)
{
    TaskModel* task = dynamic_cast<TaskModel*>(modelObject);
    if (!task)
    {
        std::runtime_error badObject(
            "PROGRAMMER ERROR: In TaskModelToDBInterface::addDataToSqlStatement model object is not task model!");
        throw badObject;
    }

    std::size_t priorityInAllTasks{1};

    appendArgToSqlStmt(task->getCreatorID(), true);
    appendArgToSqlStmt(task->getDescription(), true);
    appendArgToSqlStmt(task->getParentTaskID(), true);
    appendArgToSqlStmt(task->getEstimatedEffort(), true);
    appendArgToSqlStmt(priorityInAllTasks, true);
    appendArgToSqlStmt(task->getDueDate(), true);
    appendArgToSqlStmt(task->getScheduledStart());
}
