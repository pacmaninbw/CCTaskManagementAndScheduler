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
    m_parentTaskId{0},
    m_creatorId{taskCreatorId}
{
}

SelectTaskParentDialog::~SelectTaskParentDialog()
{
}

std::size_t SelectTaskParentDialog::getParentTask()
{
    return m_parentTaskId;
}

void SelectTaskParentDialog::setupDialogUI()
{
    resize(DefaultDialogWidth, DefaultDialogHeight);

    m_qt_selectParentDialogLayout = cqtfa_QTWidget<QVBoxLayout>("m_qt_selectParentDialogLayout", this);

    m_qt_selectParentDialogLayout->addWidget(setUpGroupBox());

    setLayout(m_qt_selectParentDialogLayout);
}

void SelectTaskParentDialog::handleParentTaskTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    m_parentTaskId = index.internalId();
}

QGroupBox *SelectTaskParentDialog::setUpGroupBox()
{
    m_qt_selectParentGB = new QGroupBox("Select Parent Task", this);
    m_qt_selectParentGB->setObjectName("m_qt_selectParentGB");
    m_qt_selectParentGB->setAlignment(Qt::AlignHCenter);

    m_qt_selectParentGBLayout = cqtfa_FormLayoutWithPolicy("m_qt_selectParentGBLayout", m_qt_selectParentGB);

    m_qt_selectParentGBLayout->addWidget(setUpParentTaskView());

    m_qt_selectParentGBLayout->addWidget(setUpDialogButtons());
    m_qt_buttonBox->setCenterButtons(true);

    return m_qt_selectParentGB;
}

QDialogButtonBox *SelectTaskParentDialog::setUpDialogButtons()
{
    m_qt_buttonBox = cqtfa_QTWidget<QDialogButtonBox>("m_qt_buttonBox", this);
    m_qt_buttonBox->setOrientation(Qt::Horizontal);
    m_qt_buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QObject::connect(m_qt_buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    QObject::connect(m_qt_buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

    return m_qt_buttonBox;
}

QTableView *SelectTaskParentDialog::setUpParentTaskView()
{
    m_qt_selectParentTableView = cqtfa_QTWidget<QTableView>("m_qt_selectParentTableView", this);
    m_qt_selectParentTableView->setModel(setUpParentTaskTable());
    m_qt_selectParentTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_qt_selectParentTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    m_qt_selectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(m_qt_selectParentTableView,  &QTableView::clicked, this, &SelectTaskParentDialog::handleParentTaskTableClicked);

    return m_qt_selectParentTableView;
}

QAbstractTableModel *SelectTaskParentDialog::setUpParentTaskTable()
{
    SelectParentTaskTable* parentTable = new SelectParentTaskTable(m_creatorId, this);
    parentTable->fillTable();
    return parentTable;
}
