#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include "UserModel.h"
#include <vector>

class TaskModel
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

    bool isInDatabase() const { return taskID > 0; };
    bool isModified() const { return modified; };
    bool hasRequiredValues() const;
    void clearModified() { modified = false; };
    void addEffortHours(double hours);
    void markComplete()
    {
        setCompletionDate(getTodaysDate());
        setStatus(TaskModel::TaskStatus::Complete);
    }
    std::size_t getTaskID() const { return taskID; };
    std::size_t getCreatorID() const { return creatorID; };
    std::size_t getAssignToID() const { return assignToID; };
    std::string getDescription() const { return description; };
    TaskModel::TaskStatus getStatus() const { return status.value_or(TaskModel::TaskStatus::Not_Started); };
    unsigned int getStatusIntVal() const { return static_cast<unsigned int>(getStatus()); };
    std::size_t getParentTaskID() const { return parentTaskID.value_or(0); };
    std::optional<std::size_t> rawParentTaskID() const { return parentTaskID; };
    double getPercentageComplete() const { return percentageComplete; };
    std::chrono::year_month_day getCreationDate() const { return creationDate; };
    std::chrono::year_month_day getDueDate() const { return dueDate; };
    std::chrono::year_month_day getScheduledStart() const { return scheduledStart; };
    std::chrono::year_month_day getactualStartDate() const;
    std::optional<std::chrono::year_month_day> rawActualStartDate() const { return actualStartDate; };
    std::chrono::year_month_day getEstimatedCompletion() const;
    std::optional<std::chrono::year_month_day> rawEstimatedCompletion() const { return estimatedCompletion; };
    std::chrono::year_month_day getCompletionDate() const ;
    std::optional<std::chrono::year_month_day> rawCompletionDate() const { return completionDate; };
    unsigned int getEstimatedEffort() const { return estimatedEffort; };
    double getactualEffortToDate() const { return actualEffortToDate; };
    unsigned int getPriorityGroup() const { return priorityGroup; };
    unsigned int getPriority() const { return priority; };
    std::vector<std::size_t> getDependencies() { return dependencies; };
    bool isPersonal() const { return personal; };
    void setCreatorID(std::size_t creatorID);
    void setCreatorID(UserModel_shp creator) { setCreatorID(creator->getUserID()); };
    void setAssignToID(std::size_t assignedID);
    void setAssignToID(UserModel_shp assignedUser) { setAssignToID(assignedUser->getUserID()); };
    void setDescription(std::string description);
    void setStatus(TaskModel::TaskStatus status);
    void setStatus(std::string statusStr) { setStatus(stringToStatus(statusStr)); };
    void setParentTaskID(std::size_t parentTaskID);
    void setParentTaskID(std::shared_ptr<TaskModel> parentTask) { setParentTaskID(parentTask->getTaskID()); };
    void setPercentageComplete(double percentComplete);
    void setCreationDate(std::chrono::year_month_day creationDate);
    void setDueDate(std::chrono::year_month_day dueDate);
    void setScheduledStart(std::chrono::year_month_day startDate);
    void setactualStartDate(std::chrono::year_month_day startDate);
    void setEstimatedCompletion(std::chrono::year_month_day completionDate);
    void setCompletionDate(std::chrono::year_month_day completionDate);
    void setEstimatedEffort(unsigned int estimatedHours);
    void setActualEffortToDate(double effortHoursYTD);
    void setPriorityGroup(unsigned int priorityGroup);
    void setPriorityGroupC(const char priorityGroup);
    void setPriority(unsigned int priority);
    void setPersonal(bool personalIn);
    void addDependency(std::size_t taskId);
    void addDependency(TaskModel& dependency) { addDependency(dependency.getTaskID()); };
    void addDependency(std::shared_ptr<TaskModel> dependency) { addDependency(dependency->getTaskID()); };
    void setTaskID(std::size_t newID);
    std::string taskStatusString() const;
    TaskModel::TaskStatus stringToStatus(std::string statusName) const;

    bool operator==(TaskModel& other)
    {
        return diffTask(other);
    }
    bool operator==(std::shared_ptr<TaskModel> other)
    {
        return diffTask(*other);
    }

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& task)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << "TaskModel:\n";
        os << std::format(outFmtStr, "Task ID", task.taskID);
        os << std::format(outFmtStr, "Creator ID", task.creatorID);
        os << std::format(outFmtStr, "Assigned To ID", task.assignToID);
        os << std::format(outFmtStr, "Description", task.description);
        os << std::format(outFmtStr, "Percentage Complete", task.percentageComplete);
        os << std::format(outFmtStr, "Creation Date", task.creationDate);
        os << std::format(outFmtStr, "Scheduled Start Date", task.scheduledStart);
        os << std::format(outFmtStr, "Due Date", task.dueDate);

        os << "Optional Fields\n";
        if (task.status.has_value())
        {
            os << std::format(outFmtStr, "Status", task.taskStatusString());
        }
        if (task.parentTaskID.has_value())
        {
            os << std::format(outFmtStr, "Parent ID", task.parentTaskID.value());
        }
        if (task.actualStartDate.has_value())
        {
            os << std::format(outFmtStr, "Actual Start Date", task.actualStartDate.value());
        }
        if (task.estimatedCompletion.has_value())
        {
            os << std::format(outFmtStr, "Estimated Completion Date", task.estimatedCompletion.value());
        }
        if (task.completionDate.has_value())
        {
            os << std::format(outFmtStr, "Completed Date", task.completionDate.value());
        }

        return os;
    };


private:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };
    bool diffTask(TaskModel& other);

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
    bool personal;
    std::vector<std::size_t> dependencies;

};

using TaskModel_shp = std::shared_ptr<TaskModel>;
using TaskList = std::vector<TaskModel_shp>;

#endif // TASKMODEL_H_


