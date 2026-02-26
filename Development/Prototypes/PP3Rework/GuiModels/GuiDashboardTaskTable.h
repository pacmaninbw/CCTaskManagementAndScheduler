#ifndef GUIDASHBOARDTASKTABLE_H
#define GUIDASHBOARDTASKTABLE_H

// Project Header Files
#include "GuiTaskTableBase.h"

// QT Header Files

// Standard C++ Header Files

class GuiDashboardTaskTable : public GuiTaskTableBase
{
    Q_OBJECT

public:

    explicit GuiDashboardTaskTable(QObject *parent = nullptr);
    explicit GuiDashboardTaskTable(GuiUserModel *userDataPtr, QObject* parent=nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    bool setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role = Qt::EditRole) override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:
//    void makeFakeQList();
};

#endif // GUIDASHBOARDTASKTABLE_H
