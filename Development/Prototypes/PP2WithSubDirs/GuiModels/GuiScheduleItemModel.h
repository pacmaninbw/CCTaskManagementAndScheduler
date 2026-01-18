#ifndef GUISCHEDULEITEMMODEL_H
#define GUISCHEDULEITEMMODEL_H

// Project Header Files

// QT Headeer Files
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files

class GuiScheduleItemModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(std::size_t userId MEMBER m_UserID NOTIFY userIdChanged)
    Q_PROPERTY(std::size_t scheduleItemId MEMBER m_DbScheduleItemId NOTIFY scheduleItemIdChanged)
    Q_PROPERTY(QString title MEMBER m_Title NOTIFY titleChanged)
    Q_PROPERTY(QDateTime startTime MEMBER m_StartTime NOTIFY startTimeChanged)
    Q_PROPERTY(QDateTime endTime MEMBER m_EndTime NOTIFY endTimeChanged)
    Q_PROPERTY(bool personal MEMBER m_Personal NOTIFY personalChanged)
    Q_PROPERTY(QString location MEMBER m_Location NOTIFY locationChanged)
    Q_PROPERTY(QString creationTS MEMBER m_CreationTimeStamp NOTIFY creationTSChanged)
    Q_PROPERTY(QString lastUpdateTS MEMBER m_LastUpdateTimreStamp NOTIFY lastUpdateTSChanged)

public:
    explicit GuiScheduleItemModel(QObject *parent = nullptr);

signals:
    void userIdChanged();
    void scheduleItemIdChanged();
    void titleChanged();
    void startTimeChanged();
    void endTimeChanged();
    void personalChanged();
    void locationChanged();
    void creationTSChanged();
    void lastUpdateTSChanged();

private:
    std::size_t m_UserID;
    std::size_t m_DbScheduleItemId;
    QString m_Title;
    QDateTime m_StartTime;
    QDateTime m_EndTime;
    bool m_Personal;
    QString m_Location;
    QString m_CreationTimeStamp;
    QString m_LastUpdateTimreStamp;
};

#endif // GUISCHEDULEITEMMODEL_H
