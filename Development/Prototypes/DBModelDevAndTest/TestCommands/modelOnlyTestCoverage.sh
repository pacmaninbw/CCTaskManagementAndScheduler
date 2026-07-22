#!/bin/sh
#
# usage protoTestCoverage.sh SQLUSER SQLUSERPASSWORD
#
# Runs the regression test and generates test coverage information.
# The regression test is run without the verbose flag. This provides
# just the output from the tests. If any of the tests fail the vgprototest.sh
# test script can be run to determine what is causing the failure.
#
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"
build_dir=build/CMakeFiles/protoPersonalPlanner.dir

# Remove previously generated files
rm -f $build_dir/*.gcda $build_dir/{common,Models,Testing}/*.gcda
rm -rf TestCoverage

#
# Combine the data definitions with the test data and create the test database
#
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql > combinedInput.sql
mysql -u $sqluser -p$sqlpassword < combinedInput.sql || exit
rm combinedInput.sql

protoPersonalPlanner -u $sqluser -p $sqlpassword  2>&1 > Testing/testOut.txt
echo "Diff"
diff Testing/testOut.txt Testing/testOut_forDiff.txt

mkdir TestCoverage || exit
echo "copy Gcov data"
cp -t TestCoverage build/protoPersonalPlanner \
    build/CMakeFiles/protoPersonalPlanner.dir/*.gc* \
    build/CMakeFiles/protoPersonalPlanner.dir/*/*.gc*
#
# Provide test coverage on just the models which are used by the application
#
lcov --directory . --capture --output-file TestCoverage/protoPersonalPlanner_Total.info  2>&1  > lcovOut.txt
lcov --remove TestCoverage/protoPersonalPlanner_Total.info '/usr/include/*' '*/boost/*' -o TestCoverage/protoPersonalPlanner_filtered.info
lcov --remove TestCoverage/protoPersonalPlanner_filtered.info '*/Models/*SelfTest*' -o TestCoverage/ModelOnly.info
genhtml TestCoverage/ModelOnly.info --output-directory TestCoverage
