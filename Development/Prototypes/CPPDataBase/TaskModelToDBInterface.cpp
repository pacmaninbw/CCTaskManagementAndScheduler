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
