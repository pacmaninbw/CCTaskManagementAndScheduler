#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

class UserModel;
class ModelSubWindow;
class GoalWindow;

// Project Header Files

// QT Header Files
#include <QAction>
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
#include <vector>

class UserDashboard : public QMainWindow
{
    Q_OBJECT

public:
    UserDashboard(std::shared_ptr<UserModel> loggedInUser, QWidget *parent = nullptr);
    ~UserDashboard();

Q_SIGNALS:

private Q_SLOTS:
    void handleAddUserAction();
    void handleEditUserAction();
    void handleDateChanged(const QDate &newDate);
    void handleToDoMenuClicked();
    void handleOpenGoalWindowClicked();
    void handleOpenScheduleWindowClicked();
    void handleOpenNotesWindowClicked();

private:
    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpUserMenu();
    void setUpViewsMenu();
    QGroupBox* setUpUserIdBox();
    QHBoxLayout* setUpPerDayLayout();
    void initPerDayViewWindows();
    void updatePerDayView();
    void fillUserIdBoxData();

    QString m_ProgNameStr;
    std::shared_ptr<UserModel> m_UserDataPtr = nullptr;
    QDate m_DashboardDate;

    QMenu* m_UserMenu = nullptr;
    QMenu* m_ViewsMenu = nullptr;
    QAction* m_AddUserProfileUserMenu = nullptr;
    QAction* m_EditUserProfileUserMenu = nullptr;
    QAction* m_LogoutUserMenu = nullptr;
    QAction* m_OpenTodoMenu = nullptr;
    QAction* m_OpenGoalMenu = nullptr;
    QAction* m_OpenScheduleMenu = nullptr;
    QAction* m_OpenNotesMenu = nullptr;
    QWidget* m_centralwidget = nullptr;
    QGroupBox* m_userIDGroupBox = nullptr;
    QLineEdit* m_UserFirstNameDisplay = nullptr;
    QLineEdit* m_UserMiddleInitialDisplay = nullptr;
    QLineEdit* m_UserLastNameDisplay = nullptr;
    QLineEdit* m_dUserLoginDisplay = nullptr;
    QDateEdit* m_DateSelector = nullptr;
    GoalWindow* m_goalWindow = nullptr;

    std::vector<ModelSubWindow*> m_PerDayViewWindows;

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


