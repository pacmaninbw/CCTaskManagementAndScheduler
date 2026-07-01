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
    m_qt_EventDate{nullptr}
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
    m_qt_EventDate{nullptr}
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
    m_qt_EventDate->setDate(startTime.toLocalTime().date());
    initDateTimeEdit(m_qt_StartTime, m_StartTime);
    initDateTimeEdit(m_qt_EndTime, m_EndTime);

    initCompletersFromDB();

/*
 * Wait until all date information is assigned before connecting this, it
 * is to allow the user to change the date after initialization.
 */
    connect(m_qt_EventDate, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleEventDate_DateChanged);
}

void ScheduleItemEditorDialog::setUpEditorUI()
{
    m_qt_EditorLayout = new QVBoxLayout(this);
    m_qt_EditorLayout->setObjectName("m_qt_EditorLayout");

    m_qt_EditorDialogFormGB = setUpEditorDialogForm();
    m_qt_EditorDialogFormGB->setObjectName("m_qt_EditorDialogFormGB");

    m_qt_EditorLayout->addWidget(m_qt_EditorDialogFormGB);

    m_qt_ButtonBox = setUpEditorButtonBox();
    m_qt_ButtonBox->setObjectName("m_qt_ButtonBox");
    m_qt_EditorLayout->addWidget(m_qt_ButtonBox);

    setLayout(m_qt_EditorLayout);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    adjustSize();
}

QGroupBox *ScheduleItemEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* formGroupBox = new QGroupBox("Event Details:", this);
    formGroupBox->setObjectName("formGroupBox");
    formGroupBox->setAlignment(Qt::AlignHCenter);

    m_qt_EditorFormLayout = new QFormLayout(formGroupBox);
    m_qt_EditorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_EventDate = cqtfa_DateEditWithCalendarPopUpCurrentDate("m_qt_EventDate", formGroupBox);
    m_qt_EditorFormLayout->addRow("Date:", m_qt_EventDate);

    m_qt_StartTime = new QDateTimeEdit(formGroupBox);
    m_qt_StartTime->setObjectName("m_qt_StartTime");
    m_qt_EditorFormLayout->addRow("Start:", m_qt_StartTime);
    
    m_qt_EndTime = new QDateTimeEdit(formGroupBox);
    m_qt_EndTime->setObjectName("m_qt_EndTime");
    m_qt_EditorFormLayout->addRow("End:", m_qt_EndTime);

    if (m_DBModelID)
    {
        m_qt_EditTitle = cqtfa_flexibleWidthPlainTextEdit("m_qt_EditTitle",
            formGroupBox, EventTextEditMinWidth, EventTextEditMaxWidth, TitleLineCount);
        m_qt_EditorFormLayout->addRow("What:", m_qt_EditTitle);

        m_qt_EditLocation = cqtfa_flexibleWidthPlainTextEdit("m_qt_EditLocation", formGroupBox,
            EventTextEditMinWidth, EventTextEditMaxWidth, LocationLineCount);
        m_qt_EditorFormLayout->addRow("Where:", m_qt_EditLocation);
    }
    else
    {
        m_qt_AddTitle = cqtfa_LineEditFixedWidthByCharCount("m_qt_AddTitle", formGroupBox, MaxCharLineEdit);
        m_qt_EditorFormLayout->addRow("What:", m_qt_AddTitle);

        m_qt_AddLocation = cqtfa_LineEditFixedWidthByCharCount("m_qt_AddLocation", formGroupBox, MaxCharLineEdit);
        m_qt_EditorFormLayout->addRow("Where:", m_qt_AddLocation);
    }

    m_qt_Personal = cqtfa_QTWidgetWithText<QCheckBox>("Personal", "m_qt_Personal", formGroupBox);
    m_qt_EditorFormLayout->addRow(m_qt_Personal);

    formGroupBox->setLayout(m_qt_EditorFormLayout);

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
    QDate newDate = m_qt_EventDate->date();

    m_qt_StartTime->setDate(newDate);
    m_qt_EndTime->setDate(newDate);

    m_qt_StartTime->setMinimumDate(newDate.addDays(-2));
    m_qt_EndTime->setMinimumDate(m_qt_StartTime->minimumDate());

    m_qt_StartTime->setMaximumDate(newDate.addYears(1));
    m_qt_EndTime->setMaximumDate(m_qt_StartTime->maximumDate());
}

void ScheduleItemEditorDialog::initCompletersFromDB()
{
    // QCompleter doesn't work with QPlainTextEdit
    if (m_qt_EditTitle)
    {
        return;
    }

    ScheduleItemQueryProcessor previousEventFinder(m_UserID);
    std::vector<std::string> previousEventTitles = previousEventFinder.findEventsForRepeatCompletion();
    QStringList previousEventList;

    for (const std::string& str : previousEventTitles) {
        previousEventList.append(QString::fromStdString(str));
    }

    m_qt_TitleCompleter = new QCompleter(previousEventList, m_qt_EditorDialogFormGB);
    m_qt_AddTitle->setCompleter(m_qt_TitleCompleter);

    std::vector<std::string> previousLocations = previousEventFinder.findLocationsForRepeatCompletion();
    QStringList locationList;

    for (const std::string& str : previousLocations) {
        locationList.append(QString::fromStdString(str));
    }

    m_qt_LocationCompleter = new QCompleter(locationList, m_qt_EditorDialogFormGB);
    m_qt_AddLocation->setCompleter(m_qt_LocationCompleter);
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
        if (m_qt_EditTitle)
        {
            eventData->setTitle(m_qt_EditTitle->toPlainText().toStdString());
            eventData->setLocation(m_qt_EditLocation->toPlainText().toStdString());
        }
        else
        {
            eventData->setTitle(m_qt_AddTitle->text().toStdString());
            eventData->setLocation(m_qt_AddLocation->text().toStdString());
        }

        eventData->setStartDateAndTime(qDateTimeToChrono(m_qt_StartTime->dateTime()));
        eventData->setEndDateAndTime(qDateTimeToChrono(m_qt_EndTime->dateTime()));
        eventData->setPersonal(m_qt_Personal->isChecked());
    }
}

void ScheduleItemEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_DBObjectModel)
    {
        ScheduleItemModel_shp eventData = std::dynamic_pointer_cast<ScheduleItemModel>(m_DBObjectModel);

        m_qt_EditTitle->setPlainText(QString::fromStdString(eventData->getTitle()));
        m_qt_EditLocation->setPlainText(QString::fromStdString(eventData->getLocation()));
        m_qt_Personal->setChecked(eventData->isPersonal());

        m_StartTime = eventData->getStartTime();
        m_EndTime = eventData->getEndTime();
    }
}
