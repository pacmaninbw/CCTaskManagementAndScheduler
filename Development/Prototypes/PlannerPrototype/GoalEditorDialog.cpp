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
    goalID{goalId},
    maxGroupBoxHeight{0},
    maxButtonBoxHeight{0}
{
    setUpGoalEditorDialogUI();
}

GoalEditorDialog::~GoalEditorDialog()
{

}

void GoalEditorDialog::setUpGoalEditorDialogUI()
{
    QVBoxLayout* goalEditorDialogLayout = new QVBoxLayout(this);
    goalEditorDialogLayout->setObjectName("goalEditorDialogLayout");

    QString groupBoxTitle = goalID? "Edit Goal" : "Add Goal";

    editGoalGB = new QGroupBox(groupBoxTitle, this);
    editGoalGB->setLayout(setUpGoalEditorDialogGroupBoxContents());

    goalEditorDialogLayout->addWidget(editGoalGB);

    goalEditorDialogLayout->addWidget(setUpGoalEditorDialogButtonBox(), 0, Qt::AlignHCenter);

    setLayout(goalEditorDialogLayout);

    limitDialogRowth();

    adjustSize();

    groupBoxTitle += " Dialog";

    setWindowTitle(groupBoxTitle);
}

QFormLayout *GoalEditorDialog::setUpGoalEditorDialogGroupBoxContents()
{
    QFormLayout* goalEditorDialogFormLayout = cqtfa_FormLayoutWithPolicy(
        "goalEditorDialogFormLayout", editGoalGB);

    editGoalDescriptionTE = cqtfa_flixbleTextEditEbasedOnCharCount("editGoalDescriptionTE", editGoalGB,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    goalEditorDialogFormLayout->addRow("Goal Description:", editGoalDescriptionTE);

    editGoalPriorityLE = cqtfa_LineEditFixedWidthByCharCount("editGoalPriorityLE",
        editGoalGB, goalPriorityMaxChar);
    goalEditorDialogFormLayout->addRow("Priority:", editGoalPriorityLE);

    editGoalSelectParentGoalPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "editGoalSelectParentGoalPB", editGoalGB);
    goalEditorDialogFormLayout->addWidget(editGoalSelectParentGoalPB);

    maxGroupBoxHeight = 80 + editGoalDescriptionTE->height();
    maxGroupBoxHeight += editGoalPriorityLE->height();
    maxGroupBoxHeight += editGoalSelectParentGoalPB->height();

    return goalEditorDialogFormLayout;
}

QDialogButtonBox *GoalEditorDialog::setUpGoalEditorDialogButtonBox()
{
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName("buttonBox");
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    maxButtonBoxHeight = buttonBox->height() + marginAndSpacing;

    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    return buttonBox;
}

void GoalEditorDialog::limitDialogRowth()
{
    editGoalGB->setMaximumHeight(maxGroupBoxHeight);

    buttonBox->setMaximumHeight(maxButtonBoxHeight);

    int maxDialogWidth = editGoalDescriptionTE->maximumSize().width() + marginAndSpacing;
    int maxDialogHeight = maxGroupBoxHeight;
    maxDialogHeight += maxButtonBoxHeight;
    maxDialogHeight += marginAndSpacing;

    setMaximumWidth(maxDialogWidth);
    setMaximumHeight(maxDialogHeight);
}
