// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleItemList.h"
#include "ScheduleItemModel.h"
#include "stdChronoToQTConversions.h"

// QT Header Files
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
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QVariant>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <string>
#include <vector>

const std::chrono::seconds nextHour{3599}; // There are 3600 seconds in an hour.

// Create a new event
ScheduleItemEditorDialog::ScheduleItemEditorDialog(std::size_t userId, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId},
    m_DBModelID{0},
    m_DBModelData{nullptr},
    m_StartTime{std::chrono::system_clock::now()},
    m_EndTime{m_StartTime + nextHour},
    eventDateDE{nullptr}
{
    setUpDialogUI();
}

// Edit an existing scheduled event
ScheduleItemEditorDialog::ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId},
    m_DBModelID{eventId},
    m_DBModelData{nullptr},
    eventDateDE{nullptr}
{
    setUpDialogUI();
}

// Edit an empty event in the day schedule
ScheduleItemEditorDialog::ScheduleItemEditorDialog(
    std::size_t userId,
    std::chrono::system_clock::time_point startTime,
    std::chrono::system_clock::time_point endTime,
    QWidget *parent
)
    : QDialog(parent),
    m_UserID{userId},
    m_DBModelID{0},
    m_DBModelData{nullptr},
    m_StartTime{startTime},
    m_EndTime{endTime},
    eventDateDE{nullptr}
{
    m_UserPresetTime = true;

    setUpDialogUI();
}

ScheduleItemEditorDialog::~ScheduleItemEditorDialog()
{
}

// Do not initialize the constructor from the database, this function must
// be called after the constructor has been executed.
void ScheduleItemEditorDialog::initEditFields()
{
    m_DBModelData = std::make_shared<ScheduleItemModel>();
    m_DBModelData->setUserID(m_UserID);
    m_DBModelData->setScheduleItemID(m_DBModelID);

    // If we are editing a previously existing schedule item
    if (m_DBModelID)
    {
        m_DBModelData->retrieve();

        eventTitleTE->setPlainText(QString::fromStdString(m_DBModelData->getTitle()));
        locationTE->setPlainText(QString::fromStdString(m_DBModelData->getLocation()));
        isPersonalCB->setChecked(m_DBModelData->isPersonal());

        m_StartTime = m_DBModelData->getStartTime();
        m_EndTime = m_DBModelData->getEndTime();
    }

    QDateTime startTime = initValidDateTime(m_StartTime);
    eventDateDE->setDate(startTime.toLocalTime().date());
    initDateTimeEdit(startTimeDTE, m_StartTime);
    initDateTimeEdit(endTimeDTE, m_EndTime);

    initCompletersFromDB();

    connectAllSignalsAndSlots();
}

void ScheduleItemEditorDialog::accept()
{
    bool updateSuccessful = (m_DBModelData->getScheduleItemID() > 0)
            ? udpateDatabase()
            : addToDatabase();

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "Schedule edit failed.\n";
        errorReport += QString::fromStdString(m_DBModelData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void ScheduleItemEditorDialog::setUpDialogUI()
{
    dialogLayout = new QVBoxLayout(this);
    dialogLayout->setObjectName("dialogLayout");

    setLayout(dialogLayout);

    dialogLayout->addWidget(setUpGroupBoxForm());

    dialogLayout->addWidget(setUpDialogButtonBox());

    dialogLayout->setContentsMargins(20, 20, 20, 20);
    dialogLayout->setSpacing(15);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    adjustSize();

    QString titleStr = m_DBModelID? "Edit" : "Add";
    titleStr += " Schedule Dialog";
    setWindowTitle(titleStr);
}

QGroupBox *ScheduleItemEditorDialog::setUpGroupBoxForm()
{
    formGroupBox = new QGroupBox("Event Details:", this);
    formGroupBox->setObjectName("formGroupBox");
    formGroupBox->setAlignment(Qt::AlignHCenter);

    formGroupBoxLayout = cqtfa_FormLayoutWithPolicy("formGroupBoxLayout", formGroupBox);

    eventDateDE = cqtfa_DateEditWithCalendarPopUpCurrentDate("eventDateDE", formGroupBox);
    formGroupBoxLayout->addRow("Date:", eventDateDE);

    startTimeDTE = new QDateTimeEdit(formGroupBox);
    startTimeDTE->setObjectName("startTimeDTE");
    formGroupBoxLayout->addRow("Start:", startTimeDTE);
    
    endTimeDTE = new QDateTimeEdit(formGroupBox);
    endTimeDTE->setObjectName("endTimeDTE");
    formGroupBoxLayout->addRow("End:", endTimeDTE);

    if (m_DBModelID)
    {
        eventTitleTE = cqtfa_flexibleWidthPlainTextEdit("eventTitleTE",
            formGroupBox, sied_TextEditMinWidth, sied_TextEditMaxWidth, TitleLineCount);
        formGroupBoxLayout->addRow("What:", eventTitleTE);

        locationTE = cqtfa_flexibleWidthPlainTextEdit("locationTE", formGroupBox,
            sied_TextEditMinWidth, sied_TextEditMaxWidth, LocationLineCount);
        formGroupBoxLayout->addRow("Where:", locationTE);
    }
    else
    {
        eventTitleLE = cqtfa_LineEditFixedWidthByCharCount("eventTitleLE", formGroupBox, MaxCharLineEdit);
        formGroupBoxLayout->addRow("What:", eventTitleLE);

        locationLE = cqtfa_LineEditFixedWidthByCharCount("locationLE", formGroupBox, MaxCharLineEdit);
        formGroupBoxLayout->addRow("Where:", locationLE);
    }

    isPersonalCB = cqtfa_QTWidgetWithText<QCheckBox>("Personal",
        "isPersonalCB", formGroupBox);
    formGroupBoxLayout->addRow(isPersonalCB);

    return formGroupBox;
}

QDialogButtonBox *ScheduleItemEditorDialog::setUpDialogButtonBox()
{
    dialogButtonBox = new QDialogButtonBox(this);

    dialogButtonBox->setObjectName(QString::fromUtf8("dialogButtonBox"));
    dialogButtonBox->setOrientation(Qt::Horizontal);
    dialogButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    return dialogButtonBox;
}

void ScheduleItemEditorDialog::initDateTimeEdit(
    QDateTimeEdit* dtEdit,
    std::chrono::system_clock::time_point initValue
)
{
    QDateTime initDateTime = initValidDateTime(initValue);

    dtEdit->setDateTime(initDateTime);

    dtEdit->setDisplayFormat("HH:mm");

    dtEdit->setMinimumDateTime(initDateTime.addDays(-2));
    dtEdit->setMaximumDateTime(initDateTime.addYears(1));
    
    dtEdit->setCalendarPopup(false);
}

void ScheduleItemEditorDialog::handleEventDate_DateChanged()
{
    QDate newDate = eventDateDE->date();

    startTimeDTE->setDate(newDate);
    endTimeDTE->setDate(newDate);

    startTimeDTE->setMinimumDate(newDate.addDays(-2));
    endTimeDTE->setMinimumDate(startTimeDTE->minimumDate());

    startTimeDTE->setMaximumDate(newDate.addYears(1));
    endTimeDTE->setMaximumDate(startTimeDTE->maximumDate());
}

bool ScheduleItemEditorDialog::addToDatabase()
{
    transferFieldsToDataModel();
    return m_DBModelData->insert();
}

bool ScheduleItemEditorDialog::udpateDatabase()
{
    transferFieldsToDataModel();
    return m_DBModelData->update();
}

void ScheduleItemEditorDialog::transferFieldsToDataModel()
{
    if (eventTitleTE)
    {
        m_DBModelData->setTitle(eventTitleTE->toPlainText().toStdString());
        m_DBModelData->setLocation(locationTE->toPlainText().toStdString());
    }
    else
    {
        m_DBModelData->setTitle(eventTitleLE->text().toStdString());
        m_DBModelData->setLocation(locationLE->text().toStdString());
    }

    m_DBModelData->setStartDateAndTime(qDateTimeToChrono(startTimeDTE->dateTime()));
    m_DBModelData->setEndDateAndTime(qDateTimeToChrono(endTimeDTE->dateTime()));
    m_DBModelData->setPersonal(isPersonalCB->isChecked());
}
    
void ScheduleItemEditorDialog::initCompletersFromDB()
{
    // QCompleter doesn't work with QPlainTextEdit
    if (eventTitleTE)
    {
        return;
    }

    ScheduleItemList previousEventFinder(m_UserID);
    std::vector<std::string> previousEventTitles = previousEventFinder.findEventsForRepeatCompletion();
    QStringList previousEventList;

    for (const std::string& str : previousEventTitles) {
        previousEventList.append(QString::fromStdString(str));
    }

    titleCompleterQC = new QCompleter(previousEventList, formGroupBox);
    eventTitleLE->setCompleter(titleCompleterQC);

    std::vector<std::string> previousLocations = previousEventFinder.findLocationsForRepeatCompletion();
    QStringList locationList;

    for (const std::string& str : previousLocations) {
        locationList.append(QString::fromStdString(str));
    }

    locationCompleterQC = new QCompleter(locationList, formGroupBox);
    locationLE->setCompleter(locationCompleterQC);
}

QDateTime ScheduleItemEditorDialog::initValidDateTime(std::chrono::system_clock::time_point dateTime)
{
    QDateTime tempDate(chronoTimePointToQDateTime(dateTime));
    if (!tempDate.isValid())
    {
        tempDate = QDateTime::currentDateTime();
    }
    
    return tempDate;
}

void ScheduleItemEditorDialog::connectAllSignalsAndSlots()
{
    connect(eventDateDE, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleEventDate_DateChanged);

    connect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ScheduleItemEditorDialog::disconnectAllSignalsAndSlots()
{
    disconnect(eventDateDE, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleEventDate_DateChanged);

    disconnect(dialogButtonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(dialogButtonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
