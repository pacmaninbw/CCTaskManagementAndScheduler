#!/bin/sh
#
# usage modelOnlyTestCoverage.sh SQLUSER SQLUSERPASSWORD
#
# Runs the regression test and generates test coverage information.
# The regression test is run without the verbose flag. This provides
# just the output from the tests. If any of the tests fail the vgprototest.sh
# test script can be run to determine what is causing the failure.
#
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

# Remove previously generated files
rm -rf TestCoverage
find . -type f -name "*.gcda" -delete

echo "Creating test database with test data"
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword || { echo "Exiting now Database Load Failed"; exit 1; }

echo "Running Regression Tests"
# If regression tests fails the diff and test coverage reports are still valid
build/protoPersonalPlanner -u "$sqluser" -p "$sqlpassword" --time-tests > Testing/modelTestOut.txt || echo "REGRESSION TESTS FAILED!"

echo "Diff"
diff Testing/modelTestOut.txt Testing/modelTestOut_forDiff.txt

mkdir TestCoverage || exit
echo "copy Gcov data"
# Using ln -s creates broken links and moving the files forces a rebuild to regenerate 
# files built at compile time.
find . -type f -name "*.gc*" -exec cp -t TestCoverage {} +

echo "Running Lcov"
lcov --directory . --capture --output-file TestCoverage/ModelOnly.info --ignore-errors gcov,mismatch 2>&1 > TestCoverage/lcovOut.txt || { echo "LCOV gather data failed."; exit 1; }
echo "Removing Library Coverage" 
lcov --remove TestCoverage/ModelOnly.info '/usr/include/*' '*/boost/*' -o TestCoverage/ModelOnly.info 2>&1 >> TestCoverage/lcovOut.txt || { echo "LCOV remove libraries failed"; exit 1; }
echo "Removing Self Test Coverage"
lcov --remove TestCoverage/ModelOnly.info '*/Models/*SelfTest*' -o TestCoverage/ModelOnly.info 2>&1 >> TestCoverage/lcovOut.txt || { echo "LCOV remove selftest failed"; exit 1; }
echo "Generating test coverage report"
genhtml TestCoverage/ModelOnly.info --output-directory TestCoverage 2>&1 > TestCoverage/genhtmlOut.txt || { echo "HTML report generation failed"; exit 1; }
