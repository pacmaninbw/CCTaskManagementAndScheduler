cmake_minimum_required(VERSION 4.5)

project(MyDayPlanner
	VERSION 0.1.0
	DESCRIPTION "An app for planning day to day activities, tracking tasks and maintaining a journal."
	LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

include(FetchContent)
set(DOCTEST_VERSION v2.4.11)
FetchContent_Declare(doctest
    URL  https://raw.githubusercontent.com/doctest/doctest/${DOCTEST_VERSION}/doctest/doctest.h
    DOWNLOAD_NO_EXTRACT TRUE
)
FetchContent_MakeAvailable(doctest)

set(DOCTEST_INCLUDE_DIR ${doctest_SOURCE_DIR})
# Include doctest_discover_tests(<target>) function
#include("${doctest_SOURCE_DIR}/scripts/cmake/doctest.cmake")

# Use doctest in a target
#target_link_libraries(my_target PUBLIC doctest::doctest)

# Or use it globally
#link_libraries(doctest::doctest)

# The library and the unit tests for the library do not depend on Qt
# This is an optimization of the build process and disc space. The library
# and the unit tests do depend on doctest and the boost libraries.

add_subdirectory(libdbmodels)

include(CTest)

enable_testing()

add_subdirectory(tests)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

qt_add_executable(${PROJECT_NAME})

add_subdirectory(app)

