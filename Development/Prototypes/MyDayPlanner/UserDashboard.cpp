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
    m_dashboardDate{QDate::currentDate()}
{
    m_progNameStr = QString::fromStdString(programOptions.progName);

    m_userDataPtr = loggedInUser;

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

    m_qt_centralwidget = new QWidget(this);
    m_qt_centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(dashboardWidth, dashboardHeight);

    QVBoxLayout* dashboardLayout = new QVBoxLayout(m_qt_centralwidget);
    dashboardLayout->setObjectName("dashboardLayout");

    dashboardLayout->addWidget(setUpUserIdBox(), 0, Qt::AlignHCenter);

    QFormLayout* dashboardDateForm = cqtfa_FormLayoutWithPolicy("dashboardDateForm", nullptr);

    m_qt_dateSelector = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_dateSelector", this);
    
    connect(m_qt_dateSelector, &QDateEdit::dateChanged, this, &UserDashboard::handleDateChanged);

    dashboardDateForm->addRow("Date:", m_qt_dateSelector);
    dashboardDateForm->setFormAlignment(Qt::AlignHCenter);
    dashboardLayout->addLayout(dashboardDateForm);
    dashboardLayout->addLayout(setUpPerDayLayout());

    setCentralWidget(m_qt_centralwidget);

    setWindowTitle(m_progNameStr);
}

void UserDashboard::setUpDashboardMenuBar()
{
    setUpUserMenu();
    setUpViewsMenu();
}

void UserDashboard::setUpUserMenu()
{
    m_qt_addUserProfileUserMenu = new QAction("Add User", this);
    m_qt_addUserProfileUserMenu->setStatusTip(tr("Create a new User"));
    connect(m_qt_addUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleAddUserAction);

    m_qt_editUserProfileUserMenu = new QAction("Edit User", this);
    m_qt_editUserProfileUserMenu->setStatusTip(tr("Edit your profile"));
    connect(m_qt_editUserProfileUserMenu, &QAction::triggered, this, &UserDashboard::handleEditUserAction);

    m_qt_userMenu = menuBar()->addMenu("&User");
    m_qt_userMenu->addAction(m_qt_addUserProfileUserMenu);
    m_qt_userMenu->addAction(m_qt_editUserProfileUserMenu);
    m_qt_userMenu->addSeparator();
}

void UserDashboard::setUpViewsMenu()
{
    m_qt_openTodoMenu = new QAction("Todo List", this);
    m_qt_openTodoMenu->setStatusTip(tr("Open the Todo List Window"));
    connect(m_qt_openTodoMenu, &QAction::triggered, this, &UserDashboard::handleToDoMenuClicked);

    m_qt_openScheduleMenu = new QAction("Open Schedule Window", this);
    m_qt_openScheduleMenu->setStatusTip("Open Schedule Window");
    connect(m_qt_openScheduleMenu, &QAction::triggered, this, &UserDashboard::handleOpenScheduleWindowClicked);

    m_qt_openNotesMenu = new QAction("Open Notes Window", this);
    m_qt_openNotesMenu->setStatusTip("Open Notes Window");
    connect(m_qt_openNotesMenu, &QAction::triggered, this, &UserDashboard::handleOpenNotesWindowClicked);

    m_qt_openGoalMenu = new QAction("Open Goal Window", this);
    m_qt_openGoalMenu->setStatusTip("Open Goal Window");
    connect(m_qt_openGoalMenu, &QAction::triggered, this, &UserDashboard::handleOpenGoalWindowClicked);

    m_qt_viewsMenu = menuBar()->addMenu("&Views");
    m_qt_viewsMenu->addAction(m_qt_openTodoMenu);
    m_qt_viewsMenu->addAction(m_qt_openScheduleMenu);
    m_qt_viewsMenu->addAction(m_qt_openNotesMenu);
    m_qt_viewsMenu->addAction(m_qt_openGoalMenu);
    m_qt_viewsMenu->addSeparator();
}

QGroupBox *UserDashboard::setUpUserIdBox()
{
    m_qt_userIdGroupBox = new QGroupBox("User ID:", m_qt_centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    m_qt_userFirstName = cqtfa_LineEditFixedWidthByCharCount("m_qt_userFirstName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userFirstName);

    m_qt_userMiddleInitial = cqtfa_LineEditFixedWidthByCharCount("m_qt_userMiddleInitial", m_qt_userIdGroupBox, userNameWidth / 4);
    uiBoxLaytout->addWidget(m_qt_userMiddleInitial);

    m_qt_userLastName = cqtfa_LineEditFixedWidthByCharCount("m_qt_userLastName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userLastName);

    m_qt_userLogin = cqtfa_LineEditFixedWidthByCharCount("m_qt_userLogin", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userLogin);

    m_qt_userIdGroupBox->setLayout(uiBoxLaytout);
    m_qt_userIdGroupBox->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    m_qt_userIdGroupBox->setAlignment(Qt::AlignCenter);

    return m_qt_userIdGroupBox;
}

QHBoxLayout *UserDashboard::setUpPerDayLayout()
{
    QHBoxLayout* perDayLayout = new QHBoxLayout;
    perDayLayout->setObjectName("perDayLayout");

    initPerDayViewWindows();

    for (auto perDayWindow: m_perDayViewWindows)
    {
        perDayWindow->setUpWindowUi();
        perDayWindow->show();
        perDayLayout->addWidget(perDayWindow);
    }
    return perDayLayout;
}

void UserDashboard::initPerDayViewWindows()
{
    m_perDayViewWindows.push_back(new TodoWindow(m_userDataPtr, m_dashboardDate, true, this));
    m_perDayViewWindows.push_back(new ScheduleWindow(m_userDataPtr, m_dashboardDate, true, this));
    m_perDayViewWindows.push_back(new NotesWindow(m_userDataPtr, m_dashboardDate, true, this));
}

void UserDashboard::updatePerDayView()
{
    for (auto perDayWindow: m_perDayViewWindows)
    {
        perDayWindow->changeDataRefreshTable(m_userDataPtr, m_dashboardDate);
    }
}

void UserDashboard::fillUserIdBoxData()
{
    m_qt_userFirstName->setText(QString::fromStdString(m_userDataPtr->getFirstName()));
    m_qt_userMiddleInitial->setText(QString::fromStdString(m_userDataPtr->getMiddleInitial()));
    m_qt_userLastName->setText(QString::fromStdString(m_userDataPtr->getLastName()));
    m_qt_userLogin->setText(QString::fromStdString(m_userDataPtr->getLoginName()));
}

void UserDashboard::handleAddUserAction()
{
    UserEditorDialog addUserDialog(this);
    addUserDialog.exec();
}

void UserDashboard::handleEditUserAction()
{
    UserEditorDialog editUserDialog(m_userDataPtr, this);
    editUserDialog.exec();    
    fillUserIdBoxData();
    updatePerDayView();
}

void UserDashboard::handleDateChanged(const QDate &newDate)
{
    m_dashboardDate = newDate;
    updatePerDayView();
    fillUserIdBoxData();
}

void UserDashboard::handleToDoMenuClicked()
{
    TodoWindow* todoExternal = new TodoWindow(m_userDataPtr, m_dashboardDate, false, this);
    todoExternal->setUpWindowUi();
    todoExternal->show();
}

void UserDashboard::handleOpenGoalWindowClicked()
{
    GoalWindow* goalExternalWindow = new GoalWindow(m_userDataPtr, m_dashboardDate, false, this);
    goalExternalWindow->setUpWindowUi();
    goalExternalWindow->show();
}

void UserDashboard::handleOpenScheduleWindowClicked()
{
    ScheduleWindow* scheduleExternalWindow = new ScheduleWindow(m_userDataPtr, m_dashboardDate, false, this);
    scheduleExternalWindow->setUpWindowUi();
    scheduleExternalWindow->show();
}

void UserDashboard::handleOpenNotesWindowClicked()
{
    NotesWindow* noteExternalWindow = new NotesWindow(m_userDataPtr, m_dashboardDate, false, this);
    noteExternalWindow->setUpWindowUi();
    noteExternalWindow->show();
}
