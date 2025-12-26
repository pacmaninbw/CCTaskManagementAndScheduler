#!/bin/bash
sqluser="$1"
sqlpassword="$2"
rm -f build/CMakeFiles/protoPersonalPlanner.dir/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/common/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gcda
mysql -u $sqluser -p$sqlpassword < PlannerTaskScheduleDB.sql
protoPersonalPlanner -u $sqluser -p $sqlpassword >& Testing/testOut.txt
mysql -u $sqluser -p$sqlpassword < PlannerTaskScheduleDB.sql
valgrind --track-origins=yes protoPersonalPlanner -u $sqluser -p $sqlpassword  2>&1 | sed 's/^==[0-9]*== //' > Testing/valgrindOut.txt
echo "Diff"
diff -w Testing/testOut.txt Testing/testOut_forDiff.txt
echo "valgrind Diff"
diff Testing/valgrindOut.txt Testing/valgrindOut_forDiff.txt
