// Project Header
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DataBaseConnectionDialog.h"
#include "GoalWindow.h"
#include "LoginDialog.h"
#include "NotesWindow.h"
#include "ScheduleWindow.h"
#include "TodoWindow.h"
#include "UserDashboard.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files
#include <string>

UserDashboard::UserDashboard(QWidget *parent)
    : QMainWindow(parent),
    m_UserDataPtr{nullptr},
    m_DashboardDate{QDate::currentDate()}
{
    m_ProgNameStr = QString::fromStdString(programOptions.progName);

    m_UserDataPtr = std::make_shared<UserModel>();

    setUpUserDashboardUi();
}

UserDashboard::UserDashboard(std::shared_ptr<UserModel> loggedInUser, QWidget *parent)
    : UserDashboard(parent)
{
    if (loggedInUser)
    {
        m_UserDataPtr = loggedInUser;

        fillUserIdBoxData();
        updatePerDayView();
    }
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
    setUpUserMenu();
    setUpGoalMenu();
    setUpDbConnectionMenu();
    setUpTodoMenu();
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

void UserDashboard::setUpGoalMenu()
{
    udActionOpenGoalWindow = new QAction("Open Goal Window", this);
    udActionOpenGoalWindow->setStatusTip("Open Goal Window");
    connect(udActionOpenGoalWindow, &QAction::triggered, this, &UserDashboard::handleOpenGoalWindowClicked);

    udGoalMenu = menuBar()->addMenu("&Goal");
    udGoalMenu->addAction(udActionOpenGoalWindow);
    udGoalMenu->addSeparator();
}

void UserDashboard::setUpTodoMenu()
{
    udActionOpenTodoWindow = new QAction("Todo List", this);
    udActionOpenTodoWindow->setStatusTip(tr("Create a new Goal"));
    connect(udActionOpenTodoWindow, &QAction::triggered, this, &UserDashboard::handleToDoMenuClicked);

    udTodo = menuBar()->addMenu("&Todo List");
    udTodo->addAction(udActionOpenTodoWindow);
    udTodo->addSeparator();
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

bool UserDashboard::userIsLoggedIn()
{
    if (!dbIsConnected())
    {
        return false;
    }

    if (m_UserDataPtr == nullptr)
    {
        Q_EMIT handleUserLoginAction();
        return false;
    }

    return true;
}

bool UserDashboard::dbIsConnected()
{
    if (programOptions.mySqlDBName.empty() || programOptions.mySqlPassword.empty())
    {
        Q_EMIT handleDatabaseConnectionAction();
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

    todoWindowInWindow = setUpTodoList();
    perDayLayout->addWidget(todoWindowInWindow);

    scheduleWindowInWindow = setUpSchedule();
    perDayLayout->addWidget(scheduleWindowInWindow);

    noteWindowInWindow = setUpNotesWindow();
    perDayLayout->addWidget(noteWindowInWindow);

    return perDayLayout;
}

TodoWindow *UserDashboard::setUpTodoList()
{
    TodoWindow* todoListWindow = new TodoWindow(true, this);
    if (m_UserDataPtr)
    {
        todoListWindow->setUser(m_UserDataPtr);
    }
    todoListWindow->setDate(m_DashboardDate);
    todoListWindow->setUpWindowUi();
    todoListWindow->show();

    return todoListWindow;
}

ScheduleWindow *UserDashboard::setUpSchedule()
{
    ScheduleWindow* scheduleWindow = new ScheduleWindow(true, this);
    if (m_UserDataPtr)
    {
        scheduleWindow->setUser(m_UserDataPtr);
    }
    scheduleWindow->setDate(m_DashboardDate);
    scheduleWindow->setUpWindowUi();
    scheduleWindow->show();
    scheduleWindow->refresh();

    return scheduleWindow;
}

NotesWindow *UserDashboard::setUpNotesWindow()
{
    NotesWindow* noteWindow;
    if (m_UserDataPtr != nullptr)
    {
        noteWindow = new NotesWindow(m_UserDataPtr, m_DashboardDate, true, this);
    }
    else
    {
        noteWindow = new NotesWindow(true, this);
        noteWindow->setDate(m_DashboardDate);
    }

    noteWindow->setUpWindowUi();
    noteWindow->show();
    noteWindow->refresh();

    return noteWindow;
}

void UserDashboard::updatePerDayView()
{
    if (todoWindowInWindow)
    {
        todoWindowInWindow->setUser(m_UserDataPtr);
        todoWindowInWindow->setDate(m_DashboardDate);
        todoWindowInWindow->refresh();
    }

    if (scheduleWindowInWindow)
    {
        scheduleWindowInWindow->setUser(m_UserDataPtr);
        scheduleWindowInWindow->setDate(m_DashboardDate);
        scheduleWindowInWindow->refresh();
    }

    if (noteWindowInWindow)
    {
        noteWindowInWindow->setUser(m_UserDataPtr);
        noteWindowInWindow->setDate(m_DashboardDate);
        noteWindowInWindow->refresh();
    }
}

void UserDashboard::fillUserIdBoxData()
{
    udUserFirstNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getFirstName()));
    udUserMiddleInitialDisplay->setText(QString::fromStdString(m_UserDataPtr->getMiddleInitial()));
    udUserLastNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLastName()));
    udUserNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLoginName()));
}

QString UserDashboard::groupBoxTitleWithDate(QString gbTitleBase)
{
    QString titleWithDate = gbTitleBase + " for " + m_DashboardDate.toString(Qt::TextDate) + " :";

    return titleWithDate;
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

    UserEditorDialog editUserDialog(m_UserDataPtr, this);

    editUserDialog.exec();
    
    fillUserIdBoxData();

    updatePerDayView();
}

void UserDashboard::handleUserLoginAction()
{
    if (!dbIsConnected())
    {
        Q_EMIT handleDatabaseConnectionAction();
    }

    LoginDialog userLogin(this);

    userLogin.exec();

    m_UserDataPtr = userLogin.GetUserData();

    fillUserIdBoxData();

    updatePerDayView();
}

void UserDashboard::handleDatabaseConnectionAction()
{
    DataBaseConnectionDialog dbConnectionDialog(this);

    dbConnectionDialog.exec();
}

void UserDashboard::handleDateChanged(const QDate &newDate)
{
    m_DashboardDate = newDate;

    updatePerDayView();

    fillUserIdBoxData();
}

void UserDashboard::handleToDoMenuClicked()
{
    TodoWindow* todoExternal = new TodoWindow(m_UserDataPtr, m_DashboardDate, false, this);

    todoExternal->setUpWindowUi();

    todoExternal->show();
}

void UserDashboard::handleOpenGoalWindowClicked()
{
    GoalWindow* goalExternalWindow = new GoalWindow(m_UserDataPtr, m_DashboardDate, false, this);

    goalExternalWindow->setUpWindowUi();

    goalExternalWindow->show();
}
