cmake_minimum_required(VERSION 3.31)

project(protoTaskPlanner LANGUAGES CXX)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

add_executable(protoTaskPlanner
    main.cpp
    CommandLineParser.cpp
    CSVReader.h
    PTS_DataField.h 
    PTS_DataField.cpp
    UserModel.h
    UserModel.cpp
    TaskModel.h
    TaskModel.cpp
    ModelBase.h
    ModelBase.cpp
    DBInterface.h
    DBInterface.cpp
)

target_compile_options(protoTaskPlanner PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(protoTaskPlanner PRIVATE cxx_std_23)

target_link_libraries(protoTaskPlanner  ${Boost_LIBRARIES} ssl crypto)
