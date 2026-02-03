#ifndef DASHBOARDTASKVIEWER_H
#define DASHBOARDTASKVIEWER_H


class GuiUserModel;
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
    void setUserId(GuiUserModel* userDataPtr);
    void setUserIdAndDate(GuiUserModel* userDataPtr, QDate dashboardDate);
    void update();

signals:

private slots:

private:
    GuiUserModel* m_UserDataPtr;
    QDate m_DashboardDate;
    GuiDashboardTaskTable* m_TaskTable;
};

#endif // DASHBOARDTASKVIEWER_H
