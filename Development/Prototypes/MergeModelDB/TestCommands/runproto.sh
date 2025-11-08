#!/bin/bash
sqluser="$1"
sqlpassword="$2"
rm -f build/CMakeFiles/protoPersonalPlanner.dir/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/common/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gcda
mysql -u $sqluser -p$sqlpassword < PlannerTaskScheduleDB.sql
protoPersonalPlanner -u $sqluser -p $sqlpassword --time-tests
