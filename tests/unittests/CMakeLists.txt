
target_sources(unit_tests PRIVATE
    unit_test_CoreDBInterface.h
    unit_test_GoalQueryProcessor.h
    unit_test_ModelDBInterface.h
    unit_test_NoteModel.h
    unit_test_NoteQueryProcessor.h
    unit_test_OrganizationModel.h
    unit_test_OrganizationQueryProcessor.h
    unit_test_QueryProcessor.h
    unit_test_ScheduleItemModel.h
    unit_test_ScheduleItemQueryProcessor.h
    unit_test_TaskModel.h
    unit_test_TaskQueryProcessor.h
    unit_test_UserGoalModel.h
    unit_test_UserModel.h
    unit_test_UserQueryProcessor.h
)

target_sources(unit_tests PRIVATE unit_test_main.cpp)

target_compile_definitions(unit_tests PRIVATE
    TESTCOMMANDLINEENABLED=1
    BUILD_FOR_TESTING=1
)

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>)

target_include_directories(unit_tests PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

target_compile_options(unit_tests PRIVATE -Wall -Wextra -pedantic -Werror -g -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(unit_tests PRIVATE cxx_std_23)

target_link_libraries(unit_tests ${PROJECT_NAME}::libdbmodels ${Boost_LIBRARIES} ssl crypto gcov)
