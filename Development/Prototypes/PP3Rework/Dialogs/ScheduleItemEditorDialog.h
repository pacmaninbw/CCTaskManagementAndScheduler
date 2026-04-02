#ifndef SCHEDULEITEMEDITORDIALOG_H
#define SCHEDULEITEMEDITORDIALOG_H

class ScheduleItemModel;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QCheckBox>
#include <QCompleter>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <chrono>
#include <memory>


class ScheduleItemEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleItemEditorDialog(std::size_t userId, QWidget* parent = nullptr);
    explicit ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId, QWidget* parent = nullptr);
    explicit ScheduleItemEditorDialog(std::size_t userId, std::chrono::system_clock::time_point startTime,
            std::chrono::system_clock::time_point endTime, QWidget* parent = nullptr);
    ~ScheduleItemEditorDialog();
    void initEditFields();

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void handleAddItemDate_DateChanged();
    void handleScheduleItemTitle_TextChanged();

private:
    void setUpScheduleItemEditorDialogUI();
    QGroupBox* setUpScheduleTimeControls();
    QDialogButtonBox* setUpScheduleItemButtonBox();
    void initDateTimeEdit(QDateTimeEdit* dtEdit, std::chrono::system_clock::time_point initValue);
    bool addToDatabase();
    bool udpateDatabase();
    void transferFieldsToDataModel();
    void initCompletersFromDB();
    QDateTime initValidDateTime(std::chrono::system_clock::time_point dateTime);
    void connectAllSignalsAndSlots();
    void disconnectAllSignalsAndSlots();

    std::size_t m_UserID;
    std::size_t m_ScheduleItemDBId;
    std::shared_ptr<ScheduleItemModel> m_ScheduleItemData;
    std::chrono::system_clock::time_point m_StartTime;
    std::chrono::system_clock::time_point m_EndTime;
    bool m_UserPresetTime = false;

    QVBoxLayout* seid_scheduleItemEditorDialogLayout;
    QDialogButtonBox* sied_buttonBox;
    QGroupBox* sied_scheduleTimeControls;
    QFormLayout* seid_groupBoxLayout;
    QDateEdit* seid_AddItemDate;
    QDateTimeEdit* sied_scheduleItemStartTimeDTEdit;
    QDateTimeEdit* sied_scheduleItemEndTimeDTEdit;
    QPlainTextEdit* sied_scheduleItemTitleTE;
    QCompleter* sied_titleCompleter;
    QPlainTextEdit* sied_locationTE;
    QCompleter* sied_locationCompleter;
    QCheckBox* sied_scheduleItemIsPersonalCB;

    const int sied_TextEditMinWidth = 300;
    const int sied_TextEditMaxWidth = 700;
};

#endif // SCHEDULEITEMEDITORDIALOG_H
