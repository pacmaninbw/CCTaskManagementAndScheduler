// Project Header Files
#include "ScheduleItemModel.h"

// QT Headeer Files
#include "GuiScheduleItemModel.h"
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files

GuiScheduleItemModel::GuiScheduleItemModel(QObject *parent)
    : QObject{parent},
    m_UserID{0},
    m_DbScheduleItemId{0},
    m_DbScheduleItemDataPtr{nullptr}
{}

GuiScheduleItemModel::GuiScheduleItemModel(std::shared_ptr<ScheduleItemModel> dbScheduleItemDataPtr, QObject *parent)
    : GuiScheduleItemModel(parent)
{
    m_DbScheduleItemDataPtr = dbScheduleItemDataPtr;
    m_DbScheduleItemId = dbScheduleItemDataPtr->getScheduleItemID();
    m_UserID = dbScheduleItemDataPtr->getUserID();
    m_Title = QString::fromStdString(dbScheduleItemDataPtr->getTitle());
    m_StartTime = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getStartTime());
    m_EndTime = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getEndTime());

    QDateTime tempqdt = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getCreationDate());
    m_CreationTimeStamp = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getLastUpdate());
    m_LastUpdateTimreStamp = tempqdt.toString(Qt::ISODate);
}

void GuiScheduleItemModel::setUserID(std::size_t inValue)
{
    if (m_UserID != inValue)
    {
        m_UserID = inValue;
        emit userIdChanged();
    }
}

void GuiScheduleItemModel::setTitle(QString inValue)
{
    if (m_Title != inValue)
    {
        m_Title = inValue;
        emit titleChanged();
    }
}

void GuiScheduleItemModel::setStartTime(QDateTime inValue)
{
    if (m_StartTime != inValue)
    {
        m_StartTime = inValue;
        emit startTimeChanged();
    }
}

void GuiScheduleItemModel::setEndTime(QDateTime inValue)
{
    if (m_EndTime != inValue)
    {
        m_EndTime = inValue;
        emit endTimeChanged();
    }
}

void GuiScheduleItemModel::setPersonal(bool inValue)
{
    if (m_Personal != inValue)
    {
        m_Personal = inValue;
        emit personalChanged();
    }
}

void GuiScheduleItemModel::setLocation(QString inValue)
{
    if (m_Location != inValue)
    {
        m_Location = inValue;
        emit locationChanged();
    }
}
