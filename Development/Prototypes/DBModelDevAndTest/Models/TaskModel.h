#ifndef TASKMODEL_H_
#define TASKMODEL_H_
#include "CommandLineParser.h"

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
    TaskModel(std::size_t creatorID, std::string description);
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
    std::size_t getTaskID() const { return m_PrimaryKey; };
    std::size_t getCreatorID() const { return m_CreatorID; };
    std::size_t getAssignToID() const { return m_AssignToID; };
    std::string getDescription() const { return m_Description; };
    TaskModel::TaskStatus getStatus() const { return m_Status.value_or(TaskModel::TaskStatus::Not_Started); };
    unsigned int getStatusIntVal() const { return static_cast<unsigned int>(getStatus()); };
    std::string getStatusStringVal() const;
    std::size_t getParentTaskID() const { return m_ParentTaskID.value_or(0); };
    std::optional<std::size_t> rawParentTaskID() const { return m_ParentTaskID; };
    std::chrono::system_clock::time_point getCreationDate() const { return m_Created.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return m_LastUpdate.value(); };
    std::chrono::year_month_day getDueDate() const { return m_DueDate.value(); };
    std::chrono::year_month_day getScheduledStart() const { return m_PlanedStart.value(); };
    std::chrono::year_month_day getactualStartDate() const;
    std::optional<std::chrono::year_month_day> rawActualStartDate() const { return m_ActualStart; };
    std::chrono::year_month_day getEstimatedCompletion() const;
    std::optional<std::chrono::year_month_day> rawEstimatedCompletion() const { return m_EstimatedCompletion; };
    std::chrono::year_month_day getCompletionDate() const ;
    std::optional<std::chrono::year_month_day> rawCompletionDate() const { return m_Completed; };
    unsigned int getEstimatedEffort() const { return m_EstimatedEffort; };
    double getactualEffortToDate() const { return m_ActualEffort; };
    unsigned int getPriorityGroup() const { return m_PriorityCategory; };
    unsigned int getPriority() const { return m_Priority; };
    std::vector<std::size_t> getDependencies() { return m_Dependencies; };
    bool isPersonal() const { return m_Personal; };
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
    void setTaskID(std::size_t taskID);
    std::string taskStatusString(TaskModel::TaskStatus status) const;
    TaskModel::TaskStatus stringToStatus(std::string statusName) const;

/*
 * Required fields.
 */
    bool isMissingDescription() const noexcept { return (m_Description.empty() || m_Description.length() < MinimumDescriptionLength); };
    bool isMissingCreatorID() const noexcept { return m_CreatorID == 0; };
    bool isMissingAssignedID() const noexcept { return m_AssignToID == 0; };
    bool isMissingEffortEstimate() const noexcept { return m_EstimatedEffort == 0; };
    bool isMissingPriorityGroup() const noexcept { return m_PriorityCategory == 0; };
    bool isMissingCreationDate() const noexcept { return !m_Created.has_value(); };
    bool isMissingScheduledStart() const noexcept { return !m_PlanedStart.has_value(); };
    bool isMissingDueDate() const noexcept { return !m_DueDate.has_value(); };

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
        os << std::format(outFmtStr, "Task ID", task.m_PrimaryKey);
        os << std::format(outFmtStr, "Creator ID", task.m_CreatorID);
        os << std::format(outFmtStr, "Assigned To ID", task.m_AssignToID);
        os << std::format(outFmtStr, "Description", task.m_Description);
        os << std::format(outFmtStr, "Status", task.getStatusIntVal());
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", task.m_Created.value_or(std::chrono::system_clock::now()));
        }
        os << std::format(outFmtStr, "Scheduled Start Date", task.m_PlanedStart.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Due Date", task.m_DueDate.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Estimated Effort Hours", task.m_EstimatedEffort);
        os << std::format(outFmtStr, "Actual Effort Hours", task.m_ActualEffort);
        os << std::format(outFmtStr, "Priority Group", task.m_PriorityCategory);
        os << std::format(outFmtStr, "Priority", task.m_Priority);
        os << std::format(outFmtStr, "Dependency Count", task.m_Dependencies.size());

        os << "Optional Fields\n";
        if (task.m_Status.has_value())
        {
            os << std::format(outFmtStr, "Status", task.getStatusStringVal());
        }
        if (task.m_ParentTaskID.has_value())
        {
            os << std::format(outFmtStr, "Parent ID", task.m_ParentTaskID.value());
        }
        if (task.m_ActualStart.has_value())
        {
            os << std::format(outFmtStr, "Actual Start Date", task.m_ActualStart.value());
        }
        if (task.m_EstimatedCompletion.has_value())
        {
            os << std::format(outFmtStr, "Estimated Completion Date", task.m_EstimatedCompletion.value());
        }
        if (task.m_Completed.has_value())
        {
            os << std::format(outFmtStr, "Completed Date", task.m_Completed.value());
        }

        os << std::format(outFmtStr, "Deleted ", (task.m_Deleted? "TRUE" : "FALSE"));

        return os;
    };


protected:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };
    bool diffTask(TaskModel& other);
    virtual std::string formatInsertStatement() override;
    virtual std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    void initRequiredFields() override;
    void addDependencies(const std::string& dependenciesText);
    std::string buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept;

    static const std::size_t MinimumDescriptionLength = 10;

    std::size_t m_CreatorID;
    std::size_t m_AssignToID;
    std::string m_Description;
    std::optional<TaskStatus> m_Status;
    std::optional<std::size_t> m_ParentTaskID;
    std::optional<std::chrono::system_clock::time_point> m_Created;
/*
 * dueDate and scheduledStart are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::year_month_day> m_DueDate;
    std::optional<std::chrono::year_month_day> m_PlanedStart;
    std::optional<std::chrono::year_month_day> m_ActualStart;
    std::optional<std::chrono::year_month_day> m_EstimatedCompletion;
    std::optional<std::chrono::year_month_day> m_Completed;
    unsigned int m_EstimatedEffort;
    double m_ActualEffort;
    unsigned int m_PriorityCategory;
    unsigned int m_Priority;
    bool m_Personal;
    std::vector<std::size_t> m_Dependencies;
    std::optional<std::chrono::system_clock::time_point> m_LastUpdate;
};

using TaskModel_shp = std::shared_ptr<TaskModel>;

#endif // TASKMODEL_H_


