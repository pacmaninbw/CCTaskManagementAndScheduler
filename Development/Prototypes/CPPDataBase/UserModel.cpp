#include <exception>
#include "UserModel.h"
#include <stdexcept>
#include <string>


UserModel::UserModel()
{

}

UserModel::UserModel(const char *lastIn, const char *firstIn, const char *middleIIn, const char *email)
    : lastName{lastIn},
    firstName{firstIn},
    middleInitial{middleIIn},
    eMail{email},
    userID{0},
    scheduleDayStart{"8:30 AM"},
    scheduleDayEnd{"5:00 PM"},
    includePriorityInSchedule{true},
    includeMinorPriorityInSchedule{true},
    useLettersForMajorPriority{true},
    separatePriorityWithDot{false}
{
    createLoginBasedOnUserName(lastIn, firstIn, middleIIn);
}

UserModel::UserModel(std::size_t dbUserId, const char *dbLoginName, const char *dbPassWord, const char *dbLastName,
    const char *dbFirstName, const char *dbMiddleInit, const char *dbEmail, const char *dbDayStart,
    const char *dbDayEnd, int dbPriorityinSched, int dbMinorPriorityInSched, int dbLettersForMajorP,
    int dbUseDot)
    : lastName{dbLastName},
    firstName{dbFirstName},
    middleInitial{dbMiddleInit},
    eMail{dbEmail},
    userID{dbUserId},
    loginName{dbLoginName},
    hashedPassWord{dbPassWord},
    scheduleDayStart{dbDayStart},
    scheduleDayEnd{dbDayEnd}
{
    includePriorityInSchedule = dbPriorityinSched != 0;
    includeMinorPriorityInSchedule = dbMinorPriorityInSched != 0;
    useLettersForMajorPriority = dbLettersForMajorP != 0;
    separatePriorityWithDot = dbUseDot != 0;
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

    loginName = tempLoginName;
    hashedPassWord = tempLoginName;
}
