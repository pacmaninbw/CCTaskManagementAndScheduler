#ifndef GUITASKMODEL_H_
#define GUITASKMODEL_H_

// Project Header Files

// QT Header File
#include <QDate>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files

class GuiTaskModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::size_t dbTaskID MEMBER m_DbTaskId NOTIFY dbTaskIdChanged)
    Q_PROPERTY(std::size_t creatorUserId MEMBER m_CreatorUserId NOTIFY creatorIdChanged)
    Q_PROPERTY(std::size_t assigneeUserId MEMBER m_AssigneeUserId NOTIFY assigneeIdChanged)
    Q_PROPERTY(QString description MEMBER m_Description NOTIFY descriptionChanged)
    Q_PROPERTY(GUITaskStatus status MEMBER m_status NOTIFY statusChanged)
    Q_PROPERTY(std::size_t parentTaskId MEMBER m_ParentTaskId NOTIFY parentTaskIdChanged)
    Q_PROPERTY(QDate dueDate MEMBER m_DueDate NOTIFY dueDateChanged)
    Q_PROPERTY(QDate scheduledStart MEMBER m_ScheduledStart NOTIFY scheduledStartChanged)
    Q_PROPERTY(QDate actualStart MEMBER m_ActualStartDate NOTIFY actualStartChanged)
    Q_PROPERTY(QDate estimatedCompletion MEMBER m_EstimatedCompletion NOTIFY estimatedCompletionChanged)
    Q_PROPERTY(QDate completionDate MEMBER m_CompletionDate NOTIFY completionDateChanged)
    Q_PROPERTY(QString estimatedEffort MEMBER m_EstimatedEffort NOTIFY estimatedEffortChanged)
    Q_PROPERTY(QString actualEffort MEMBER m_ActualEffortToDate NOTIFY actualEffortToDateChanged)
    Q_PROPERTY(QString priorityGroup MEMBER m_PriorityGroup NOTIFY priorityGroupChanged)
    Q_PROPERTY(QString priority MEMBER m_Priority NOTIFY priorityChanged)
    Q_PROPERTY(bool personal MEMBER m_Personal NOTIFY personalChanged)
    Q_PROPERTY(QString creation MEMBER m_CreationTimeStamp NOTIFY creationTimeStampChanged)
    Q_PROPERTY(QString lastUpdate MEMBER m_LastUpdateTimeStamp NOTIFY lastUpdateTimeStampChanged)
    Q_PROPERTY(QList<std::size_t> dependencies MEMBER m_Depenedencies NOTIFY depenedenciesChanged)

public:
    enum class GUITaskStatus
    {
        Not_Started, On_Hold, Waiting_for_Dependency, Work_in_Progress, Complete
    };
    Q_ENUM(GUITaskStatus)

    explicit GuiTaskModel(QObject *parent = nullptr);

signals:
    void dbTaskIdChanged();
    void creatorIdChanged();
    void assigneeIdChanged();
    void descriptionChanged();
    void statusChanged();
    void parentTaskIdChanged();
    void dueDateChanged();
    void scheduledStartChanged();
    void actualStartChanged();
    void estimatedCompletionChanged();
    void completionDateChanged();
    void estimatedEffortChanged();
    void actualEffortToDateChanged();
    void priorityGroupChanged();
    void priorityChanged();
    void personalChanged();
    void percentageCompleteChanged();
    void creationTimeStampChanged();
    void lastUpdateTimeStampChanged();
    void depenedenciesChanged();

private:
    std::size_t m_DbTaskId;
    std::size_t m_CreatorUserId;
    std::size_t m_AssigneeUserId;
    QString m_Description;
    GUITaskStatus m_status;
    std::size_t m_ParentTaskId;
    QDate m_DueDate;
    QDate m_ScheduledStart;
    QDate m_ActualStartDate;
    QDate m_EstimatedCompletion;
    QDate m_CompletionDate;
    QString m_EstimatedEffort;
    QString m_ActualEffortToDate;
    QString m_PriorityGroup;
    QString m_Priority;
    bool m_Personal;
    QString m_PercentageComplete;
    QString m_CreationTimeStamp;
    QString m_LastUpdateTimeStamp;
    QList<std::size_t> m_Depenedencies;
};

#endif // GUITASKMODEL_H_
