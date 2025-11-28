#!/bin/bash
sqluser="$1"
sqlpassword="$2"
rm -f build/CMakeFiles/protoPersonalPlanner.dir/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/common/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gcda
mkdir -p TestCoverage
mysql -u $sqluser -p$sqlpassword < PlannerTaskScheduleDB.sql
protoPersonalPlanner -u $sqluser -p $sqlpassword >& Testing/testOut.txt
echo "Diff"
diff Testing/testOut.txt Testing/testOut_forDiff.txt
cp build/protoPersonalPlanner TestCoverage
cp build/CMakeFiles/protoPersonalPlanner.dir/*.gc* TestCoverage
cp build/CMakeFiles/protoPersonalPlanner.dir/common/*.gc* TestCoverage
cp build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gc* TestCoverage
cp build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gc* TestCoverage
lcov --directory . --capture --output-file TestCoverage/protoPersonalPlanner_Total.info >& lcovOut.txt
lcov --remove TestCoverage/protoPersonalPlanner_Total.info '/usr/include/*' '*/boost/*' -o TestCoverage/protoPersonalPlanner_filtered.info
lcov --remove TestCoverage/protoPersonalPlanner_filtered.info '*/Models/*SelfTest*' -o TestCoverage/ModelOnly.info
genhtml TestCoverage/ModelOnly.info --output-directory TestCoverage
