#ifndef GUIDASHBOARDSCHEDULETABLE_H
#define GUIDASHBOARDSCHEDULETABLE_H

class GuiUserModel;
class GuiScheduleItemModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

class GuiDashboardScheduleTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GuiDashboardScheduleTable(QObject *parent = nullptr);
    void setUser(GuiUserModel* userData);
    void setDate(QDate dateOfSchedule);
    void setUserAndDate(GuiUserModel* userData, QDate dateOfSchedule);
    void fillSchedule();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    void makeBlankSchedule();
    void addScheduledItems();
    GuiUserModel *m_UserDataPtr;
    QList<GuiScheduleItemModel*> m_data;
    QDate m_DateOfSchedule;

};

#endif // GUIDASHBOARDSCHEDULETABLE_H
