#!/bin/sh

#
# Combine the data definitions with the test data and create the test database
#
echo "Creating test database with test data"
pwd
cat test_ptsdb.sql AdditionalFunctionalTestData.sql | mysql -u ${CHERNICKSW_PLANNERORGANIZER_DBUSERNAME} -p${CHERNICKSW_PLANNERORGANIZER_DBPASSWORD}

