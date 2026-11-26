#!/bin/sh

#
# Combine the data definitions with the test data and create the test database
#
echo "Creating test database with test data"
pwd

file="./test_ptsdb.sql"

if [ ! -f "$file" ]; then
    echo "Error: $file not found." >&2
    exit 1
fi

file="./AdditionalFunctionalTestData.sql"

if [ ! -f "$file" ]; then
    echo "Error: $file not found." >&2
    exit 1
fi

cat test_ptsdb.sql AdditionalFunctionalTestData.sql | mysql -u ${CHERNICKSW_PLANNERORGANIZER_DBUSERNAME} -p${CHERNICKSW_PLANNERORGANIZER_DBPASSWORD}

