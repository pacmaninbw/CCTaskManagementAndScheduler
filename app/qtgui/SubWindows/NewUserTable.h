#ifndef NEWUSERTABLE_H_
#define NEWUSERTABLE_H_

class UserModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>
#include <vector>


class NewUserTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit NewUserTable(QObject *parent = nullptr);
    void refillTable();
    void fillTable();
    void append(std::shared_ptr<UserModel>userData);
    void clearData();
    std::shared_ptr<UserModel> getDatabaseObject(const QModelIndex& index);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

private:
    std::vector<std::shared_ptr<UserModel>> m_data;
};

#endif // NEWUSERTABLE_H_
