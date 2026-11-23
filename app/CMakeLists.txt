
configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
    "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/app>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/common>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/dbmodels>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/qtgui>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/qtgui/SubWindows>
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src/qtgui/Dialogs>
)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommandLineParser.h
    ${CMAKE_CURRENT_SOURCE_DIR}/MyDayPlannerMain.cpp 
    ${CMAKE_CURRENT_SOURCE_DIR}/PlannerInitializer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/PlannerInitializer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TestDBConnection.h
)

target_compile_definitions(${PROJECT_NAME} PRIVATE QT_NO_KEYWORDS)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic -Werror -fprofile-arcs -ftest-coverage -lgcov)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} ssl crypto gcov)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
