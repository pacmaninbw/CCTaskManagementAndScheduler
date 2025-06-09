#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <iostream>
#include <string>
#include "ModelBase.h"
#include "UserModel.h"

class TaskModel : public ModelBase
{
public:
    enum class TaskStatus
    {
        Not_Started=1, On_Hold=2, Waiting_for_Dependency=3, Work_in_Progress=4, Complete=5
    };

    TaskModel();
    // new task from GUI or program.
    TaskModel(UserModel& creator, std::string& descriptionIn, unsigned int estimatedHoursEffort, std::string dueDate,
        std::string startDate, TaskModel* parentTaskp = nullptr, TaskStatus statusIn = TaskStatus::Not_Started,
        unsigned int majorPriority = 1, unsigned int minorPriority = 1
    );
    // existing task from database.
    TaskModel(std::size_t taskIDfromDB, std::size_t createdByUidfromDb, std::size_t assignedToUidfromDb,
        std::string descriptionfromDb, std::size_t parentTaskIDfromDb, unsigned int statusfromDb,
        double percentageCompletefromDb, std::string createdOnDatefromDb, std::string requiredDeliveryDatefromDb,
        std::string scheduledStartDatefromDb, std::string actualStartDatefromDb, std::string estimatedCompletionDatefromDb,
        std::string completedDatefromDb, unsigned int estimatedEffortHoursfromDb, double actualEffortHoursfromDb,
        unsigned int priorityGroupfromDb, unsigned int priorityfromDb      
    );
    ~TaskModel();
    void dbSetTaskID(std::size_t dbTaskID) { setPrimaryKey(dbTaskID); };
    std::size_t getTaskID() const { return getPrimaryKey(); };
    std::size_t getCreatorID();
    std::size_t getAssignToID();
    std::string getDescription();
    TaskModel::TaskStatus getStatus();
    std::size_t getParentTaskID();
    double getPercentageComplete();
    std::string getCreationDate();
    std::string getDueDate();
    std::string getScheduledStart();
    std::string getactualStartDate();
    std::string getEstimatedCompletion();
    std::string getCompletionDate();
    unsigned int getEstimatedEffort();
    double getactualEffortToDate();
    unsigned int getPriorityGoup();
    unsigned int getPriority();
    std::string taskStatusString() const;

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& obj)
    {
        os << "TaskModel:\n";
        os << obj.primaryKeyFieldName;
        return os;
    };


private:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };

/*
 * Member variables.
 */
    // std::size_t taskID; // Using primaryKey as TaskID
    TaskStatus status;
};

#endif // TASKMODEL_H_


