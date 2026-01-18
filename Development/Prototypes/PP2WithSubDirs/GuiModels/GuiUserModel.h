#ifndef GUIUSERMODEL_H_
#define GUIUSERMODEL_H_

// Project Header Files

// QT Header Files
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>

class GuiUserModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString loginName MEMBER m_LoginName NOTIFY loginNameChanged)
    Q_PROPERTY(QString password MEMBER m_Password NOTIFY passwordChanged)
    Q_PROPERTY(QString email MEMBER m_Email NOTIFY emailChanged)
    Q_PROPERTY(QString lastName MEMBER m_LastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString firstName MEMBER m_FirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString middleInitial MEMBER m_MiddleInitial NOTIFY middleInitialChanged)

public:
    explicit GuiUserModel(QObject *parent = nullptr);

signals:
    void loginNameChanged();
    void passwordChanged();
    void emailChanged();
    void lastNameChanged();
    void firstNameChanged();
    void middleInitialChanged();
    void dbUserIdChanged();
    void creationTimeStampChanged();
    void lastLoginTimeStampChanged();

private:
    QString m_LoginName;
    QString m_Password;
    QString m_Email;
    QString m_LastName;
    QString m_FirstName;
    QString m_MiddleInitial;
    std::chrono::system_clock::time_point m_created;
    std::chrono::system_clock::time_point m_lastLogin;
    std::size_t m_dbUserId;
};

#endif // GUIUSERMODEL_H_
