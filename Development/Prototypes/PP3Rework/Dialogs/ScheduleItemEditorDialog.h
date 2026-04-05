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
#include <QLineEdit>
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
    void handleEventDate_DateChanged();

private:
    void setUpDialogUI();
    QGroupBox* setUpGroupBoxForm();
    QDialogButtonBox* setUpDialogButtonBox();
    void initDateTimeEdit(QDateTimeEdit* dtEdit, std::chrono::system_clock::time_point initValue);
    bool addToDatabase();
    bool udpateDatabase();
    void transferFieldsToDataModel();
    void initCompletersFromDB();
    QDateTime initValidDateTime(std::chrono::system_clock::time_point dateTime);
    void connectAllSignalsAndSlots();
    void disconnectAllSignalsAndSlots();

    std::size_t m_UserID;
    std::size_t m_DBModelID;
    std::shared_ptr<ScheduleItemModel> m_DBModelData;
    std::chrono::system_clock::time_point m_StartTime;
    std::chrono::system_clock::time_point m_EndTime;
    bool m_UserPresetTime = false;

    QVBoxLayout* dialogLayout = nullptr;
    QDialogButtonBox* dialogButtonBox = nullptr;
    QGroupBox* formGroupBox = nullptr;
    QFormLayout* formGroupBoxLayout = nullptr;
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
