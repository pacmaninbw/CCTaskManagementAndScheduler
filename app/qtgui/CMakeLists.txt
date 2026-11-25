
target_sources(${PROJECT_NAME} PRIVATE
    ControlPanel.h
    ControlPanel.cpp
    CommonWidgetExtensions.h
    CommonWidgetExtensions.cpp
    UserDashboard.h
    UserDashboard.cpp
    Dialogs/DataBaseConnectionDialog.cpp
    Dialogs/DataBaseConnectionDialog.h
    Dialogs/GoalEditorDialog.h
    Dialogs/LoginDialog.h
    Dialogs/NoteEditorDialog.h
    Dialogs/ScheduleItemEditorDialog.h
    Dialogs/SelectTaskParentDialog.h
    Dialogs/SelectTaskParentDialog.cpp
    Dialogs/TaskEditorDialog.h
    Dialogs/UserEditorDialog.h
    Dialogs/GoalEditorDialog.cpp
    Dialogs/LoginDialog.cpp
    Dialogs/NoteEditorDialog.cpp
    Dialogs/ScheduleItemEditorDialog.cpp
    Dialogs/TaskEditorDialog.cpp
    Dialogs/UserEditorDialog.cpp
    Dialogs/BaseObjectEditorDialog.h
    Dialogs/BaseObjectEditorDialog.cpp
    SubWindows/GoalWindow.h
    SubWindows/GoalWindow.cpp
    SubWindows/ModelSubWindow.h
    SubWindows/ModelSubWindow.cpp
    SubWindows/NotesWindow.h
    SubWindows/NotesWindow.cpp
    SubWindows/OrganizationWindow.h
    SubWindows/OrganizationWindow.cpp
    SubWindows/ScheduleWindow.h
    SubWindows/ScheduleWindow.cpp
    SubWindows/TodoWindow.h
    SubWindows/TodoWindow.cpp
    SubWindows/UserWindow.h
    SubWindows/UserWindow.cpp
    SubWindows/GuiDashboardTaskTable.cpp
    SubWindows/GuiDashboardTaskTable.h
    SubWindows/GuiDashboardScheduleTable.h
    SubWindows/GuiDashboardScheduleTable.cpp
    SubWindows/GuiTaskTableBase.h
    SubWindows/GuiTaskTableBase.cpp
    SubWindows/SelectParentTaskTable.h
    SubWindows/SelectParentTaskTable.cpp
    SubWindows/DashboardNoteTable.h
    SubWindows/DashboardNoteTable.cpp
    SubWindows/DefaultGoalDisplayTable.h
    SubWindows/DefaultGoalDisplayTable.cpp
    SubWindows/NewUserTable.h SubWindows/NewUserTable.cpp
    SubWindows/BaseUserTable.h SubWindows/BaseUserTable.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/SubWindows
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>/Dialogs
)



