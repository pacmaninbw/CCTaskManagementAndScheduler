#ifndef TASKMODEL_H_
#define TASKMODEL_H_

// Project Header Files
#include "commonUtilities.h"
#include "ModelDBInterface.h"

// Standard C++ Header Files
#include <chrono>
#include <format>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class TaskModel : public ModelDBInterface
{
public:
    enum class TaskStatus
    {
        Not_Started, On_Hold, Waiting_for_Dependency, Work_in_Progress, Complete
    };

    TaskModel();
    TaskModel(std::size_t creatorID);
    TaskModel(std::size_t creatorID, std::string descriptionIn);
    TaskModel(std::size_t taskId,
        std::size_t creatorID,
        std::size_t assignToID,
        std::string description,
        std::optional<TaskStatus> status,
        std::optional<std::size_t> parentTaskID,
        std::optional<std::chrono::year_month_day> dueDate,
        std::optional<std::chrono::year_month_day> scheduledStart,
        std::optional<std::chrono::year_month_day> actualStartDate,
        std::optional<std::chrono::year_month_day> estimatedCompletion,
        std::optional<std::chrono::year_month_day> completionDate,
        unsigned int estimatedEffort,
        double actualEffortToDate,
        unsigned int priorityGroup,
        unsigned int priority,
        bool personal,
        std::size_t dependencyCount,
        std::string dependencies,
        std::optional<std::chrono::system_clock::time_point> creationTimeStamp,
        std::optional<std::chrono::system_clock::time_point> lastUpdate,
        bool hidden = false
    );
    virtual ~TaskModel() = default;

    virtual bool hide(std::size_t userRequestingDelete) noexcept override;
    void addEffortHours(double hours);
    void markComplete()
    {
        setCompletionDate(getTodaysDate());
        setStatus(TaskModel::TaskStatus::Complete);
    }
    std::size_t getTaskID() const { return primaryKey; };
    std::size_t getCreatorID() const { return creatorID; };
    std::size_t getAssignToID() const { return assignToID; };
    std::string getDescription() const { return description; };
    TaskModel::TaskStatus getStatus() const { return status.value_or(TaskModel::TaskStatus::Not_Started); };
    unsigned int getStatusIntVal() const { return static_cast<unsigned int>(getStatus()); };
    std::string getStatusStringVal() const;
    std::size_t getParentTaskID() const { return parentTaskID.value_or(0); };
    std::optional<std::size_t> rawParentTaskID() const { return parentTaskID; };
    std::chrono::system_clock::time_point getCreationDate() const { return creationTimeStamp.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return lastUpdate.value(); };
    std::chrono::year_month_day getDueDate() const { return dueDate.value(); };
    std::chrono::year_month_day getScheduledStart() const { return scheduledStart.value(); };
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
    void setAssignToID(std::size_t assignedID);
    void setDescription(std::string description);
    void setStatus(TaskModel::TaskStatus status);
    void setStatus(std::string statusStr) { setStatus(stringToStatus(statusStr)); };
    void setParentTaskID(std::size_t parentTaskID);
    void setParentTaskID(std::shared_ptr<TaskModel> parentTask) { setParentTaskID(parentTask->getTaskID()); };
    void setCreationDate(std::chrono::system_clock::time_point creationDate);
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
    void setLastUpdate(std::chrono::system_clock::time_point lastUpdateTS);
    void addDependency(std::size_t taskId);
    void addDependency(TaskModel& dependency) { addDependency(dependency.getTaskID()); };
    void addDependency(std::shared_ptr<TaskModel> dependency) { addDependency(dependency->getTaskID()); };
    void setTaskID(std::size_t newID);
    std::string taskStatusString(TaskModel::TaskStatus inVal) const;
    TaskModel::TaskStatus stringToStatus(std::string statusName) const;

/*
 * Required fields.
 */
    bool isMissingDescription() const noexcept { return (description.empty() || description.length() < MinimumDescriptionLength); };
    bool isMissingCreatorID() const noexcept { return creatorID == 0; };
    bool isMissingAssignedID() const noexcept { return assignToID == 0; };
    bool isMissingEffortEstimate() const noexcept { return estimatedEffort == 0; };
    bool isMissingPriorityGroup() const noexcept { return priorityGroup == 0; };
    bool isMissingCreationDate() const noexcept { return !creationTimeStamp.has_value(); };
    bool isMissingScheduledStart() const noexcept { return !scheduledStart.has_value(); };
    bool isMissingDueDate() const noexcept { return !dueDate.has_value(); };

/*
 * Operators
 */
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
        os << std::format(outFmtStr, "Task ID", task.primaryKey);
        os << std::format(outFmtStr, "Creator ID", task.creatorID);
        os << std::format(outFmtStr, "Assigned To ID", task.assignToID);
        os << std::format(outFmtStr, "Description", task.description);
        os << std::format(outFmtStr, "Status", task.getStatusIntVal());
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", task.creationTimeStamp.value_or(std::chrono::system_clock::now()));
        }
        os << std::format(outFmtStr, "Scheduled Start Date", task.scheduledStart.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Due Date", task.dueDate.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Estimated Effort Hours", task.estimatedEffort);
        os << std::format(outFmtStr, "Actual Effort Hours", task.actualEffortToDate);
        os << std::format(outFmtStr, "Priority Group", task.priorityGroup);
        os << std::format(outFmtStr, "Priority", task.priority);
        os << std::format(outFmtStr, "Dependency Count", task.dependencies.size());

        os << "Optional Fields\n";
        if (task.status.has_value())
        {
            os << std::format(outFmtStr, "Status", task.getStatusStringVal());
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

        os << std::format(outFmtStr, "Deleted ", (task.deleted? "TRUE" : "FALSE"));

        return os;
    };


protected:
/*
 * Implementation
 */
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };
    bool diffTask(TaskModel& other);
    virtual std::string formatInsertStatement() override;
    virtual std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    void initRequiredFields() override;
    void addDependencies(const std::string& dependenciesText);
    std::string buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept;

 /*
  * Member Variables
  */
    std::size_t creatorID;
    std::size_t assignToID;
    std::string description;
    std::optional<TaskStatus> status;
    std::optional<std::size_t> parentTaskID;
    std::optional<std::chrono::system_clock::time_point> creationTimeStamp;
/*
 * dueDate and scheduledStart are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::year_month_day> dueDate;
    std::optional<std::chrono::year_month_day> scheduledStart;
    std::optional<std::chrono::year_month_day> actualStartDate;
    std::optional<std::chrono::year_month_day> estimatedCompletion;
    std::optional<std::chrono::year_month_day> completionDate;
    unsigned int estimatedEffort;
    double actualEffortToDate;
    unsigned int priorityGroup;
    unsigned int priority;
    bool personal;
    static const std::size_t MinimumDescriptionLength = 10;
    std::vector<std::size_t> dependencies;
    std::optional<std::chrono::system_clock::time_point> lastUpdate;
};

using TaskModel_shp = std::shared_ptr<TaskModel>;

#endif // TASKMODEL_H_


