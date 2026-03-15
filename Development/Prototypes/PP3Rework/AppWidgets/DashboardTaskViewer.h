#ifndef DASHBOARDTASKVIEWER_H
#define DASHBOARDTASKVIEWER_H

class UserModel;
class GuiDashboardTaskTable;

// Project Header Files

// QT Header Files
#include <QWidget>
#include <QTableView>
#include <QDate>

// Standard C++ Header Files

class DashboardTaskViewer : public QTableView
{
    Q_OBJECT

public:
    explicit DashboardTaskViewer(QWidget *parent = nullptr);
    void setDate(QDate dashboardDate);
    void setUserId(UserModel* userDataPtr);
    void setUserIdAndDate(UserModel* userDataPtr, QDate dashboardDate);
    void update();

private:
    UserModel* m_UserDataPtr;
    QDate m_DashboardDate;
    GuiDashboardTaskTable* m_TaskTable;
};

#endif // DASHBOARDTASKVIEWER_H
