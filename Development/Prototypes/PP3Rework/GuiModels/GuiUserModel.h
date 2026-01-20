#ifndef GUIUSERMODEL_H_
#define GUIUSERMODEL_H_

// Project Header Files
class UserModel;

// QT Header Files
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>

class GuiUserModel : public QObject
{
    Q_OBJECT

public:
    explicit GuiUserModel(QObject *parent = nullptr);
    explicit GuiUserModel(std::shared_ptr<UserModel> dbUserDataPtr, QObject *parent = nullptr);

    QString getLoginName() const { return m_LoginName; };
    QString getPassword()  const { return m_Password; };
    QString getEmail() const { return m_Email; };
    QString getLastName() const { return m_LastName; };
    QString getFirstName() const { return m_FirstName; };
    QString getMiddleInitial() const { return m_MiddleInitial; };
    QString getCreated() const { return m_CreatedTS; };
    QString getLastLogin() const { return m_LastLoginTS; };
    std::size_t getDbUserId() const { return m_dbUserId; };

    void setLoginName(QString val);
    void setPassword(QString val);
    void setEmail(QString val);
    void setLastName(QString val);
    void setFirstName(QString val);
    void setMiddleInitial(QString val);

signals:
    void loginNameChanged();
    void passwordChanged();
    void emailChanged();
    void lastNameChanged();
    void firstNameChanged();
    void middleInitialChanged();

private:
    QString m_LoginName;
    QString m_Password;
    QString m_Email;
    QString m_LastName;
    QString m_FirstName;
    QString m_MiddleInitial;
    QString m_CreatedTS;
    QString m_LastLoginTS;
    std::size_t m_dbUserId;
    std::shared_ptr<UserModel> m_DbUserDataPtr;
};

#endif // GUIUSERMODEL_H_
