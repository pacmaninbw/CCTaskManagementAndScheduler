#ifndef SCHEDULEWINDOW_H_
#define SCHEDULEWINDOW_H_

/*
 * Provide the window to display the user's schedule and allow the user to edit
 * their schedule.
 */
class GuiDashboardScheduleTable;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QTableView>

// Standard C++ Header Files

class ScheduleWindow  : public ModelSubWindow
{
public:
    ScheduleWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddEvent();
    void handleScheduleItemClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    void tableViewReset(QTableView* tableView);

    GuiDashboardScheduleTable* m_scheduleTable = nullptr;
};

#endif // SCHEDULEWINDOW_H_
