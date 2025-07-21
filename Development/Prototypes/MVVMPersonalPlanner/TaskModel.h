#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <chrono>
#include <iostream>
#include <memory>
#include "ModelBase.h"
#include <string>
#include "UserModel.h"
#include <vector>

class TaskModel : public ModelBase
{
public:
    enum class TaskStatus
    {
        Not_Started, On_Hold, Waiting_for_Dependency, Work_in_Progress, Complete
    };

    TaskModel();
    TaskModel(UserModel_shp creator);
    TaskModel(UserModel_shp creator, std::string descriptionIn);
    virtual ~TaskModel() = default;

    void addEffortHours(double hours);
    std::size_t getTaskID() const;
    std::size_t getCreatorID() const;
    std::size_t getAssignToID() const;
    std::string getDescription() const;
    TaskModel::TaskStatus getStatus() const;
    unsigned int getStatusIntVal() const;
    std::size_t getParentTaskID() const;
    double getPercentageComplete() const;
    std::chrono::year_month_day getCreationDate() const;
    std::chrono::year_month_day getDueDate() const;
    std::chrono::year_month_day getScheduledStart() const;
    std::chrono::year_month_day getactualStartDate() const;
    std::chrono::year_month_day getEstimatedCompletion() const;
    std::chrono::year_month_day getCompletionDate() const;
    unsigned int getEstimatedEffort() const;
    double getactualEffortToDate() const;
    unsigned int getPriorityGroup() const;
    unsigned int getPriority() const;
    bool hasOptionalFieldStatus() const;
    bool hasOptionalFieldParentTaskID() const;
    bool hasOptionalFieldActualStartDate() const;
    bool hasOptionalFieldEstimatedCompletion() const;
    bool hasOptionalFieldCompletionDate() const;
    void setCreatorID(std::size_t creatorID);
    void setCreatorID(UserModel_shp creator);
    void setAssignToID(std::size_t assignedID);
    void setAssignToID(UserModel_shp assignedUser);
    void setDescription(std::string description);
    void setStatus(TaskModel::TaskStatus status);
    void setStatus(std::string statusStr);
    void setParentTaskID(std::size_t parentTaskID);
    void setParentTaskID(std::shared_ptr<TaskModel> parentTask);
    void setPercentageComplete(double percentComplete);
    void setCreationDate(std::chrono::year_month_day creationDate);
    void setDueDate(std::chrono::year_month_day dueDate);
    void setScheduledStart(std::chrono::year_month_day startDate);
    void setactualStartDate(std::chrono::year_month_day startDate);
    void setEstimatedCompletion(std::chrono::year_month_day completionDate);
    void setCompletionDate(std::chrono::year_month_day completionDate);
    void setEstimatedEffort(unsigned int estimatedHours);
    void setactualEffortToDate(double effortHoursYTD);
    void setPriorityGroup(unsigned int priorityGroup);
    void setPriorityGroup(const char priorityGroup);
    void setPriority(unsigned int priority);

    std::string taskStatusString() const;
    TaskModel::TaskStatus stringToStatus(std::string statusName) const;

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& obj)
    {
        os << "TaskModel:\n";
        os << obj.primaryKeyFieldName;
        os << "Column Names: ";
        for (const auto& [key, value] : obj.dataFields)
        {
            os << key;
            os << ", ";
        }
        os << "\n";
        for (const auto& [key, value] : obj.dataFields)
        {
            PTS_DataField_shp currentField = value;
            os << currentField->fieldInfo();
            os << "\n";
        }
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

using TaskModel_shp = std::shared_ptr<TaskModel>;
using TaskList = std::vector<TaskModel_shp>;

#endif // TASKMODEL_H_


