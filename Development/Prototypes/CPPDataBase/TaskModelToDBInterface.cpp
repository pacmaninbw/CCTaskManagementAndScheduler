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
        appendErrorMessage("PROGRAMMER ERROR: In TaskModelToDBInterface::addDataToSqlStatement model object is not task model!");
        return false;
    }

    bool isValid = task->allRequiredFieldsHaveData();
    if (!isValid)
    {
        appendErrorMessage(task->reportMissingRequiredFields());
    }

    return isValid;
}

bool TaskModelToDBInterface::addDataToSqlStatement(ModelBase* modelObject)
{
    TaskModel* task = dynamic_cast<TaskModel*>(modelObject);
    if (!task)
    {
        appendErrorMessage("PROGRAMMER ERROR: In TaskModelToDBInterface::addDataToSqlStatement model object is not task model!");
        return false;
    }

    std::size_t priorityInAllTasks{1};

    appendArgToSqlStmt(task->getCreatorID(), true);
    appendArgToSqlStmt(task->getDescription(), true);
    appendArgToSqlStmt(task->getParentTaskID(), true);
    appendArgToSqlStmt(task->getEstimatedEffort(), true);
    appendArgToSqlStmt(priorityInAllTasks, true);
    appendArgToSqlStmt(task->dateToString(task->getDueDate()), true);
    appendArgToSqlStmt(task->dateToString(task->getScheduledStart()));

    return true;
}


