// Project Header Files
#include "commonUtilities.h"
#include "GenericDictionary.h"
#include "TaskModel.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
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

void TaskModel::setCreationDate(std::chrono::system_clock::time_point inCreationDate)
{
    modified = true;
    creationTimeStamp = inCreationDate;
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
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE Description = {} AND AsignedTo = {}", description, assignedUserID);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

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
    errorMessages.clear();

    try
    {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, baseQuery);
        boost::mysql::format_sql_to(fctx, " WHERE TaskID = {}", taskID);

        boost::mysql::results localResult = runQueryAsync(std::move(fctx).get().value());

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
    errorMessages.clear();

    try {
        constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed IS NULL AND (Status IS NOT NULL AND Status <> {})",
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
    errorMessages.clear();

    try {
        constexpr unsigned int notStarted = static_cast<unsigned int>(TaskModel::TaskStatus::Not_Started);

        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND ScheduledStart < {} AND (Status IS NULL OR Status = {})",
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
    errorMessages.clear();

    try {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND Completed >= {}",
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
    errorMessages.clear();

    try {
        initFormatOptions();
        boost::mysql::format_context fctx(format_opts.value());
        boost::mysql::format_sql_to(fctx, listQueryBase);
        boost::mysql::format_sql_to(fctx, " WHERE AsignedTo = {} AND ParentTask = {}", assignedUserID, parentID);

        return std::move(fctx).get().value();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskModel::formatSelectTasksByAssignedIDandParentID({}) : {}", assignedUserID, e.what()));
    }

    return std::string();
}

bool TaskModel::runSelfTest()
{
    inSelfTest = true;

    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog << "Running TaskModel Self Test\n";
    }

    if (!testExceptionHandling())
    {
        std::clog << "Exception handling FAILED!\n";
        allSelfTestsPassed = false;
    }
    
    if (!testSave())
    {
        allSelfTestsPassed = false;
    }

    if (!testAccessorFunctionsPassed())
    {
        std::clog << "One or more get or set functions FAILED!\n";
        allSelfTestsPassed = false;
    }

    if (verboseOutput)
    {
        std::clog << "Test Ouput: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        std::clog << "Test of all insertion failures FAILED!\n";
    }

    inSelfTest = false;
    
    return allSelfTestsPassed;
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

    if (isMissingCreationDate())
    {
        creationTimeStamp = std::chrono::system_clock::now();
    }
    lastUpdate = std::chrono::system_clock::now();

    return boost::mysql::format_sql(format_opts.value(),
        "INSERT INTO Tasks (CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn, "
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup, Personal, DependencyCount, Dependencies, LastUpdateTS)"
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15}, {16}, {17}, {18}, {19})",
            creatorID,
            assignToID,
            description,
            parentTaskID,
            getStatusIntVal(),
            percentageComplete,
            optionalDateTimeConversion(creationTimeStamp),
            stdchronoDateToBoostMySQLDate(dueDate.value()),
            stdchronoDateToBoostMySQLDate(scheduledStart.value()),
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
            optionalDateTimeConversion(lastUpdate)
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

    return boost::mysql::format_sql(format_opts.value(),
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
            optionalDateTimeConversion(creationTimeStamp),
            stdchronoDateToBoostMySQLDate(dueDate.value()),
            stdchronoDateToBoostMySQLDate(scheduledStart.value()),
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
    errorMessages.clear();

    initFormatOptions();
    boost::mysql::format_context fctx(format_opts.value());
    boost::mysql::format_sql_to(fctx, baseQuery);
    boost::mysql::format_sql_to(fctx, " WHERE TaskID = {}", primaryKey);

    return std::move(fctx).get().value();
}

void TaskModel::initRequiredFields()
{
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingDescription, this), "description"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingCreatorID, this), "user ID for creator"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingAssignedID, this), "user ID for assigned user"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingEffortEstimate, this), "estimated effort in hours"});
    missingRequiredFieldsTests.push_back({std::bind(&TaskModel::isMissingPriorityGroup, this), "priority"});
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

void TaskModel::processResultRow(boost::mysql::row_view rv)
{
    // Required fields.
    primaryKey = rv.at(taskIdIdx).as_uint64();
    creatorID = rv.at(createdByIdx).as_uint64();
    assignToID = rv.at(assignedToIdx).as_uint64();
    description = rv.at(descriptionIdx).as_string();
    percentageComplete = rv.at(percentageCompleteIdx).as_double();
    creationTimeStamp = boostMysqlDateTimeToChronoTimePoint(rv.at(createdOnIdx).as_datetime());
    dueDate = boostMysqlDateToChronoDate(rv.at(requiredDeliveryIdx).as_date());
    scheduledStart = boostMysqlDateToChronoDate(rv.at(scheduledStartIdx).as_date());
    estimatedEffort = rv.at(estimatedEffortHoursIdx).as_uint64();
    actualEffortToDate = rv.at(actualEffortHoursIdx).as_double();
    priorityGroup = rv.at(schedulePriorityGroupIdx).as_uint64();
    priority = rv.at(priorityInGroupIdx).as_uint64();
    personal = rv.at(personalIdx).as_int64();
    lastUpdate = boostMysqlDateTimeToChronoTimePoint(rv.at(lastUpdate_Idx).as_datetime());


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
}

bool TaskModel::testExceptionHandling()
{
    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;
    forceException = true;
    std::vector<ExceptionTestElement> exceptionTests =
    {
        {std::bind(&TaskModel::testExceptionSelectByTaskID, this), "selectByTaskID"},
        {std::bind(&TaskModel::testExceptionSelectByDescriptionAndAssignedUser, this), "selectByDescriptionAndAssignedUser"},
        {std::bind(&TaskModel::testExceptionInsert, this), "testExceeptionInsert"},
        {std::bind(&TaskModel::testExceptionUpdate, this), "testExceptionUpdate"},
        {std::bind(&TaskModel::testExceptionFormatSelectActiveTasksForAssignedUser, this),
            "selectByDescriptionAndAssignedUser"},
        {std::bind(&TaskModel::testExceptionFormatSelectUnstartedDueForStartForAssignedUser, this),
            "formatSelectActiveTasksForAssignedUser"},
        {std::bind(&TaskModel::testExceptionFormatSelectTasksCompletedByAssignedAfterDate, this),
            "formatSelectTasksCompletedByAssignedAfterDate"},
        {std::bind(&TaskModel::testExceptionFormatSelectTasksByAssignedIDandParentID, this),
            "formatSelectTasksByAssignedIDandParentID"}
    };

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
}

bool TaskModel::testExceptionInsert()
{
    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();

    setDescription("Testing Exception handling");
    setEstimatedEffort(6);
    setScheduledStart(getTodaysDate());
    setDueDate(getTodaysDatePlus(2));
    setPriorityGroup('A');
    setPriority(1);
    setCreationDate(timeStamp);
    
    return insert() != true;
}

bool TaskModel::testExceptionUpdate()
{
    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();

    setTaskID(1);
    setDescription("Testing Exception handling");
    setEstimatedEffort(6);
    setScheduledStart(getTodaysDate());
    setDueDate(getTodaysDatePlus(2));
    setPriorityGroup('A');
    setPriority(1);
    setCreationDate(timeStamp);
    
    return update() != true;
}

bool TaskModel::testAccessorFunctionsPassed()
{
     bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests = 
    {
        {std::bind(&TaskModel::testTaskIdAccesss, this)},
        {std::bind(&TaskModel::testCreatorIDAccess, this)},
        {std::bind(&TaskModel::testAssignToIDAccess, this)},
        {std::bind(&TaskModel::testDescriptionAccess, this)},
//        {std::bind(&TaskModel::testStatusAccess, this)},
//        {std::bind(&TaskModel::testParentTaskIDAccess, this)},
        {std::bind(&TaskModel::testPercentageCompleteAccess, this)},
        {std::bind(&TaskModel::testCreationDateAccess, this)},
        {std::bind(&TaskModel::testDueDateAccess, this)},
        {std::bind(&TaskModel::testScheduledStartAccess, this)},
        {std::bind(&TaskModel::testActualStartDateAccess, this)},
        {std::bind(&TaskModel::testEstimatedCompletionAccess, this)},
        {std::bind(&TaskModel::testCompletionDateAccess, this)},
        {std::bind(&TaskModel::testEstimatedEffortAccess, this)},
        {std::bind(&TaskModel::testActualEffortToDateAccess, this)},
        {std::bind(&TaskModel::testPriorityGroupAccess, this)},
        {std::bind(&TaskModel::testPriorityGroupCAccess, this)},
        {std::bind(&TaskModel::testPriorityAccess, this)},
        {std::bind(&TaskModel::testPersonalAccess, this)},
    };

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool TaskModel::testTaskIdAccesss()
{
    std::size_t testPrimaryKey = 33;

    return testAccessorFunctions<std::size_t>(testPrimaryKey, &primaryKey, "Primary Key",
        std::bind(&TaskModel::setTaskID, this, std::placeholders::_1),
        std::bind(&TaskModel::getTaskID, this));
}

bool TaskModel::testCreatorIDAccess()
{
    std::size_t testValue = 1;

    return testAccessorFunctions<std::size_t>(testValue, &creatorID, "Creator User ID",
        std::bind(&TaskModel::setCreatorID, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreatorID, this));
}

bool TaskModel::testAssignToIDAccess()
{
    std::size_t testValue = 2;

    return testAccessorFunctions<std::size_t>(testValue, &assignToID, "Assigned User ID",
        std::bind(&TaskModel::setAssignToID, this, std::placeholders::_1),
        std::bind(&TaskModel::getAssignToID, this));
}

bool TaskModel::testDescriptionAccess()
{
    std::string testValue("Test task description access");
    return testAccessorFunctions<std::string>(testValue, &description, "Description",
        std::bind(&TaskModel::setDescription, this, std::placeholders::_1),
        std::bind(&TaskModel::getDescription, this));
}

bool TaskModel::testStatusAccess()
{
//    TaskStatus testValue = TaskModel::TaskStatus::On_Hold;
    return false;
}

bool TaskModel::testParentTaskIDAccess()
{
    return false;
}

bool TaskModel::testPercentageCompleteAccess()
{
    double testValue = 86.5;
    return testAccessorFunctions<double>(testValue, &percentageComplete, "Percentage Complete",
        std::bind(&TaskModel::setPercentageComplete, this, std::placeholders::_1),
        std::bind(&TaskModel::getPercentageComplete, this));
}

bool TaskModel::testCreationDateAccess()
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &creationTimeStamp, "Creation TimeStamp",
        std::bind(&TaskModel::setCreationDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getCreationDate, this));
}

bool TaskModel::testDueDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDate();
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &dueDate, "Due Date",
        std::bind(&TaskModel::setDueDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getDueDate, this));
}

bool TaskModel::testScheduledStartAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(OneWeek);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &dueDate, "Scheduled Start Date",
        std::bind(&TaskModel::setScheduledStart, this, std::placeholders::_1),
        std::bind(&TaskModel::getScheduledStart, this));
}

bool TaskModel::testActualStartDateAccess()
{
    std::chrono::year_month_day testValue = getTodaysDateMinus(2);
    return testOptionalAccessorFunctions<std::chrono::year_month_day>(testValue, &actualStartDate, "Actual Start Date",
        std::bind(&TaskModel::setactualStartDate, this, std::placeholders::_1),
        std::bind(&TaskModel::getactualStartDate, this));
}

bool TaskModel::testEstimatedEffortAccess()
{
    return false;
}

bool TaskModel::testCompletionDateAccess()
{
    return false;
}

bool TaskModel::testEstimatedCompletionAccess()
{
    return false;
}

bool TaskModel::testActualEffortToDateAccess()
{
    return false;
}

bool TaskModel::testPriorityGroupAccess()
{
    return false;
}

bool TaskModel::testPriorityGroupCAccess()
{
    return false;
}

bool TaskModel::testPriorityAccess()
{
    return false;
}

bool TaskModel::testPersonalAccess()
{
    return false;
}
