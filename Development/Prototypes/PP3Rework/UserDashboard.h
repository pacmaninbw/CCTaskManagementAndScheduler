#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

// Project Header Files

// QT Header Files
#include "GuiUserModel.h"
#include "GuiTaskModel.h"
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QDateEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <string>

class UserDashboard : public QMainWindow
{
    Q_OBJECT

public:
    UserDashboard(QWidget *parent = nullptr);
    ~UserDashboard();

public slots:

signals:

private slots:
    void handleAddTaskAction();
    void handleEditTaskAction();
    void handleAddUserAction();
    void handleEditUserAction();
    void handleAddNoteAction();
    void handleUserLoginAction();
    void handleEditNoteAction();
    void handleAddGoalAction();
    void handleEditGoalAction();
    void handleAddScheduleItemAction();
    void handleEditScheduleItemAction();
    void handleDatabaseConnectionAction();

private:
    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpTaskMenu();
    void setUpUserMenu();
    void setUpNoteMenu();
    void setUpGoalMenu();
    void setUpScheduleItemMenu();
    void setUpDbConnectionMenu();
    void updateDashboardDisplayData();
    QGroupBox* setUpUserIdBox();
    QHBoxLayout* setUpPerDayLayout();
    QGroupBox* setUpPerDayTaskGB();
    QGroupBox* setUpPerDayScheduleGB();
    QGroupBox* setUpPerDayNotesGB();
    void fakeFillGroupBoxLayout(std::string fieldPartialName, QVBoxLayout* layoutToFill);
    void fillUserIdBoxData();
    QGroupBox* updateTaskList();
    QGroupBox* updateSchedule();
    QGroupBox* listNotes();

    QString m_ProgNameStr;
    std::size_t m_UserID;
    GuiUserModel* m_UserDataPtr;
    GuiTaskModel* m_TaskToEdit;

    QMenu* udTaskMenu;
    QMenu* udUserMenu;
    QMenu* udNoteMenu;
    QMenu* udGoalMenu;
    QMenu* udScheduleItemMenu;
    QMenu* udDBConnectionMenu;
    QAction* udActionAddTask;
    QAction* udActionEditTask;
    QAction* udActionAddScheduleItem;
    QAction* udActionEditScheduleItem;
    QAction* udActionAddNote;
    QAction* udActionEditNote;
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


