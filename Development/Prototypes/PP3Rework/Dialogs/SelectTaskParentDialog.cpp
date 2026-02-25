// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "SelectTaskParentDialog.h"
#include "GuiTaskModel.h"
//#include "GuiUserModel.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QAbstractTableModel>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QList>
#include <QObject>
#include <QString>
#include <QTableView>

// Standard C++ Header Files

SelectTaskParentDialog::SelectTaskParentDialog(QWidget *parent)
    : QDialog(parent),
    childTaskData{nullptr},
    parentTaskDBID{nullptr}
{
    setupDialogUI();
}

SelectTaskParentDialog::SelectTaskParentDialog(GuiTaskModel *orphanTask, QWidget *parent)
    : QDialog(parent),
    childTaskData{orphanTask},
    parentTaskDBID{nullptr}
{
    setupDialogUI();
}

SelectTaskParentDialog::~SelectTaskParentDialog()
{
}

void SelectTaskParentDialog::setupDialogUI()
{
    setObjectName("SelectTaskParentDialog");

    resize(defaultDialogWidth, defaultDialogHeight);

    selectTaskParentDialogLayout = cqtfa_QTWidget<QVBoxLayout>("selectTaskParentDialogLayout", this);

    selectTaskParentDialogLayout->addWidget(setUpGroupBox());

    setLayout(selectTaskParentDialogLayout);
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
    selectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    return selectParentTableView;
}

QAbstractTableModel *SelectTaskParentDialog::setUpParentTaskTable()
{
    return nullptr;
}
