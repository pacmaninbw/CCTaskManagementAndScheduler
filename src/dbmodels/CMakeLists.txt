cmake_minimum_required(VERSION 4.5)

project(protoPersonalPlanner LANGUAGES CXX)

add_executable(protoPersonalPlanner)

target_sources(protoPersonalPlanner PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/common/commonUtilities.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/commonTestValues.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/common/chronoBoostConversions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/CoreDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ModelDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/OrganizationModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/OrganizationQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/UserModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/TaskModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/QueryProcessor.h
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/UserQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/TaskQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/NoteModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/NoteQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ScheduleItemModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/ScheduleItemQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/UserGoalModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Models/GoalQueryProcessor.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestDBInterfaceCore.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestUserDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestTaskDBInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestGoalModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestNoteModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestOrganizationModel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Testing/TestScheduleItemModel.cpp
)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

#include(ExternalProject)
#find_package(Git REQUIRED)
#
#ExternalProject_Add(
#    doctest
#    PREFIX ${CMAKE_BINARY_DIR}/doctest
#    GIT_REPOSITORY https://github.com/doctest/doctest.git
#    TIMEOUT 10
#    UPDATE_COMMAND ${GIT_EXECUTABLE} pull
#    CONFIGURE_COMMAND ""
#    BUILD_COMMAND ""
#    INSTALL_COMMAND ""
#    LOG_DOWNLOAD ON
#)

# Expose required variable (DOCTEST_INCLUDE_DIR) to parent scope
#ExternalProject_Get_Property(doctest source_dir)
#set(DOCTEST_INCLUDE_DIR ${source_dir}/doctest CACHE INTERNAL "Path to include folder for doctest")


target_compile_definitions(protoPersonalPlanner PRIVATE TESTCOMMANDLINEENABLED=1)

target_include_directories(protoPersonalPlanner PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Testing>
    ${DOCTEST_INCLUDE_DIR}
)

# target_compile_options(protoPersonalPlanner PRIVATE -Wall -Wextra -pedantic -Werror -pg -fprofile-arcs -ftest-coverage -lgcov)
target_compile_options(protoPersonalPlanner PRIVATE -Wall -Wextra -pedantic -Werror -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(protoPersonalPlanner PRIVATE cxx_std_23)

# target_link_options(protoPersonalPlanner PRIVATE -pg)

target_link_libraries(protoPersonalPlanner  ${Boost_LIBRARIES} ssl crypto gcov)
