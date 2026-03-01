// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "GuiTaskTableBase.h"
#include "GuiUserModel.h"

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

SelectTaskParentDialog::SelectTaskParentDialog(QWidget *parent)
    : QDialog(parent),
    childTaskData{nullptr},
    parentTaskModel{nullptr},
    creator{nullptr}
{
    setObjectName("SelectTaskParentDialog");

    setWindowTitle("Select Parent Task");
}

SelectTaskParentDialog::SelectTaskParentDialog(std::shared_ptr<GuiTaskModel> orphanTask, QWidget *parent)
    : SelectTaskParentDialog(parent)
{
    childTaskData = orphanTask;
}

SelectTaskParentDialog::~SelectTaskParentDialog()
{
}

std::shared_ptr<GuiTaskModel> SelectTaskParentDialog::getParentTaskID()
{
    return std::make_shared<GuiTaskModel>(parentTaskModel);
}

void SelectTaskParentDialog::setupDialogUI()
{
    if (!childTaskData)
    {
        QMessageBox::critical(nullptr, "Critical Error", "No Child Task Data, can't continue!", QMessageBox::Ok);
    }

    creator = getCreatorFromChildTask();

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

    parentTaskModel = static_cast<GuiTaskModel*>(index.internalPointer());
}

GuiUserModel *SelectTaskParentDialog::getCreatorFromChildTask()
{
    if (!childTaskData)
    {
        return nullptr;
    }

    std::size_t creatorUserId = childTaskData->getCreatorUserId();
    GuiUserModel* taskCreator = new GuiUserModel(creatorUserId, this);

    return taskCreator;
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
    selectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    connect(selectParentTableView,  &QTableView::clicked, this, &SelectTaskParentDialog::handleParentTaskTableClicked);

    return selectParentTableView;
}

QAbstractTableModel *SelectTaskParentDialog::setUpParentTaskTable()
{
    GuiTaskTableBase* parentTable = new GuiTaskTableBase(creator, this);
    parentTable->fillTable();
    return parentTable;
}
