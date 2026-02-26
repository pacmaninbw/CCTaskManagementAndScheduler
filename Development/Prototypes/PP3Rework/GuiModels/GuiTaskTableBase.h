#ifndef GUITASKTABLEBASE_H
#define GUITASKTABLEBASE_H

class GuiTaskModel;
class GuiUserModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files


class GuiTaskTableBase : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit GuiTaskTableBase(QObject *parent = nullptr);
    explicit GuiTaskTableBase(GuiUserModel *userDataPtr, QObject *parent = nullptr);
    void setUserRefillTable(GuiUserModel *userDataPtr);
    void fillTable();
    void append(GuiTaskModel* taskData);
    void clearData();

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    // Fetch data dynamically:
    bool hasChildren(const QModelIndex &parent = QModelIndex()) const override;

    bool canFetchMore(const QModelIndex &parent) const override;
    void fetchMore(const QModelIndex &parent) override;

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

    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

protected:
    void makeFakeQList();
    GuiUserModel *m_UserDataPtr;
    QList<GuiTaskModel*> m_data;
};

#endif // GUITASKTABLEBASE_H
