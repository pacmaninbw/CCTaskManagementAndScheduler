#!/bin/sh
#
# usage justsql.sh SQLUSER SQLUSERPASSWORD
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql > combinedInput.sql

mysql -u $sqluser -p$sqlpassword < combinedInput.sql

rm combinedInput.sql

