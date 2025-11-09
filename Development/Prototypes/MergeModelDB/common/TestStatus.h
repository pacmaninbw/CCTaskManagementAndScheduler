#ifndef TESTSTATUS_H_
#define TESTSTATUS_H_

enum class TestStatus {TestPassed, TestFailed};
const TestStatus TESTFAILED = TestStatus::TestFailed;
const TestStatus TESTPASSED = TestStatus::TestPassed;

#endif	// TESTSTATUS_H_


