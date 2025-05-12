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

    bool isValid = true;

    reportIfError(requiredKeyHasValue(task->getTaskID()), "Task ID alreay has a value, the task is in the database\n", isValid);
    reportIfError(!requiredKeyHasValue(task->getCreatorID()), "Creator ID not set\n", isValid);
    reportIfError(!requiredFieldHaseData(task->getDescription()), "Missing Task Description\n", isValid);
    reportIfError((task->getEstimatedEffort() == 0), "Estimated effor in hours not set\n", isValid);
    reportIfError(!requiredFieldHaseData(task->getDueDate()), "The due date has not been set\n", isValid);
    reportIfError(!requiredFieldHaseData(task->getScheduledStart()), "The scheduled start date has not been set\n", isValid);

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
    appendArgToSqlStmt(task->getDueDate(), true);
    appendArgToSqlStmt(task->getScheduledStart());

    return true;
}


