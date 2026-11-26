
add_subdirectory(SubWindows)

add_subdirectory(Dialogs)

target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/ControlPanel.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ControlPanel.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonWidgetExtensions.h
    ${CMAKE_CURRENT_SOURCE_DIR}/CommonWidgetExtensions.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserDashboard.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserDashboard.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)



