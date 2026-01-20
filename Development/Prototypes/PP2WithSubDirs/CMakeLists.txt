cmake_minimum_required(VERSION 3.31)

project(PP2WithSubDirs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost 1.87.0 REQUIRED COMPONENTS system charconv program_options)

set(PROJECT_SOURCES
        main.cpp
        UserDashboard.cpp
        UserDashboard.h
        UserDashboard.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(PP2WithSubDirs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        common/commonUtilities.cpp
        common/commonUtilities.h
        common/commonTestValues.h
        common/commonTestValues.cpp
        commonQTWidgetsForApp.cpp
        common/GenericDictionary.h
        Models/CoreDBInterface.h
        Models/CoreDBInterface.cpp
        Models/ModelDBInterface.h
        Models/ModelDBInterface.cpp
        Models/ListDBInterface.h
        Models/UserModel.h
        Models/UserModel.cpp
        Models/UserList.h
        Models/UserList.cpp
        Models/ScheduleItemList.h
        Models/ScheduleItemList.cpp
        Models/ScheduleItemModel.h
        Models/ScheduleItemModel.cpp
        Models/TaskModel.h
        Models/TaskModel.cpp
        Models/TaskList.h
        Models/TaskList.cpp
        Models/NoteModel.h
        Models/NoteModel.cpp
        Models/NoteList.h
        Models/NoteList.cpp
        Models/UserGoalModel.h
        Models/UserGoalModel.cpp
        Models/UserGoalList.h
        Models/UserGoalList.cpp
        TestDBConnection.h
        TestDBConnection.cpp
        GuiModels/GuiTaskTableShortView.cpp
        GuiModels/GuiTaskTableShortView.h
        GuiModels/GuiTaskModel.h
        GuiModels/GuiTaskModel.cpp
        GuiModels/GuiUserModel.h
        GuiModels/GuiUserModel.cpp
        GuiModels/GuiScheduleItemModel.h GuiModels/GuiScheduleItemModel.cpp
        GuiModels/GuiNoteModel.h GuiModels/GuiNoteModel.cpp
        GuiModels/stdChronoToQTConversions.h
        GuiModels/stdChronoToQTConversions.cpp
        Dialogs/UserEditorDialog.cpp
        Dialogs/DataBaseConnectionDialog.cpp
        Dialogs/GoalEditorDialog.cpp Dialogs/GoalEditorDialog.h Dialogs/GoalEditorDialog.ui
        Dialogs/ScheduleItemEditorDialog.cpp Dialogs/ScheduleItemEditorDialog.h Dialogs/ScheduleItemEditorDialog.ui
        Dialogs/TaskEditorDialog.cpp Dialogs/TaskEditorDialog.h Dialogs/TaskEditorDialog.ui
        Dialogs/UserEditorDialog.cpp Dialogs/UserEditorDialog.h Dialogs/UserEditorDialog.ui
        Dialogs/NoteEditorDialog.cpp Dialogs/NoteEditorDialog.h Dialogs/NoteEditorDialog.ui
        Dialogs/LoginDialog.h Dialogs/LoginDialog.cpp Dialogs/LoginDialog.ui
        GuiModels/GuiGoalModel.h GuiModels/GuiGoalModel.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET PP2WithSubDirs APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(PP2WithSubDirs SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(PP2WithSubDirs
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_include_directories(PP2WithSubDirs PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/common>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Models>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/GuiModels>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Dialogs>
)
target_compile_options(PP2WithSubDirs PRIVATE -Wall -Wextra -pedantic -Werror)

target_compile_features(PP2WithSubDirs PRIVATE cxx_std_23)

target_link_libraries(PP2WithSubDirs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${Boost_LIBRARIES} ssl crypto)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.PP2WithSubDirs)
endif()
set_target_properties(PP2WithSubDirs PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS PP2WithSubDirs
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(PP2WithSubDirs)
endif()
