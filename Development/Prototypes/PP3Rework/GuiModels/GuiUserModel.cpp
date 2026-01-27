// Project Header Files
#include <UserModel.h>

// QT Header Files
#include "GuiUserModel.h"
#include "stdChronoToQTConversions.h"

// Standard C++ Header Files
//#include <iostream>

GuiUserModel::GuiUserModel(QObject *parent)
    : QObject{parent},
    m_dbUserId{0},
    m_DbUserDataPtr{nullptr},
    m_AutoGenerateLoginAndPassword{false},
    m_FieldsChangedValue{false}
{}

GuiUserModel::GuiUserModel(std::shared_ptr<UserModel> dbUserDataPtr, QObject *parent)
    : GuiUserModel{parent}
{
    setUserDataPtr(dbUserDataPtr);
}

bool GuiUserModel::isValid()
{
    return false;
}

void GuiUserModel::setLoginName(QString val)
{
    if (m_LoginName != val)
    {
        m_LoginName = val;
        m_FieldsChangedValue = true;
        emit loginNameChanged();
    }
}

void GuiUserModel::setPassword(QString val)
{
    if (m_Password != val)
    {
        m_Password = val;
        m_FieldsChangedValue = true;
        emit passwordChanged();
    }
}

void GuiUserModel::setEmail(QString val)
{
    if (m_Email != val)
    {
        m_Email = val;
        m_FieldsChangedValue = true;
        emit emailChanged();
    }
}

void GuiUserModel::setLastName(QString val)
{
    if (m_LastName != val)
    {
        m_LastName = val;
        m_FieldsChangedValue = true;
        emit lastNameChanged();
    }
}

void GuiUserModel::setFirstName(QString val)
{
        if (m_FirstName != val)
    {
        m_FirstName = val;
        m_FieldsChangedValue = true;
        emit firstNameChanged();
    }
}

void GuiUserModel::setMiddleInitial(QString val)
{
    if (m_MiddleInitial != val)
    {
        m_MiddleInitial = val;
        m_FieldsChangedValue = true;
        emit middleInitialChanged();
    }
}

void GuiUserModel::setAutoGenerateLoginData(bool val)
{
    if (m_AutoGenerateLoginAndPassword != val)
    {
        m_AutoGenerateLoginAndPassword = val;
        emit autoGenerateLoginDataChanged();
    }
}

void GuiUserModel::setUserDataPtr(std::shared_ptr<UserModel> dbUserDataPtr)
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

bool GuiUserModel::attemptLogin()
{
    bool loginSuccessful = false;
    std::string loginName = m_LoginName.toStdString();
    std::string password = m_Password.toStdString();

    if (loginName.empty() || password.empty())
    {
        return loginSuccessful;
    }

    UserModel_shp retrievedUser = std::make_shared<UserModel>();
    if (retrievedUser->selectByLoginAndPassword(loginName, password))
    {
        retrievedUser->setLastLogin(std::chrono::system_clock::now());
        if (retrievedUser->update())
        {
            setUserDataPtr(retrievedUser);
            loginSuccessful = true;
        }
        else
        {
            m_DbErrorMessages = QString::fromStdString(retrievedUser->getAllErrorMessages());
        }
    }
    else
    {
        m_DbErrorMessages = QString::fromStdString(retrievedUser->getAllErrorMessages());
    }

    return loginSuccessful;
}

bool GuiUserModel::attemptLogin(QString loginName, QString password)
{
    m_LoginName = loginName;
    m_Password = password;

    return attemptLogin();
}

bool GuiUserModel::attemptAddUser()
{
    bool userAdded = false;

    UserModel_shp newUser = std::make_shared<UserModel>();

    newUser->setFirstName(m_FirstName.toStdString());
    newUser->setMiddleInitial(m_MiddleInitial.toStdString());
    newUser->setLastName(m_LastName.toStdString());
    newUser->setEmail(m_Email.toStdString());

    if (!addLoginData(newUser))
    {
        return userAdded;
    }

    if (newUser->insert())
    {
        userAdded = true;
        m_dbUserId = newUser->getUserID();

        QDateTime tempqdt = chronoTimePointToQDateTime(newUser->getCreationDate());
        m_CreatedTS = tempqdt.toString(Qt::ISODate);

        if (newUser->getLastLogin().has_value())
        {
            tempqdt = chronoTimePointToQDateTime(newUser->getLastLogin().value());
            m_LastLoginTS = tempqdt.toString(Qt::ISODate);
        }
    }
    else
    {
        m_DbErrorMessages = QString::fromStdString(newUser->getAllErrorMessages());
    }

    return userAdded;
}

bool GuiUserModel::attemptUpdateUser()
{
    if (!m_FieldsChangedValue)
    {
        return true;
    }

    m_DbUserDataPtr->setFirstName(m_FirstName.toStdString());
    m_DbUserDataPtr->setMiddleInitial(m_MiddleInitial.toStdString());
    m_DbUserDataPtr->setLastName(m_LastName.toStdString());
    m_DbUserDataPtr->setEmail(m_Email.toStdString());
    m_DbUserDataPtr->setLoginName(m_LoginName.toStdString());
    m_DbUserDataPtr->setPassword(m_Password.toStdString());

    if (!m_DbUserDataPtr->update())
    {
        m_DbErrorMessages = QString::fromStdString(m_DbUserDataPtr->getAllErrorMessages());
        return false;
    }
    return true;
}

bool GuiUserModel::addLoginData(std::shared_ptr<UserModel> newUser)
{
    if (m_AutoGenerateLoginAndPassword)
    {
        if (m_LoginName.length() > 0 || m_Password.length() > 0)
        {
            if (m_LoginName.length() > 0)
            {
                m_DbErrorMessages += "\nCan't use Auto Generate Login and Password with an existing Login Name";
            }
            if (m_Password.length() > 0)
            {
                m_DbErrorMessages += "\nCan't use Auto Generate Login and Password with an existing Password";
            }
            return false;
        }
        newUser->autoGenerateLoginAndPassword();
    }
    else
    {
        newUser->setLoginName(m_LoginName.toStdString());
        newUser->setPassword(m_Password.toStdString());
    }

    return true;
}

