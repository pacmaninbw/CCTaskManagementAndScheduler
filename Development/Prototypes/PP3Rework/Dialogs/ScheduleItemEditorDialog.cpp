// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "ScheduleItemEditorDialog.h"

// QT Header Files
#include <QMessageBox>

// Standard C++ Header Files

ScheduleItemEditorDialog::ScheduleItemEditorDialog(QWidget* parent, std::size_t userId, GuiScheduleItemModel* scheduleItemToEdit)
    : QDialog(parent),
    m_UserID{userId},
    m_ScheduleItemData{scheduleItemToEdit},
    seid_AddItemDate{nullptr}
{
    setUpScheduleItemEditorDialogUI();

    initEditFields();
}

ScheduleItemEditorDialog::~ScheduleItemEditorDialog()
{
}

void ScheduleItemEditorDialog::accept()
{
    bool updateSuccessful = (m_ScheduleItemData->getScheduleItemId() > 0)
            ? udpateDatabase()
            : addToDatabase();

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "User edit failed.\n";
        errorReport += m_ScheduleItemData->getErrorMessages();
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

    QString titleStr = m_ScheduleItemData? "Edit" : "Add";
    titleStr += " Schedule Dialog";
    setWindowTitle(titleStr);
}

QGroupBox *ScheduleItemEditorDialog::setUpScheduleTimeControls()
{
    sied_scheduleTimeControls = new QGroupBox("Event Details:", this);
    sied_scheduleTimeControls->setObjectName("sied_scheduleTimeControls");
    sied_scheduleTimeControls->setAlignment(Qt::AlignHCenter);

    seid_groupBoxLayout = cqtfa_FormLayoutWithPolicy("seid_groupBoxLayout", sied_scheduleTimeControls);

    if (!m_ScheduleItemData || m_ScheduleItemData->getScheduleItemId() < 1)
    {
        seid_AddItemDate = cqtfa_DateEditWithCalendarPopUpCurrentDate(
            "seid_AddItemDate", sied_scheduleTimeControls);
    
        seid_groupBoxLayout->addRow("Date:", seid_AddItemDate);

        connect(seid_AddItemDate, &QDateEdit::dateChanged, this,
            &ScheduleItemEditorDialog::handleAddItemDate_DateChanged);
    }

    sied_scheduleItemStartTimeDTEdit = createAndInitDateTimeEdit("sied_scheduleItemStartTimeDTEdit");
    seid_groupBoxLayout->addRow("Start:", sied_scheduleItemStartTimeDTEdit);
    
    sied_scheduleItemEndTimeDTEdit = createAndInitDateTimeEdit("sied_scheduleEndTimeDTEdit");
    seid_groupBoxLayout->addRow("End:", sied_scheduleItemEndTimeDTEdit);

    sied_scheduleItemTitleTE = cqtfa_flexibleWidthTextEdit("sied_scheduleItemTitleTE",
        sied_scheduleTimeControls, sied_TextEditMinWidth, sied_TextEditMaxWidth, 2);
    seid_groupBoxLayout->addRow("What:", sied_scheduleItemTitleTE);

    sied_locationTE = cqtfa_flexibleWidthTextEdit("sied_locationTE", sied_scheduleTimeControls,
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

    connect(sied_buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(sied_buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return sied_buttonBox;
}

QDateTimeEdit* ScheduleItemEditorDialog::createAndInitDateTimeEdit(const char *objName)
{
    QDateTime initDateTime = QDateTime::currentDateTime();

    if (seid_AddItemDate)
    {
        initDateTime.setDate(seid_AddItemDate->date());
    }

    QDateTimeEdit* newDateTimeElement = new QDateTimeEdit(QDateTime::currentDateTime(),
        sied_scheduleTimeControls);
    newDateTimeElement->setObjectName(objName);

    newDateTimeElement->setDisplayFormat("MM/dd/yyyy HH:mm:ss");

    newDateTimeElement->setMinimumDateTime(initDateTime.addDays(-2));
    newDateTimeElement->setMaximumDateTime(initDateTime.addYears(1));

    newDateTimeElement->setCalendarPopup(true);

    return newDateTimeElement;
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

bool ScheduleItemEditorDialog::addToDatabase()
{
    transferFieldsToDataModel();
    return m_ScheduleItemData->addToDatabase();
}

bool ScheduleItemEditorDialog::udpateDatabase()
{
    transferFieldsToDataModel();
    return m_ScheduleItemData->updateInDatabase();
}

void ScheduleItemEditorDialog::transferFieldsToDataModel()
{
    m_ScheduleItemData->setTitle(sied_scheduleItemTitleTE->toPlainText());
    m_ScheduleItemData->setStartTime(sied_scheduleItemStartTimeDTEdit->dateTime());
    m_ScheduleItemData->setEndTime(sied_scheduleItemEndTimeDTEdit->dateTime());
    m_ScheduleItemData->setLocation(sied_locationTE->toPlainText());
    m_ScheduleItemData->setPersonal(sied_scheduleItemIsPersonalCB->isChecked());
}
    
void ScheduleItemEditorDialog::initEditFields()
{
    if (!m_ScheduleItemData)
    {
        m_ScheduleItemData = new GuiScheduleItemModel(m_UserID, parent());
        return;
    }

    QDateTime startTime = initValidDateTime(m_ScheduleItemData->getStartTime());
    if (seid_AddItemDate)
    {
        seid_AddItemDate->setDate(startTime.date());
    }

    sied_scheduleItemStartTimeDTEdit->setDateTime(startTime);
    sied_scheduleItemEndTimeDTEdit->setDateTime(initValidDateTime(m_ScheduleItemData->getEndTime()));
    sied_scheduleItemTitleTE->setText(m_ScheduleItemData->getTitle());
    sied_locationTE->setText(m_ScheduleItemData->getLocation());
    sied_scheduleItemIsPersonalCB->setChecked(m_ScheduleItemData->getPersonal());
}

QDateTime ScheduleItemEditorDialog::initValidDateTime(QDateTime fieldData)
{
    QDateTime tempDate(fieldData);
    if (!tempDate.isValid())
    {
        tempDate = QDateTime::currentDateTime();
    }
    
    return tempDate;
}
