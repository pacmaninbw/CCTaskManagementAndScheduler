#ifndef DASHBOARDTASKVIEWER_H
#define DASHBOARDTASKVIEWER_H

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
    void setUserId(std::size_t userID);
    void setUserIdAndDate(std::size_t userID, QDate dashboardDate);
    void update();

private:
    std::size_t m_UserID;
    QDate m_DashboardDate;
    GuiDashboardTaskTable* m_TaskTable;
};

#endif // DASHBOARDTASKVIEWER_H
