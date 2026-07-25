#!/bin/sh
#
# usage protofulltest.sh SQLUSER SQLUSERPASSWORD
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

# Remove previously generated files
rm -rf TestCoverage
find . -type f -name "*.gcda" -delete

echo "Creating test database with test data"
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword

# Run the basic regression test
protoPersonalPlanner -u $sqluser -p $sqlpassword 2>&1 > Testing/testOut.txt

# Reset the test database because the previous test altered the database
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword

# Run the valgrind regression test
valgrind --track-origins=yes protoPersonalPlanner -u "$sqluser" -p "$sqlpassword" --verbose --time-tests 2>&1 | 
   sed 's/^==[0-9]*== //' > Testing/valgrindOut.txt
echo "Diff"
diff -w Testing/testOut.txt Testing/testOut_forDiff.txt > protoTestDiff.txt || true

echo "valgrind Diff"
diff Testing/valgrindOut.txt Testing/valgrindOut_forDiff.txt > valgrindTestDiff.txt || true

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
