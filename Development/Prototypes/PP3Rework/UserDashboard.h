#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

class UserModel;
class NotesWindow;
class ScheduleWindow;
class TodoWindow;
class GoalWindow;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QDate>
#include <QDateEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

class UserDashboard : public QMainWindow
{
    Q_OBJECT

public:
    UserDashboard(QWidget *parent = nullptr);
    UserDashboard(std::shared_ptr<UserModel> loggedInUser, QWidget *parent = nullptr);
    ~UserDashboard();

Q_SIGNALS:

private Q_SLOTS:
    void handleAddUserAction();
    void handleEditUserAction();
    void handleUserLoginAction();
    void handleDatabaseConnectionAction();
    void handleDateChanged(const QDate &newDate);
    void handleToDoMenuClicked();
    void handleOpenGoalWindowClicked();

private:
    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpUserMenu();
    void setUpGoalMenu();
    void setUpTodoMenu();
    void setUpDbConnectionMenu();
    bool userIsLoggedIn();
    bool dbIsConnected();
    QGroupBox* setUpUserIdBox();
    QHBoxLayout* setUpPerDayLayout();
    TodoWindow* setUpTodoList();
    ScheduleWindow* setUpSchedule();
    NotesWindow* setUpNotesWindow();
    void updatePerDayView();
    void fillUserIdBoxData();
    QString groupBoxTitleWithDate(QString gbTitleBase);

    QString m_ProgNameStr;
    std::shared_ptr<UserModel> m_UserDataPtr;
    QDate m_DashboardDate;

    QMenu* m_UserMenu = nullptr;
    QMenu* m_GoalMenu = nullptr;
    QMenu* m_DBConnectionMenu = nullptr;
    QMenu* m_TodoMenu = nullptr;
    QAction* m_AddUserProfileUserMenu = nullptr;
    QAction* m_EditUserProfileUserMenu = nullptr;
    QAction* m_LoginUserMenu = nullptr;
    QAction* m_LogoutUserMenu = nullptr;
    QAction* m_ConnectDBMenuItem = nullptr;
    QAction* m_OpenTodoMenu = nullptr;
    QAction* m_OpenGoalMenu = nullptr;
    QWidget* m_centralwidget = nullptr;
    QGroupBox* m_userIDGroupBox = nullptr;
    QLineEdit* m_UserFirstNameDisplay = nullptr;
    QLineEdit* m_UserMiddleInitialDisplay = nullptr;
    QLineEdit* m_UserLastNameDisplay = nullptr;
    QLineEdit* m_dUserLoginDisplay = nullptr;
    QDateEdit* m_DateSelector = nullptr;
    TodoWindow* m_todoWindow = nullptr;
    ScheduleWindow* m_scheduleWindow = nullptr;
    NotesWindow* m_noteWindow = nullptr;
    GoalWindow* m_goalWindow = nullptr;

/*
 * Size and positioning constants.
 */
    const int dashboardWidth = 1000;
    const int dashboardHeight = 720;
    const int userNameWidth = 40;
    const int maxOjectWidth = static_cast<int>(dashboardWidth * 0.8);
    const int perDayGroupBoxWidth = 300;
    const int perDayGroupBoxHeight = 400;
    const int perDayLineEditWidth = 280;
};
#endif // USERDASHBOARD_H


