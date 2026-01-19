// Project Header Files
#include <UserModel.h>

// QT Header Files
#include "GuiUserModel.h"
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files

GuiUserModel::GuiUserModel(QObject *parent)
    : QObject{parent},
    m_dbUserId{0}
{}

GuiUserModel::GuiUserModel(std::shared_ptr<UserModel> dbUserDataPtr, QObject *parent)
    : GuiUserModel{parent}
{
    m_DbUserDataPtr = dbUserDataPtr;
    m_dbUserId = dbUserDataPtr->getUserID();
    m_LoginName = QString::fromStdString(dbUserDataPtr->getLoginName());
    m_Password = QString::fromStdString(dbUserDataPtr->getPassword());
    m_Email = QString::fromStdString(dbUserDataPtr->getEmail());
    m_LastName = QString::fromStdString(dbUserDataPtr->getLastName());
    m_FirstName = QString::fromStdString(dbUserDataPtr->getFirstName());
    m_MiddleInitial = QString::fromStdString(dbUserDataPtr->getMiddleInitial());

    QDateTime tempqdt = chronoTimePointToQDateTime(dbUserDataPtr->getCreationDate());
    m_CreatedTS = tempqdt.toString(Qt::ISODate);

    if (dbUserDataPtr->getLastLogin().has_value())
    {
        tempqdt = chronoTimePointToQDateTime(dbUserDataPtr->getLastLogin().value());
        m_LastLoginTS = tempqdt.toString(Qt::ISODate);
    }
}

void GuiUserModel::setLoginName(QString val)
{
    if (m_LoginName != val)
    {
        m_LoginName = val;
        emit loginNameChanged();
    }
}

void GuiUserModel::setPassword(QString val)
{
    if (m_Password != val)
    {
        m_Password = val;
        emit passwordChanged();
    }
}

void GuiUserModel::setEmail(QString val)
{
    if (m_Email != val)
    {
        m_Email = val;
        emit emailChanged();
    }
}

void GuiUserModel::setLastName(QString val)
{
    if (m_LastName != val)
    {
        m_LastName = val;
        emit lastNameChanged();
    }
}

void GuiUserModel::setFirstName(QString val)
{
        if (m_FirstName != val)
    {
        m_FirstName = val;
        emit firstNameChanged();
    }
}

void GuiUserModel::setMiddleInitial(QString val)
{
    if (m_MiddleInitial != val)
    {
        m_MiddleInitial = val;
        emit middleInitialChanged();
    }
}
