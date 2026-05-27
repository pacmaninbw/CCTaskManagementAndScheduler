#!/bin/sh
rm -f build/CMakeFiles/protoPersonalPlanner.dir/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/common/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gcda
mysql -u "$1" -p"$2" < PlannerTaskScheduleDB.sql
mysql -u "$1" -p"$2" < AdditionalFunctionalTestData.sql
valgrind --track-origins=yes protoPersonalPlanner -u "$1" -p "$2" --verbose --time-tests 2>&1 | sed 's/^==[0-9]*== //' > Testing/valgrindOut.txt
echo "valgrind Diff"
date
diff -w Testing/valgrindOut.txt Testing/valgrindOut_forDiff.txt
date
