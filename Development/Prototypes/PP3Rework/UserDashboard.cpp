// Project Header
#include "CommandLineParser.h"
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DashboardNotesViewer.h"
#include "DashboardTaskViewer.h"
#include "DataBaseConnectionDialog.h"
#include "GoalEditorDialog.h"
#include "LoginDialog.h"
#include "NoteEditorDialog.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleTablerViewer.h"
#include "TaskEditorDialog.h"
#include "TodoWindow.h"
#include "UserDashboard.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMenuBar>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files
#include <string>

UserDashboard::UserDashboard(QWidget *parent)
    : QMainWindow(parent),
    m_UserDataPtr{nullptr},
    m_DashboardDate{QDate::currentDate()},
    udScheduleTableView{nullptr},
    udNotesTableView{nullptr}
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

void UserDashboard::setUpTodoMenu()
{
    udActionTodoWindow = new QAction("Todo List", this);
    udActionTodoWindow->setStatusTip(tr("Create a new Goal"));
    connect(udActionTodoWindow, &QAction::triggered, this, &UserDashboard::handleToDoMenuClicked);

    udTodo = menuBar()->addMenu("&Todo List");
    udTodo->addAction(udActionTodoWindow);
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

    perDayLayout->addWidget(setUpPerDayScheduleGB());

    perDayLayout->addWidget(setUpPerDayNotesGB());

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

QGroupBox *UserDashboard::setUpPerDayScheduleGB()
{
    udScheduleGB = new QGroupBox(groupBoxTitleWithDate("Schedule"));
    QVBoxLayout* scheduleGBLayout = new QVBoxLayout;

    udAddEventPB = cqtfa_QTWidgetWithText<QPushButton>("Add Event to Schedule", "udAddEventPB", this);
    connect(udAddEventPB, &QPushButton::clicked, this, &UserDashboard::handleAddScheduleItemAction);
    scheduleGBLayout->addWidget(udAddEventPB);

    scheduleGBLayout->addWidget(updateSchedule());

    udScheduleGB->setLayout(scheduleGBLayout);

    udScheduleGB->setAlignment(Qt::AlignCenter);

    return udScheduleGB;
}

void UserDashboard::updatePerDayView()
{
    if (todoWindowInWindow)
    {
        todoWindowInWindow->setUser(m_UserDataPtr);
        todoWindowInWindow->setDate(m_DashboardDate);
        todoWindowInWindow->refresh();
    }

    if (udScheduleTableView)
    {
        udScheduleTableView->setUserId(m_UserDataPtr->getUserID());
        udScheduleTableView->setDate(m_DashboardDate);
        updateSchedule();
    }

    if (udNotesTableView)
    {
        udNotesTableView->setUserIdAndDate(m_UserDataPtr->getUserID(), m_DashboardDate);
        updateNotes();
    }
}

QGroupBox *UserDashboard::setUpPerDayNotesGB()
{
    udNotesGB = new QGroupBox(groupBoxTitleWithDate("Notes"));
    QVBoxLayout* notesGBLayOut = new QVBoxLayout;

    udAddNotePB = cqtfa_QTWidgetWithText<QPushButton>("Add a Note", "udAddNotePB", this);
    connect(udAddNotePB, &QPushButton::clicked, this, &UserDashboard::handleAddNoteAction);
    notesGBLayOut->addWidget(udAddNotePB);

    notesGBLayOut->addWidget(updateNotes());

    udNotesGB->setLayout(notesGBLayOut);

    udNotesGB->setAlignment(Qt::AlignCenter);

    return udNotesGB;
}

void UserDashboard::fillUserIdBoxData()
{
    udUserFirstNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getFirstName()));
    udUserMiddleInitialDisplay->setText(QString::fromStdString(m_UserDataPtr->getMiddleInitial()));
    udUserLastNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLastName()));
    udUserNameDisplay->setText(QString::fromStdString(m_UserDataPtr->getLoginName()));
}

ScheduleTablerViewer* UserDashboard::updateSchedule()
{
    if (!udScheduleTableView)
    {
        udScheduleTableView = new ScheduleTablerViewer(this);
        udScheduleTableView->setObjectName("udScheduleTableView");
        connect(udScheduleTableView, &QTableView::clicked, this, &UserDashboard::handleScheduleClicked);
        connect(udScheduleTableView, &QTableView::doubleClicked, this, &UserDashboard::handleScheduleClicked);
    }

    udScheduleTableView->clearSelection();
    udScheduleTableView->setUserIdAndDate(m_UserDataPtr->getUserID(), m_DashboardDate);
    udScheduleTableView->updateSchedule();
    udScheduleTableView->clearFocus();

    return udScheduleTableView;
}

DashboardNotesViewer *UserDashboard::updateNotes()
{
    if (!udNotesTableView)
    {
        udNotesTableView = new DashboardNotesViewer(m_UserDataPtr->getUserID(), m_DashboardDate, this);
        udNotesTableView->setObjectName("udNotesTableView");
        connect(udNotesTableView, &QTableView::clicked, this, &UserDashboard::handleNoteTableClicked);
        connect(udNotesTableView, &QTableView::doubleClicked, this, &UserDashboard::handleNoteTableClicked);
    }

    udNotesTableView->clearSelection();
    udNotesTableView->setUserIdAndDate(m_UserDataPtr->getUserID(), m_DashboardDate);
    udNotesTableView->update();
    udNotesTableView->clearFocus();

    return udNotesTableView;
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

void UserDashboard::handleAddNoteAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    NoteEditorDialog addNoteDialog(this, m_UserDataPtr->getUserID());

    addNoteDialog.exec();

    updateNotes();
}

void UserDashboard::handleNoteTableClicked(const QModelIndex &index)
{
    if (!userIsLoggedIn())
    {
        return;
    }

    if (!index.isValid())
    {
        return;
    }

    std::size_t noteToEdit = index.internalId();
    NoteEditorDialog editNoteDialog(this, m_UserDataPtr->getUserID(), noteToEdit);

    editNoteDialog.exec();

    updateNotes();
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

void UserDashboard::handleAddGoalAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    GoalEditorDialog addGoalDialog(m_UserDataPtr->getUserID(),this);

    addGoalDialog.exec();
}

void UserDashboard::handleEditGoalAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    std::size_t goalToEdit = 0;

    GoalEditorDialog editGoalDialog(m_UserDataPtr->getUserID(), this);
    if (goalToEdit)
    {
        if (editGoalDialog.getGoalFromDbInitFields(goalToEdit))
        {
            editGoalDialog.exec();
        }
    }
    else {
        editGoalDialog.exec();
    }

}

void UserDashboard::handleAddScheduleItemAction()
{
    if (!userIsLoggedIn())
    {
        return;
    }

    ScheduleItemEditorDialog addScheduleItemDialog(m_UserDataPtr->getUserID(), this);

    addScheduleItemDialog.initEditFields();

    addScheduleItemDialog.exec();
    
    updateSchedule();
}

void UserDashboard::handleScheduleClicked(const QModelIndex &index)
{
    if (!userIsLoggedIn())
    {
        return;
    }

    if (!index.isValid())
    {
        return;
    }

    std::size_t sheduleItemToEdit = static_cast<std::size_t>(index.internalId());

    ScheduleItemEditorDialog* editScheduleItemDialog;
    if (sheduleItemToEdit)
    {
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_UserDataPtr->getUserID(), sheduleItemToEdit, this);
    }
    else
    {
        std::chrono::system_clock::time_point startTime = udScheduleTableView->getScheduleItemStartTime(index);
        std::chrono::system_clock::time_point endTime = udScheduleTableView->getScheduleItemEndTime(index);
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_UserDataPtr->getUserID(), startTime,  endTime, this);
    }

    editScheduleItemDialog->initEditFields();

    editScheduleItemDialog->exec();
    
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

    udScheduleGB->setTitle(groupBoxTitleWithDate("Schedule"));
    udNotesGB->setTitle(groupBoxTitleWithDate("Notes"));

    updatePerDayView();

    fillUserIdBoxData();
}

void UserDashboard::handleToDoMenuClicked()
{
    TodoWindow* todoExternal = new TodoWindow(m_UserDataPtr, m_DashboardDate, false, this);

    todoExternal->setUpWindowUi();

    todoExternal->show();
}

