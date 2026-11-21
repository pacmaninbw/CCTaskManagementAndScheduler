#ifndef BASEUSERTABLE_H
#define BASEUSERTABLE_H

#include <qdatetime.h>
class OrganizationModel;

// Project Header Files
#include "UserModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDate>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <optional>
#include <vector>

class BaseUserTable : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit BaseUserTable(QObject *parent = nullptr);
    explicit BaseUserTable(QDate searchDate, std::shared_ptr<OrganizationModel> searchOrganization = nullptr, QObject *parent = nullptr);
    virtual void filltable();
    void append(UserModel_shp userData);
    void clearData();
    UserModel_shp getDatabaseObject(const QModelIndex& index);
    void setSearchDate(QDate searchDate) { m_searchDate = searchDate.toStdSysDays(); };
    void setSearchOrganization(std::shared_ptr<OrganizationModel> searchOrgaization) { m_searchOrganization = searchOrgaization; };

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;

protected:
    std::vector<UserModel_shp> m_data;
    std::optional<std::chrono::year_month_day> m_searchDate;
    std::shared_ptr<OrganizationModel> m_searchOrganization;
};

#endif // BASEUSERTABLE_H
