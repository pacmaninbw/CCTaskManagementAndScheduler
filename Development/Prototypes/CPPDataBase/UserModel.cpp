#include <exception>
#include "UserModel.h"
#include <stdexcept>
#include <string>

UserModel::UserModel()
    : ModelBase("UserModel", "UserProfile", "UserID")
{
    addDataField("LastName", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("FirstName", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("MiddleInitial", PTS_DataField::PTS_DB_FieldType::VarChar45);
    addDataField("LoginName",  PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("HashedPassWord", PTS_DataField::PTS_DB_FieldType::TinyText, true);
    addDataField("EmailAddress", PTS_DataField::PTS_DB_FieldType::VarChar256, true);
    addDataField("ScheduleDayStart", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("ScheduleDayEnd", PTS_DataField::PTS_DB_FieldType::VarChar45, true);
    addDataField("IncludePriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("IncludeMinorPriorityInSchedule", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("UseLettersForMajorPriority", PTS_DataField::PTS_DB_FieldType::Boolean);
    addDataField("SeparatePriorityWithDot", PTS_DataField::PTS_DB_FieldType::Boolean);
    initFieldValueNotChanged("IncludePriorityInSchedule", true);
    initFieldValueNotChanged("IncludeMinorPriorityInSchedule", true);
    initFieldValueNotChanged("UseLettersForMajorPriority", true);
    initFieldValueNotChanged("SeparatePriorityWithDot", false);
    initFieldValueNotChanged("ScheduleDayStart", "8:30 AM");
    initFieldValueNotChanged("ScheduleDayEnd", "5:00 PM");
}

UserModel::UserModel(std::string lastIn, std::string firstIn, std::string middleIIn, std::string email)
    : UserModel()
{
    setFieldValue("LastName", lastIn);
    setFieldValue("FirstName", firstIn);
    setFieldValue("MiddleInitial", middleIIn);
    setFieldValue("EmailAddress", email);
    createLoginBasedOnUserName(lastIn, firstIn, middleIIn);
}

void UserModel::autoGenerateLoginAndPassword()
{
    if (!fieldHasValue("LoginName") && !fieldHasValue("HashedPassWord"))
    {
        createLoginBasedOnUserName(getLastName(), getFirstName(), getMiddleInitial());
    }
}

void UserModel::createLoginBasedOnUserName(
    const std::string& lastName, const std::string& firstName, const std::string& middleInitial)
{
    std::string tempLoginName(lastName);
    tempLoginName += firstName;
    if (middleInitial.size())
    {
        tempLoginName += middleInitial[0];
    }

    setLoginName(tempLoginName);
    setPassword(tempLoginName);
}

std::string UserModel::getLastName() const
{
    return getStringFieldValue("LastName");
}

std::string UserModel::getFirstName() const
{
    return getStringFieldValue("FirstName");
}

std::string UserModel::getMiddleInitial() const
{
    return getStringFieldValue("MiddleInitial");
}

std::string UserModel::getEmail() const
{
    return getStringFieldValue("EmailAddress");
}

std::string UserModel::getLoginName() const
{
    return getStringFieldValue("LoginName");
}

std::string UserModel::getPassword() const
{
    return getStringFieldValue("HashedPassWord"); 
}

std::string UserModel::getStartTime() const
{
    return getStringFieldValue("ScheduleDayStart"); 
}

std::string UserModel::getEndTime() const
{
    return getStringFieldValue("ScheduleDayEnd"); 
}

std::size_t UserModel::getUserID() const
{
    return getPrimaryKey();
}

bool UserModel::isPriorityInSchedule() const
{
    return getBoolFieldValue("IncludePriorityInSchedule");
}

bool UserModel::isMinorPriorityInSchedule() const 
{
    return getBoolFieldValue("IncludeMinorPriorityInSchedule");
}

bool UserModel::isUsingLettersForMaorPriority() const
{
    return getBoolFieldValue("UseLettersForMajorPriority");
}

bool UserModel::isSeparatingPriorityWithDot() const
{
    return getBoolFieldValue("SeparatePriorityWithDot");
}

void UserModel::setLastName(const std::string& lastName)
{
    setFieldValue("LastName", lastName);
};

void UserModel::setFirstName(const std::string& firstName)
{
    setFieldValue("FirstName", firstName);
}

void UserModel::setMiddleInitial(const std::string& middleinit)
{
    setFieldValue("MiddleInitial", middleinit);
}

void UserModel::setEmail(const std::string& email)
{
    setFieldValue("EmailAddress", email);
}

void UserModel::setLoginName(const std::string& loginName)
{
    setFieldValue("LoginName", loginName);
}

void UserModel::setPassword(const std::string& password)
{
    setFieldValue("HashedPassWord", password);
};

void UserModel::setStartTime(const std::string& startTime)
{
    setFieldValue("ScheduleDayStart", startTime); 
}

void UserModel::setEndTime(const std::string& endTime)
{
    setFieldValue("ScheduleDayEnd", endTime); 
}

void UserModel::setPriorityInSchedule(bool inSchedule)
{
    setFieldValue("IncludePriorityInSchedule", inSchedule); 
}

void UserModel::setMinorPriorityInSchedule(bool inSchedule)
{
    setFieldValue("IncludeMinorPriorityInSchedule", inSchedule); 
}

void UserModel::setUsingLettersForMaorPriority(bool usingLetters)
{
    setFieldValue("UseLettersForMajorPriority", usingLetters);
}

void UserModel::setSeparatingPriorityWithDot(bool separate)
{
    setFieldValue("SeparatePriorityWithDot", separate);
}

