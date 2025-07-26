#include <boost/asio.hpp>
#include <boost/mysql.hpp>
#include <chrono>
#include "CommandLineParser.h"
#include "DBInterface.h"
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include "TaskDbInterface.h"
#include "TaskModel.h"
#include "UserDbInterface.h"
#include "UserModel.h"
#include <utility>

TaskDbInterface::TaskDbInterface()
: DBInterface()
{
}

std::size_t TaskDbInterface::insert(const TaskModel &task)
{
    try
    {
        boost::asio::io_context ctx;
        boost::mysql::results localResult;

        boost::asio::co_spawn(
            ctx, coRoInsertTask(task),
            [&localResult, this](std::exception_ptr ptr, boost::mysql::results result)
            {
                if (ptr)
                {
                    std::rethrow_exception(ptr);
                }
                localResult = std::move(result);
            }
        );

        ctx.run();

        return localResult.last_insert_id();
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(std::format("In TaskDbInterface::insert : {}", e.what()));
        return 0;
    }
}

TaskModel_shp TaskDbInterface::getTaskByDescriptionAndAssignedUser(std::string_view description, UserModel& assignedUser)
{
    std::cout << description << assignedUser.getUserID() << "\n";
    return TaskModel_shp();
}

TaskModel_shp TaskDbInterface::processResults(boost::mysql::results& results)
{
    std::cout << results.rows().size() << "\n";
    return nullptr;
}

boost::asio::awaitable<boost::mysql::results> TaskDbInterface::coRoInsertTask(const TaskModel &task)
{
    boost::mysql::any_connection conn(co_await boost::asio::this_coro::executor);

    co_await conn.async_connect(dbConnectionParameters);

    conn.set_meta_mode(boost::mysql::metadata_mode::minimal);

    boost::mysql::results result;

    co_await conn.async_execute(
        boost::mysql::with_params("INSERT INTO Tasks (CreatedBy, AsignedTo, Description, ParentTask, Status, PercentageComplete, CreatedOn,"
            "RequiredDelivery, ScheduledStart, ActualStart, EstimatedCompletion, Completed, EstimatedEffortHours, "
            "ActualEffortHours, SchedulePriorityGroup, PriorityInGroup)"
            " VALUES ({0}, {1}, {2}, {3}, {4}, {5}, {6}, {7}, {8}, {9}, {10}, {11}, {12}, {13}, {14}, {15})",
            task.getCreatorID(),
            task.getAssignToID(),
            task.getDescription(),
            task.rawParentTaskID(),
            task.getStatusIntVal(),
            task.getPercentageComplete(),
            convertChronoDateToBoostMySQLDate(task.getCreationDate()),
            convertChronoDateToBoostMySQLDate(task.getDueDate()),
            convertChronoDateToBoostMySQLDate(task.getScheduledStart()),
            optionalDateConversion(task.rawActualStartDate()),
            optionalDateConversion(task.rawEstimatedCompletion()),
            optionalDateConversion(task.rawCompletionDate()),
            task.getEstimatedEffort(),
            task.getactualEffortToDate(),
            task.getPriorityGroup(),
            task.getPriority()),
        result
    );

    co_await conn.async_close();

    co_return result;
}

std::optional<boost::mysql::date> TaskDbInterface::optionalDateConversion(std::optional<std::chrono::year_month_day> optDate)
{
    std::optional<boost::mysql::date> mySqlDate;

    if (optDate.has_value())
    {
        mySqlDate = convertChronoDateToBoostMySQLDate(optDate.value());
    }
    return mySqlDate;
}
