#ifndef SELECTPARENTTASKTABLE_H
#define SELECTPARENTTASKTABLE_H

class UserModel;

#include "GuiTaskTableBase.h"

class SelectParentTaskTable : public GuiTaskTableBase
{
    Q_OBJECT

public:
    explicit SelectParentTaskTable(std::size_t userID, QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // SELECTPARENTTASKTABLE_H
