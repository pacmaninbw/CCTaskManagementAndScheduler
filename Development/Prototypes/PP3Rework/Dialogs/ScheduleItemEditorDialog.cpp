// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectPreviousEventDialog.h"
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
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QVariant>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <chrono>
#include <memory>
#include <string>
#include <vector>

ScheduleItemEditorDialog::ScheduleItemEditorDialog(std::size_t userId, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId},
    m_ScheduleItemDBId{0},
    m_ScheduleItemData{nullptr},
    seid_AddItemDate{nullptr}
{
    setUpScheduleItemEditorDialogUI();
}

ScheduleItemEditorDialog::ScheduleItemEditorDialog(std::size_t userId, std::size_t eventId, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId},
    m_ScheduleItemDBId{eventId},
    m_ScheduleItemData{nullptr},
    seid_AddItemDate{nullptr}
{
    setUpScheduleItemEditorDialogUI();
}

ScheduleItemEditorDialog::ScheduleItemEditorDialog(
    std::size_t userId,
    std::chrono::system_clock::time_point startTime,
    std::chrono::system_clock::time_point endTime,
    QWidget *parent
)
    : QDialog(parent),
    m_UserID{userId},
    m_ScheduleItemDBId{0},
    m_ScheduleItemData{nullptr},
    seid_AddItemDate{nullptr}
{
    m_StartTime = startTime;
    m_EndTime = endTime;
    m_UserPresetTime = true;

    setUpScheduleItemEditorDialogUI();
}

ScheduleItemEditorDialog::~ScheduleItemEditorDialog()
{
}

// Do not initialize the constructor from the database, this function must
// be called after the constructor has been executed.
void ScheduleItemEditorDialog::initEditFields()
{
    m_ScheduleItemData = std::make_shared<ScheduleItemModel>();
    m_ScheduleItemData->setUserID(m_UserID);
    m_ScheduleItemData->setScheduleItemID(m_ScheduleItemDBId);

    // If we are editing a previously existing schedule item
    if (m_ScheduleItemDBId)
    {
        m_ScheduleItemData->retrieve();

        sied_scheduleItemTitleTE->setPlainText(QString::fromStdString(m_ScheduleItemData->getTitle()));
        sied_locationTE->setPlainText(QString::fromStdString(m_ScheduleItemData->getLocation()));
        sied_scheduleItemIsPersonalCB->setChecked(m_ScheduleItemData->isPersonal());

        m_StartTime = m_ScheduleItemData->getStartTime();
        m_EndTime = m_ScheduleItemData->getEndTime();
    }

    QDateTime startTime = initValidDateTime(m_StartTime);
    seid_AddItemDate->setDate(startTime.toLocalTime().date());
    initDateTimeEdit(sied_scheduleItemStartTimeDTEdit, m_StartTime);
    initDateTimeEdit(sied_scheduleItemEndTimeDTEdit, m_EndTime);

    connectAllSignalsAndSlots();
}

void ScheduleItemEditorDialog::accept()
{
    bool updateSuccessful = (m_ScheduleItemData->getScheduleItemID() > 0)
            ? udpateDatabase()
            : addToDatabase();

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "Schedule edit failed.\n";
        errorReport += QString::fromStdString(m_ScheduleItemData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void ScheduleItemEditorDialog::setUpScheduleItemEditorDialogUI()
{
    seid_scheduleItemEditorDialogLayout = new QVBoxLayout(this);
    seid_scheduleItemEditorDialogLayout->setObjectName("seid_scheduleItemEditorDialogLayout");

    setLayout(seid_scheduleItemEditorDialogLayout);

    seid_scheduleItemEditorDialogLayout->addWidget(setUpScheduleTimeControls());

    seid_scheduleItemEditorDialogLayout->addWidget(setUpScheduleItemButtonBox());

    seid_scheduleItemEditorDialogLayout->setContentsMargins(20, 20, 20, 20);
    seid_scheduleItemEditorDialogLayout->setSpacing(15);

    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    adjustSize();

    QString titleStr = m_ScheduleItemDBId? "Edit" : "Add";
    titleStr += " Schedule Dialog";
    setWindowTitle(titleStr);
}

QGroupBox *ScheduleItemEditorDialog::setUpScheduleTimeControls()
{
    sied_scheduleTimeControls = new QGroupBox("Event Details:", this);
    sied_scheduleTimeControls->setObjectName("sied_scheduleTimeControls");
    sied_scheduleTimeControls->setAlignment(Qt::AlignHCenter);

    seid_groupBoxLayout = cqtfa_FormLayoutWithPolicy("seid_groupBoxLayout", sied_scheduleTimeControls);

    seid_AddItemDate = cqtfa_DateEditWithCalendarPopUpCurrentDate(
        "seid_AddItemDate", sied_scheduleTimeControls);
    seid_groupBoxLayout->addRow("Date:", seid_AddItemDate);

    sied_scheduleItemStartTimeDTEdit = new QDateTimeEdit(sied_scheduleTimeControls);
    sied_scheduleItemStartTimeDTEdit->setObjectName("sied_scheduleItemStartTimeDTEdit");
    seid_groupBoxLayout->addRow("Start:", sied_scheduleItemStartTimeDTEdit);
    
    sied_scheduleItemEndTimeDTEdit = new QDateTimeEdit(sied_scheduleTimeControls);
    sied_scheduleItemEndTimeDTEdit->setObjectName("sied_scheduleItemEndTimeDTEdit");
    seid_groupBoxLayout->addRow("End:", sied_scheduleItemEndTimeDTEdit);

    sied_scheduleItemTitleTE = cqtfa_flexibleWidthPlainTextEdit("sied_scheduleItemTitleTE",
        sied_scheduleTimeControls, sied_TextEditMinWidth, sied_TextEditMaxWidth, 2);
    seid_groupBoxLayout->addRow("What:", sied_scheduleItemTitleTE);

    sied_locationTE = cqtfa_flexibleWidthPlainTextEdit("sied_locationTE", sied_scheduleTimeControls,
        sied_TextEditMinWidth, sied_TextEditMaxWidth, 3);
    seid_groupBoxLayout->addRow("Where:", sied_locationTE);

    sied_scheduleItemIsPersonalCB = cqtfa_QTWidgetWithText<QCheckBox>("Personal",
        "sied_scheduleItemIsPersonalCB", sied_scheduleTimeControls);
    seid_groupBoxLayout->addRow(sied_scheduleItemIsPersonalCB);

    return sied_scheduleTimeControls;
}

QDialogButtonBox *ScheduleItemEditorDialog::setUpScheduleItemButtonBox()
{
    sied_buttonBox = new QDialogButtonBox(this);

    sied_buttonBox->setObjectName(QString::fromUtf8("sied_buttonBox"));
    sied_buttonBox->setOrientation(Qt::Horizontal);
    sied_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    return sied_buttonBox;
}

void ScheduleItemEditorDialog::initDateTimeEdit(QDateTimeEdit* dtEdit, std::chrono::system_clock::time_point initValue)
{
    QDateTime initDateTime = initValidDateTime(initValue);

    dtEdit->setDateTime(initDateTime);

    dtEdit->setDisplayFormat("HH:mm");

    dtEdit->setMinimumDateTime(initDateTime.addDays(-2));
    dtEdit->setMaximumDateTime(initDateTime.addYears(1));
    
    dtEdit->setCalendarPopup(false);
}

void ScheduleItemEditorDialog::handleAddItemDate_DateChanged()
{
    QDate newDate = seid_AddItemDate->date();

    sied_scheduleItemStartTimeDTEdit->setDate(newDate);
    sied_scheduleItemEndTimeDTEdit->setDate(newDate);

    sied_scheduleItemStartTimeDTEdit->setMinimumDate(newDate.addDays(-2));
    sied_scheduleItemEndTimeDTEdit->setMinimumDate(sied_scheduleItemStartTimeDTEdit->minimumDate());

    sied_scheduleItemStartTimeDTEdit->setMaximumDate(newDate.addYears(1));
    sied_scheduleItemEndTimeDTEdit->setMaximumDate(sied_scheduleItemStartTimeDTEdit->maximumDate());
}

void ScheduleItemEditorDialog::handleScheduleItemTitle_TextChanged()
{
    std::string searchString = sied_scheduleItemTitleTE->toPlainText().toStdString();
    if (searchString.size() >= 5)
    {
        ScheduleItemList previousTitleFind(m_UserID);
        std::vector<std::string> previousEvents = previousTitleFind.findEventSToRepeat(searchString);
        if (previousEvents.size())
        {
            // Prevent re-entering this function until it has completed
            disconnectAllSignalsAndSlots();

            SelectPreviousEventDialog previousEventFinder(m_UserID, previousEvents, this);
            previousEventFinder.exec();
            QString selectedEvent = previousEventFinder.getSelectedEvent();
            // Don't replace contents if nothing was returned!
            if (selectedEvent.size() > 0)
            {
                sied_scheduleItemTitleTE->setPlainText(previousEventFinder.getSelectedEvent());
            }

            connectAllSignalsAndSlots();
            // Change the focus so that we don't re-enter this function unless the user
            // wants to change the returned string.
            focusNextChild();
        }
        else
        {
            std::string errors = previousTitleFind.getAllErrorMessages();
            if (errors.size() && errors.size() > 3)
            {
                disconnect(sied_scheduleItemTitleTE, &QPlainTextEdit::textChanged, this,
                    &ScheduleItemEditorDialog::handleScheduleItemTitle_TextChanged);
                QString myqErrors = "previousTitleFind.findEventSToRepeat()";
                myqErrors += QString::fromStdString(errors);
                QMessageBox::critical(nullptr, "Critical Error", myqErrors, QMessageBox::Ok);
                return;
            }
        }
    }
}

bool ScheduleItemEditorDialog::addToDatabase()
{
    transferFieldsToDataModel();
    return m_ScheduleItemData->insert();
}

bool ScheduleItemEditorDialog::udpateDatabase()
{
    transferFieldsToDataModel();
    return m_ScheduleItemData->update();
}

void ScheduleItemEditorDialog::transferFieldsToDataModel()
{
    m_ScheduleItemData->setTitle(sied_scheduleItemTitleTE->toPlainText().toStdString());
    m_ScheduleItemData->setStartDateAndTime(qDateTimeToChrono(sied_scheduleItemStartTimeDTEdit->dateTime()));
    m_ScheduleItemData->setEndDateAndTime(qDateTimeToChrono(sied_scheduleItemEndTimeDTEdit->dateTime()));
    m_ScheduleItemData->setLocation(sied_locationTE->toPlainText().toStdString());
    m_ScheduleItemData->setPersonal(sied_scheduleItemIsPersonalCB->isChecked());
}
    
void ScheduleItemEditorDialog::initCompletersFromDB()
{

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
    connect(seid_AddItemDate, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleAddItemDate_DateChanged);

    connect(sied_scheduleItemTitleTE, &QPlainTextEdit::textChanged, this,
        &ScheduleItemEditorDialog::handleScheduleItemTitle_TextChanged);

    connect(sied_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(sied_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void ScheduleItemEditorDialog::disconnectAllSignalsAndSlots()
{
    disconnect(seid_AddItemDate, &QDateEdit::dateChanged, this,
        &ScheduleItemEditorDialog::handleAddItemDate_DateChanged);

    disconnect(sied_scheduleItemTitleTE, &QPlainTextEdit::textChanged, this,
        &ScheduleItemEditorDialog::handleScheduleItemTitle_TextChanged);

    disconnect(sied_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    disconnect(sied_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}
