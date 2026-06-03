#ifndef SCHEDULEWINDOW_H
#define SCHEDULEWINDOW_H

class ScheduleTablerViewer;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files


class ScheduleWindow  : public ModelSubWindow
{
    Q_OBJECT

public:
    ScheduleWindow(bool makeSubWindow = false, QWidget *parent = nullptr);
    ScheduleWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;


Q_SIGNALS:

private Q_SLOTS:
    void handleAddEvent();
    void handleScheduleItemClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    ScheduleTablerViewer* updateSchedule();

    ScheduleTablerViewer* m_ScheduleTableView = nullptr;
};

#endif // SCHEDULEWINDOW_H
