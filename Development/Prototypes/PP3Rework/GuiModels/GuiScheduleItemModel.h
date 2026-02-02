#ifndef GUISCHEDULEITEMMODEL_H
#define GUISCHEDULEITEMMODEL_H

class ScheduleItemModel;
// Project Header Files

// QT Headeer Files
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>

class GuiScheduleItemModel : public QObject
{
    Q_OBJECT

public:
    explicit GuiScheduleItemModel(QObject *parent = nullptr);
    explicit GuiScheduleItemModel(std::shared_ptr<ScheduleItemModel> dbScheduleItemDataPtr, QObject *parent = nullptr);

    std::size_t getUserID() const { return m_UserID; };
    std::size_t getScheduleItemId() const { return m_DbScheduleItemId; };
    QString getTitle() const { return m_Title; };
    QDateTime getStartTime() const { return m_StartTime; };
    QDateTime getEndTime() const { return m_EndTime; };
    bool getPersonal() const { return m_Personal; };
    QString getLocation() const { return m_Location; };
    QString getCreationTS() const { return m_CreationTimeStamp; };
    QString getLastUpdateTS() const { return m_LastUpdateTimreStamp; };
    void debugShow();

    void setUserID(std::size_t inValue);
    void setTitle(QString inValue);
    void setStartTime(QDateTime inValue);
    void setEndTime(QDateTime inValue);
    void setPersonal(bool inValue);
    void setLocation(QString inValue);

signals:
    void userIdChanged();
    void titleChanged();
    void startTimeChanged();
    void endTimeChanged();
    void personalChanged();
    void locationChanged();

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
    std::shared_ptr<ScheduleItemModel> m_DbScheduleItemDataPtr;
};

#endif // GUISCHEDULEITEMMODEL_H
