cmake_minimum_required(VERSION 3.31)

project(MyDayPlanner VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

set(PROJECT_SOURCES
        main.cpp
        UserDashboard.cpp
        UserDashboard.h
        PlannerInitializer.h
        PlannerInitializer.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyDayPlanner
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
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
        BusinessLogic/TestDBConnection.cpp
        BusinessLogic/TestDBConnection.h
        CommandLineParser.h
        DBModels/CoreDBInterface.cpp
        DBModels/CoreDBInterface.h
        DBModels/QueryProcessor.h
        DBModels/ModelDBInterface.cpp
        DBModels/ModelDBInterface.h
        DBModels/NoteQueryProcessor.cpp
        DBModels/NoteQueryProcessor.h
        DBModels/NoteModel.cpp
        DBModels/NoteModel.h
        DBModels/ScheduleItemQueryProcessor.cpp
        DBModels/ScheduleItemQueryProcessor.h
        DBModels/ScheduleItemModel.cpp
        DBModels/ScheduleItemModel.h
        DBModels/TaskQueryProcessor.cpp
        DBModels/TaskQueryProcessor.h
        DBModels/TaskModel.cpp
        DBModels/TaskModel.h
        DBModels/GoalQueryProcessor.cpp
        DBModels/GoalQueryProcessor.h
        DBModels/UserGoalModel.cpp
        DBModels/UserGoalModel.h
        DBModels/UserQueryProcessor.cpp
        DBModels/UserQueryProcessor.h
        DBModels/UserModel.cpp
        DBModels/UserModel.h
        common/commonTestValues.cpp
        common/commonTestValues.h
        common/commonUtilities.cpp
        common/commonUtilities.h
        common/commonQTWidgetsForApp.cpp
        common/commonQTWidgetsForApp.h
        common/stdChronoToQTConversions.cpp
        common/stdChronoToQTConversions.h
        SubWindows/GoalWindow.h
        SubWindows/GoalWindow.cpp
        SubWindows/ModelSubWindow.h
        SubWindows/ModelSubWindow.cpp
        SubWindows/TodoWindow.h
        SubWindows/TodoWindow.cpp
        SubWindows/ScheduleWindow.h
        SubWindows/ScheduleWindow.cpp
        SubWindows/NotesWindow.h
        SubWindows/NotesWindow.cpp
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
        Dialogs/BaseObjectEditorDialog.h Dialogs/BaseObjectEditorDialog.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET MyDayPlanner APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(MyDayPlanner SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(MyDayPlanner
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(MyDayPlanner PRIVATE QT_NO_KEYWORDS)

target_include_directories(MyDayPlanner PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubWindows>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DBModels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dialogs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/BusinessLogic>
)

target_compile_options(MyDayPlanner PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(MyDayPlanner PRIVATE cxx_std_23)

target_link_libraries(MyDayPlanner PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} ssl crypto)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MyDayPlanner)
endif()
set_target_properties(MyDayPlanner PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS MyDayPlanner
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MyDayPlanner)
endif()
