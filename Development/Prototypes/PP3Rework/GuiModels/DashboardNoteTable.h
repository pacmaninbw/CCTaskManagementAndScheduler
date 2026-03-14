#ifndef DASHBOARDNOTETABLE_H
#define DASHBOARDNOTETABLE_H

class GuiNoteModel;
class GuiUserModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files


class DashboardNoteTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DashboardNoteTable(GuiUserModel *userDataPtr, QDate searchDate, QObject *parent = nullptr);
    void setUserRefillTable(GuiUserModel *userDataPtr);
    void fillTable();
    void append(GuiNoteModel* noteData);
    void clearData();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

private:
    GuiUserModel *m_UserDataPtr;
    QList<GuiNoteModel*> m_data;
    QDate m_SearchDate;
};

#endif // DASHBOARDNOTETABLE_H
