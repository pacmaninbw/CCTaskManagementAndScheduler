// Project Header Files
#include "ScheduleItemModel.h"

// QT Headeer Files
#include "GuiScheduleItemModel.h"
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
#include <iostream>

GuiScheduleItemModel::GuiScheduleItemModel(std::size_t userID, QObject *parent)
    : QObject{parent},
    m_UserID{userID},
    m_DbScheduleItemId{0},
    m_DbScheduleItemDataPtr{nullptr}
{}

GuiScheduleItemModel::GuiScheduleItemModel(std::shared_ptr<ScheduleItemModel> dbScheduleItemDataPtr, QObject *parent)
    : GuiScheduleItemModel(dbScheduleItemDataPtr->getUserID(), parent)
{
    m_DbScheduleItemDataPtr = dbScheduleItemDataPtr;
    m_DbScheduleItemId = dbScheduleItemDataPtr->getScheduleItemID();
//    m_UserID = dbScheduleItemDataPtr->getUserID();
    m_Title = QString::fromStdString(dbScheduleItemDataPtr->getTitle());
    m_StartTime = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getStartTime());
    m_EndTime = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getEndTime());

    QDateTime tempqdt = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getCreationDate());
    m_CreationTimeStamp = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(dbScheduleItemDataPtr->getLastUpdate());
    m_LastUpdateTimreStamp = tempqdt.toString(Qt::ISODate);
}

void GuiScheduleItemModel::debugShow()
{
    std::cerr << "GuiScheduleItemModel:" << std::endl;
    std::cerr << "Start Time raw: " << m_StartTime.toString(Qt::TextDate).toStdString() << std::endl;
    std::cerr << "Start Time only: " << m_StartTime.toString("h:m").toStdString() << std::endl;
    std::cerr << "End Time raw: " << m_EndTime.toString(Qt::TextDate).toStdString() << std::endl;
    std::cerr << "End Time only: " << m_EndTime.toString("h:m").toStdString() << std::endl;
    std::cerr << "Title: " << m_Title.toStdString() << std::endl;
    std::cerr << "Personal: " << (m_Personal? "True" : "False") << std::endl;
    std::cerr << "Location: " << m_Location.toStdString() << std::endl;
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

bool GuiScheduleItemModel::addToDatabase()
{
    m_DbErrorMessages = "";

    m_DbScheduleItemDataPtr = std::make_shared<ScheduleItemModel>();

    m_DbScheduleItemDataPtr->setUserID(m_UserID);

    transferFieldsToDbModel();

    if (m_DbScheduleItemDataPtr->insert())
    {
        m_DbScheduleItemId = m_DbScheduleItemDataPtr->getScheduleItemID();
        return true;
    }

    m_DbErrorMessages = QString::fromStdString(m_DbScheduleItemDataPtr->getAllErrorMessages());

    return false;
}

bool GuiScheduleItemModel::updateInDatabase()
{
    m_DbErrorMessages = "";

    return false;
}

void GuiScheduleItemModel::transferFieldsToDbModel()
{
    if (!m_DbScheduleItemDataPtr)
    {
        m_DbErrorMessages = "Programmer Error: DB data pointer not set";
        return;
    }

    m_DbScheduleItemDataPtr->setTitle(m_Title.toStdString());
    m_DbScheduleItemDataPtr->setStartDateAndTime(qDateTimeToChrono(m_StartTime));
    m_DbScheduleItemDataPtr->setEndDateAndTime(qDateTimeToChrono(m_EndTime));
    m_DbScheduleItemDataPtr->setPersonal(m_Personal);

    transferOptionalFieldsToDBModel();
}

void GuiScheduleItemModel::transferDbModelDataToFields()
{
    m_Title = QString::fromStdString(m_DbScheduleItemDataPtr->getTitle());
    m_StartTime = chronoTimePointToQDateTime(m_DbScheduleItemDataPtr->getStartTime());
    m_EndTime = chronoTimePointToQDateTime(m_DbScheduleItemDataPtr->getEndTime());
    m_Personal = m_DbScheduleItemDataPtr->isPersonal();
    m_Location = QString::fromStdString(m_DbScheduleItemDataPtr->getLocation());

    QDateTime tempqdt = chronoTimePointToQDateTime(m_DbScheduleItemDataPtr->getCreationDate());
    m_CreationTimeStamp = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(m_DbScheduleItemDataPtr->getLastUpdate());
    m_LastUpdateTimreStamp = tempqdt.toString(Qt::ISODate);
}

void GuiScheduleItemModel::transferOptionalFieldsToDBModel()
{
    if (!m_Location.isEmpty())
    {
        m_DbScheduleItemDataPtr->setLocation(m_Location.toStdString());
    }
}
