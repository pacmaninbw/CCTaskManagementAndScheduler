#ifndef SCHEDULETABLERVIEWER_H
#define SCHEDULETABLERVIEWER_H

class GuiUserModel;
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
    void setUserId(GuiUserModel* userDataPtr);
    void setUserIdAndDate(GuiUserModel* userDataPtr, QDate scheduleToShow);
    void updateSchedule();

signals:

private slots:

private:
    GuiUserModel* m_DBUserId;
    QDate m_DateOfSchedule;
    GuiDashboardScheduleTable* m_ScheduleTable;
};

#endif // SCHEDULETABLERVIEWER_H
