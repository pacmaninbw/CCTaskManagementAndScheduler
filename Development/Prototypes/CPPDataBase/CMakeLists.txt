cmake_minimum_required(VERSION 3.31)

project(protoTaskPlanner LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_COMPILE_WARNING_AS_ERROR ON)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv)

include(FetchContent)

FetchContent_Declare(
    genericDictionary
    GIT_REPOSITORY https://github.com/pacmaninbw/GenericDictionaryHeaderLibraryGeneric.git
    CMAKE_FIND_PACKAGE_NAME GenericDictionaryHeaderLibraryGeneric
)

FetchContent_MakeAvailable(genericDictionary)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/PrivateData/dbadmindata.h")
    add_compile_definitions(USINGPRIVATECONNECTIONDATA=1)
endif()

add_executable(protoTaskPlanner
    main.cpp 
    PTS_DataField.h PTS_DataField.cpp 
    UserModel.h UserModel.cpp TaskModel.h TaskModel.cpp ModelBase.h ModelBase.cpp
    DBInterface.h DBInterface.cpp
)

#target_compile_definitions(protoTaskPlanner PRIVATE SQL4DEBUG=1)

target_include_directories(protoTaskPlanner PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/build/_deps/genericdictionary-src)

target_link_libraries(protoTaskPlanner  ${Boost_LIBRARIES} ssl crypto)
