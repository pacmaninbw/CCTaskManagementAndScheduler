// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "GoalEditorDialog.h"
#include "UserGoalModel.h"
#include "GoalQueryProcessor.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDebug>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>


// Standard C++ Header Files
//#include <iostream>
#include <memory>

GoalEditorDialog::GoalEditorDialog(std::size_t userId, std::size_t goalId, QWidget *parent)
    : QDialog(parent),
    m_UserID{userId},
    m_DBModelID{goalId},
    m_GoalData{nullptr},
    m_ParentGoalData{nullptr},
    maxGroupBoxHeight{0},
    maxButtonBoxHeight{0}
{    
    setUpGoalEditorDialogUI();
}

bool GoalEditorDialog::getGoalFromDbInitFields()
{
    if (!m_DBModelID)
    {
        QString errorReport = "Goal Edit failed.\n";
        errorReport += " No Goal to Edit";
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
        return false;
    }

    GoalQueryProcessor goalQueryProcessor;
    m_GoalData = goalQueryProcessor.getGoalById(m_DBModelID);
    std::size_t parentGoalId = m_GoalData->getParentId();
    if (parentGoalId)
    {
        m_ParentGoalData = goalQueryProcessor.getGoalById(parentGoalId);
    }

    initEditorFieldsFromModel();

    return true;
}

GoalEditorDialog::~GoalEditorDialog()
{

}

void GoalEditorDialog::accept()
{
    bool updateSuccessful = true;
    if (!m_GoalData)
    {
        m_GoalData = std::make_shared<UserGoalModel>();
        m_GoalData->setUserId(m_UserID);
        transferEditedDataToModel();
        updateSuccessful = m_GoalData->insert();
    }
    else
    {
        transferEditedDataToModel();
        updateSuccessful = m_GoalData->update();
    }

    if (updateSuccessful)
    {
        QDialog::accept();
    }
    else
    {
        QString errorReport = "Goal edit failed.\n";
        errorReport += QString::fromStdString(m_GoalData->getAllErrorMessages());
        QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
    }
}

void GoalEditorDialog::handleDeleteButton()
{
    m_GoalData->hide(m_UserID);
}

void GoalEditorDialog::setUpGoalEditorDialogUI()
{
    QVBoxLayout* goalEditorDialogLayout = new QVBoxLayout(this);
    goalEditorDialogLayout->setObjectName("goalEditorDialogLayout");

    QString groupBoxTitle = m_DBModelID? "Edit Goal" : "Add Goal";

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
    goalEditorDialogFormLayout = cqtfa_FormLayoutWithPolicy("goalEditorDialogFormLayout", editGoalGB);

    editGoalDescriptionTE = cqtfa_flexiblePlainTextEditEbasedOnCharCount("editGoalDescriptionTE", editGoalGB,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    goalEditorDialogFormLayout->addRow("Goal Description:", editGoalDescriptionTE);

    editGoalPriorityLE = cqtfa_LineEditFixedWidthByCharCount("editGoalPriorityLE",
        editGoalGB, goalPriorityMaxChar);
    goalEditorDialogFormLayout->addRow("Priority:", editGoalPriorityLE);

    editGoalSelectParentGoalPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "editGoalSelectParentGoalPB", editGoalGB);
    goalEditorDialogFormLayout->addWidget(editGoalSelectParentGoalPB);

    if (m_DBModelID)
    {
        deleteButton = new DeleteItemButton("Goal");
        goalEditorDialogFormLayout->addWidget(deleteButton);
        connect(deleteButton, &QPushButton::clicked, this, &GoalEditorDialog::handleDeleteButton);
    }

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(goalEditorDialogFormLayout);

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

    int maxDialogWidth = cqtfa_getFormLayoutMaxWidth(goalEditorDialogFormLayout) + marginAndSpacing;

    setMaximumWidth(maxDialogWidth);
    setMaximumHeight(maxGroupBoxHeight + maxButtonBoxHeight + marginAndSpacing);
}

void GoalEditorDialog::transferEditedDataToModel()
{
    m_GoalData->setDescription(editGoalDescriptionTE->toPlainText().toStdString());
    m_GoalData->setPriority(editGoalPriorityLE->text().toUInt());
    if (m_ParentGoalData)
    {
        m_GoalData->setParentID(m_ParentGoalData->getGoalId());
    }
}

void GoalEditorDialog::initEditorFieldsFromModel()
{
    if (!m_GoalData)
    {
        return;
    }

    editGoalDescriptionTE->setPlainText(QString::fromStdString(m_GoalData->getDescription()));
    editGoalPriorityLE->setText(QString::number(m_GoalData->getPriority()));
}
