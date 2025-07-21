#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <chrono>
#include <iostream>
#include <memory>
#include "ModelBase.h"
#include <optional>
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
    std::size_t getTaskID() const { return taskID; };
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
        return os;
    };


private:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };

    bool modified;
    std::size_t taskID;
    std::size_t creatorID;
    std::size_t assignToID;
    std::string description;
    std::optional<TaskStatus> status;
    std::optional<std::size_t> parentTaskID;
    double percentageComplete;
    std::chrono::year_month_day creationDate;
    std::chrono::year_month_day dueDate;
    std::chrono::year_month_day scheduledStart;
    std::optional<std::chrono::year_month_day> actualStartDate;
    std::optional<std::chrono::year_month_day> estimatedCompletion;
    std::optional<std::chrono::year_month_day> completionDate;
    unsigned int estimatedEffort;
    double actualEffortToDate;
    unsigned int priorityGroup;
    unsigned int priority;
    bool completed;
    bool personal;

};

using TaskModel_shp = std::shared_ptr<TaskModel>;
using TaskList = std::vector<TaskModel_shp>;

#endif // TASKMODEL_H_


