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

    m_centralwidget = new QWidget(this);
    m_centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(dashboardWidth, dashboardHeight);

    QVBoxLayout* dashboardLayout = new QVBoxLayout(m_centralwidget);
    dashboardLayout->setObjectName("dashboardLayout");

    dashboardLayout->addWidget(setUpUserIdBox(), 0, Qt::AlignHCenter);

    QFormLayout* dashboardDateForm = cqtfa_FormLayoutWithPolicy("dashboardDateForm", nullptr);

    m_DateSelector = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_DateSelector", this);
    
    connect(m_DateSelector, &QDateEdit::dateChanged, this, &UserDashboard::handleDateChanged);

    dashboardDateForm->addRow("Date:", m_DateSelector);

    dashboardDateForm->setFormAlignment(Qt::AlignHCenter);

    dashboardLayout->addLayout(dashboardDateForm);

    dashboardLayout->addLayout(setUpPerDayLayout());

    setCentralWidget(m_centralwidget);

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
    m_LoginUserMenu = new QAction("Login", this);
    m_LoginUserMenu->setStatusTip(tr("Login as a user"));
    connect(m_LoginUserMenu, &QAction::triggered, this, &UserDashboard::handleUserLoginAction);

    m_AddUserProfileUserMenu = new QAction("Add User", this);
    m_AddUserProfileUserMenu->setStatusTip(tr("Create a new User"));
    connect(m_AddUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleAddUserAction);

    m_EditUserProfileUserMenu = new QAction("Edit User", this);
    m_EditUserProfileUserMenu->setStatusTip(tr("Edit your profile"));
    connect(m_EditUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleEditUserAction);

    m_LogoutUserMenu = new QAction("Logout", this);
    m_LogoutUserMenu->setStatusTip(tr("Exit the planner app"));
    connect(m_LogoutUserMenu, &QAction::triggered, this, &QApplication::quit);

    m_UserMenu = menuBar()->addMenu("&User");
    m_UserMenu->addAction(m_LoginUserMenu);
    m_UserMenu->addAction(m_AddUserProfileUserMenu);
    m_UserMenu->addAction(m_EditUserProfileUserMenu);
    m_UserMenu->addAction(m_LogoutUserMenu);
    m_UserMenu->addSeparator();
}

void UserDashboard::setUpGoalMenu()
{
    m_OpenGoalMenu = new QAction("Open Goal Window", this);
    m_OpenGoalMenu->setStatusTip("Open Goal Window");
    connect(m_OpenGoalMenu, &QAction::triggered, this, &UserDashboard::handleOpenGoalWindowClicked);

    m_GoalMenu = menuBar()->addMenu("&Goal");
    m_GoalMenu->addAction(m_OpenGoalMenu);
    m_GoalMenu->addSeparator();
}

void UserDashboard::setUpTodoMenu()
{
    m_OpenTodoMenu = new QAction("Todo List", this);
    m_OpenTodoMenu->setStatusTip(tr("Create a new Goal"));
    connect(m_OpenTodoMenu, &QAction::triggered, this, &UserDashboard::handleToDoMenuClicked);

    m_TodoMenu = menuBar()->addMenu("&Todo List");
    m_TodoMenu->addAction(m_OpenTodoMenu);
    m_TodoMenu->addSeparator();
}

void UserDashboard::setUpDbConnectionMenu()
{
    m_ConnectDBMenuItem = new QAction("Connect Database", this);
    m_ConnectDBMenuItem->setStatusTip("Add Database User and Password");
    connect(m_ConnectDBMenuItem, &QAction::triggered, this, &UserDashboard::handleDatabaseConnectionAction);

    m_DBConnectionMenu = menuBar()->addMenu("&DB Connection");
    m_DBConnectionMenu->addAction(m_ConnectDBMenuItem);
    m_DBConnectionMenu->addSeparator();
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
    m_userIDGroupBox = new QGroupBox("User ID:", m_centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    m_UserFirstNameDisplay = cqtfa_LineEditFixedWidthByCharCount("m_UserFirstNameDisplay", m_userIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_UserFirstNameDisplay);

    m_UserMiddleInitialDisplay = cqtfa_LineEditFixedWidthByCharCount("m_UserMiddleInitialDisplay", m_userIDGroupBox, userNameWidth / 4);
    uiBoxLaytout->addWidget(m_UserMiddleInitialDisplay);

    m_UserLastNameDisplay = cqtfa_LineEditFixedWidthByCharCount("m_UserLastNameDisplay", m_userIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_UserLastNameDisplay);

    m_dUserLoginDisplay = cqtfa_LineEditFixedWidthByCharCount("m_dUserLoginDisplay", m_userIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_dUserLoginDisplay);

    m_userIDGroupBox->setLayout(uiBoxLaytout);
    m_userIDGroupBox->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    m_userIDGroupBox->setAlignment(Qt::AlignCenter);

    return m_userIDGroupBox;
}

QHBoxLayout *UserDashboard::setUpPerDayLayout()
{
    QHBoxLayout* perDayLayout = new QHBoxLayout;
    perDayLayout->setObjectName("perDayLayout");

    m_todoWindow = setUpTodoList();
    perDayLayout->addWidget(m_todoWindow);

    m_scheduleWindow = setUpSchedule();
    perDayLayout->addWidget(m_scheduleWindow);

    m_noteWindow = setUpNotesWindow();
    perDayLayout->addWidget(m_noteWindow);

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
    if (m_todoWindow)
    {
        m_todoWindow->setUser(m_UserDataPtr);
        m_todoWindow->setDate(m_DashboardDate);
        m_todoWindow->refresh();
    }

    if (m_scheduleWindow)
    {
        m_scheduleWindow->setUser(m_UserDataPtr);
        m_scheduleWindow->setDate(m_DashboardDate);
        m_scheduleWindow->refresh();
    }

    if (m_noteWindow)
    {
        m_noteWindow->setUser(m_UserDataPtr);
        m_noteWindow->setDate(m_DashboardDate);
        m_noteWindow->refresh();
    }
}

void UserDashboard::fillUserIdBoxData()
{
    m_UserFirstNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getFirstName()));
    m_UserMiddleInitialDisplay->setText(QString::fromStdString(m_UserDataPtr->getMiddleInitial()));
    m_UserLastNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLastName()));
    m_dUserLoginDisplay->setText(QString::fromStdString(m_UserDataPtr->getLoginName()));
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
