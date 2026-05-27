#!/bin/sh
date
mysql -u "$1" -p"$2" < PlannerTaskScheduleDB.sql
mysql -u "$1" -p"$2" < AdditionalFunctionalTestData.sql
date
