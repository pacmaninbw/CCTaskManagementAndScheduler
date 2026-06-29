#ifndef GUIDASHBOARDSCHEDULETABLE_H_
#define GUIDASHBOARDSCHEDULETABLE_H_

/*
 * Provide a table view of the user's schedule for the selected day. Allow the
 * user to edit their schedule. The base schedule is 12 hours starting at 8:00AM,
 * in the future the user should be able to start and end their day at their
 * preferred times.
 * 
 * Since the schedule data is stored in a database use std::chrono instead of
 * QDate and use GMT time rather than local time. Adjust the display of the
 * time so that the user can think in local time rather than GMT time.
 */
class ScheduleItemModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <vector>

class GuiDashboardScheduleTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GuiDashboardScheduleTable(std::size_t userID, QDate dateOfSchedule, QObject *parent = nullptr);
    void append(std::shared_ptr<ScheduleItemModel> scheduledItem);
    void clearData();
    void fillSchedule();
    std::chrono::system_clock::time_point getScheduleItemStartTime(const QModelIndex &index);
    std::chrono::system_clock::time_point getScheduleItemEndTime(const QModelIndex &index);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

private:
    void setUserDay(std::chrono::year_month_day scheduleDate);
    bool hasNoTimeConflicts(std::chrono::system_clock::time_point proposedStartTime);
    void addBlankHoursForDisplay();
    std::size_t m_UserID;
    std::vector<std::shared_ptr<ScheduleItemModel>> m_data;
    QDate m_DateOfSchedule;
    std::chrono::system_clock::time_point m_UserStartDay;
    std::chrono::system_clock::time_point m_UserEndDay;
    std::chrono::year_month_day m_ChronDateOfSchedule;
    std::vector<std::shared_ptr<ScheduleItemModel>> m_ScheduledItems;
};

#endif // GUIDASHBOARDSCHEDULETABLE_H_
