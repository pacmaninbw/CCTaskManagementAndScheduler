#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

class UserModel;
class ScheduleItemModel;
class DashboardNotesViewer;
class TodoWindow;
class ScheduleWindow;
class NotesWindow;

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
#include <QTableView>
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
    void handleAddGoalAction();
    void handleEditGoalAction();
    void handleDatabaseConnectionAction();
    void handleDateChanged(const QDate &newDate);
    void handleToDoMenuClicked();

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
    QGroupBox* setUpPerDayNotesGB();
    void fillUserIdBoxData();
    QString groupBoxTitleWithDate(QString gbTitleBase);

    QString m_ProgNameStr;
    std::shared_ptr<UserModel> m_UserDataPtr;
    QDate m_DashboardDate;

    QMenu* udUserMenu = nullptr;
    QMenu* udGoalMenu = nullptr;
    QMenu* udDBConnectionMenu = nullptr;
    QMenu* udTodo = nullptr;
    QAction* udActionAddUserProfile = nullptr;
    QAction* udActionEditUserProfile = nullptr;
    QAction* udActionUserLogin = nullptr;
    QAction* udActionUserLogout = nullptr;
    QAction* udActionAddGoal = nullptr;
    QAction* udActionEditGoal = nullptr;
    QAction* udActionConnectDB = nullptr;
    QAction* udActionTodoWindow = nullptr;
    QWidget* centralwidget = nullptr;
    QPushButton* udAddEventPB = nullptr;
    QPushButton* udAddNotePB = nullptr;
    QGroupBox* userDashBoardIDGB = nullptr;
    QLineEdit* udUserFirstNameDisplay = nullptr;
    QLineEdit* udUserMiddleInitialDisplay = nullptr;
    QLineEdit* udUserLastNameDisplay = nullptr;
    QLineEdit* udUserNameDisplay = nullptr;
    QDateEdit* udDateSelectorDE = nullptr;
    TodoWindow* todoWindowInWindow = nullptr;
    ScheduleWindow* scheduleWindowInWindow = nullptr;
    NotesWindow* noteWindowInWindow = nullptr;

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


