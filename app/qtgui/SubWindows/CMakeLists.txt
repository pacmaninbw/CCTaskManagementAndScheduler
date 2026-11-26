
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSubWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ModelSubWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NotesWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NotesWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/OrganizationWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TodoWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/TodoWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserWindow.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiDashboardTaskTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiDashboardTaskTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiDashboardScheduleTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiDashboardScheduleTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiTaskTableBase.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GuiTaskTableBase.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/SelectParentTaskTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SelectParentTaskTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DashboardNoteTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DashboardNoteTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DefaultGoalDisplayTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/DefaultGoalDisplayTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NewUserTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NewUserTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseUserTable.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseUserTable.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)



