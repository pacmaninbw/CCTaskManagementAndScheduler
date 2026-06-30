#ifndef DEFAULTGOALDISPLAYTABLE_H_
#define DEFAULTGOALDISPLAYTABLE_H_

class UserModel;
class UserGoalModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>
#include <vector>

class DefaultGoalDisplayTable : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DefaultGoalDisplayTable(QObject *parent = nullptr);
    explicit DefaultGoalDisplayTable(std::size_t userID, QObject *parent = nullptr);
    void setUser(std::size_t userID) { m_UserID = userID; };
    void setUserRefillGoalTable(std::size_t userID);
    void refillTable();
    void append(std::shared_ptr<UserGoalModel> goalItem);
    void clearData();

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex()) override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

private:
    void fillGoalTable();
    std::size_t m_UserID;
    std::vector<std::shared_ptr<UserGoalModel>> m_data;
    std::vector<std::shared_ptr<UserGoalModel>> m_GoalList;
};

#endif // DEFAULTGOALDISPLAYTABLE_H_
