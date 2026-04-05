#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

class UserModel;
class ScheduleItemModel;
class DashboardTaskViewer;
class ScheduleTablerViewer;
class DashboardNotesViewer;

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
    void handleAddTaskAction();
    void handleTaskTableClicked(const QModelIndex &index);
    void handleAddUserAction();
    void handleEditUserAction();
    void handleAddNoteAction();
    void handleNoteTableClicked(const QModelIndex &index);
    void handleUserLoginAction();
    void handleAddGoalAction();
    void handleEditGoalAction();
    void handleAddScheduleItemAction();
    void handleScheduleClicked(const QModelIndex &index);
    void handleDatabaseConnectionAction();
    void handleDateChanged(const QDate &newDate);

private:
    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpUserMenu();
    void setUpGoalMenu();
    void setUpDbConnectionMenu();
    bool userIsLoggedIn();
    bool dbIsConnected();
    QGroupBox* setUpUserIdBox();
    QHBoxLayout* setUpPerDayLayout();
    QGroupBox* setUpPerDayTaskGB();
    QGroupBox* setUpPerDayScheduleGB();
    QGroupBox* setUpPerDayNotesGB();
    void fillUserIdBoxData();
    DashboardTaskViewer* updateTaskList();
    ScheduleTablerViewer* updateSchedule();
    DashboardNotesViewer* updateNotes();
    QString groupBoxTitleWithDate(QString gbTitleBase);

    QString m_ProgNameStr;
    std::shared_ptr<UserModel> m_UserDataPtr;
    QDate m_DashboardDate;

    QMenu* udUserMenu = nullptr;
    QMenu* udGoalMenu = nullptr;
    QMenu* udDBConnectionMenu = nullptr;
    QAction* udActionAddUserProfile = nullptr;
    QAction* udActionEditUserProfile = nullptr;
    QAction* udActionUserLogin = nullptr;
    QAction* udActionUserLogout = nullptr;
    QAction* udActionAddGoal = nullptr;
    QAction* udActionEditGoal = nullptr;
    QAction* udActionConnectDB = nullptr;
    QWidget* centralwidget = nullptr;
    QGroupBox* udTaskListGB = nullptr;
    QPushButton* udAddTaskPB = nullptr;
    QGroupBox* udScheduleGB = nullptr;
    QPushButton* udAddEventPB = nullptr;
    QGroupBox* udNotesGB = nullptr;
    QPushButton* udAddNotePB = nullptr;
    QGroupBox* userDashBoardIDGB = nullptr;
    QLineEdit* udUserFirstNameDisplay = nullptr;
    QLineEdit* udUserMiddleInitialDisplay = nullptr;
    QLineEdit* udUserLastNameDisplay = nullptr;
    QLineEdit* udUserNameDisplay = nullptr;
    QDateEdit* udDateSelectorDE = nullptr;
    DashboardTaskViewer* udTaskTableView = nullptr;
    ScheduleTablerViewer* udScheduleTableView = nullptr;
    DashboardNotesViewer* udNotesTableView = nullptr;

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


