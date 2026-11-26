
target_sources(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/DataBaseConnectionDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DataBaseConnectionDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SelectTaskParentDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/SelectTaskParentDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/UserEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/GoalEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/LoginDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NoteEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScheduleItemEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TaskEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/UserEditorDialog.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseObjectEditorDialog.h
    ${CMAKE_CURRENT_SOURCE_DIR}/BaseObjectEditorDialog.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)

