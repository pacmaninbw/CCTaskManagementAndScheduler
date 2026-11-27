

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_sources(unit_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UtilityTimer.h
)

add_subdirectory(unittests)

# Since the shell command runs in the current binary directory we need to copy
# the test database source file to this directory.

file(
    COPY "${CMAKE_SOURCE_DIR}/libdbmodels/sqlscripts/test_ptsdb.sql"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

file(
    COPY "${CMAKE_SOURCE_DIR}/libdbmodels/sqlscripts/AdditionalFunctionalTestData.sql"
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
)

add_test(
    NAME setup_test_env
    COMMAND sh ${CMAKE_SOURCE_DIR}/libdbmodels/sqlscripts/createTestDataBase.sh
)

add_test(
    NAME run_unit_tests
    COMMAND $<TARGET_FILE:unit_tests>  --verbose --time-tests 
)

set_tests_properties(setup_test_env PROPERTIES
    FIXTURES_SETUP my_env_fixture
)

set_tests_properties(run_unit_tests PROPERTIES
    FIXTURES_REQUIRED my_env_fixture
)

add_subdirectory(integrationtests)


