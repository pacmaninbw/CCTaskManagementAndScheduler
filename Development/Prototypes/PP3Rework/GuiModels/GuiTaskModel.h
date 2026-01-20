#ifndef GUITASKMODEL_H_
#define GUITASKMODEL_H_

// Project Header Files

// QT Header File
#include <QDate>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>


class TaskModel;

class GuiTaskModel : public QObject
{
    Q_OBJECT

public:
    enum class GUITaskStatus
    {
        Not_Started, On_Hold, Waiting_for_Dependency, Work_in_Progress, Complete, UNKNOWN
    };
    Q_ENUM(GUITaskStatus)

    explicit GuiTaskModel(QObject *parent = nullptr);
    explicit GuiTaskModel(std::shared_ptr<TaskModel> dbTaskDataPtr, QObject* parent = nullptr);

    std::size_t getDbTaskId() const { return m_DbTaskId; };
    std::size_t getCreatorUserId() const { return m_CreatorUserId;};
    std::size_t getAssigneeUserId() const { return m_AssigneeUserId;};
    QString getPriorityGroup() const { return m_PriorityGroup; };
    QString getPriority() const { return m_Priority; };
    QString getDescription() const { return m_Description; };
    GUITaskStatus getStatus() const { return m_status; };
    std::size_t getParentTaskId() const { return m_ParentTaskId; };
    QDate getDueDate() const { return m_DueDate; };
    QDate getScheduledStart() const { return m_ScheduledStart; };
    QDate getActualStart() const { return m_ActualStartDate; };
    QDate getEstimatedCompletion() const { return m_EstimatedCompletion; };
    QDate getCompletionDate() const { return m_CompletionDate; };
    QString getEstimatedEffort() const { return m_EstimatedEffort; };
    QString getActualEffortToDate() const { return  m_ActualEffortToDate; };
    bool getPersonal() const { return  m_Personal; }
    QString getPercentageComplete() const { return  m_PercentageComplete; };
    QString getCreationTimeStamp() const { return  m_CreationTimeStamp; };
    QString getLastUpdateTimeStamp() const { return  m_LastUpdateTimeStamp; };

    void setDbTaskId(std::size_t v);
    void setCreatorUserId(std::size_t v);
    void setAssigneeUserId(std::size_t v);
    void setPriorityGroup(QString v);
    void setPriority(QString v);
    void setDescription(QString v);
    void setStatus(GUITaskStatus v);
    void setParentTaskId(std::size_t v);
    void setDueDate(QDate v);
    void setScheduledStart(QDate v);
    void setActualStart(QDate v);
    void setEstimatedCompletion(QDate v);
    void setCompletionDate(QDate v);
    void setEstimatedEffort(QString v);
    void setActualEffortToDate(QString v);
    void setPersonal(bool v);

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
    GUITaskStatus convertFromTaskModel(std::size_t taskModelStatus);
    std::size_t TaskModelStatusFromGuiTaskStatus(GUITaskStatus taskStatus);
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
    std::shared_ptr<TaskModel> m_DbTaskDataPtr;
};

#endif // GUITASKMODEL_H_
