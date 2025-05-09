cmake_minimum_required(VERSION 3.31)

project(protoTaskPlanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(POLICY CMP0167)
  cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")


add_executable(protoTaskPlanner
    main.cpp
    DatabaseInterface/DBInterface.h DatabaseInterface/DBInterface.cpp
    Models/UserModel.h Models/UserModel.cpp
    DatabaseInterface/UserModelToDBInterface.h DatabaseInterface/UserModelToDBInterface.cpp
    Models/TaskModel.h Models/TaskModel.cpp
    DatabaseInterface/TaskModelToDBInterface.h DatabaseInterface/TaskModelToDBInterface.cpp
    Tests/UserProfileTestData.h Tests/UserProfileTestData.cpp
    Tests/UserTaskTestData.h Tests/UserTaskTestData.cpp
)

target_include_directories(protoTaskPlanner
    PUBLIC ./Models
    PUBLIC ./DatabaseInterface
    PUBLIC ./Tests
)

target_link_libraries(protoTaskPlanner  ${Boost_LIBRARIES} ssl crypto)
