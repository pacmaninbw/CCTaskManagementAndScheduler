// Project Header Files
#include "CommandLineParser.h"
#include "createNamedQTWidget.h"
#include "GoalEditorDialog.h"
#include "NoteEditorDialog.h"
#include "ScheduleItemEditorDialog.h"
#include "TaskEditorDialog.h"
#include "UserDashboard.h"
#include "UserEditorDialog.h"

// QT Header Files


// Standard C++ Header Files

#include "UserDashboard.h"

UserDashboard::UserDashboard(QWidget *parent)
    : QMainWindow(parent),
    userID{0}
{
    progNameStr = QString::fromStdString(programOptions.progName);

    setUpUserDashboardUi();
}

UserDashboard::~UserDashboard()
{

}

void UserDashboard::setUpUserDashboardUi()
{
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
    setCentralWidget(centralwidget);

    setWindowTitle(progNameStr);
}

void UserDashboard::handleAddTaskAction()
{
    TaskEditorDialog addTaskDialog(this);

    addTaskDialog.exec();
}

void UserDashboard::handleEditTaskAction()
{
    TaskEditorDialog editTaskDialog(this);

    editTaskDialog.exec();
}

void UserDashboard::handleAddUserAction()
{
    UserEditorDialog addUserDialog(this);

    addUserDialog.exec();
}

void UserDashboard::handleEditUserAction()
{
    UserEditorDialog editUserDialog(this);

    editUserDialog.exec();
}

void UserDashboard::handleAddNoteAction()
{
    NoteEditorDialog addNoteDialog(this);

    addNoteDialog.exec();
}

void UserDashboard::handleEditNoteAction()
{
    NoteEditorDialog editNoteDialog(this);

    editNoteDialog.exec();
}

void UserDashboard::handleAddGoalAction()
{
    GoalEditorDialog addGoalDialog(this);

    addGoalDialog.exec();
}

void UserDashboard::handleEditGoalAction()
{
    GoalEditorDialog editGoalDialog(this);

    editGoalDialog.exec();
}

void UserDashboard::handleAddScheduleItemAction()
{
    ScheduleItemEditorDialog addScheduleItemDialog(this);

    addScheduleItemDialog.exec();
}

void UserDashboard::handleEditScheduleItemAction()
{
    ScheduleItemEditorDialog editScheduleItemDialog(this);

    editScheduleItemDialog.exec();
}
