#ifndef SCHEDULEITEMEDITORDIALOG_H_
#define SCHEDULEITEMEDITORDIALOG_H_

class ScheduleItemModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QCheckBox>
#include <QCompleter>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <chrono>


class ScheduleItemEditorDialog : public BaseObjectEditorDialog
{
public:
    explicit ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId = 0, QWidget* parent = nullptr);
// Edit an empty event in the day schedule
    explicit ScheduleItemEditorDialog(std::size_t userId, std::chrono::system_clock::time_point startTime,
            std::chrono::system_clock::time_point endTime, QWidget* parent = nullptr);
    ~ScheduleItemEditorDialog();
    virtual void initEditorFieldsFromDataBase() override;

private Q_SLOTS:
    void handleEventDate_DateChanged();

private:
    const int EventTextEditMinWidth = 300;
    const int EventTextEditMaxWidth = 700;
    const int MaxCharLineEdit = 128;
    const int TitleLineCount = 2;
    const int LocationLineCount = 3;

    virtual void setUpEditorUI() override;
    virtual QGroupBox* setUpEditorDialogForm() override;
    void initDateTimeEdit(QDateTimeEdit* dtEdit, std::chrono::system_clock::time_point initValue);
    void initCompletersFromDB();
    QDateTime initValidDateTime(std::chrono::system_clock::time_point dateTime);
    virtual void createSharedPtrDBModelForAddObject() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;

    std::chrono::system_clock::time_point m_startTime;
    std::chrono::system_clock::time_point m_endTime;
    bool m_userPresetTime = false;

    QDateEdit* m_qt_eventDate = nullptr;
    QDateTimeEdit* m_qt_startTime = nullptr;
    QDateTimeEdit* m_qt_endTime = nullptr;
    QPlainTextEdit* m_qt_editTitle = nullptr;
    QLineEdit* m_qt_addTitle = nullptr;
    QCompleter* m_qt_titleCompleter = nullptr;
    QLineEdit* m_qt_addLocation = nullptr;
    QPlainTextEdit* m_qt_editLocation = nullptr;
    QCompleter* m_qt_locationCompleter = nullptr;
    QCheckBox* m_qt_personal = nullptr;

};

#endif // SCHEDULEITEMEDITORDIALOG_H_
