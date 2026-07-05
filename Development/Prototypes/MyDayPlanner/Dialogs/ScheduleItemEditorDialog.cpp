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
    m_qt_eventDate{nullptr}
{
    if (!eventId)
    {
        m_startTime = std::chrono::system_clock::now();
        m_endTime = m_startTime + nextHour;
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
    m_startTime{startTime},
    m_endTime{endTime},
    m_qt_eventDate{nullptr}
{
    m_userPresetTime = true;

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
    if (m_dbModelId)
    {
        ScheduleItemQueryProcessor scheduleItemQueryProcessor(m_userID);
        eventData = scheduleItemQueryProcessor.getScheduleItemById(m_dbModelId);
        m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(eventData);
        transferDBModelDataToEditorFields();
    }
    else
    {
        eventData = std::make_shared<ScheduleItemModel>();
        eventData->setUserID(m_userID);
        m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(eventData);
    }

    QDateTime startTime = initValidDateTime(m_startTime);
    m_qt_eventDate->setDate(startTime.toLocalTime().date());
    initDateTimeEdit(m_qt_startTime, m_startTime);
    initDateTimeEdit(m_qt_endTime, m_endTime);

    initCompletersFromDB();

/*
 * Wait until all date information is assigned before connecting this, it
 * is to allow the user to change the date after initialization.
 */
    connect(m_qt_eventDate, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleEventDate_DateChanged);
}

void ScheduleItemEditorDialog::setUpEditorUI()
{
    m_qt_editorLayout = new QVBoxLayout(this);
    m_qt_editorLayout->setObjectName("m_qt_editorLayout");

    m_qt_editorDialogFormGB = setUpEditorDialogForm();
    m_qt_editorDialogFormGB->setObjectName("m_qt_editorDialogFormGB");

    m_qt_editorLayout->addWidget(m_qt_editorDialogFormGB);

    m_qt_buttonBox = setUpEditorButtonBox();
    m_qt_buttonBox->setObjectName("m_qt_buttonBox");
    m_qt_editorLayout->addWidget(m_qt_buttonBox);

    setLayout(m_qt_editorLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    adjustSize();
}

QGroupBox *ScheduleItemEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* formGroupBox = new QGroupBox("Event Details:", this);
    formGroupBox->setObjectName("formGroupBox");
    formGroupBox->setAlignment(Qt::AlignHCenter);

    m_qt_editorFormLayout = new QFormLayout(formGroupBox);
    m_qt_editorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_eventDate = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_eventDate", formGroupBox);
    m_qt_editorFormLayout->addRow("Date:", m_qt_eventDate);

    m_qt_startTime = new QDateTimeEdit(formGroupBox);
    m_qt_startTime->setObjectName("m_qt_startTime");
    m_qt_editorFormLayout->addRow("Start:", m_qt_startTime);
    
    m_qt_endTime = new QDateTimeEdit(formGroupBox);
    m_qt_endTime->setObjectName("m_qt_endTime");
    m_qt_editorFormLayout->addRow("End:", m_qt_endTime);

    if (m_dbModelId)
    {
        m_qt_editTitle = cqtfa_flexibleWidthPlainTextEdit("m_qt_editTitle",
            formGroupBox, EventTextEditMinWidth, EventTextEditMaxWidth, TitleLineCount);
        m_qt_editorFormLayout->addRow("What:", m_qt_editTitle);

        m_qt_editLocation = cqtfa_flexibleWidthPlainTextEdit("m_qt_editLocation", formGroupBox,
            EventTextEditMinWidth, EventTextEditMaxWidth, LocationLineCount);
        m_qt_editorFormLayout->addRow("Where:", m_qt_editLocation);
    }
    else
    {
        m_qt_addTitle = cqtfa_LineEditFixedWidthByCharCount("m_qt_addTitle", formGroupBox, MaxCharLineEdit);
        m_qt_editorFormLayout->addRow("What:", m_qt_addTitle);

        m_qt_addLocation = cqtfa_LineEditFixedWidthByCharCount("m_qt_addLocation", formGroupBox, MaxCharLineEdit);
        m_qt_editorFormLayout->addRow("Where:", m_qt_addLocation);
    }

    m_qt_personal = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "m_qt_personal", formGroupBox);
    m_qt_editorFormLayout->addRow(m_qt_personal);

    formGroupBox->setLayout(m_qt_editorFormLayout);

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
    QDate newDate = m_qt_eventDate->date();

    m_qt_startTime->setDate(newDate);
    m_qt_endTime->setDate(newDate);

    m_qt_startTime->setMinimumDate(newDate.addDays(-2));
    m_qt_endTime->setMinimumDate(m_qt_startTime->minimumDate());

    m_qt_startTime->setMaximumDate(newDate.addYears(1));
    m_qt_endTime->setMaximumDate(m_qt_startTime->maximumDate());
}

void ScheduleItemEditorDialog::initCompletersFromDB()
{
    // QCompleter doesn't work with QPlainTextEdit
    if (m_qt_editTitle)
    {
        return;
    }

    ScheduleItemQueryProcessor previousEventFinder(m_userID);
    std::vector<std::string> previousEventTitles = previousEventFinder.findEventsForRepeatCompletion();
    QStringList previousEventList;

    for (const std::string& str : previousEventTitles) {
        previousEventList.append(QString::fromStdString(str));
    }

    m_qt_titleCompleter = new QCompleter(previousEventList, m_qt_editorDialogFormGB);
    m_qt_addTitle->setCompleter(m_qt_titleCompleter);

    std::vector<std::string> previousLocations = previousEventFinder.findLocationsForRepeatCompletion();
    QStringList locationList;

    for (const std::string& str : previousLocations) {
        locationList.append(QString::fromStdString(str));
    }

    m_qt_locationCompleter = new QCompleter(locationList, m_qt_editorDialogFormGB);
    m_qt_addLocation->setCompleter(m_qt_locationCompleter);
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
    newEvent->setUserID(m_userID);

    m_dbObjectModel = std::dynamic_pointer_cast<ScheduleItemModel>(newEvent);
}

void ScheduleItemEditorDialog::transferEditorValuesToDBModel()
{
    if (m_dbObjectModel)
    {
        ScheduleItemModel_shp eventData = std::dynamic_pointer_cast<ScheduleItemModel>(m_dbObjectModel);
        if (m_qt_editTitle)
        {
            eventData->setTitle(m_qt_editTitle->toPlainText().toStdString());
            eventData->setLocation(m_qt_editLocation->toPlainText().toStdString());
        }
        else
        {
            eventData->setTitle(m_qt_addTitle->text().toStdString());
            eventData->setLocation(m_qt_addLocation->text().toStdString());
        }

        eventData->setStartDateAndTime(qDateTimeToChrono(m_qt_startTime->dateTime()));
        eventData->setEndDateAndTime(qDateTimeToChrono(m_qt_endTime->dateTime()));
        eventData->setPersonal(m_qt_personal->isChecked());
    }
}

void ScheduleItemEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_dbObjectModel)
    {
        ScheduleItemModel_shp eventData = std::dynamic_pointer_cast<ScheduleItemModel>(m_dbObjectModel);

        m_qt_editTitle->setPlainText(QString::fromStdString(eventData->getTitle()));
        m_qt_editLocation->setPlainText(QString::fromStdString(eventData->getLocation()));
        m_qt_personal->setChecked(eventData->isPersonal());

        m_startTime = eventData->getStartTime();
        m_endTime = eventData->getEndTime();
    }
}
