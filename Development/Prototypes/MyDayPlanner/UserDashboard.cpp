// Project Header
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "GoalWindow.h"
#include "LoginDialog.h"
#include "NotesWindow.h"
#include "ScheduleWindow.h"
#include "ModelSubWindow.h"
#include "TodoWindow.h"
#include "UserDashboard.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files

UserDashboard::UserDashboard(std::shared_ptr<UserModel> loggedInUser, QWidget *parent)
    : QMainWindow(parent),
    m_DashboardDate{QDate::currentDate()}
{
    m_ProgNameStr = QString::fromStdString(programOptions.progName);

    m_UserDataPtr = loggedInUser;

    setUpUserDashboardUi();

    fillUserIdBoxData();
    updatePerDayView();
}

UserDashboard::~UserDashboard()
{
}

void UserDashboard::setUpUserDashboardUi()
{
    setUpDashboardMenuBar();

    m_qt_Centralwidget = new QWidget(this);
    m_qt_Centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(dashboardWidth, dashboardHeight);

    QVBoxLayout* dashboardLayout = new QVBoxLayout(m_qt_Centralwidget);
    dashboardLayout->setObjectName("dashboardLayout");

    dashboardLayout->addWidget(setUpUserIdBox(), 0, Qt::AlignHCenter);

    QFormLayout* dashboardDateForm = cqtfa_FormLayoutWithPolicy("dashboardDateForm", nullptr);

    m_qt_DateSelector = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_DateSelector", this);
    
    connect(m_qt_DateSelector, &QDateEdit::dateChanged, this, &UserDashboard::handleDateChanged);

    dashboardDateForm->addRow("Date:", m_qt_DateSelector);
    dashboardDateForm->setFormAlignment(Qt::AlignHCenter);
    dashboardLayout->addLayout(dashboardDateForm);
    dashboardLayout->addLayout(setUpPerDayLayout());

    setCentralWidget(m_qt_Centralwidget);

    setWindowTitle(m_ProgNameStr);
}

void UserDashboard::setUpDashboardMenuBar()
{
    setUpUserMenu();
    setUpViewsMenu();
}

void UserDashboard::setUpUserMenu()
{
    m_qt_AddUserProfileUserMenu = new QAction("Add User", this);
    m_qt_AddUserProfileUserMenu->setStatusTip(tr("Create a new User"));
    connect(m_qt_AddUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleAddUserAction);

    m_qt_EditUserProfileUserMenu = new QAction("Edit User", this);
    m_qt_EditUserProfileUserMenu->setStatusTip(tr("Edit your profile"));
    connect(m_qt_EditUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleEditUserAction);

    m_qt_UserMenu = menuBar()->addMenu("&User");
    m_qt_UserMenu->addAction(m_qt_AddUserProfileUserMenu);
    m_qt_UserMenu->addAction(m_qt_EditUserProfileUserMenu);
    m_qt_UserMenu->addSeparator();
}

void UserDashboard::setUpViewsMenu()
{
    m_qt_OpenTodoMenu = new QAction("Todo List", this);
    m_qt_OpenTodoMenu->setStatusTip(tr("Open the Todo List Window"));
    connect(m_qt_OpenTodoMenu, &QAction::triggered, this, &UserDashboard::handleToDoMenuClicked);

    m_qt_OpenScheduleMenu = new QAction("Open Schedule Window", this);
    m_qt_OpenScheduleMenu->setStatusTip("Open Schedule Window");
    connect(m_qt_OpenScheduleMenu, &QAction::triggered, this, &UserDashboard::handleOpenScheduleWindowClicked);

    m_qt_OpenNotesMenu = new QAction("Open Notes Window", this);
    m_qt_OpenNotesMenu->setStatusTip("Open Notes Window");
    connect(m_qt_OpenNotesMenu, &QAction::triggered, this, &UserDashboard::handleOpenNotesWindowClicked);

    m_qt_OpenGoalMenu = new QAction("Open Goal Window", this);
    m_qt_OpenGoalMenu->setStatusTip("Open Goal Window");
    connect(m_qt_OpenGoalMenu, &QAction::triggered, this, &UserDashboard::handleOpenGoalWindowClicked);

    m_qt_ViewsMenu = menuBar()->addMenu("&Views");
    m_qt_ViewsMenu->addAction(m_qt_OpenTodoMenu);
    m_qt_ViewsMenu->addAction(m_qt_OpenScheduleMenu);
    m_qt_ViewsMenu->addAction(m_qt_OpenNotesMenu);
    m_qt_ViewsMenu->addAction(m_qt_OpenGoalMenu);
    m_qt_ViewsMenu->addSeparator();
}

QGroupBox *UserDashboard::setUpUserIdBox()
{
    m_qt_UserIDGroupBox = new QGroupBox("User ID:", m_qt_Centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    m_qt_UserFirstNameDisplay = cqtfa_LineEditFixedWidthByCharCount("m_qt_UserFirstNameDisplay", m_qt_UserIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_UserFirstNameDisplay);

    m_qt_UserMiddleInitialDisplay = cqtfa_LineEditFixedWidthByCharCount("m_qt_UserMiddleInitialDisplay", m_qt_UserIDGroupBox, userNameWidth / 4);
    uiBoxLaytout->addWidget(m_qt_UserMiddleInitialDisplay);

    m_qt_UserLastNameDisplay = cqtfa_LineEditFixedWidthByCharCount("m_qt_UserLastNameDisplay", m_qt_UserIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_UserLastNameDisplay);

    m_qt_UserLoginDisplay = cqtfa_LineEditFixedWidthByCharCount("m_qt_UserLoginDisplay", m_qt_UserIDGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_UserLoginDisplay);

    m_qt_UserIDGroupBox->setLayout(uiBoxLaytout);
    m_qt_UserIDGroupBox->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    m_qt_UserIDGroupBox->setAlignment(Qt::AlignCenter);

    return m_qt_UserIDGroupBox;
}

QHBoxLayout *UserDashboard::setUpPerDayLayout()
{
    QHBoxLayout* perDayLayout = new QHBoxLayout;
    perDayLayout->setObjectName("perDayLayout");

    initPerDayViewWindows();

    for (auto perDayWindow: m_PerDayViewWindows)
    {
        perDayWindow->setUpWindowUi();
        perDayWindow->show();
        perDayLayout->addWidget(perDayWindow);
    }
    return perDayLayout;
}

void UserDashboard::initPerDayViewWindows()
{
    m_PerDayViewWindows.push_back(new TodoWindow(m_UserDataPtr, m_DashboardDate, true, this));
    m_PerDayViewWindows.push_back(new ScheduleWindow(m_UserDataPtr, m_DashboardDate, true, this));
    m_PerDayViewWindows.push_back(new NotesWindow(m_UserDataPtr, m_DashboardDate, true, this));
}

void UserDashboard::updatePerDayView()
{
    for (auto perDayWindow: m_PerDayViewWindows)
    {
        perDayWindow->changeDataRefreshTable(m_UserDataPtr, m_DashboardDate);
    }
}

void UserDashboard::fillUserIdBoxData()
{
    m_qt_UserFirstNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getFirstName()));
    m_qt_UserMiddleInitialDisplay->setText(QString::fromStdString(m_UserDataPtr->getMiddleInitial()));
    m_qt_UserLastNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLastName()));
    m_qt_UserLoginDisplay->setText(QString::fromStdString(m_UserDataPtr->getLoginName()));
}

void UserDashboard::handleAddUserAction()
{
    UserEditorDialog addUserDialog(this);
    addUserDialog.exec();
}

void UserDashboard::handleEditUserAction()
{
    UserEditorDialog editUserDialog(m_UserDataPtr, this);
    editUserDialog.exec();    
    fillUserIdBoxData();
    updatePerDayView();
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

void UserDashboard::handleOpenScheduleWindowClicked()
{
    ScheduleWindow* scheduleExternalWindow = new ScheduleWindow(m_UserDataPtr, m_DashboardDate, false, this);
    scheduleExternalWindow->setUpWindowUi();
    scheduleExternalWindow->show();
}

void UserDashboard::handleOpenNotesWindowClicked()
{
    NotesWindow* noteExternalWindow = new NotesWindow(m_UserDataPtr, m_DashboardDate, false, this);
    noteExternalWindow->setUpWindowUi();
    noteExternalWindow->show();
}
