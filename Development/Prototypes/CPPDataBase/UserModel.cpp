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
    addDataField("EmailAddress", PTS_DataField::PTS_DB_FieldType::VarChar256, true);
    addDataField("ScheduleDayStart", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("ScheduleDayEnd", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("IncludePriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("IncludeMinorPriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("UseLettersForMajorPriority", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("SeparatePriorityWithDot", PTS_DataField::PTS_DB_FieldType::Boolean);
}

UserModel::UserModel(const char *lastIn, const char *firstIn, const char *middleIIn, const char *email)
    : UserModel()
{
    setFieldValue("LastName", lastIn);
    setFieldValue("FirstName", firstIn);
    setFieldValue("MiddleInitial", middleIIn);
    setFieldValue("EmailAddress", email);
    createLoginBasedOnUserName(lastIn, firstIn, middleIIn);
    initFieldValueNotChanged("IncludePriorityInSchedule", true);
    initFieldValueNotChanged("IncludeMinorPriorityInSchedule", true);
    initFieldValueNotChanged("UseLettersForMajorPriority", true);
    initFieldValueNotChanged("SeparatePriorityWithDot", false);
    initFieldValueNotChanged("ScheduleDayStart", "8:30 AM");
    initFieldValueNotChanged("ScheduleDayEnd", "5:00 PM");
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
    initFieldValueNotChanged("EmailAddress", dbEmail);
    initFieldValueNotChanged("LoginName", dbLoginName);
    initFieldValueNotChanged("HashedPassWord", dbPassWord);
    initFieldValueNotChanged("IncludePriorityInSchedule", dbPriorityinSched != 0);
    initFieldValueNotChanged("IncludeMinorPriorityInSchedule", dbMinorPriorityInSched != 0);
    initFieldValueNotChanged("UseLettersForMajorPriority", dbLettersForMajorP != 0);
    initFieldValueNotChanged("SeparatePriorityWithDot", dbUseDot != 0);
    initFieldValueNotChanged("ScheduleDayStart", dbDayStart);
    initFieldValueNotChanged("ScheduleDayEnd", dbDayEnd);
}

UserModel::~UserModel()
{

}

void UserModel::createLoginBasedOnUserName(std::string lastName, std::string firstName, std::string middleInitial)
{
    std::string tempLoginName(lastName);
    tempLoginName += firstName;
    tempLoginName += middleInitial[0];

    setFieldValue("LoginName", tempLoginName);
    setFieldValue("HashedPassWord", tempLoginName);
}

std::string UserModel::getLastName() const { return getStringFieldValue("LastName"); };
std::string UserModel::getFirstName() const { return getStringFieldValue("FirstName");  };
std::string UserModel::getMiddleInitial() const { return getStringFieldValue("MiddleInitial"); };
std::string UserModel::getEmail() const { return getStringFieldValue("EmailAddress"); };
std::string UserModel::getLoginName() const {return getStringFieldValue("LoginName"); };
std::string UserModel::getPassword() const { return getStringFieldValue("HashedPassWord");  };
std::string UserModel::getStartTime() const { return getStringFieldValue("ScheduleDayStart"); };
std::string UserModel::getEndTime() const { return getStringFieldValue("ScheduleDayEnd"); };
std::size_t UserModel::getUserID() const { return getPrimaryKey(); };
bool UserModel::isPriorityInSchedule() const { return getBoolFieldValue("IncludePriorityInSchedule"); };
bool UserModel::isMinorPriorityInSchedule() const { return getBoolFieldValue("IncludeMinorPriorityInSchedule"); };
bool UserModel::isUsingLettersForMaorPriority() const { return getBoolFieldValue("UseLettersForMajorPriority"); };
bool UserModel::isSeparatingPriorityWithDot() const { return getBoolFieldValue("SeparatePriorityWithDot"); };

