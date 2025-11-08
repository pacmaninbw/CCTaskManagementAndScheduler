#!/bin/bash
rm -f build/CMakeFiles/protoPersonalPlanner.dir/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/common/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Models/*.gcda
rm -f build/CMakeFiles/protoPersonalPlanner.dir/Testing/*.gcda
mysql -u "$1" -p"$2" < PlannerTaskScheduleDB.sql
valgrind --track-origins=yes protoPersonalPlanner -u "$1" -p "$2" --time-tests
