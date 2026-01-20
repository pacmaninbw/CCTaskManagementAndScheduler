// Project Header Files
#include "UserGoalModel.h"

// QT Header Files
#include "GuiGoalModel.h"
#include <QObject>
#include <QString>
#include "stdChronoToQTConversions.h"


// Standard C++ Header Files
#include <string>


GuiGoalModel::GuiGoalModel(QObject *parent)
    : QObject{parent},
    m_GoalId{0},
    m_UserId{0},
    m_ParentId{0},
    m_DBGoalDataptr{nullptr}
{}

GuiGoalModel::GuiGoalModel(std::shared_ptr<UserGoalModel> dbGoalDataptr, QObject *parent)
    : GuiGoalModel(parent)
{
    m_DBGoalDataptr = dbGoalDataptr;
    m_UserId = dbGoalDataptr->getUserId();
    m_Description = QString::fromStdString(dbGoalDataptr->getDescription());
    m_Priority = QString::fromStdString(std::to_string(dbGoalDataptr->getPriority()));
    m_ParentId = dbGoalDataptr->getParentId();

    QDateTime tempqdt = chronoTimePointToQDateTime(dbGoalDataptr->getCreationTimeStamp());
    m_CreationTS = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(dbGoalDataptr->getLastUpdateTimeStamp());
    m_LastUpdateTS = tempqdt.toString(Qt::ISODate);
}

void GuiGoalModel::setUserId(std::size_t userId)
{
    if (m_UserId != userId)
    {
        m_UserId = userId;
        emit userIdChanged();
    }
}

void GuiGoalModel::setDescription(QString description)
{
    if (m_Description != description)
    {
        m_Description = description;
        emit descriptionChanged();
    }
}

void GuiGoalModel::setPriority(QString priority)
{
    if (m_Priority != priority)
    {
        m_Priority = priority;
        emit priorityChanged();
    }
}

void GuiGoalModel::setParentID(std::size_t parentID)
{
    if (m_ParentId != parentID)
    {
        m_ParentId = parentID;
        emit parentIdChanged();
    }
}
