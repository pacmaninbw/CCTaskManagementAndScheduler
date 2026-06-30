// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleItemQueryProcessor.h"
#include "ScheduleItemModel.h"
#include "stdChronoToQTConversions.h"

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
#include <memory>
#include <string>
#include <vector>

const std::chrono::seconds nextHour{3599}; // There are 3600 seconds in an hour.

ScheduleItemEditorDialog::ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId, QWidget *parent)
    : BaseObjectEditorDialog("Event", userId, eventId, parent),
    eventDateDE{nullptr}
{
    if (!eventId)
    {
        m_StartTime = std::chrono::system_clock::now();
        m_EndTime = m_StartTime + nextHour;
    }
    
    setUpEditorUI();
}

// Edit an empty event in the day schedule
ScheduleItemEditorDialog::ScheduleItemEditorDialog(
    std::size_t userId,
    std::chrono::system_clock::time_point startTime,
    std::chrono::system_clock::time_point endTime,
    QWidget *parent
)
    : BaseObjectEditorDialog("Event", userId, 0, parent),
    m_StartTime{startTime},
    m_EndTime{endTime},
    eventDateDE{nullptr}
{
    m_UserPresetTime = true;

    setUpEditorUI();
}

ScheduleItemEditorDialog::~ScheduleItemEditorDialog()
{
}

// Do not initialize the constructor from the database, this function must
// be called after the constructor has been executed.
void ScheduleItemEditorDialog::initEditorFieldsFromDataBase()
{
    ScheduleItemModel_shp eventData;        

    // If we are editing a previously existing schedule item
    if (m_DBModelID)
    {
        ScheduleItemQueryProcessor scheduleItemQueryProcessor(m_UserID);
        eventData = scheduleItemQueryProcessor.getScheduleItemById(m_DBModelID);
        m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(eventData);
        transferDBModelDataToEditorFields();
    }
    else
    {
        eventData = std::make_shared<ScheduleItemModel>();
        eventData->setUserID(m_UserID);
        m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(eventData);
    }

    QDateTime startTime = initValidDateTime(m_StartTime);
    eventDateDE->setDate(startTime.toLocalTime().date());
    initDateTimeEdit(startTimeDTE, m_StartTime);
    initDateTimeEdit(endTimeDTE, m_EndTime);

    initCompletersFromDB();

/*
 * Wait until all date information is assigned before connecting this, it
 * is to allow the user to change the date after initialization.
 */
    connect(eventDateDE, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleEventDate_DateChanged);
}

void ScheduleItemEditorDialog::setUpEditorUI()
{
    m_Qt_EditorLayout = new QVBoxLayout(this);
    m_Qt_EditorLayout->setObjectName("m_Qt_EditorLayout");

    m_Qt_EditorDialogFormGB = setUpEditorDialogForm();
    m_Qt_EditorDialogFormGB->setObjectName("m_Qt_EditorDialogFormGB");

    m_Qt_EditorLayout->addWidget(m_Qt_EditorDialogFormGB);

    m_Qt_ButtonBox = setUpEditorButtonBox();
    m_Qt_ButtonBox->setObjectName("m_Qt_ButtonBox");
    m_Qt_EditorLayout->addWidget(m_Qt_ButtonBox);

    setLayout(m_Qt_EditorLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    adjustSize();
}

QGroupBox *ScheduleItemEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* formGroupBox = new QGroupBox("Event Details:", this);
    formGroupBox->setObjectName("formGroupBox");
    formGroupBox->setAlignment(Qt::AlignHCenter);

    m_Qt_EditorFormLayout = new QFormLayout(formGroupBox);
    m_Qt_EditorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    eventDateDE = cqtfa_DateEditWithCalendarPopUpCurrentDate("eventDateDE", formGroupBox);
    m_Qt_EditorFormLayout->addRow("Date:", eventDateDE);

    startTimeDTE = new QDateTimeEdit(formGroupBox);
    startTimeDTE->setObjectName("startTimeDTE");
    m_Qt_EditorFormLayout->addRow("Start:", startTimeDTE);
    
    endTimeDTE = new QDateTimeEdit(formGroupBox);
    endTimeDTE->setObjectName("endTimeDTE");
    m_Qt_EditorFormLayout->addRow("End:", endTimeDTE);

    if (m_DBModelID)
    {
        eventTitleTE = cqtfa_flexibleWidthPlainTextEdit("eventTitleTE",
            formGroupBox, sied_TextEditMinWidth, sied_TextEditMaxWidth, TitleLineCount);
        m_Qt_EditorFormLayout->addRow("What:", eventTitleTE);

        locationTE = cqtfa_flexibleWidthPlainTextEdit("locationTE", formGroupBox,
            sied_TextEditMinWidth, sied_TextEditMaxWidth, LocationLineCount);
        m_Qt_EditorFormLayout->addRow("Where:", locationTE);
    }
    else
    {
        eventTitleLE = cqtfa_LineEditFixedWidthByCharCount("eventTitleLE", formGroupBox, MaxCharLineEdit);
        m_Qt_EditorFormLayout->addRow("What:", eventTitleLE);

        locationLE = cqtfa_LineEditFixedWidthByCharCount("locationLE", formGroupBox, MaxCharLineEdit);
        m_Qt_EditorFormLayout->addRow("Where:", locationLE);
    }

    isPersonalCB = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "isPersonalCB", formGroupBox);
    m_Qt_EditorFormLayout->addRow(isPersonalCB);

    formGroupBox->setLayout(m_Qt_EditorFormLayout);

    return formGroupBox;
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

void ScheduleItemEditorDialog::initCompletersFromDB()
{
    // QCompleter doesn't work with QPlainTextEdit
    if (eventTitleTE)
    {
        return;
    }

    ScheduleItemQueryProcessor previousEventFinder(m_UserID);
    std::vector<std::string> previousEventTitles = previousEventFinder.findEventsForRepeatCompletion();
    QStringList previousEventList;

    for (const std::string& str : previousEventTitles) {
        previousEventList.append(QString::fromStdString(str));
    }

    titleCompleterQC = new QCompleter(previousEventList, m_Qt_EditorDialogFormGB);
    eventTitleLE->setCompleter(titleCompleterQC);

    std::vector<std::string> previousLocations = previousEventFinder.findLocationsForRepeatCompletion();
    QStringList locationList;

    for (const std::string& str : previousLocations) {
        locationList.append(QString::fromStdString(str));
    }

    locationCompleterQC = new QCompleter(locationList, m_Qt_EditorDialogFormGB);
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

void ScheduleItemEditorDialog::createSharedPtrDBModelForAddObject()
{
    ScheduleItemModel_shp newEvent = std::make_shared<ScheduleItemModel>();
    newEvent->setUserID(m_UserID);

    m_DBObjectModel = std::dynamic_pointer_cast<ScheduleItemModel>(newEvent);
}

void ScheduleItemEditorDialog::transferEditorValuesToDBModel()
{
    if (m_DBObjectModel)
    {
        ScheduleItemModel_shp eventData = std::dynamic_pointer_cast<ScheduleItemModel>(m_DBObjectModel);
        if (eventTitleTE)
        {
            eventData->setTitle(eventTitleTE->toPlainText().toStdString());
            eventData->setLocation(locationTE->toPlainText().toStdString());
        }
        else
        {
            eventData->setTitle(eventTitleLE->text().toStdString());
            eventData->setLocation(locationLE->text().toStdString());
        }

        eventData->setStartDateAndTime(qDateTimeToChrono(startTimeDTE->dateTime()));
        eventData->setEndDateAndTime(qDateTimeToChrono(endTimeDTE->dateTime()));
        eventData->setPersonal(isPersonalCB->isChecked());
    }
}

void ScheduleItemEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_DBObjectModel)
    {
        ScheduleItemModel_shp eventData = std::dynamic_pointer_cast<ScheduleItemModel>(m_DBObjectModel);

        eventTitleTE->setPlainText(QString::fromStdString(eventData->getTitle()));
        locationTE->setPlainText(QString::fromStdString(eventData->getLocation()));
        isPersonalCB->setChecked(eventData->isPersonal());

        m_StartTime = eventData->getStartTime();
        m_EndTime = eventData->getEndTime();
    }
}
