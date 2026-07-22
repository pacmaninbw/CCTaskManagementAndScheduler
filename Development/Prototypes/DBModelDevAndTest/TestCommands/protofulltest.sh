#!/bin/sh
#
# usage protofulltest.sh SQLUSER SQLUSERPASSWORD
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"
build_dir=build/CMakeFiles/protoPersonalPlanner.dir

# Remove previously generated files
rm -f $build_dir/*.gcda $build_dir/{common,Models,Testing}/*.gcda combinedInput.sql
rm -rf TestCoverage

#
# Combine the data definitions with the test data and create the test database
#
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql > combinedInput.sql
mysql -u $sqluser -p$sqlpassword < combinedInput.sql

# Run the basic regression test
protoPersonalPlanner -u $sqluser -p $sqlpassword 2>&1 > Testing/testOut.txt

# Reset the test database because the previous set altered the database
mysql -u $sqluser -p$sqlpassword < combinedInput.sql

# Run the valgrind regression test
valgrind --track-origins=yes protoPersonalPlanner -u $sqluser -p $sqlpassword  2>&1 | sed 's/^==[0-9]*== //' > Testing/valgrindOut.txt
echo "Diff"
diff -w Testing/testOut.txt Testing/testOut_forDiff.txt > protoTestDiff.txt || true

echo "valgrind Diff"
diff Testing/valgrindOut.txt Testing/valgrindOut_forDiff.txt > valgrindTestDiff.txt || true

mkdir TestCoverage
echo "copy Gcov data"
cp -t TestCoverage build/protoPersonalPlanner \
    build/CMakeFiles/protoPersonalPlanner.dir/*.gc* \
    build/CMakeFiles/protoPersonalPlanner.dir/*/*.gc*
    
echo "Running Lcov"
lcov --directory . --capture --output-file TestCoverage/protoPersonalPlanner_Total.info --ignore-errors gcov,mismatch 2>&1 > lcovOut.txt
echo "Removing External Coverage" 
lcov --remove TestCoverage/protoPersonalPlanner_Total.info '/usr/include/*' '*/boost/*' -o TestCoverage/protoPersonalPlanner_filtered.info 2>&1 >> lcovOut.txt
genhtml TestCoverage/protoPersonalPlanner_filtered.info --output-directory TestCoverage 2>&1 > genhtmlOut.txt
