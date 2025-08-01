cmake_minimum_required(VERSION 3.31)

project(protoPersonalPlanner LANGUAGES CXX)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

add_executable(protoPersonalPlanner
    main.cpp
    CommandLineParser.cpp
    CSVReader.h
    UserModel.h
    UserModel.cpp
    TaskModel.h
    TaskModel.cpp
    DBInterface.h
    DBInterface.cpp
    UserDbInterface.h
    UserDbInterface.cpp
    TaskDbInterface.h
    TaskDbInterface.cpp
)

target_compile_options(protoPersonalPlanner PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(protoPersonalPlanner PRIVATE cxx_std_23)

target_link_libraries(protoPersonalPlanner  ${Boost_LIBRARIES} ssl crypto)
