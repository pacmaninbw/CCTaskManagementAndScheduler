#ifndef GUIGOALMODEL_H
#define GUIGOALMODEL_H

class UserGoalModel;
// Project Header Files

// QT Header Files
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>


class GuiGoalModel : public QObject
{
    Q_OBJECT
public:
    explicit GuiGoalModel(QObject *parent = nullptr);
    explicit GuiGoalModel(std::shared_ptr<UserGoalModel> dbGoalDataptr, QObject *parent = nullptr);

    std::size_t getGoalId() const noexcept { return m_GoalId; };
    std::size_t getUserId() const noexcept { return m_UserId; };
    QString getDescription() const noexcept { return m_Description; };
    QString getPriority() const noexcept { return m_Priority; };
    std::size_t getParentId() const noexcept { return m_ParentId; };
    QString getCreationTimeStamp() const noexcept { return m_CreationTS; };
    QString getLastUpdateTimeStamp() const noexcept { return m_LastUpdateTS; };

    void setUserId(std::size_t userId);
    void setDescription(QString description);
    void setPriority(QString priority);
    void setParentID(std::size_t parentID);

signals:
    void userIdChanged();
    void descriptionChanged();
    void priorityChanged();
    void parentIdChanged();

private:
    std::size_t m_GoalId;
    std::size_t m_UserId;
    QString m_Description;
    QString m_Priority;
    std::size_t m_ParentId;
    QString m_CreationTS;
    QString m_LastUpdateTS;
    std::shared_ptr<UserGoalModel> m_DBGoalDataptr;
};

#endif // GUIGOALMODEL_H
