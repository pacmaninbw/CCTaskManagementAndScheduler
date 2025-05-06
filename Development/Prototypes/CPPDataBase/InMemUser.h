#ifndef INMEMUSER_H_
#define INMEMUSER_H_

#include <iostream>
#include <string>

/*
 * In Memory User
 */
class InMemUser
{
public:
    InMemUser();
    /*
     * Add new user from GUI
     */
    InMemUser(const char* lastIn, const char* firstIn, const char* middleIIn, const char* email="");
    /*
     * Create user from database storeage, used on login for existing user.
     */
    InMemUser(std::size_t userId, const char* dbLoginName, const char* dbPassWord, const char* dbLastName,
        const char* dbFirstName, const char* dbMiddleInit, const char* dbEmail, const char* dbDayStart,
        const char* dbDayEnd, int dbPriorityinSched, int dbMinorPriorityInSched, int dbLettersForMajorP,
        int dbUseDot);
    ~InMemUser();
    std::string getLastName() const { return lastName; };
    std::string getFirstName() const { return firstName; };
    std::string getMiddleInitial() const { return middleInitial; };
    std::string getEmail() const { return eMail; };
    std::string getLoginName() const {return loginName; };
    std::string getPassword() const { return hashedPassWord; };
    std::string get24hourStartTime() { return convertTo24hours(scheduleDayStart); };
    std::string get24hourEndTime() { return convertTo24hours(scheduleDayEnd); };
    std::string getStartTime() const { return scheduleDayStart; };
    std::string getEndTime() const { return scheduleDayEnd; };
    std::size_t getUserID() const { return userID; };
    bool isPriorityInSchedule() const { return includePriorityInSchedule; };
    bool isMinorPriorityInSchedule() const { return includeMinorPriorityInSchedule; };
    bool isUsingLettersForMaorPriority() const { return useLettersForMajorPriority; };
    bool isSeparatingPriorityWithDot() const { return separatePriorityWithDot; };
    void dbSetUserId(std::size_t uID) { userID = uID; };


    friend std::ostream& operator<<(std::ostream& os, const InMemUser& obj)
    {
        os << "User Name:\n" << "\tLast Name:\t" << obj.lastName << "\n\tFirst Name\t" << obj.firstName <<
            "\n\tMiddle Initial:\t" << obj.middleInitial << "\n";
        os << "User Login Data:\n" << "\tLogin Name:\t" << obj.loginName << "\tPassword:\t" << obj.hashedPassWord << "\n";
        os << "Preferences:\n";
        os << "\tDaily Start:\t" << obj.scheduleDayStart << "\n";
        os << "\tDaily End  :\t" << obj.scheduleDayEnd << "\n";
        os << "\tUse Priority In Schedule:\t" << ((obj.includePriorityInSchedule)? "True" : "False") << "\n";
        os << "\tUse Minor Priority In Schedule:\t" << ((obj.includeMinorPriorityInSchedule)? "True" : "False") << "\n";
        os << "\tMajor Priority is Letter:\t" << ((obj.includeMinorPriorityInSchedule)? "True" : "False") << "\n";
        os << "\tUse Major.Minor:\t" << ((obj.separatePriorityWithDot)? "True" : "False") << "\n";
        return os;
    };

private:
    std::string convertTo24hours(std::string sechduledTime);
    void createLoginBasedOnUserName(std::string lastName, std::string firstName, std::string middleInitial);
    std::string lastName;
    std::string firstName;
    std::string middleInitial;
    std::string eMail;
/* 
 * The userID will only have value after the user has been added to the database.
 */
    std::size_t userID;
    std::string loginName;
    std::string hashedPassWord;

/*
 * User preferences
 */
    std::string scheduleDayStart;
    std::string scheduleDayEnd;
    bool includePriorityInSchedule;
    bool includeMinorPriorityInSchedule;
    bool useLettersForMajorPriority;
    bool separatePriorityWithDot;
};

#endif // INMEMUSER_H_
