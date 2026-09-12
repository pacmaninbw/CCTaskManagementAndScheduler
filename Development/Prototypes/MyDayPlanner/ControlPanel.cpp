// Project Header
#include "CommandLineParser.h"
#include "CommonWidgetExtensions.h"  // cqtfa_ functions
#include "ControlPanel.h"
#include "LoginDialog.h"
#include "NotesWindow.h"
#include "ScheduleWindow.h"
#include "ModelSubWindow.h"
#include "TodoWindow.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files
#include <memory>
#include <vector>



ControlPanel::ControlPanel(std::shared_ptr<UserModel> loggedInUser, QWidget *parent)
    : QMainWindow(parent),
    m_userDataPtr{loggedInUser},
    m_currentDate{QDate::currentDate()}
{
    m_progNameStr = QString::fromStdString(programOptions.progName);
    m_progNameStr += " : Control Panel";

    setUpControlPanelUi();

    fillUserIdBox();

    updateViewWindows();
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::handleAddUserAction()
{
    UserEditorDialog addUserDialog(this);
    addUserDialog.exec();
}

void ControlPanel::handleEditUserAction()
{
    UserEditorDialog editUserDialog(m_userDataPtr, this);
    editUserDialog.exec();
}

void ControlPanel::handleDateChanged(const QDate &newDate)
{
    m_currentDate = newDate;
    updateViewWindows();
}

void ControlPanel::handleToDoMenuClicked()
{
    TodoWindow* todoExternal = new TodoWindow(m_userDataPtr, m_currentDate, false, this);
    todoExternal->setUpWindowUi();
    todoExternal->show();
}

void ControlPanel::handleOpenGoalWindowClicked()
{
}

void ControlPanel::handleOpenScheduleWindowClicked()
{
    ScheduleWindow* scheduleExternalWindow = new ScheduleWindow(m_userDataPtr, m_currentDate, false, this);
    scheduleExternalWindow->setUpWindowUi();
    scheduleExternalWindow->show();
}

void ControlPanel::handleOpenNotesWindowClicked()
{
    NotesWindow* noteExternalWindow = new NotesWindow(m_userDataPtr, m_currentDate, false, this);
    noteExternalWindow->setUpWindowUi();
    noteExternalWindow->show();
}

void ControlPanel::setUpControlPanelUi()
{
    setUpMenuBar();

    m_qt_centralwidget = new QWidget(this);
    m_qt_centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(controlPanelWidth, controlPanelHeight);

    QVBoxLayout* controlPanelLayout = new QVBoxLayout(m_qt_centralwidget);
    controlPanelLayout->setObjectName("dashboardLayout");

    controlPanelLayout->addWidget(setUpUserIdBox(), 0, Qt::AlignHCenter);

    QFormLayout* dashboardDateForm = common::formLayoutWithPolicy("dashboardDateForm", nullptr);

    m_qt_dateSelector = common::dateEditWithCalendar("m_qt_dateSelector", this);

    connect(m_qt_dateSelector, &QDateEdit::dateChanged, this, &ControlPanel::handleDateChanged);

    dashboardDateForm->addRow("Date:", m_qt_dateSelector);
    dashboardDateForm->setFormAlignment(Qt::AlignHCenter);
    controlPanelLayout->addLayout(dashboardDateForm);
    controlPanelLayout->addLayout(setUpSubWindowsLayout());

    setCentralWidget(m_qt_centralwidget);

    setWindowTitle(m_progNameStr);
}

void ControlPanel::setUpMenuBar()
{
    setUpUserMenu();
    setUpViewsMenu();
}

void ControlPanel::setUpUserMenu()
{
    m_qt_addUserProfileUserMenu = new QAction("Add User", this);
    m_qt_addUserProfileUserMenu->setStatusTip(tr("Create a new User"));
    connect(m_qt_addUserProfileUserMenu, &QAction::triggered, this, &ControlPanel::handleAddUserAction);

    m_qt_editUserProfileUserMenu = new QAction("Edit User", this);
    m_qt_editUserProfileUserMenu->setStatusTip(tr("Edit your profile"));
    connect(m_qt_editUserProfileUserMenu, &QAction::triggered, this, &ControlPanel::handleEditUserAction);

    m_qt_userMenu = menuBar()->addMenu("&User");
    m_qt_userMenu->addAction(m_qt_addUserProfileUserMenu);
    m_qt_userMenu->addAction(m_qt_editUserProfileUserMenu);
    m_qt_userMenu->addSeparator();
}

void ControlPanel::setUpViewsMenu()
{
    m_qt_openTodoMenu = new QAction("Todo List", this);
    m_qt_openTodoMenu->setStatusTip(tr("Open the Todo List Window"));
    connect(m_qt_openTodoMenu, &QAction::triggered, this, &ControlPanel::handleToDoMenuClicked);

    m_qt_openScheduleMenu = new QAction("Open Schedule Window", this);
    m_qt_openScheduleMenu->setStatusTip("Open Schedule Window");
    connect(m_qt_openScheduleMenu, &QAction::triggered, this, &ControlPanel::handleOpenScheduleWindowClicked);

    m_qt_openNotesMenu = new QAction("Open Notes Window", this);
    m_qt_openNotesMenu->setStatusTip("Open Notes Window");
    connect(m_qt_openNotesMenu, &QAction::triggered, this, &ControlPanel::handleOpenNotesWindowClicked);

    m_qt_openGoalMenu = new QAction("Open Goal Window", this);
    m_qt_openGoalMenu->setStatusTip("Open Goal Window");
    connect(m_qt_openGoalMenu, &QAction::triggered, this, &ControlPanel::handleOpenGoalWindowClicked);

    m_qt_viewsMenu = menuBar()->addMenu("&Views");
    m_qt_viewsMenu->addAction(m_qt_openTodoMenu);
    m_qt_viewsMenu->addAction(m_qt_openScheduleMenu);
    m_qt_viewsMenu->addAction(m_qt_openNotesMenu);
    m_qt_viewsMenu->addAction(m_qt_openGoalMenu);
    m_qt_viewsMenu->addSeparator();
}

void ControlPanel::setUpOrganizationMenu()
{
}

QHBoxLayout* ControlPanel::setUpSubWindowsLayout()
{
    QHBoxLayout* subWindowsLayout = new QHBoxLayout;
    subWindowsLayout->setObjectName("subWindowsLayout");

    initViewWindows();

    for (auto subWindow: m_viewWindows)
    {
        subWindow->setUpWindowUi();
        subWindow->show();
        subWindowsLayout->addWidget(subWindow);
    }

    return subWindowsLayout;
}

void ControlPanel::initViewWindows()
{
    m_viewWindows.push_back(new ScheduleWindow(m_userDataPtr, m_currentDate, true, this));
    m_viewWindows.push_back(new TodoWindow(m_userDataPtr, m_currentDate, true, this));
}

void ControlPanel::updateViewWindows()
{
    for (auto view: m_viewWindows)
    {
        view->changeDataRefreshTable(m_userDataPtr, m_currentDate);
    }
}

QGroupBox* ControlPanel::setUpUserIdBox()
{
    QGroupBox* userIdGroupBox = new QGroupBox("User ID:", m_qt_centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    m_qt_userFirstName = common::lineEditWidthByCharCount("m_qt_userFirstName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userFirstName);

    m_qt_userLastName = common::lineEditWidthByCharCount("m_qt_userLastName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userLastName);

    m_qt_userLogin = common::lineEditWidthByCharCount("m_qt_userLogin", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userLogin);

    userIdGroupBox->setLayout(uiBoxLaytout);
    userIdGroupBox->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    userIdGroupBox->setAlignment(Qt::AlignCenter);

    return userIdGroupBox;
}

void ControlPanel::fillUserIdBox()
{
    m_qt_userFirstName->setText(QString::fromStdString(m_userDataPtr->getFirstName()));
    m_qt_userLastName->setText(QString::fromStdString(m_userDataPtr->getLastName()));
    m_qt_userLogin->setText(QString::fromStdString(m_userDataPtr->getLoginName()));
}
