// Project Header Files
#include "createNamedQTWidget.h"
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


GoalEditorDialog::GoalEditorDialog(QWidget *parent, std::size_t goalId)
    : QDialog(parent),
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

    groupBoxTitle += " Dialog";

    setWindowTitle(groupBoxTitle);
}

QFormLayout *GoalEditorDialog::setUpGoalEditorDialogGroupBoxContents()
{
    QFormLayout* goalEditorDialogFormLayout = createNamedFormLayoutWithPolicy(
        "goalEditorDialogFormLayout", editGoalGB);

    goalEditorDialogFormLayout->addRow("Goal Description:", setUpGoalDescriptionTextEdit());

    goalEditorDialogFormLayout->addRow("Priority:",
        createNamedLineEditWithWidthAndLength("editGoalPriorityLE", this, 113, 5));

    editGoalSelectParentGoalPB = createNameQTWidgetWithText<QPushButton>(
        "editGoalSelectParentGoalPB", "Select Parent Goal", this);
    editGoalSelectParentGoalPB->setGeometry(QRect(30, 270, 200, 26));

    goalEditorDialogFormLayout->addWidget(editGoalSelectParentGoalPB);

    return goalEditorDialogFormLayout;
}

QTextEdit *GoalEditorDialog::setUpGoalDescriptionTextEdit()
{
    editGoalDescriptionTE = new QTextEdit(editGoalGB);
    editGoalDescriptionTE->setObjectName("editGoalDescriptionTE");
    editGoalDescriptionTE->setGeometry(QRect(30, 55, 550, 150));

    return editGoalDescriptionTE;
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
