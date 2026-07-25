#!/bin/sh
#
# usage vgprototest.sh SQLUSER SQLUSERPASSWORD
#
# runs a full regression test of the database models. All SQL commands are dumped to the output
# The regression test is run through valgrind to check for memory leakage as well as memory
# usage errors. The test database is recreated each time and provided with previous versions
# of real data. The verbose output flag is used to generate additional debug information.
#
# Since the diff is the last command the -e flag can be used to exit the script
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

echo "Creating test database with test data"
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword

#
# sed is run to remove the process id from the valgrind output to allow for comparing the output
#
echo "Running regression test with verbose output"
valgrind --track-origins=yes protoPersonalPlanner -u "$sqluser" -p "$sqlpassword" --verbose --time-tests 2>&1 | 
   sed 's/^==[0-9]*== //' > Testing/valgrindOut.txt

echo "valgrind Diff"
diff -w Testing/valgrindOut.txt Testing/valgrindOut_forDiff.txt > valgrindTestDiffOut.txt || true

