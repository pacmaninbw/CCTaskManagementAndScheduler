#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

class UserModel;
class GuiScheduleItemModel;
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
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <memory>
#include <string>

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
    void handleEditScheduleItemAction();
    void handleScheduleClicked(const QModelIndex &index);
    void handleDatabaseConnectionAction();
    void handleDateChanged(const QDate &newDate);

private:
    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpTaskMenu();
    void setUpUserMenu();
    void setUpNoteMenu();
    void setUpGoalMenu();
    void setUpScheduleItemMenu();
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

    QString m_ProgNameStr;
    std::shared_ptr<UserModel> m_UserDataPtr;
    GuiScheduleItemModel* m_ScheduleItemToEdit;
    QDate m_DashboardDate;

    QMenu* udTaskMenu;
    QMenu* udUserMenu;
    QMenu* udNoteMenu;
    QMenu* udGoalMenu;
    QMenu* udScheduleItemMenu;
    QMenu* udDBConnectionMenu;
    QAction* udActionAddTask;
    QAction* udActionAddScheduleItem;
    QAction* udActionEditScheduleItem;
    QAction* udActionAddNote;
    QAction* udActionAddUserProfile;
    QAction* udActionEditUserProfile;
    QAction* udActionUserLogin;
    QAction* udActionUserLogout;
    QAction* udActionAddGoal;
    QAction* udActionEditGoal;
    QAction* udActionConnectDB;
    QWidget* centralwidget;
    QGroupBox* udTaskListGB;
    QGroupBox* udScheduleGB;
    QGroupBox* udNotesGB;
    QGroupBox* userDashBoardIDGB;
    QLineEdit* udUserFirstNameDisplay;
    QLineEdit* udUserMiddleInitialDisplay;
    QLineEdit* udUserLastNameDisplay;
    QLineEdit* udUserNameDisplay;
    QDateEdit* udDateSelectorDE;
    DashboardTaskViewer* udTaskTableView;
    ScheduleTablerViewer* udScheduleTableView;
    DashboardNotesViewer* udNotesTableView;

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


