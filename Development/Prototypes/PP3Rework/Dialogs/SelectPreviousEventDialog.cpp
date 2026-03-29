// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectPreviousEventDialog.h"
//#include "ScheduleItemModel.h"

// QT Header Files
#include <QAbstractButton>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QListView>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QVariant>
#include <QVBoxLayout>
#include <QVector>

// Standard C++ Header Files
#include <string>
#include <vector>

SelectPreviousEventDialog::SelectPreviousEventDialog(std::size_t userId, std::vector<std::string> previousEventTitles, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId}
{
    for (const std::string& str : previousEventTitles) {
        m_PreviousTitles.append(QString::fromStdString(str));
    }

    setupDialogUI();
}

SelectPreviousEventDialog::~SelectPreviousEventDialog()
{
}

void SelectPreviousEventDialog::setupDialogUI()
{
    resize(defaultDialogWidth, defaultDialogHeight);

    SelectPreviousEventDialogLayout = cqtfa_QTWidget<QVBoxLayout>("SelectPreviousEventDialogLayout", this);

    SelectPreviousEventDialogLayout->addWidget(setUpGroupBox());

    setLayout(SelectPreviousEventDialogLayout);

    adjustSize();
}

QGroupBox *SelectPreviousEventDialog::setUpGroupBox()
{
    selectPreviousEventGroupBox = new QGroupBox("Select Parent Task", this);
    selectPreviousEventGroupBox->setObjectName("selectPreviousEventGroupBox");
    selectPreviousEventGroupBox->setAlignment(Qt::AlignHCenter);

    selectPreviousEventGroupBoxLayout = cqtfa_FormLayoutWithPolicy("selectPreviousEventGroupBoxLayout", selectPreviousEventGroupBox);

    QStringListModel *model = new QStringListModel();
    model->setStringList(m_PreviousTitles);

    previousEventSelector = cqtfa_QTWidget<QListView>("previousEventSelector", this);
    previousEventSelector->setModel(model);

    selectPreviousEventGroupBoxLayout->addWidget(previousEventSelector);

    selectPreviousEventGroupBoxLayout->addWidget(setUpDialogButtons());
    buttonBox->setCenterButtons(true);

    return selectPreviousEventGroupBox;
}

QDialogButtonBox *SelectPreviousEventDialog::setUpDialogButtons()
{
    buttonBox = cqtfa_QTWidget<QDialogButtonBox>("buttonBox", this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&SelectPreviousEventDialog::handleSelectedEvent_accepted));
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    return buttonBox;
}

void SelectPreviousEventDialog::handleSelectedEvent_accepted()
{
    QModelIndex selectedEvent = previousEventSelector->currentIndex();
    m_SelectedEvent = selectedEvent.data(Qt::DisplayRole).toString();

    QDialog::accept();
}

