#ifndef GUIDASHBOARDTASKTABLE_H
#define GUIDASHBOARDTASKTABLE_H

class GuiTaskModel;
class GuiUserModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files


class GuiDashboardTaskTable : public QAbstractTableModel
{
    Q_OBJECT


public:

    explicit GuiDashboardTaskTable(QObject *parent = nullptr);
    explicit GuiDashboardTaskTable(GuiUserModel *userDataPtr, QObject* parent=nullptr);
    void setUserRefillTable(GuiUserModel *userDataPtr);
    void fillTable();

    // Header:
    QVariant headerData(int section,
                        Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section,
                       Qt::Orientation orientation,
                       const QVariant &value,
                       int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;

private:
    void makeFakeQList();
    GuiUserModel *m_UserDataPtr;
    QList<GuiTaskModel*> m_data;
};

#endif // GUIDASHBOARDTASKTABLE_H
