cmake_minimum_required(VERSION 3.31)

project(protoPersonalPlanner LANGUAGES CXX)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

add_executable(protoPersonalPlanner
    common/commonUtilities.cpp
    CommandLineParser.cpp
    Models/CoreDBInterface.cpp
    Models/ModelDBInterface.cpp
    Models/UserModel.cpp
    Models/TaskModel.cpp
    Models/ListDBInterface.h
    Models/UserList.cpp
    Models/TaskList.cpp
    main.cpp
    UnitTests/TestDBInterfaceCore.cpp
    UnitTests/TestUserDBInterface.cpp
    UnitTests/TestTaskDBInterface.cpp
)

target_include_directories(protoPersonalPlanner PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/UnitTests>
)

target_compile_options(protoPersonalPlanner PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(protoPersonalPlanner PRIVATE cxx_std_23)

target_link_libraries(protoPersonalPlanner  ${Boost_LIBRARIES} ssl crypto)
