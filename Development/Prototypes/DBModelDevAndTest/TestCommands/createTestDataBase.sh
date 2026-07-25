#!/bin/sh
#
# usage justsql.sh SQLUSER SQLUSERPASSWORD
#
set -e
sqluser="${1:-no_username_supplied}"
sqlpassword="${2:-no_password_supplied}"

#
# Combine the data definitions with the test data and create the test database
#
echo "Creating test database with test data"
cat PlannerTaskScheduleDB.sql AdditionalFunctionalTestData.sql | mysql -u $sqluser -p$sqlpassword

