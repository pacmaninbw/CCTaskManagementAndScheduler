#ifndef SCHEDULETABLERVIEWER_H
#define SCHEDULETABLERVIEWER_H

class GuiDashboardScheduleTable;

// Project Header Files

// QT Header Files
#include <QWidget>
#include <QTableView>
#include <QDate>

// Standard C++ Header Files

class ScheduleTablerViewer : public QTableView
{
    Q_OBJECT
public:
    explicit ScheduleTablerViewer(QWidget *parent = nullptr);
    void setDate(QDate scheduleToShow);
    void setUserId(std::size_t userID);
    void setUserIdAndDate(std::size_t userID, QDate scheduleToShow);
    void updateSchedule();

private:
    std::size_t m_DBUserId;
    QDate m_DateOfSchedule;
    GuiDashboardScheduleTable* m_ScheduleTable;
};

#endif // SCHEDULETABLERVIEWER_H
