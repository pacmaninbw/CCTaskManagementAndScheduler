// Project Header Files
#include "createNamedQTWidget.h"
#include "ScheduleItemEditorDialog.h"

// QT Header Files

// Standard C++ Header Files

ScheduleItemEditorDialog::ScheduleItemEditorDialog(QWidget* parent, std::size_t userId, std::size_t siId)
    : QDialog(parent),
    userID{userId},
    scheduleItemID{siId},
    seid_AddItemDate{nullptr}
{
    setUpScheduleItemEditorDialogUI();
}

ScheduleItemEditorDialog::~ScheduleItemEditorDialog()
{
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

    QString titleStr = scheduleItemID? "Edit" : "Add";
    titleStr += " Schedule Dialog";
    setWindowTitle(titleStr);
}

QGroupBox *ScheduleItemEditorDialog::setUpScheduleTimeControls()
{
    sied_scheduleTimeControls = new QGroupBox("Event Details:", this);
    sied_scheduleTimeControls->setObjectName("sied_scheduleTimeControls");
    sied_scheduleTimeControls->setAlignment(Qt::AlignHCenter);

    seid_groupBoxLayout = createNamedFormLayoutWithPolicy("seid_groupBoxLayout", sied_scheduleTimeControls);

    if (scheduleItemID < 1)
    {
        seid_AddItemDate = createNamedDateEditWithCalendarPopUpCurrentDate(
            "seid_AddItemDate", sied_scheduleTimeControls);
    
        seid_groupBoxLayout->addRow("Date:", seid_AddItemDate);

        connect(seid_AddItemDate, &QDateEdit::dateChanged, this,
            &ScheduleItemEditorDialog::handleAddItemDate_DateChanged);
    }

    sied_scheduleItemStartTimeDTEdit = createAndInitDateTimeEdit("sied_scheduleItemStartTimeDTEdit");
    seid_groupBoxLayout->addRow("Start:", sied_scheduleItemStartTimeDTEdit);
    
    sied_scheduleItemEndTimeDTEdit = createAndInitDateTimeEdit("sied_scheduleEndTimeDTEdit");
    seid_groupBoxLayout->addRow("End:", sied_scheduleItemEndTimeDTEdit);

    sied_scheduleItemTitleTE = createAndInitTextEdit("sied_scheduleItemTitleTE",
        sied_TextEditMinWidth, sied_TextEditMaxWidth, 2);
    seid_groupBoxLayout->addRow("What:", sied_scheduleItemTitleTE);

    sied_locationTE = createAndInitTextEdit("sied_locationTE", sied_TextEditMinWidth,
        sied_TextEditMaxWidth, 3);
    seid_groupBoxLayout->addRow("Where:", sied_locationTE);

    sied_scheduleItemIsPersonalCB = createNameQTWidgetWithText<QCheckBox>("Personal",
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

QTextEdit* ScheduleItemEditorDialog::createAndInitTextEdit(const char *objName, int boxWidthMin, int boxWidthMax, int rowCount)
{
    QTextEdit* newTextEdit = new QTextEdit(sied_scheduleTimeControls);

    newTextEdit->setObjectName(objName);

    newTextEdit->setMinimumHeight(newTextEdit->fontMetrics().height() * rowCount);

    newTextEdit->setMaximumHeight(newTextEdit->fontMetrics().height() * (rowCount + 2));

    newTextEdit->setMinimumWidth(boxWidthMin);

    newTextEdit->setMaximumWidth(boxWidthMax);

    newTextEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    return newTextEdit;
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
