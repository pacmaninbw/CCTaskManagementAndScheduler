cmake_minimum_required(VERSION 4.5)

project(MyDayPlanner
	VERSION 0.1.0
	DESCRIPTION "An app for planning day to day activities, tracking tasks and maintaining a journal."
	LANGUAGES CXX
)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

configure_file(version.h.in version.h)

add_executable(${PROJECT_NAME})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/app>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/dbmodels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/qtgui>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/qtgui/SubWindows>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/qtgui/Dialogs>
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} ssl crypto gcov)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

add_subdirectory(src)

add_subdirectory(app)
