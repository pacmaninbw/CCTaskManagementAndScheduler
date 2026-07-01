// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "SelectParentTaskTable.h"

// QT Header Files
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QMessageBox>
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

    m_qt_SelectParentDialogLayout = cqtfa_QTWidget<QVBoxLayout>("m_qt_SelectParentDialogLayout", this);

    m_qt_SelectParentDialogLayout->addWidget(setUpGroupBox());

    setLayout(m_qt_SelectParentDialogLayout);
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
    m_qt_SelectParentGB = new QGroupBox("Select Parent Task", this);
    m_qt_SelectParentGB->setObjectName("m_qt_SelectParentGB");
    m_qt_SelectParentGB->setAlignment(Qt::AlignHCenter);

    m_qt_SelectParentGBLayout = cqtfa_FormLayoutWithPolicy("m_qt_SelectParentGBLayout", m_qt_SelectParentGB);

    m_qt_SelectParentGBLayout->addWidget(setUpParentTaskView());

    m_qt_SelectParentGBLayout->addWidget(setUpDialogButtons());
    m_qt_ButtonBox->setCenterButtons(true);

    return m_qt_SelectParentGB;
}

QDialogButtonBox *SelectTaskParentDialog::setUpDialogButtons()
{
    m_qt_ButtonBox = cqtfa_QTWidget<QDialogButtonBox>("m_qt_ButtonBox", this);
    m_qt_ButtonBox->setOrientation(Qt::Horizontal);
    m_qt_ButtonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(m_qt_ButtonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    QObject::connect(m_qt_ButtonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    return m_qt_ButtonBox;
}

QTableView *SelectTaskParentDialog::setUpParentTaskView()
{
    m_qt_SelectParentTableView = cqtfa_QTWidget<QTableView>("m_qt_SelectParentTableView", this);
    m_qt_SelectParentTableView->setModel(setUpParentTaskTable());
    m_qt_SelectParentTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_qt_SelectParentTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_qt_SelectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(m_qt_SelectParentTableView,  &QTableView::clicked, this, &SelectTaskParentDialog::handleParentTaskTableClicked);

    return m_qt_SelectParentTableView;
}

QAbstractTableModel *SelectTaskParentDialog::setUpParentTaskTable()
{
    SelectParentTaskTable* parentTable = new SelectParentTaskTable(m_CreatorID, this);
    parentTable->fillTable();
    return parentTable;
}
