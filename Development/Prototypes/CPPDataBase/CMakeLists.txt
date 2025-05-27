cmake_minimum_required(VERSION 3.28)

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
    DateData.h DateData.cpp
    UserModel.h UserModel.cpp TaskModel.h TaskModel.cpp ModelBase.h ModelBase.cpp
    UserModelToDBInterface.h UserModelToDBInterface.cpp
    DBInterface.h DBInterface.cpp
    TaskModelToDBInterface.h TaskModelToDBInterface.cpp
    UserProfileTestData.h UserProfileTestData.cpp
    UserTaskTestData.h UserTaskTestData.cpp
)

target_link_libraries(protoTaskPlanner  ${Boost_LIBRARIES} ssl crypto)
