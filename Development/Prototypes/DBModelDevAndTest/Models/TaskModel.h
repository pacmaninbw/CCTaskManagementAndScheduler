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

/*
 * Used to retrieve values from the database by the boost library. The names
 * of the variables in the struct must match the names of the columns in
 * the user goal table in the database because the boost reflection library is 
 * used to retrieve them.
 */
struct TaskDbQueryValues
{
    std::int64_t task_id;
    std::uint64_t created_by;
    std::uint64_t assigned_to;
    std::string description;
    std::optional<std::uint64_t> task_status;
    boost::mysql::datetime creation_timestamp;
    boost::mysql::date due_date;
    boost::mysql::date planned_start;
    std::optional<boost::mysql::date> actual_start;
    std::optional<boost::mysql::date> estimated_delivery;
    std::optional<boost::mysql::date> delivered;
    double est_hours_effort;
    double hours_effort;
    std::uint64_t priority_category;
    std::uint64_t priority;
    std::int64_t personal;
    boost::mysql::datetime last_modified_time_stamp;
    std::optional<std::uint64_t> dependent_task;
    std::int64_t deleted;
};

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
    TaskModel(const TaskDbQueryValues& dbTranslator);
    virtual ~TaskModel() = default;

    virtual bool insert() noexcept override;
    virtual bool update() noexcept override;
    virtual bool hide(std::size_t userRequestingDelete) noexcept override;
    void addEffortHours(double hours);
    void markComplete()
    {
        setCompletionDate(getTodaysDate());
        setStatus(TaskModel::TaskStatus::Complete);
    }
    std::size_t getTaskID() const { return m_primaryKey; };
    std::size_t getCreatorID() const { return m_creatorID; };
    std::size_t getAssignToID() const { return m_assignToID; };
    std::string getDescription() const { return m_description; };
    TaskModel::TaskStatus getStatus() const { return m_status.value_or(TaskModel::TaskStatus::Not_Started); };
    unsigned int getStatusIntVal() const { return static_cast<unsigned int>(getStatus()); };
    std::string getStatusStringVal() const;
    std::size_t getParentTaskID() const { return m_parentTaskID.value_or(0); };
    std::optional<std::size_t> rawParentTaskID() const { return m_parentTaskID; };
    std::chrono::system_clock::time_point getCreationDate() const { return m_created.value(); };
    std::chrono::system_clock::time_point getLastUpdate() const { return m_lastUpdate.value(); };
    std::chrono::year_month_day getDueDate() const { return m_dueDate.value(); };
    std::chrono::year_month_day getScheduledStart() const { return m_planedStart.value(); };
    std::chrono::year_month_day getactualStartDate() const;
    std::optional<std::chrono::year_month_day> rawActualStartDate() const { return m_actualStart; };
    std::chrono::year_month_day getEstimatedCompletion() const;
    std::optional<std::chrono::year_month_day> rawEstimatedCompletion() const { return m_estimatedCompletion; };
    std::chrono::year_month_day getCompletionDate() const ;
    std::optional<std::chrono::year_month_day> rawCompletionDate() const { return m_completed; };
    double getEstimatedEffort() const { return m_estimatedEffort; };
    double getactualEffortToDate() const { return m_actualEffort; };
    unsigned int getPriorityGroup() const { return m_priorityCategory; };
    unsigned int getPriority() const { return m_priority; };
    bool isPersonal() const { return m_personal; };
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
    void setEstimatedEffort(double estimatedHours);
    void setActualEffortToDate(double effortHoursYTD);
    void setPriorityGroup(unsigned int priorityGroup);
    void setPriorityGroupC(const char priorityGroup);
    void setPriority(unsigned int priority);
    void setPersonal(bool personalIn);
    void setLastUpdate(std::chrono::system_clock::time_point lastUpdateTS);
    void setTaskID(std::size_t taskID);
    std::string taskStatusString(TaskModel::TaskStatus status) const;
    TaskModel::TaskStatus stringToStatus(std::string statusName) const;

/*
 * Required fields.
 */
    bool isMissingDescription() const noexcept { return (m_description.empty() || m_description.length() < MinimumDescriptionLength); };
    bool isMissingCreatorID() const noexcept { return m_creatorID == 0; };
    bool isMissingAssignedID() const noexcept { return m_assignToID == 0; };
    bool isMissingEffortEstimate() const noexcept { return m_estimatedEffort == 0.0; };
    bool isMissingPriorityGroup() const noexcept { return m_priorityCategory == 0; };
    bool isMissingCreationDate() const noexcept { return !m_created.has_value(); };
    bool isMissingScheduledStart() const noexcept { return !m_planedStart.has_value(); };
    bool isMissingDueDate() const noexcept { return !m_dueDate.has_value(); };

/*
 * Operators
 */
    bool operator==(TaskModel& other)
    {
        return diffTask(other);
    }

    friend std::ostream& operator<<(std::ostream& os, const TaskModel& task)
    {
        constexpr const char* outFmtStr = "\t{}: {}\n";
        os << "TaskModel:\n";
        os << std::format(outFmtStr, "Task ID", task.m_primaryKey);
        os << std::format(outFmtStr, "Creator ID", task.m_creatorID);
        os << std::format(outFmtStr, "Assigned To ID", task.m_assignToID);
        os << std::format(outFmtStr, "Description", task.m_description);
        os << std::format(outFmtStr, "Status", task.getStatusIntVal());
        if (programOptions.showTimeStamps)
        {
            os << std::format(outFmtStr, "Creation Date", task.m_created.value_or(std::chrono::system_clock::now()));
        }
        os << std::format(outFmtStr, "Scheduled Start Date", task.m_planedStart.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Due Date", task.m_dueDate.value_or(getTodaysDate()));
        os << std::format(outFmtStr, "Estimated Effort Hours", task.m_estimatedEffort);
        os << std::format(outFmtStr, "Actual Effort Hours", task.m_actualEffort);
        os << std::format(outFmtStr, "Priority Group", task.m_priorityCategory);
        os << std::format(outFmtStr, "Priority", task.m_priority);

        os << "Optional Fields\n";
        if (task.m_status.has_value())
        {
            os << std::format(outFmtStr, "Status", task.getStatusStringVal());
        }
        if (task.m_parentTaskID.has_value())
        {
            os << std::format(outFmtStr, "Parent ID", task.m_parentTaskID.value());
        }
        if (task.m_actualStart.has_value())
        {
            os << std::format(outFmtStr, "Actual Start Date", task.m_actualStart.value());
        }
        if (task.m_estimatedCompletion.has_value())
        {
            os << std::format(outFmtStr, "Estimated Completion Date", task.m_estimatedCompletion.value());
        }
        if (task.m_completed.has_value())
        {
            os << std::format(outFmtStr, "Completed Date", task.m_completed.value());
        }

        os << std::format(outFmtStr, "Deleted ", (task.m_deleted? "TRUE" : "FALSE"));

        return os;
    };


protected:
    TaskStatus statusFromInt(unsigned int statusI) const { return static_cast<TaskModel::TaskStatus>(statusI); };
    bool diffTask(TaskModel& other);
    virtual std::string formatInsertStatement() override;
    virtual std::string formatUpdateStatement() override;
    virtual std::string formatDeleteStatement() override;
    void initRequiredFields() override;
    void insertDependency();
    void updateDependency();

    static const std::size_t MinimumDescriptionLength = 10;

    std::size_t m_creatorID;
    std::size_t m_assignToID;
    std::string m_description;
    std::optional<TaskStatus> m_status;
    std::optional<std::size_t> m_parentTaskID;
    std::optional<std::chrono::system_clock::time_point> m_created;
/*
 * dueDate and scheduledStart are not optional in the database, We are using
 * std::optional for those 2 fields to remove errors in valgrind and possible
 * exceptions in some compilers.
 */
    std::optional<std::chrono::year_month_day> m_dueDate;
    std::optional<std::chrono::year_month_day> m_planedStart;
    std::optional<std::chrono::year_month_day> m_actualStart;
    std::optional<std::chrono::year_month_day> m_estimatedCompletion;
    std::optional<std::chrono::year_month_day> m_completed;
    double m_estimatedEffort;
    double m_actualEffort;
    unsigned int m_priorityCategory;
    unsigned int m_priority;
    bool m_personal;
    std::optional<std::chrono::system_clock::time_point> m_lastUpdate;
};

using TaskModel_shp = std::shared_ptr<TaskModel>;

#endif // TASKMODEL_H_


