// Project Header Files
#include "SelectTaskParentDialog.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableView>

// Standard C++ Header Files

SelectTaskParentDialog::SelectTaskParentDialog(QWidget *parent)
    : QDialog(parent),
    parentTaskDBID{nullptr}
{
    setupDialogUI();
}

SelectTaskParentDialog::~SelectTaskParentDialog()
{
}

void SelectTaskParentDialog::setupDialogUI()
{

    if (objectName().isEmpty())
        setObjectName("SelectTaskParentDialog");
    resize(defaultDialogWidth, defaultDialogHeight);
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName("buttonBox");
    buttonBox->setGeometry(QRect(240, 280, 201, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    selectParentgroupBox = new QGroupBox(this);
    selectParentgroupBox->setObjectName("selectParentgroupBox");
    selectParentgroupBox->setGeometry(QRect(parentTableMargin, parentTableMargin, 651, 231));
    selectParentgroupBox->setMinimumSize(QSize(minimumGroupBoxWidth, minimumGroupBoxHeight));
    selectParentgroupBox->setMaximumSize(QSize(maximumGroupBoxWidth, maximumGroupBoxHeight));
    selectParentgroupBox->setAlignment(Qt::AlignCenter);
    selectParentTableView = new QTableView(selectParentgroupBox);
    selectParentTableView->setObjectName("selectParentTableView");
    selectParentTableView->setGeometry(QRect(parentTableMargin, parentTableMargin, 631, 200));
    selectParentTableView->setMinimumSize(QSize(minimumTableViewWidth, minimumTableViewHeight));
    selectParentTableView->setMaximumSize(QSize(maximumTableViewWidth, maximumTableViewHeight));
    selectParentTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

    QObject::connect(buttonBox, &QDialogButtonBox::accepted, this, qOverload<>(&QDialog::accept));
    QObject::connect(buttonBox, &QDialogButtonBox::rejected, this, qOverload<>(&QDialog::reject));

}
