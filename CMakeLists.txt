#
# Build and test the MyDayPlanner application
#
# Each subdirectory contains a CMakeLists.txt file that adds the necessary files
# to the proper target and adds that directory to the target include files.
#
# Failed unit tests or failed integration tests should stop the build for the
# application

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

# The library and the unit tests for the library do not depend on Qt
# This is an optimization of the build process and disc space. The library
# and the unit tests do depend on doctest and the boost libraries.


include(CTest)
enable_testing()

add_executable(unit_tests)

add_subdirectory(tests)

add_library(libdbmodels STATIC)
add_library(${PROJECT_NAME}::libdbmodels ALIAS libdbmodels)

add_subdirectory(libdbmodels)


set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

qt_add_executable(${PROJECT_NAME})

add_subdirectory(app)

