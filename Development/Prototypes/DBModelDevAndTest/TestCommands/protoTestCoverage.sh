#!/bin/sh
#
# usage protoTestCoverage.sh SQLUSER SQLUSERPASSWORD
#
# Runs the regression test and generates test coverage information.
# The regression test is run without the verbose flag. This provides
# just the output from the tests. If any of the tests fail the vgprototest.sh
# test script can be run to determine what is causing the failure.
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

# Remove previously generated files
rm -rf TestCoverage
find . -type f -name "*.gcda" -delete

echo "Creating test database with test data"
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword

echo "Running Regression Tests"
# If regression tests fails the diff and test coverage reports are still valid
build/protoPersonalPlanner -u "$sqluser" -p "$sqlpassword" --time-tests > Testing/testOut.txt || echo "REGRESSION TESTS FAILED!"

echo "Diff"
diff -w Testing/testOut.txt Testing/testOut_forDiff.txt  > protoTestDiffOut.txt || true

mkdir TestCoverage
echo "copy Gcov data"
# Using ln -s creates broken links and moving the files forces a rebuild to regenerate 
# files built at compile time.
find . -type f -name "*.gc*" -exec cp -t TestCoverage {} +

echo "Running Lcov"
lcov --directory . --capture --output-file TestCoverage/protoPersonalPlanner_Total.info --ignore-errors gcov,mismatch 2>&1 > TestCoverage/lcovOut.txt
echo "Removing Library Coverage" 
lcov --remove TestCoverage/protoPersonalPlanner_Total.info '/usr/include/*' '*/boost/*' -o TestCoverage/protoPersonalPlanner_filtered.info 2>&1 >> TestCoverage/lcovOut.txt
echo "Generating test coverage report"
genhtml TestCoverage/protoPersonalPlanner_filtered.info --output-directory TestCoverage 2>&1 > TestCoverage/genhtmlOut.txt
