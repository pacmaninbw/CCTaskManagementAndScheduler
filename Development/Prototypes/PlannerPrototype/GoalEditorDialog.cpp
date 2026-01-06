// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "GoalEditorDialog.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QVBoxLayout>


// Standard C++ Header Files


GoalEditorDialog::GoalEditorDialog(QWidget *parent, std::size_t userId, std::size_t goalId)
    : QDialog(parent),
    userID{userId},
    goalID{goalId}
{
    setUpGoalEditorDialogUI();
}

GoalEditorDialog::~GoalEditorDialog()
{

}

void GoalEditorDialog::setUpGoalEditorDialogUI()
{
    resize(goalEditorDialogWidth, goalEditorDialogHeight);

    QVBoxLayout* goalEditorDialogLayout = new QVBoxLayout(this);
    goalEditorDialogLayout->setObjectName("goalEditorDialogLayout");

    QString groupBoxTitle = goalID? "Edit Goal" : "Add Goal";

    editGoalGB = new QGroupBox(groupBoxTitle, this);
    editGoalGB->setLayout(setUpGoalEditorDialogGroupBoxContents());

    goalEditorDialogLayout->addWidget(editGoalGB);

    goalEditorDialogLayout->addWidget(setUpGoalEditorDialogButtonBox());

    setLayout(goalEditorDialogLayout);

    adjustSize();

    groupBoxTitle += " Dialog";

    setWindowTitle(groupBoxTitle);
}

QFormLayout *GoalEditorDialog::setUpGoalEditorDialogGroupBoxContents()
{
    QFormLayout* goalEditorDialogFormLayout = cqtfa_FormLayoutWithPolicy(
        "goalEditorDialogFormLayout", editGoalGB);

    editGoalDescriptionTE = cqtfa_flixbleTextEditEbasedOnCharCount("", editGoalGB, 40, 80, 3);
    goalEditorDialogFormLayout->addRow("Goal Description:", editGoalDescriptionTE);

    editGoalPriorityLE = cqtfa_LineEditFixedWidthByCharCount("editGoalPriorityLE", editGoalGB, 5);
    goalEditorDialogFormLayout->addRow("Priority:", editGoalPriorityLE);

    editGoalSelectParentGoalPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "editGoalSelectParentGoalPB", editGoalGB);
    editGoalSelectParentGoalPB->setGeometry(QRect(30, 270, 200, 26));

    goalEditorDialogFormLayout->addWidget(editGoalSelectParentGoalPB);

    return goalEditorDialogFormLayout;
}

QDialogButtonBox *GoalEditorDialog::setUpGoalEditorDialogButtonBox()
{
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName("buttonBox");
    buttonBox->setGeometry(QRect(30, 350, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}
