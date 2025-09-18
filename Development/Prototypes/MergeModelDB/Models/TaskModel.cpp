#include <chrono>
#include "commonUtilities.h"
#include <functional>
#include "GenericDictionary.h"
#include <iostream>
#include <memory>
#include <string>
#include "TaskModel.h"
//#include "UserModel.h"
#include <vector>

static const TaskModel::TaskStatus UnknowStatus = static_cast<TaskModel::TaskStatus>(-1);

static std::vector<GenericDictionary<TaskModel::TaskStatus, std::string>::DictType> statusConversionsDefs = {
    {TaskModel::TaskStatus::Not_Started, "Not Started"},
    {TaskModel::TaskStatus::On_Hold, "On Hold"},
    {TaskModel::TaskStatus::Waiting_for_Dependency, "Waiting for Dependency"},
    {TaskModel::TaskStatus::Work_in_Progress, "Work in Progress"},
    {TaskModel::TaskStatus::Complete, "Completed"}
};

static GenericDictionary<TaskModel::TaskStatus, std::string> taskStatusConversionTable(statusConversionsDefs);

TaskModel::TaskModel()
: ModelDBInterface("Task")
{
  creatorID = 0;
  assignToID = 0;
  description = "";
  percentageComplete = 0.0;
  estimatedEffort = 0;
  actualEffortToDate = 0.0;
  priorityGroup = 0;
  priority = 0;
  personal = false;
}

TaskModel::TaskModel(std::size_t creatorID)
: TaskModel()
{
    setCreatorID(creatorID);
    setAssignToID(creatorID);
}

TaskModel::TaskModel(std::size_t creatorID, std::string description)
: TaskModel()
{
    setCreatorID(creatorID);
    setAssignToID(creatorID);
    setDescription(description);
}

void TaskModel::addEffortHours(double hours)
{
    double actualEffortHours = getactualEffortToDate();
    actualEffortHours += hours;
    setActualEffortToDate(actualEffortHours);
}

std::chrono::year_month_day TaskModel::getactualStartDate() const
{
    return actualStartDate.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getEstimatedCompletion() const
{
    return estimatedCompletion.value_or(std::chrono::year_month_day());
}

std::chrono::year_month_day TaskModel::getCompletionDate() const
{
    return completionDate.value_or(std::chrono::year_month_day());
}

void TaskModel::setCreatorID(std::size_t inCreatorID)
{
    modified = true;
    creatorID = inCreatorID;
}

void TaskModel::setAssignToID(std::size_t inAssignedID)
{
    modified = true;
    assignToID = inAssignedID;
}

void TaskModel::setDescription(std::string inDescription)
{
    modified = true;
    description = inDescription;
}

void TaskModel::setStatus(TaskModel::TaskStatus inStatus)
{
    modified = true;
    status = inStatus;
}

void TaskModel::setParentTaskID(std::size_t inParentTaskID)
{
    modified = true;
    parentTaskID = inParentTaskID;
}

void TaskModel::setPercentageComplete(double inPercentComplete)
{
    modified = true;
    percentageComplete = inPercentComplete;
}

void TaskModel::setCreationDate(std::chrono::year_month_day inCreationDate)
{
    modified = true;
    creationDate = inCreationDate;
}

void TaskModel::setDueDate(std::chrono::year_month_day inDueDate)
{
    modified = true;
    dueDate = inDueDate;
}

void TaskModel::setScheduledStart(std::chrono::year_month_day startDate)
{
    modified = true;
    scheduledStart = startDate;
}

void TaskModel::setactualStartDate(std::chrono::year_month_day startDate)
{
    modified = true;
    actualStartDate = startDate;
}

void TaskModel::setEstimatedCompletion(std::chrono::year_month_day completionDate)
{
    modified = true;
    estimatedCompletion = completionDate;
}

void TaskModel::setCompletionDate(std::chrono::year_month_day inCompletionDate)
{
    modified = true;
    completionDate = inCompletionDate;
}

void TaskModel::setEstimatedEffort(unsigned int inEstimatedHours)
{
    modified = true;
    estimatedEffort = inEstimatedHours;
}

void TaskModel::setActualEffortToDate(double effortHoursYTD)
{
    modified = true;
    actualEffortToDate = effortHoursYTD;
}

void TaskModel::setPriorityGroup(unsigned int inPriorityGroup)
{
    modified = true;
    priorityGroup = inPriorityGroup;
}

void TaskModel::setPriorityGroupC(const char priorityGroup)
{
    unsigned int group = priorityGroup - 'A' + 1;
    setPriorityGroup(group);
}

void TaskModel::setPriority(unsigned int inPriority)
{
    modified = true;
    priority = inPriority;
}

void TaskModel::setPersonal(bool personalIn)
{
    modified = true;
    personal = personalIn;
}

void TaskModel::addDependency(std::size_t taskId)
{
    modified = true;
    dependencies.push_back(taskId);
}

void TaskModel::setTaskID(std::size_t newID)
{
    modified = true;
    primaryKey = newID;
}

bool TaskModel::selectByDescriptionAndAssignedUser(std::string_view description, std::size_t assignedUserID)
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE Description = {} AND AsignedTo = {}", description, assignedUserID);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::selectByDescriptionAndAssignedUser({}) : {}", description, e.what()));
        return false;
    }
}

bool TaskModel::selectByTaskID(std::size_t taskID)
{
    prepareForRunQueryAsync();

    try
    {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, baseQuery);
        NSBM::format_sql_to(fctx, " WHERE TaskID = {}", taskID);

        NSBM::results localResult = runQueryAsync(std::move(fctx).get().value());

        return processResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::selectByTaskID({}) : {}", taskID, e.what()));
        return false;
    }
}

std::string TaskModel::formatSelectActiveTasksForAssignedUser(std::size_t assignedUserID)
{
    prepareForRunQueryAsync();

    try {
        constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, listQueryBase);
        NSBM::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed IS NULL AND (Status IS NOT NULL AND Status <> {})",
            assignedUserID, stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)), notStarted);

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::formatSelectUnstartedDueForStartForAssignedUser({}) : {}", assignedUserID, e.what()));
    }

    return std::string();
}

std::string TaskModel::formatSelectUnstartedDueForStartForAssignedUser(std::size_t assignedUserID)
{
    prepareForRunQueryAsync();

    try {
        constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, listQueryBase);
        NSBM::format_sql_to(fctx, " WHERE AsignedTo = {} AND ScheduledStart < {} AND (Status IS NULL OR Status = {})",
            assignedUserID, stdchronoDateToBoostMySQLDate(getTodaysDatePlus(OneWeek)), notStarted);

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::formatSelectUnstartedDueForStartForAssignedUser({}) : {}", assignedUserID, e.what()));
    }

    return std::string();
}

std::string TaskModel::formatSelectTasksCompletedByAssignedAfterDate(std::size_t assignedUserID, std::chrono::year_month_day& searchStartDate)
{
    prepareForRunQueryAsync();

    try {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, listQueryBase);
        NSBM::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed >= {}",
            assignedUserID, stdchronoDateToBoostMySQLDate(searchStartDate));

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::formatSelectTasksCompletedByAssignedAfterDate({}) : {}", assignedUserID, e.what()));
    }

    return std::string();
}

std::string TaskModel::formatSelectTasksByAssignedIDandParentID(std::size_t assignedUserID, std::size_t parentID)
{
    prepareForRunQueryAsync();

    try {
        NSBM::format_context fctx(format_opts.value());
        NSBM::format_sql_to(fctx, listQueryBase);
        NSBM::format_sql_to(fctx, " WHERE AsignedTo = {} AND ParentTask = {}", assignedUserID, parentID);

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::formatSelectTasksByAssignedIDandParentID({}) : {}", assignedUserID, e.what()));
    }

    return std::string();
}

std::string TaskModel::taskStatusString() const
{
    TaskModel::TaskStatus status = getStatus();
    auto statusName = taskStatusConversionTable.lookupName(status);
    return statusName.has_value()? *statusName : "Unknown TaskStatus Value";
}

TaskModel::TaskStatus TaskModel::stringToStatus(std::string statusName) const
{
    auto status = taskStatusConversionTable.lookupID(statusName);
    return status.has_value()? *status : UnknowStatus;
}

bool TaskModel::diffTask(TaskModel& other)
{
    // Ignoring optional fields
    return (primaryKey == other.primaryKey &&
        description == other.description &&
        other.creatorID == creatorID &&
        assignToID == other.assignToID &&
        percentageComplete == other.percentageComplete &&
        creationDate == other.creationDate &&
        dueDate == other.dueDate &&
        scheduledStart == other.scheduledStart &&
        scheduledStart == other.scheduledStart &&
        actualEffortToDate == other.actualEffortToDate &&
        priorityGroup == other.priorityGroup &&
        priority == other.priority &&
        personal == other.personal &&
        dependencies.size() == other.dependencies.size()
    );
}

std::string TaskModel::formatInsertStatement()
{
    std::size_t dependencyCount = getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    return NSBM::format_sql(format_opts.value(),
        "INSERT INTO Tasks (CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount, Dependencies)"
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {18})",
            creatorID,
            assignToID,
            description,
            parentTaskID,
            getStatusIntVal(),
            percentageComplete,
            stdchronoDateToBoostMySQLDate(creationDate),
            stdchronoDateToBoostMySQLDate(dueDate),
            stdchronoDateToBoostMySQLDate(scheduledStart),
            optionalDateConversion(actualStartDate),
            optionalDateConversion(estimatedCompletion),
            optionalDateConversion(completionDate),
            estimatedEffort,
            actualEffortToDate,
            priorityGroup,
            priority,
            personal,
            dependencyCount,
            depenenciesText
    );
}

std::string TaskModel::formatUpdateStatement()
{
    std::size_t dependencyCount = getDependencies().size();
    std::optional<std::string> depenenciesText;
    if (dependencyCount)
    {
        std::vector<std::size_t> dependencyList = getDependencies();
        depenenciesText = buildDependenciesText(dependencyList);
    }

    return NSBM::format_sql(format_opts.value(),
        "UPDATE Tasks SET"
            " CreatedBy = {0},"
            " AsignedTo = {1},"
            " Description = {2},"
            " ParentTask = {3},"
            " Status = {4},"
            " PercentageComplete = {5},"
            " CreatedOn = {6},"
            " RequiredDelivery = {7},"
            " ScheduledStart = {8},"
            " ActualStart = {9},"
            " EstimatedCompletion = {10},"
            " Completed = {11},"
            " EstimatedEffortHours = {12},"
            " ActualEffortHours = {13},"
            " SchedulePriorityGroup = {14},"
            " PriorityInGroup = {15},"
            " Personal = {16},"
            " DependencyCount = {17},"
            "Dependencies = {18}"
        " WHERE TaskID = {19} ",
            creatorID,
            assignToID,
            description,
            parentTaskID,
            getStatusIntVal(),
            percentageComplete,
            stdchronoDateToBoostMySQLDate(creationDate),
            stdchronoDateToBoostMySQLDate(dueDate),
            stdchronoDateToBoostMySQLDate(scheduledStart),
            optionalDateConversion(actualStartDate),
            optionalDateConversion(estimatedCompletion),
            optionalDateConversion(completionDate),
            estimatedEffort,
            actualEffortToDate,
            priorityGroup,
            priority,
            personal,
            dependencyCount,
            depenenciesText,
        primaryKey
    );}

std::string TaskModel::formatSelectStatement()
{
    prepareForRunQueryAsync();

    NSBM::format_context fctx(format_opts.value());
    NSBM::format_sql_to(fctx, baseQuery);
    NSBM::format_sql_to(fctx, " WHERE TaskID = {}", primaryKey);

    return std::move(fctx).get().value();
}

void TaskModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDescription, this), "description"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingCreatorID, this), "user ID for creator"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingAssignedID, this), "user ID for assigned user"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingEffortEstimate, this), "estimated effort in hours"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingPriorityGroup, this), "priority"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingCreationDate, this), "date of creation"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingScheduledStart, this), "scheduled start date"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDueDate, this), "due date (deadline)"});
}

void TaskModel::addDependencies(const std::string & dependenciesText)
{
    std::vector<std::string> dependencyStrings = explodeTextField(dependenciesText);

    if (!dependencyStrings.empty())
    {
        for (auto dependencyStr: dependencyStrings)
        {
            dependencies.push_back(static_cast<std::size_t>(std::stol(dependencyStr)));
        }
    }
    else
    {
        std::runtime_error NoExpectedDependencies("Dependencies expected but not found!");
        throw NoExpectedDependencies;
    }
}

std::string TaskModel::buildDependenciesText(std::vector<std::size_t>& dependencyList) noexcept
{
    if (dependencyList.size() > 1)
    {
        std::sort(dependencyList.begin(), dependencyList.end());
    }

    std::vector<std::string> dependencyStrings;
    for (auto dependency: dependencyList)
    {
        dependencyStrings.push_back(std::to_string(dependency));
    }

    return implodeTextField(dependencyStrings);
}

void TaskModel::processResultRow(NSBM::row_view rv)
{
    // Required fields.
    primaryKey = rv.at(taskIdIdx).as_uint64();
    creatorID = rv.at(createdByIdx).as_uint64();
    assignToID = rv.at(assignedToIdx).as_uint64();
    description = rv.at(descriptionIdx).as_string();
    percentageComplete = rv.at(percentageCompleteIdx).as_double();
    creationDate = boostMysqlDateToChronoDate(rv.at(createdOnIdx).as_date());
    dueDate = boostMysqlDateToChronoDate(rv.at(requiredDeliveryIdx).as_date());
    scheduledStart = boostMysqlDateToChronoDate(rv.at(scheduledStartIdx).as_date());
    estimatedEffort = rv.at(estimatedEffortHoursIdx).as_uint64();
    actualEffortToDate = rv.at(actualEffortHoursIdx).as_double();
    priorityGroup = rv.at(schedulePriorityGroupIdx).as_uint64();
    priority = rv.at(priorityInGroupIdx).as_uint64();
    personal = rv.at(personalIdx).as_int64();

    // Optional fields.
    if (!rv.at(parentTaskIdx).is_null())
    {
        parentTaskID = rv.at(parentTaskIdx).as_uint64();
    }

    if (!rv.at(statusIdx).is_null())
    {
        setStatus(static_cast<TaskModel::TaskStatus>(rv.at(statusIdx).as_uint64()));
    }

    if (!rv.at(actualStartIdx).is_null())
    {
        actualStartDate = boostMysqlDateToChronoDate(rv.at(actualStartIdx).as_date());
    }

    if (!rv.at(estimatedCompletionIdx).is_null())
    {
        estimatedCompletion = boostMysqlDateToChronoDate(rv.at(estimatedCompletionIdx).as_date());
    }

    if (!rv.at(completedIdx).is_null())
    {
        completionDate = boostMysqlDateToChronoDate(rv.at(completedIdx).as_date());
    }

    std::size_t dependencyCount = rv.at(dependencyCountIdx).as_uint64();
    if (dependencyCount > 0)
    {
        std::string dependenciesText = rv.at(depenedenciesTextIdx).as_string();
        addDependencies(dependenciesText);
    }

    // All the set functions set modified, since this user is new in memory it is not modified.
    modified = false;

}

