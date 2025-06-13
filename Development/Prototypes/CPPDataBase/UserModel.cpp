#include <exception>
#include "UserModel.h"
#include <stdexcept>
#include <string>

UserModel::UserModel()
    : ModelBase("UserModel", "UserID")
{
    addDataField("LastName", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("FirstName", PTS_DataField::PTS_DB_FieldType::TinyText, true);
    addDataField("MiddleInitial", PTS_DataField::PTS_DB_FieldType::TinyText);
    addDataField("LoginName",  PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("HashedPassWord", PTS_DataField::PTS_DB_FieldType::TinyText, true);
    addDataField("emailAddress", PTS_DataField::PTS_DB_FieldType::VarChar256, true);
    addDataField("ScheduleDayStart", PTS_DataField::PTS_DB_FieldType::DateTime, true);
    addDataField("ScheduleDayEnd", PTS_DataField::PTS_DB_FieldType::DateTime, true);
    addDataField("IncludePriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("IncludeMinorPriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("UseLettersForMajorPriority", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("SeparatePriorityWithDot", PTS_DataField::PTS_DB_FieldType::Boolean);
}

UserModel::UserModel(const char *lastIn, const char *firstIn, const char *middleIIn, const char *email)
    : UserModel()
    /*,
    scheduleDayStart{"8:30 AM"},
    scheduleDayEnd{"5:00 PM"},
    */
{
    setFieldValue("LastName", lastIn);
    setFieldValue("FirstName", firstIn);
    setFieldValue("MiddleInitial", middleIIn);
    setFieldValue("emailAddress", email);
    createLoginBasedOnUserName(lastIn, firstIn, middleIIn);
    initFieldValueNotChanged("IncludePriorityInSchedule", true);
    initFieldValueNotChanged("IncludeMinorPriorityInSchedule", true);
    initFieldValueNotChanged("UseLettersForMajorPriority", true);
    initFieldValueNotChanged("SeparatePriorityWithDot", false);
}

UserModel::UserModel(std::size_t dbUserId, const char *dbLoginName, const char *dbPassWord, const char *dbLastName,
    const char *dbFirstName, const char *dbMiddleInit, const char *dbEmail, const char *dbDayStart,
    const char *dbDayEnd, int dbPriorityinSched, int dbMinorPriorityInSched, int dbLettersForMajorP,
    int dbUseDot)
    : UserModel()
{
    setPrimaryKey(dbUserId);
    initFieldValueNotChanged("LastName", dbLastName);
    initFieldValueNotChanged("FirstName", dbFirstName);
    initFieldValueNotChanged("MiddleInitial", dbMiddleInit);
    initFieldValueNotChanged("emailAddress", dbEmail);
    initFieldValueNotChanged("LoginName", dbLoginName);
    initFieldValueNotChanged("HashedPassWord", dbPassWord);
    initFieldValueNotChanged("IncludePriorityInSchedule", dbPriorityinSched != 0);
    initFieldValueNotChanged("IncludeMinorPriorityInSchedule", dbMinorPriorityInSched != 0);
    initFieldValueNotChanged("UseLettersForMajorPriority", dbLettersForMajorP != 0);
    initFieldValueNotChanged("SeparatePriorityWithDot", dbUseDot != 0);
    std::cout << "Start Day" << dbDayStart << "\n";
    std::cout << "End Day" << dbDayEnd << "\n";
}

UserModel::~UserModel()
{

}

std::string UserModel::convertTo24hours(std::string sechduledTime)
{
    std::string original(sechduledTime);

    std::size_t colonIdx = original.find(':');
    if (colonIdx == std::string::npos) {

        std::runtime_error noColon("Colon not found between hours and minutes in UserModel::convertTo24hours()");
        throw noColon;
    }

    std::size_t spaceIdx = original.find(' ');
    if (spaceIdx == std::string::npos) {

        std::runtime_error noSpace("Space not found after minutes in UserModel::convertTo24hours()");
        throw noSpace;
    }

    std::string hourSubStr = original.substr(0, colonIdx - 1); // May be 1 or 2 digits.
    std::string minuteStr = original.substr(colonIdx + 1, spaceIdx - (colonIdx + 1));
    std::string ampm = original.substr(spaceIdx + 1, 2);

    int startHour = std::stoi(hourSubStr);
    
    std::string newTime;
    if (ampm[0] == 'A')
    {
        newTime = "0" + std::to_string(startHour);
    }
    else
    {
        newTime = std::to_string(startHour + 12);
    }

    newTime += ":" + minuteStr;

    return static_cast<std::string>(newTime);
}

void UserModel::createLoginBasedOnUserName(std::string lastName, std::string firstName, std::string middleInitial)
{
    std::string tempLoginName(lastName);
    tempLoginName += firstName;
    tempLoginName += middleInitial[0];

    setFieldValue("LoginName", tempLoginName);
    setFieldValue("HashedPassWord", tempLoginName);
}


