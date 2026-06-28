#ifndef SCHEDULEITEMEDITORDIALOG_H
#define SCHEDULEITEMEDITORDIALOG_H

class ScheduleItemModel;

// Project Header Files
#include "BaseObjectEditorDialog.h"

// QT Header Files
#include <QVariant>
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
#include <memory>


class ScheduleItemEditorDialog : public BaseObjectEditorDialog
{
public:
    explicit ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId = 0, QWidget* parent = nullptr);
    explicit ScheduleItemEditorDialog(std::size_t userId, std::chrono::system_clock::time_point startTime,
            std::chrono::system_clock::time_point endTime, QWidget* parent = nullptr);
    ~ScheduleItemEditorDialog();
    virtual void initEditorFieldsFromDataBase() override;

private Q_SLOTS:
    void handleEventDate_DateChanged();

private:
    virtual void setUpEditorUI() override;
    virtual QGroupBox* setUpEditorDialogForm() override;
    void initDateTimeEdit(QDateTimeEdit* dtEdit, std::chrono::system_clock::time_point initValue);
    void initCompletersFromDB();
    QDateTime initValidDateTime(std::chrono::system_clock::time_point dateTime);
    virtual void createSharedPtrDBModelForAddObject() override;
    virtual void transferEditorValuesToDBModel() override;
    virtual void transferDBModelDataToEditorFields() override;

    std::chrono::system_clock::time_point m_StartTime;
    std::chrono::system_clock::time_point m_EndTime;
    bool m_UserPresetTime = false;

    QDateEdit* eventDateDE = nullptr;
    QDateTimeEdit* startTimeDTE = nullptr;
    QDateTimeEdit* endTimeDTE = nullptr;
    QPlainTextEdit* eventTitleTE = nullptr;
    QLineEdit* eventTitleLE = nullptr;
    QCompleter* titleCompleterQC = nullptr;
    QLineEdit* locationLE = nullptr;
    QPlainTextEdit* locationTE = nullptr;
    QCompleter* locationCompleterQC = nullptr;
    QCheckBox* isPersonalCB = nullptr;

    const int sied_TextEditMinWidth = 300;
    const int sied_TextEditMaxWidth = 700;
    const int MaxCharLineEdit = 128;
    const int TitleLineCount = 2;
    const int LocationLineCount = 3;
};

#endif // SCHEDULEITEMEDITORDIALOG_H
