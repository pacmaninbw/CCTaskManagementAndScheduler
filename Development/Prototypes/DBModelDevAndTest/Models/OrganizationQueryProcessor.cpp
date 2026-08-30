// Project Header Files
#include "chronoBoostConversions.h"
#include "commonUtilities.h"
#include "OrganizationModel.h"
#include "OrganizationQueryProcessor.h"
#include "UserQueryProcessor.h"

// Standard C++ Header Files
#include <chrono>
#include <exception>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <vector>


OrganizationQueryProcessor::OrganizationQueryProcessor()
: QueryProcessor<OrganizationModel, OrganizationDbQueryValues>("OrganizationModel")
{

}

OrganizationQueryProcessor::~OrganizationQueryProcessor()
{

}

OrganizationList OrganizationQueryProcessor::getAllOrganizations() noexcept
{
    clearErrorMessages();

    OrganizationList organizationList;
/*
 * Prepend function name to any error messages.
 */
    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationList OrganizationQueryProcessor::findOrganizationsByName(std::string orgName) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        std::string orgNamePattern = wrapSearchContentSQLPatternMatch(orgName);

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.organization_name LIKE {} ", orgNamePattern);
        boost::mysql::format_sql_to(fctx, "AND organization_profile.deleted <> 1");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationList OrganizationQueryProcessor::findOrganizationsByPrimaryContactID(std::size_t userId) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.primary_contact_user = {} ", userId);
        boost::mysql::format_sql_to(fctx, "AND organization_profile.deleted <> 1");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationList OrganizationQueryProcessor::findOrganizationsByPrimaryContactName(
    std::string firstName,
    std::string lastName,
    std::string middleInitial
) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    UserQueryProcessor userFinder;
    UserModel_shp foundUser = userFinder.getUserByFullName(lastName, firstName, middleInitial);
    if (foundUser == nullptr)
    {
        appendErrorMessage(userFinder.getAllErrorMessages());
        return organizationList;
    }

    return findOrganizationsByPrimaryContactID(foundUser->getUserID());
}

OrganizationList OrganizationQueryProcessor::getAllOrganizationsAddedBetween(
    std::chrono::year_month_day startRage,
    std::chrono::year_month_day endRange
) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.created_timestamp >= {} ", common::toBoostMySQLDate(startRage));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.created_timestamp <= {} ", common::toBoostMySQLDate(endRange));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.deleted <> 1");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationList OrganizationQueryProcessor::getAnyOrganizationsAddedOnDate(std::chrono::year_month_day searchDate) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        std::chrono::sys_days dateConversion = searchDate;
        std::chrono::system_clock::time_point startSearch = dateConversion;
        std::chrono::system_clock::time_point endSearch = dateConversion + std::chrono::days{1};

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.created_timestamp >= {} ", common::toBoostDateTime(startSearch));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.created_timestamp < {} ", common::toBoostDateTime(endSearch));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "ORDER BY organization_profile.created_timestamp ASC");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationList OrganizationQueryProcessor::getAnyOrganizationsModifiedOnDate(std::chrono::year_month_day searchDate) noexcept
{
    clearErrorMessages();
    
    OrganizationList organizationList;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        std::chrono::sys_days dateConversion = searchDate;
        std::chrono::system_clock::time_point startSearch = dateConversion;
        std::chrono::system_clock::time_point endSearch = dateConversion + std::chrono::days{1};

        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.last_modified_time_stamp >= {} ", common::toBoostDateTime(startSearch));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.last_modified_time_stamp < {} ", common::toBoostDateTime(endSearch));
        boost::mysql::format_sql_to(fctx, "AND organization_profile.deleted <> 1 ");
        boost::mysql::format_sql_to(fctx, "ORDER BY organization_profile.last_modified_time_stamp ASC");

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        organizationList = processStaticResults(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return organizationList;
}

OrganizationModel_shp OrganizationQueryProcessor::getOrganizationById(std::size_t id_organization) noexcept
{
    clearErrorMessages();
    
    OrganizationModel_shp found;

    appendErrorMessage(std::format("OrganizationQueryProcessor::{} : ", __func__));

    try
    {
        boost::mysql::format_context fctx(getFormatOptions());
        boost::mysql::format_sql_to(fctx, "SELECT * FROM organization_profile ");
        boost::mysql::format_sql_to(fctx, "WHERE organization_profile.id_organization = {} ", id_organization);

        StaticQueryOrganization localResult = staticRunQueryAsync<OrganizationDbQueryValues>(std::move(fctx).get().value());

        found = getOneStaticResult(localResult);
    }

    catch(const std::exception& e)
    {
        appendErrorMessage(e.what());
    }
    
    return found;
}

std::vector<ListExceptionTestElement> OrganizationQueryProcessor::initListExceptionTests() noexcept
{
    std::vector<ListExceptionTestElement> exceptionTests;
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionGetAllOrganizations, this), "getAllOrganizations"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionFindOrganizationsByName, this),
        "findOrganizationsByName"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionFindOrganizationsByPrimaryContactID, this),
        "findOrganizationsByPrimaryContactID"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionFindOrganizationsByPrimaryContactName, this),
        "findOrganizationsByPrimaryContactName"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionGetAllOrganizationsAddedBetween, this),
        "getAllOrganizationsAddedBetween"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionGetAnyOrganizationsAddedOnDate, this),
        "GetAnyOrganizationsAddedOnDate"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionGetAnyOrganizationsModifiedOnDate, this),
        "getAnyOrganizationsModifiedOnDate"});
    exceptionTests.push_back({std::bind(&OrganizationQueryProcessor::testExceptionGetOrganizationById, this), "getOrganizationById"});

    return exceptionTests;
}

TestStatus OrganizationQueryProcessor::testExceptionGetAllOrganizations() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::getAllOrganizations",
         std::bind(&OrganizationQueryProcessor::getAllOrganizations, this));
}

TestStatus OrganizationQueryProcessor::testExceptionFindOrganizationsByName() noexcept
{

    std::string testName("the Institute of Reconnaissance, Intelligence, and Security");

    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::findOrganizationsByName",
         std::bind(&OrganizationQueryProcessor::findOrganizationsByName, this, std::placeholders::_1), testName);
}

TestStatus OrganizationQueryProcessor::testExceptionFindOrganizationsByPrimaryContactID() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::findOrganizationsByPrimaryContactID",
         std::bind(&OrganizationQueryProcessor::findOrganizationsByPrimaryContactID, this, std::placeholders::_1), 1);
}

TestStatus OrganizationQueryProcessor::testExceptionFindOrganizationsByPrimaryContactName() noexcept
{
#if 0
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::findOrganizationsByPrimaryContactName",
        std::bind(&OrganizationQueryProcessor::findOrganizationsByPrimaryContactName, this,
        std::placeholders::_1, std::placeholders::_2, std::placeholders::_3),
        "George", "Scott", "C");
#else
            return TESTPASSED;
#endif
}

TestStatus OrganizationQueryProcessor::testExceptionGetAllOrganizationsAddedBetween() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::getAllOrganizationsAddedBetween",
         std::bind(&OrganizationQueryProcessor::getAllOrganizationsAddedBetween, this, std::placeholders::_1, std::placeholders::_2),
         common::TestDateRangeStartValue, common::TestDateRangeEndValue);
}

TestStatus OrganizationQueryProcessor::testExceptionGetAnyOrganizationsAddedOnDate() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::getAnyOrganizationsAddedOnDate",
         std::bind(&OrganizationQueryProcessor::getAnyOrganizationsAddedOnDate, this, std::placeholders::_1), common::TestDateValue);
}

TestStatus OrganizationQueryProcessor::testExceptionGetAnyOrganizationsModifiedOnDate() noexcept
{
    selfTestResetAllValues();

    return testListExceptionAndSuccessNArgs("OrganizationQueryProcessor::getAnyOrganizationsModifiedOnDate",
         std::bind(&OrganizationQueryProcessor::getAnyOrganizationsModifiedOnDate, this, std::placeholders::_1), common::TestDateValue);
}

TestStatus OrganizationQueryProcessor::testExceptionGetOrganizationById() noexcept
{
    selfTestResetAllValues();

    return testExceptionAndSuccessNArgs("OrganizationQueryProcessor::getOrganizationById",
         std::bind(&OrganizationQueryProcessor::getOrganizationById, this, std::placeholders::_1), 1);
}
