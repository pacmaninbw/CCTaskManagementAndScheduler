#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QDateEdit>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMainWindow>
#include <QMenu>
#include <QMenuBar>
#include <QStatusBar>
#include <QWidget>

// Standard C++ Header Files

QT_BEGIN_NAMESPACE
namespace Ui {
class UserDashboard;
}
QT_END_NAMESPACE

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
    void handleEditNoteAction();
    void handleAddGoalAction();
    void handleEditGoalAction();
    void handleAddScheduleItemAction();
    void handleEditScheduleItemAction();

private:
    void setUpUserDashboardUi();
    QMenuBar* buildDashboardMenuBar();
    void connectMenuActions();
    QGroupBox* fillUserIdBox();
    QGroupBox* updateTaskList();
    QGroupBox* updateSchedule();
    QGroupBox* listNotes();

    QString progNameStr;
    std::size_t userID;

    QMenu* udTaskMenu;
    QAction* udActionAddTask;
    QAction* udActionEditTask;
    QAction* udActionAddSchedule_Item;
    QAction* udActionEditSchedule_Item;
    QAction* udActionAddNote;
    QAction* udActionEditNote;
    QAction* udActionAddUserProfile;
    QAction* udActionEditUserProfile;
    QAction* actionAdd_Goal;
    QWidget* centralwidget;
    QGroupBox* udScheduleGB;
    QGroupBox* udNotesGB;
    QGroupBox* userDashBoardIDGB;
    QLineEdit* udUserFirstNameDispaly;
    QLineEdit* udUserMiddleInitialDisplay;
    QLineEdit* udUserLastNameDisplay;
    QLineEdit* udUserNameDisplay;
    QDateEdit* udDateSelectorDE;
    QGroupBox* udTaskListGB;
    QMenuBar* udMenubar;
    QMenu* menuUser_Dashboard;
    QStatusBar *statusbar;
};
#endif // USERDASHBOARD_H


