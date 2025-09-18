#ifndef TASKMODEL_H_
#define TASKMODEL_H_

#include <chrono>
#include "commonUtilities.h"
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include "ModelDBInterface.h"
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
    virtual ~TaskModel() = default;

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
    void setAssignToID(std::size_t assignedID);
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
/*
 * Select with arguments
 */
    bool selectByDescriptionAndAssignedUser(std::string_view description, std::size_t assignedUserID);
    bool selectByTaskID(std::size_t taskID);
    // Return multiple Tasks.
    std::string formatSelectActiveTasksForAssignedUser(std::size_t assignedUserID);
    std::string formatSelectUnstartedDueForStartForAssignedUser(std::size_t assignedUserID);
    std::string formatSelectTasksCompletedByAssignedAfterDate(std::size_t assignedUserID,
        std::chrono::year_month_day& searchStartDate);
    std::string formatSelectTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID);

/*
 * Required fields.
 */
    bool isMissingDescription() { return (description.empty() || description.length() < MinimumDescriptionLength); };
    bool isMissingCreatorID() { return creatorID == 0; };
    bool isMissingAssignedID() { return assignToID == 0; };
    bool isMissingEffortEstimate() { return estimatedEffort == 0; };
    bool isMissingPriorityGroup() { return priorityGroup == 0; };
    bool isMissingCreationDate() { return !creationDate.ok(); };
    bool isMissingScheduledStart() { return !scheduledStart.ok(); };
    bool isMissingDueDate() { return !dueDate.ok(); };


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
        os << std::format(outFmtStr, "Percentage Complete", task.percentageComplete);
        os << std::format(outFmtStr, "Creation Date", task.creationDate);
        os << std::format(outFmtStr, "Scheduled Start Date", task.scheduledStart);
        os << std::format(outFmtStr, "Due Date", task.dueDate);
        os << std::format(outFmtStr, "Estimated Effort Hours", task.estimatedEffort);
        os << std::format(outFmtStr, "Actual Effort Hours", task.actualEffortToDate);
        os << std::format(outFmtStr, "Priority Group", task.priorityGroup);
        os << std::format(outFmtStr, "Priority", task.priority);

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
    std::string formatInsertStatement() override;
    std::string formatUpdateStatement() override;
    std::string formatSelectStatement() override;
    void initRequiredFields() override;
    void addDependencies(const std::string& dependenciesText);
    std::string buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept;
    void processResultRow(NSBM::row_view rv) override;

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
    const std::size_t MinimumDescriptionLength = 10;
    std::vector<std::size_t> dependencies;

/*
 * The indexes below are based on the following select statement, maintain this order
 * baseQuery could be SELECT * FROM Tasks, but this way the order of the columns
 * returned are known.
 */
    NSBM::constant_string_view baseQuery = "SELECT TaskID, CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount, Dependencies FROM Tasks ";

    const std::size_t taskIdIdx = 0;
    const std::size_t createdByIdx = 1;
    const std::size_t assignedToIdx = 2;
    const std::size_t descriptionIdx = 3;
    const std::size_t parentTaskIdx = 4;
    const std::size_t statusIdx = 5;
    const std::size_t percentageCompleteIdx = 6;
    const std::size_t createdOnIdx = 7;
    const std::size_t requiredDeliveryIdx = 8;
    const std::size_t scheduledStartIdx = 9;
    const std::size_t actualStartIdx = 10;
    const std::size_t estimatedCompletionIdx = 11;
    const std::size_t completedIdx = 12;
    const std::size_t estimatedEffortHoursIdx = 13;
    const std::size_t actualEffortHoursIdx = 14;
    const std::size_t schedulePriorityGroupIdx = 15;
    const std::size_t priorityInGroupIdx = 16;
    const std::size_t personalIdx = 17;
    const std::size_t dependencyCountIdx = 18;
    const std::size_t depenedenciesTextIdx = 19;

    NSBM::constant_string_view listQueryBase = "SELECT TaskID FROM Tasks ";
};

using TaskModel_shp = std::shared_ptr<TaskModel>;

#endif // TASKMODEL_H_


