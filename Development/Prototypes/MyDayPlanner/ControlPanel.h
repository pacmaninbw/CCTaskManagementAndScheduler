#ifndef CONTROLPANEL_H_
#define CONTROLPANEL_H_

/*
 * Provide a control panel for a super user or power user to perform operations
 * that provide overall control of the application, such as adding or modifying
 * an organization, or adding or modifying a user.
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

class ControlPanel : public QMainWindow
{
    Q_OBJECT

public:
    explicit ControlPanel(std::shared_ptr<UserModel> loggedInUser, QWidget *parent = nullptr);
    ~ControlPanel();

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
    const int controlPanelWidth = 1000;
    const int controlPanelHeight = 720;
    const int userNameWidth = 40;
    const int maxOjectWidth = static_cast<int>(controlPanelWidth * 0.8);
    const int subWindowGroupBoxWidth = 300;
    const int subWindowGroupBoxHeight = 400;
    const int subWindowLineEditWidth = 280;

    void setUpControlPanelUi();
    void setUpMenuBar();
    void setUpUserMenu();
    void setUpViewsMenu();
    void setUpOrganizationMenu();
    QHBoxLayout* setUpSubWindowsLayout();
    void initViewWindows();
    void updateViewWindows();
    QGroupBox* setUpUserIdBox();
    void fillUserIdBox();

    QMenu* m_qt_userMenu = nullptr;
    QMenu* m_qt_viewsMenu = nullptr;
    QAction* m_qt_addUserProfileUserMenu = nullptr;
    QAction* m_qt_editUserProfileUserMenu = nullptr;
    QAction* m_qt_openGoalMenu = nullptr;
    QAction* m_qt_openScheduleMenu = nullptr;
    QAction* m_qt_openNotesMenu = nullptr;
    QAction* m_qt_openTodoMenu = nullptr;
    QWidget* m_qt_centralwidget = nullptr;
    QGroupBox* m_qt_userIdGroupBox = nullptr;
    QLineEdit* m_qt_userFirstName = nullptr;
    QLineEdit* m_qt_userLastName = nullptr;
    QLineEdit* m_qt_userLogin = nullptr;
    QDateEdit* m_qt_dateSelector = nullptr;

    QString m_progNameStr;
    std::shared_ptr<UserModel> m_userDataPtr = nullptr;
    QDate m_currentDate;
    std::vector<ModelSubWindow*> m_viewWindows;
};

#endif // CONTROLPANEL_H_
