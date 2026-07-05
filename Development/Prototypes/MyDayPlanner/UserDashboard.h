#ifndef USERDASHBOARD_H_
#define USERDASHBOARD_H_

/*
 * Provide a day at a glance view for the user, opens with the current day but
 * the user can select any day. The default day at a glance shows the todo list,
 * the users schedule and any notes for the day selected.
 */
class UserModel;
class ModelSubWindow;

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
#include <QVBoxLayout>

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

    void setUpUserDashboardUi();
    void setUpDashboardMenuBar();
    void setUpUserMenu();
    void setUpViewsMenu();
    QGroupBox* setUpUserIdBox();
    QHBoxLayout* setUpPerDayLayout();
    void initPerDayViewWindows();
    void updatePerDayView();
    void fillUserIdBoxData();

    QMenu* m_qt_userMenu = nullptr;
    QMenu* m_qt_viewsMenu = nullptr;
    QAction* m_qt_addUserProfileUserMenu = nullptr;
    QAction* m_qt_editUserProfileUserMenu = nullptr;
    QAction* m_qt_openTodoMenu = nullptr;
    QAction* m_qt_openGoalMenu = nullptr;
    QAction* m_qt_openScheduleMenu = nullptr;
    QAction* m_qt_openNotesMenu = nullptr;
    QWidget* m_qt_centralwidget = nullptr;
    QGroupBox* m_qt_userIdGroupBox = nullptr;
    QLineEdit* m_qt_userFirstName = nullptr;
    QLineEdit* m_qt_userMiddleInitial = nullptr;
    QLineEdit* m_qt_userLastName = nullptr;
    QLineEdit* m_qt_userLogin = nullptr;
    QDateEdit* m_qt_dateSelector = nullptr;

    QString m_progNameStr;
    std::shared_ptr<UserModel> m_userDataPtr = nullptr;
    QDate m_dashboardDate;
    std::vector<ModelSubWindow*> m_perDayViewWindows;
};
#endif // USERDASHBOARD_H_


