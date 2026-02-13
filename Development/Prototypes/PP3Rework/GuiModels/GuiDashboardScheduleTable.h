#ifndef GUIDASHBOARDSCHEDULETABLE_H
#define GUIDASHBOARDSCHEDULETABLE_H

class GuiUserModel;
class GuiScheduleItemModel;
class ScheduleItemModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <vector>

class GuiDashboardScheduleTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GuiDashboardScheduleTable(QObject *parent = nullptr);
    explicit GuiDashboardScheduleTable(GuiUserModel* userData, QDate dateOfSchedule, QObject *parent = nullptr);
    void setUser(GuiUserModel* userData);
    void setDate(QDate dateOfSchedule);
    void setUserAndDateRefillSchedule(GuiUserModel* userData, QDate dateOfSchedule);
    void append(GuiScheduleItemModel* scheduledItem);
    void clearData();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation,
            int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    // Editable:

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    void fillSchedule();
    std::chrono::system_clock::time_point getLocalMidnight(std::chrono::year_month_day scheduleDate);
    void setUserDay(std::chrono::year_month_day scheduleDate);
    bool hasNoTimeConflicts(std::chrono::system_clock::time_point proposedStartTime);
    void addBlankHoursForDisplay();
    GuiUserModel *m_UserDataPtr;
    QList<GuiScheduleItemModel*> m_data;
    QDate m_DateOfSchedule;
    std::chrono::system_clock::time_point m_UserStartDay;
    std::chrono::system_clock::time_point m_UserEndDay;
    std::chrono::year_month_day m_ChronDateOfSchedule;
    std::vector<std::shared_ptr<ScheduleItemModel>> m_ScheduledItems;
};

#endif // GUIDASHBOARDSCHEDULETABLE_H
