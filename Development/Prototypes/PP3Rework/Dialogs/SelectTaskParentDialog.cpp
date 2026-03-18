// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "SelectParentTaskTable.h"
#include "TaskModel.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QList>
#include <QMessageBox>
#include <QObject>
#include <QString>
#include <QTableView>

// Standard C++ Header Files

SelectTaskParentDialog::SelectTaskParentDialog(std::size_t taskCreatorId, QWidget *parent)
    : QDialog(parent),
    m_ParentTaskID{0},
    m_CreatorID{taskCreatorId}
{
}

SelectTaskParentDialog::~SelectTaskParentDialog()
{
}

std::size_t SelectTaskParentDialog::getParentTask()
{
    return m_ParentTaskID;
}

void SelectTaskParentDialog::setupDialogUI()
{
    resize(defaultDialogWidth, defaultDialogHeight);

    selectTaskParentDialogLayout = cqtfa_QTWidget<QVBoxLayout>("selectTaskParentDialogLayout", this);

    selectTaskParentDialogLayout->addWidget(setUpGroupBox());

    setLayout(selectTaskParentDialogLayout);
}

void SelectTaskParentDialog::handleParentTaskTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    m_ParentTaskID = index.internalId();
}

QGroupBox *SelectTaskParentDialog::setUpGroupBox()
{
    selectParentgroupBox = new QGroupBox("Select Parent Task", this);
    selectParentgroupBox->setObjectName("selectParentgroupBox");
    selectParentgroupBox->setAlignment(Qt::AlignHCenter);

    selectParentGroupBoxLayout = cqtfa_FormLayoutWithPolicy("selectParentGroupBoxLayout", selectParentgroupBox);

    selectParentGroupBoxLayout->addWidget(setUpParentTaskView());

    selectParentGroupBoxLayout->addWidget(setUpDialogButtons());
    buttonBox->setCenterButtons(true);

    return selectParentgroupBox;
}

QDialogButtonBox *SelectTaskParentDialog::setUpDialogButtons()
{
    buttonBox = cqtfa_QTWidget<QDialogButtonBox>("buttonBox", this);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    return buttonBox;
}

QTableView *SelectTaskParentDialog::setUpParentTaskView()
{
    selectParentTableView = cqtfa_QTWidget<QTableView>("selectParentTableView", this);
    selectParentTableView->setModel(setUpParentTaskTable());
    selectParentTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    selectParentTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    selectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(selectParentTableView,  &QTableView::clicked, this, &SelectTaskParentDialog::handleParentTaskTableClicked);

    return selectParentTableView;
}

QAbstractTableModel *SelectTaskParentDialog::setUpParentTaskTable()
{
    SelectParentTaskTable* parentTable = new SelectParentTaskTable(m_CreatorID, this);
    parentTable->fillTable();
    return parentTable;
}
