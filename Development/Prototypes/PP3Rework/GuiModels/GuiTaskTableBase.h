#ifndef GUITASKTABLEBASE_H
#define GUITASKTABLEBASE_H

class TaskModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <vector>

class GuiTaskTableBase : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GuiTaskTableBase(std::size_t userID, QObject *parent = nullptr);
    void setUserRefillTable(std::size_t userID);
    void fillTable();
    void clearData();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;
    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

protected:
    void makeFakeQList();
    std::size_t m_UserID;
    std::vector<std::shared_ptr<TaskModel>> m_data;
};

#endif // GUITASKTABLEBASE_H
