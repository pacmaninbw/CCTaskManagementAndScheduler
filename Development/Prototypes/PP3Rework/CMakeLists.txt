cmake_minimum_required(VERSION 3.31)

project(PP3Rework VERSION 0.1 LANGUAGES CXX)

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
    qt_add_executable(PP3Rework
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
        GuiModels/GuiDashboardTaskTable.cpp
        GuiModels/GuiDashboardTaskTable.h
        GuiModels/stdChronoToQTConversions.cpp
        GuiModels/stdChronoToQTConversions.h
        GuiModels/GuiDashboardScheduleTable.h
        GuiModels/GuiDashboardScheduleTable.cpp
        GuiModels/GuiTaskTableBase.h
        GuiModels/GuiTaskTableBase.cpp
        GuiModels/SelectParentTaskTable.h
        GuiModels/SelectParentTaskTable.cpp
        GuiModels/DashboardNoteTable.h
        GuiModels/DashboardNoteTable.cpp
        AppWidgets/commonQTWidgetsForApp.cpp
        AppWidgets/commonQTWidgetsForApp.h
        AppWidgets/ScheduleTablerViewer.h
        AppWidgets/ScheduleTablerViewer.cpp
        AppWidgets/DashboardTaskViewer.h
        AppWidgets/DashboardTaskViewer.cpp
        AppWidgets/DashboardNotesViewer.h
        AppWidgets/DashboardNotesViewer.cpp
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
        common/commonUtilities.cpp
        SubWindows/GoalWindow.h SubWindows/GoalWindow.cpp
        SubWindows/ModelSubWindow.h SubWindows/ModelSubWindow.cpp
        SubWindows/TodoWindow.h SubWindows/TodoWindow.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PP3Rework APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PP3Rework SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PP3Rework
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_compile_definitions(PP3Rework PRIVATE QT_NO_KEYWORDS)

target_include_directories(PP3Rework PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SubWindows>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/DBModels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GuiModels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dialogs>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/AppWidgets>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/BusinessLogic>
)

target_compile_options(PP3Rework PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(PP3Rework PRIVATE cxx_std_23)

target_link_libraries(PP3Rework PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} ssl crypto)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PP3Rework)
endif()
set_target_properties(PP3Rework PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PP3Rework
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PP3Rework)
endif()
