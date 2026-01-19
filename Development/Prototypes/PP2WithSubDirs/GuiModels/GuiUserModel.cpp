#include <UserModel.h>

#include "GuiUserModel.h"

GuiUserModel::GuiUserModel(QObject *parent)
    : QObject{parent},
    m_dbUserId{0}
{}

GuiUserModel::GuiUserModel(std::shared_ptr<UserModel> dbUserDataPtr, QObject *parent)
    : GuiUserModel{parent}
{
    m_DbUserDataPtr = dbUserDataPtr;
    m_dbUserId = dbUserDataPtr->getUserID();
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
