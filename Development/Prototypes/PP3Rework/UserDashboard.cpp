// Project Header
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DashboardTaskViewer.h"
#include "DataBaseConnectionDialog.h"
#include "GoalEditorDialog.h"
#include "GuiDashboardTaskTable.h"
#include "LoginDialog.h"
#include "NoteEditorDialog.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleTablerViewer.h"
#include "TaskEditorDialog.h"
#include "UserDashboard.h"
#include "UserEditorDialog.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files
#include <format>
#include <iostream>
#include <string>

UserDashboard::UserDashboard(QWidget *parent)
    : QMainWindow(parent),
    m_UserDataPtr{nullptr},
    m_TaskToEdit{nullptr},
    m_NoteToEdit{nullptr},
    m_ScheduleItemToEdit{nullptr},
    m_DashboardDate{QDate::currentDate()},
    udTaskTableView{nullptr},
    udScheduleTableView{nullptr}
{
    m_ProgNameStr = QString::fromStdString(programOptions.progName);

    setUpUserDashboardUi();
}

UserDashboard::~UserDashboard()
{

}

void UserDashboard::setUpUserDashboardUi()
{
    setUpDashboardMenuBar();

    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(dashboardWidth, dashboardHeight);

    QVBoxLayout* dashboardLayout = new QVBoxLayout(centralwidget);
    dashboardLayout->setObjectName("dashboardLayout");

    dashboardLayout->addWidget(setUpUserIdBox(), 0, Qt::AlignHCenter);

    QFormLayout* dashboardDateForm = cqtfa_FormLayoutWithPolicy("dashboardDateForm", nullptr);

    udDateSelectorDE = cqtfa_DateEditWithCalendarPopUpCurrentDate("udDateSelectorDE", this);
    
    connect(udDateSelectorDE, &QDateEdit::dateChanged, this, &UserDashboard::handleDateChanged);

    dashboardDateForm->addRow("Date:", udDateSelectorDE);

    dashboardDateForm->setFormAlignment(Qt::AlignHCenter);

    dashboardLayout->addLayout(dashboardDateForm);

    dashboardLayout->addLayout(setUpPerDayLayout());

    setCentralWidget(centralwidget);

    setWindowTitle(m_ProgNameStr);
}

void UserDashboard::setUpDashboardMenuBar()
{
    setUpTaskMenu();
    setUpUserMenu();
    setUpNoteMenu();
    setUpGoalMenu();
    setUpScheduleItemMenu();
    setUpDbConnectionMenu();
}

void UserDashboard::setUpTaskMenu()
{
    udActionAddTask = new QAction("Add Task", this);
    udActionAddTask->setStatusTip(tr("Create a new Task"));
    connect(udActionAddTask, &QAction::triggered, this, &UserDashboard::handleAddTaskAction);

    udActionEditTask = new QAction("Edit Task", this);
    udActionEditTask->setStatusTip(tr("Edit a Task"));
    connect(udActionEditTask, &QAction::triggered, this, &UserDashboard::handleEditTaskAction);

    udTaskMenu = menuBar()->addMenu("&Task");
    udTaskMenu->addAction(udActionAddTask);
    udTaskMenu->addAction(udActionEditTask);
    udTaskMenu->addSeparator();
}

void UserDashboard::setUpUserMenu()
{
    udActionUserLogin = new QAction("Login", this);
    udActionUserLogin->setStatusTip(tr("Login as a user"));
    connect(udActionUserLogin, &QAction::triggered, this, &UserDashboard::handleUserLoginAction);

    udActionAddUserProfile = new QAction("Add User", this);
    udActionAddUserProfile->setStatusTip(tr("Create a new User"));
    connect(udActionAddUserProfile, &QAction::triggered, this, &UserDashboard::handleAddUserAction);

    udActionEditUserProfile = new QAction("Edit User", this);
    udActionEditUserProfile->setStatusTip(tr("Edit your profile"));
    connect(udActionEditUserProfile, &QAction::triggered, this, &UserDashboard::handleEditUserAction);

    udActionUserLogout = new QAction("Logout", this);
    udActionUserLogout->setStatusTip(tr("Exit the planner app"));
    connect(udActionUserLogout, &QAction::triggered, this, &QApplication::quit);

    udUserMenu = menuBar()->addMenu("&User");
    udUserMenu->addAction(udActionUserLogin);
    udUserMenu->addAction(udActionAddUserProfile);
    udUserMenu->addAction(udActionEditUserProfile);
    udUserMenu->addAction(udActionUserLogout);
    udUserMenu->addSeparator();
}

void UserDashboard::setUpNoteMenu()
{
    udActionAddNote = new QAction("Add Note", this);
    udActionAddNote->setStatusTip(tr("Create a new Note"));
    connect(udActionAddNote, &QAction::triggered, this, &UserDashboard::handleAddNoteAction);

    udActionEditNote = new QAction("Edit Note", this);
    udActionEditNote->setStatusTip(tr("Edit a Note"));
    connect(udActionEditNote, &QAction::triggered, this, &UserDashboard::handleEditNoteAction);

    udNoteMenu = menuBar()->addMenu("&Note");
    udNoteMenu->addAction(udActionAddNote);
    udNoteMenu->addAction(udActionEditNote);
    udNoteMenu->addSeparator();
}

void UserDashboard::setUpGoalMenu()
{
    udActionAddGoal = new QAction("Add Goal", this);
    udActionAddGoal->setStatusTip(tr("Create a new Goal"));
    connect(udActionAddGoal, &QAction::triggered, this, &UserDashboard::handleAddGoalAction);

    udActionEditGoal = new QAction("Edit Goal", this);
    udActionEditGoal->setStatusTip(tr("Edit a Goal"));
    connect(udActionEditGoal, &QAction::triggered, this, &UserDashboard::handleEditGoalAction);

    udGoalMenu = menuBar()->addMenu("&Goal");
    udGoalMenu->addAction(udActionAddGoal);
    udGoalMenu->addAction(udActionEditGoal);
    udGoalMenu->addSeparator();
}

void UserDashboard::setUpScheduleItemMenu()
{
    udActionAddScheduleItem = new QAction("Add to Schedule", this);
    udActionAddScheduleItem->setStatusTip(tr("Add an item to your schedule"));
    connect(udActionAddScheduleItem, &QAction::triggered, this, &UserDashboard::handleAddScheduleItemAction);

    udActionEditTask = new QAction("Edit Schedule", this);
    udActionEditTask->setStatusTip(tr("Edit a scheduled event"));
    connect(udActionEditTask, &QAction::triggered, this, &UserDashboard::handleEditScheduleItemAction);

    udScheduleItemMenu = menuBar()->addMenu("&Scheduling");
    udScheduleItemMenu->addAction(udActionAddScheduleItem);
    udScheduleItemMenu->addAction(udActionEditTask);
    udScheduleItemMenu->addSeparator();
}

void UserDashboard::setUpDbConnectionMenu()
{
    udActionConnectDB = new QAction("Connect Database", this);
    udActionConnectDB->setStatusTip("Add Database User and Password");
    connect(udActionConnectDB, &QAction::triggered, this, &UserDashboard::handleDatabaseConnectionAction);

    udDBConnectionMenu = menuBar()->addMenu("&DB Connection");
    udDBConnectionMenu->addAction(udActionConnectDB);
    udDBConnectionMenu->addSeparator();
}

void UserDashboard::updateDashboardDisplayData()
{
    fillUserIdBoxData();
    updateTaskList();
    updateSchedule();
}

bool UserDashboard::userIsLoggedIn()
{
    if (!dbIsConnected())
    {
        return false;
    }

    if (m_UserDataPtr == nullptr)
    {
        emit handleUserLoginAction();
        return false;
    }

    return true;
}

bool UserDashboard::dbIsConnected()
{
    if (programOptions.mySqlDBName.empty() || programOptions.mySqlPassword.empty())
    {
        emit handleDatabaseConnectionAction();
        return false;
    }
    return true;
}

QGroupBox *UserDashboard::setUpUserIdBox()
{
    userDashBoardIDGB = new QGroupBox("User ID:", centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    udUserFirstNameDisplay = cqtfa_LineEditFixedWidthByCharCount("udUserFirstNameDisplay", userDashBoardIDGB, userNameWidth);
    uiBoxLaytout->addWidget(udUserFirstNameDisplay);

    udUserMiddleInitialDisplay = cqtfa_LineEditFixedWidthByCharCount("udUserMiddleInitialDisplay", userDashBoardIDGB, userNameWidth / 4);
    uiBoxLaytout->addWidget(udUserMiddleInitialDisplay);

    udUserLastNameDisplay = cqtfa_LineEditFixedWidthByCharCount("udUserLastNameDisplay", userDashBoardIDGB, userNameWidth);
    uiBoxLaytout->addWidget(udUserLastNameDisplay);

    udUserNameDisplay = cqtfa_LineEditFixedWidthByCharCount("udUserNameDisplay", userDashBoardIDGB, userNameWidth);
    uiBoxLaytout->addWidget(udUserNameDisplay);

    userDashBoardIDGB->setLayout(uiBoxLaytout);
    userDashBoardIDGB->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    userDashBoardIDGB->setAlignment(Qt::AlignCenter);

    return userDashBoardIDGB;
}

QHBoxLayout *UserDashboard::setUpPerDayLayout()
{
    QHBoxLayout* perDayLayout = new QHBoxLayout;
    perDayLayout->setObjectName("perDayLayout");

    perDayLayout->addWidget(setUpPerDayTaskGB());

    perDayLayout->addWidget(setUpPerDayScheduleGB());

    perDayLayout->addWidget(setUpPerDayNotesGB());

    return perDayLayout;
}

QGroupBox *UserDashboard::setUpPerDayTaskGB()
{
    udTaskListGB = new QGroupBox("Tasks:");
    QVBoxLayout* taskLisGBLayout = new QVBoxLayout;

    taskLisGBLayout->addWidget(updateTaskList());

    udTaskListGB->setLayout(taskLisGBLayout);

    udTaskListGB->setAlignment(Qt::AlignCenter);

    return udTaskListGB;
}

QGroupBox *UserDashboard::setUpPerDayScheduleGB()
{
    udScheduleGB = new QGroupBox("Schedule:");
    QVBoxLayout* scheduleGBLayout = new QVBoxLayout;

    if (!udScheduleTableView)
    {
        udScheduleTableView = new ScheduleTablerViewer(this);
        udScheduleTableView->setObjectName("udScheduleTableView");
    }
    scheduleGBLayout->addWidget(udScheduleTableView);

    udScheduleGB->setLayout(scheduleGBLayout);

    udScheduleGB->setAlignment(Qt::AlignCenter);

    return udScheduleGB;
}

QGroupBox *UserDashboard::setUpPerDayNotesGB()
{
    udNotesGB = new QGroupBox("Notes:");
    QVBoxLayout* notesGBLayOut = new QVBoxLayout;

    fakeFillGroupBoxLayout("Note", notesGBLayOut);

    udNotesGB->setLayout(notesGBLayOut);

    udNotesGB->setAlignment(Qt::AlignCenter);

    return udNotesGB;
}

void UserDashboard::fakeFillGroupBoxLayout(std::string fieldPartialName, QVBoxLayout *layoutToFill)
{
    for (int i = 0; i < 15; ++i)
    {
        std::string listElementObjectName(std::format("tempLineEdit{}{}", fieldPartialName, i));
        QLineEdit* tempElement = cqtfa_LineEditWithWidthAndLength(listElementObjectName.c_str(), this, perDayLineEditWidth);
        QString qstr = QString::fromStdString(fieldPartialName);
        tempElement->setText(qstr);
        layoutToFill->addWidget(tempElement);
    }
}

void UserDashboard::fillUserIdBoxData()
{
    udUserFirstNameDisplay->setText(m_UserDataPtr->getFirstName());
    udUserMiddleInitialDisplay->setText(m_UserDataPtr->getMiddleInitial());
    udUserLastNameDisplay->setText(m_UserDataPtr->getLastName());
    udUserNameDisplay->setText(m_UserDataPtr->getLoginName());
}

DashboardTaskViewer* UserDashboard::updateTaskList()
{
    if (!udTaskTableView)
    {
        udTaskTableView = new DashboardTaskViewer(this);
        udTaskTableView->setObjectName("udTaskTableView");
        connect(udTaskTableView, &QTableView::clicked, this, &UserDashboard::handleTaskTableClicked);
        connect(udTaskTableView, &QTableView::doubleClicked, this, &UserDashboard::handleTaskTableClicked);
    }
    else
    {
        udTaskTableView->update();
    }

    return udTaskTableView;
}

ScheduleTablerViewer* UserDashboard::updateSchedule()
{
    if (!udScheduleTableView)
    {
        udScheduleTableView = new ScheduleTablerViewer(this);
        udScheduleTableView->setObjectName("udScheduleTableView");
    }

    udScheduleTableView->setUserIdAndDate(m_UserDataPtr, m_DashboardDate);
    udScheduleTableView->updateSchedule();

    return udScheduleTableView;
}

void UserDashboard::handleAddTaskAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    TaskEditorDialog addTaskDialog(this, m_UserDataPtr);

    addTaskDialog.exec();
}

void UserDashboard::handleEditTaskAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    TaskEditorDialog editTaskDialog(this, m_UserDataPtr);
    editTaskDialog.setTaskDataAndInitDisplayFields(m_TaskToEdit);

    editTaskDialog.exec();
}

void UserDashboard::handleTaskTableClicked(const QModelIndex &index)
{
    std::cerr << "Entered handleTaskTableClicked" << std::endl;
    if (!userIsLoggedIn())
    {
        // There is no valid data to edit.
        QMessageBox::critical(nullptr, "Critical Error", "Please log into the app before attempting edits", QMessageBox::Ok);
        return;
    }

    if (!index.isValid())
    {
        std::cerr << "Index invalid" << std::endl;
        return;
    }

    std::cerr << "Row " << index.row() << " Column " << index.column() << std::endl;
    m_TaskToEdit = static_cast<GuiTaskModel*>(index.internalPointer());

    TaskEditorDialog editTaskDialog(this, m_UserDataPtr);
    editTaskDialog.setTaskDataAndInitDisplayFields(m_TaskToEdit);

    editTaskDialog.exec();
}

void UserDashboard::handleAddUserAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    UserEditorDialog addUserDialog(this);

    addUserDialog.exec();
}

void UserDashboard::handleEditUserAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    UserEditorDialog editUserDialog(this, m_UserDataPtr);

    editUserDialog.exec();
    updateDashboardDisplayData();
}

void UserDashboard::handleAddNoteAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    NoteEditorDialog addNoteDialog(this, m_UserDataPtr->getDbUserId());

    addNoteDialog.exec();
}

void UserDashboard::handleUserLoginAction()
{
    if (!dbIsConnected())
    {
        emit handleDatabaseConnectionAction();
    }

    LoginDialog userLogin(this);

    userLogin.exec();

    m_UserDataPtr = userLogin.GetUserData();
    if (udTaskTableView)
    {
        udTaskTableView->setUserId(m_UserDataPtr);
    }
    if (udScheduleTableView)
    {
        udScheduleTableView->setUserId(m_UserDataPtr);
    }
    updateDashboardDisplayData();
}

void UserDashboard::handleEditNoteAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    NoteEditorDialog editNoteDialog(this,  m_UserDataPtr->getDbUserId(), m_NoteToEdit);

    editNoteDialog.exec();
}

void UserDashboard::handleAddGoalAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    GoalEditorDialog addGoalDialog(this,  m_UserDataPtr->getDbUserId());

    addGoalDialog.exec();
}

void UserDashboard::handleEditGoalAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    GoalEditorDialog editGoalDialog(this,  m_UserDataPtr->getDbUserId(), m_GoalToEdit);

    editGoalDialog.exec();
}

void UserDashboard::handleAddScheduleItemAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    ScheduleItemEditorDialog addScheduleItemDialog(this,  m_UserDataPtr->getDbUserId());

    addScheduleItemDialog.exec();
}

void UserDashboard::handleEditScheduleItemAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    ScheduleItemEditorDialog editScheduleItemDialog(this,  m_UserDataPtr->getDbUserId(), m_ScheduleItemToEdit);

    editScheduleItemDialog.exec();
    updateSchedule();
}

void UserDashboard::handleDatabaseConnectionAction()
{
    DataBaseConnectionDialog dbConnectionDialog(this);

    dbConnectionDialog.exec();
}

void UserDashboard::handleDateChanged(const QDate &newDate)
{
    m_DashboardDate = newDate;
    if (udTaskTableView)
    {
        udTaskTableView->setDate(newDate);
    }
    if (udScheduleTableView)
    {
        udScheduleTableView->setDate(newDate);
    }
    updateDashboardDisplayData();
}
