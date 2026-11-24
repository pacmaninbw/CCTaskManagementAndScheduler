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

add_executable(${PROJECT_NAME})

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)


if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${PROJECT_NAME})
endif()

enable_testing()

add_executable(unit_tests)

add_subdirectory(src)

add_subdirectory(app)

add_subdirectory(tests)
