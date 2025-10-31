#include "NoteSelfTest.h"

NoteSelfTest::NoteSelfTest()
{
}

bool NoteSelfTest::runSelfTest()
{
    inSelfTest = true;
    verboseOutput = true;

    bool allSelfTestsPassed = true;

    if (verboseOutput)
    {
        std::clog << "Running NoteModel Self Test\n";
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

    if (!diffTest())
    {
        allSelfTestsPassed = false;
    }
    
    if (verboseOutput)
    {
        std::clog << "Test Ouput: " << *this << "\n";
    }

    if (testAllInsertFailures() != TESTPASSED)
    {
        allSelfTestsPassed = false;
        std::clog << "Test of all insertion failures FAILED!\n";
    }

    inSelfTest = false;
    verboseOutput = false;

    return allSelfTestsPassed;
}

void NoteSelfTest::selfTestResetAllValues()
{
    modified = false;
    primaryKey = 0;
    content.clear();
    creationDate = {};
    lastUpdate = {};
}

bool NoteSelfTest::testAccessorFunctionsPassed()
{
    selfTestResetAllValues();

    bool allAccessorFunctionsPassed = true;
    std::vector<std::function<bool(void)>> accessTests;
    accessTests.push_back({std::bind(&NoteSelfTest::testNoteIdAccesss, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testUserIdAccesss, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testContentAccess, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testDateAddedAccess, this)});
    accessTests.push_back({std::bind(&NoteSelfTest::testLastUpdateAccess, this)});

    for (auto accessTest: accessTests)
    {
        if (!accessTest())
        {
            allAccessorFunctionsPassed = false;
        }
    }

    return allAccessorFunctionsPassed;
}

bool NoteSelfTest::testExceptionHandling() noexcept
{
    selfTestResetAllValues();

    bool exceptionHandlingPassed = true;
    bool globalForceException = forceException;
    std::vector<ExceptionTestElement> exceptionTests;
//    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionSelectByUserID, this), "selectByUserID"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionInsert, this), "testExceptionInsert"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionUpdate, this), "testExceptionUpdate"});
    exceptionTests.push_back({std::bind(&NoteSelfTest::testExceptionRetrieve, this), "testExceptionRetrieve"});

    if (!forceExceptionsLoop(exceptionTests))
    {
        exceptionHandlingPassed = false;
    }

    forceException = globalForceException;

    return exceptionHandlingPassed;
}

bool NoteSelfTest::testExceptionInsert() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setContent("Testing insertion exception");
    setUserId(27);
    setDateAdded(timeStamp);
    setLastModified(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::insert", std::bind(&NoteModel::insert, this));
}

bool NoteSelfTest::testExceptionUpdate() noexcept
{
   selfTestResetAllValues();

    std::chrono::system_clock::time_point timeStamp = std::chrono::system_clock::now();
    setNoteId(1);
    setContent("Testing insertion exception");
    setUserId(27);
    setDateAdded(timeStamp);
    setLastModified(timeStamp);

    return testExceptionAndSuccessNArgs("NoteModel::update", std::bind(&NoteModel::update, this));
}

bool NoteSelfTest::testExceptionRetrieve() noexcept
{
    selfTestResetAllValues();

    setNoteId(1);

    return testExceptionAndSuccessNArgs("NoteModel::retrieve", std::bind(&NoteModel::retrieve, this));
}

bool NoteSelfTest::testNoteIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(27, &primaryKey, "Primary Key",
        std::bind(&NoteModel::setNoteId, this, std::placeholders::_1),
        std::bind(&NoteModel::getNoteId, this));
}

bool NoteSelfTest::testUserIdAccesss() noexcept
{
    return testAccessorFunctions<std::size_t>(31, &userID, "User ID",
        std::bind(&NoteModel::setUserId, this, std::placeholders::_1),
        std::bind(&NoteModel::getUserId, this));
}

bool NoteSelfTest::testContentAccess() noexcept
{
    return testAccessorFunctions<std::string>("Test note content access", &content, "Content",
        std::bind(&NoteModel::setContent, this, std::placeholders::_1),
        std::bind(&NoteModel::getContent, this));
}

bool NoteSelfTest::testDateAddedAccess() noexcept
{
    return testTimeStampAccessorFunctions(std::chrono::system_clock::now(), &creationDate, "Date Added",
        std::bind(&NoteModel::setDateAdded, this, std::placeholders::_1),
        std::bind(&NoteModel::getDateAdded, this));
}

bool NoteSelfTest::testLastUpdateAccess() noexcept
{
    std::chrono::system_clock::time_point testValue = std::chrono::system_clock::now();
    return testTimeStampAccessorFunctions(testValue, &lastUpdate, "Date Added",
        std::bind(&NoteModel::setLastModified, this, std::placeholders::_1),
        std::bind(&NoteModel::getLastModified, this));
}

bool NoteSelfTest::diffTest() noexcept
{
    NoteModel other;

    if (*this == other)
    {
        std::clog << "NoteModel Diff FAILED!\n";
        return false;
    }

    other.setNoteId(primaryKey);
    other.setUserId(userID);
    other.setContent(content);
    other.setDateAdded(creationDate.value());
    other.setLastModified(lastUpdate.value());

    return *this == other;
}

