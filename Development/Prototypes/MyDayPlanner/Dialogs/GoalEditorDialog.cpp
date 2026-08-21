// Project Header Files
#include "CommonWidgetExtensions.h"
#include "GoalEditorDialog.h"
#include "UserGoalModel.h"
#include "GoalQueryProcessor.h"

// QT Header Files
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <memory>

GoalEditorDialog::GoalEditorDialog(std::size_t userId, std::shared_ptr<UserGoalModel> goalToEdit, QWidget *parent)
: BaseObjectEditorDialog("Goal", userId, goalToEdit, parent),
  m_parentGoalData{nullptr}
{
    setUpEditorUI();
    transferDBModelDataToEditorFields();
}

GoalEditorDialog::~GoalEditorDialog()
{
}

QGroupBox *GoalEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* mainEditorGroupBox = new QGroupBox(m_editorTitleString, this);
    m_qt_editorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_qt_editorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_description = common::flexPlainTextBasedOnCharCount("m_qt_description", mainEditorGroupBox,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    m_qt_editorFormLayout->addRow("Goal Description:", m_qt_description);

    m_qt_priority = common::lineEditWidthByCharCount("m_qt_priority",
        mainEditorGroupBox, goalPriorityMaxChar);
    m_qt_editorFormLayout->addRow("Priority:", m_qt_priority);

    m_qt_selectParentGoal = common::namedWidgetWithText<QPushButton>(
        "Select Parent Goal", "m_qt_selectParentGoal", mainEditorGroupBox);
    m_qt_editorFormLayout->addWidget(m_qt_selectParentGoal);

    m_maxGroupBoxHeight = common::getMaxHeight(m_qt_editorFormLayout);

    mainEditorGroupBox->setLayout(m_qt_editorFormLayout);

    return mainEditorGroupBox;
}

void GoalEditorDialog::transferEditorValuesToDBModel()
{
    std::shared_ptr<UserGoalModel> goalData = std::dynamic_pointer_cast<UserGoalModel>(m_dbObjectModel);
    goalData->setDescription(m_qt_description->toPlainText().toStdString());
    goalData->setPriority(m_qt_priority->text().toUInt());
    if (m_parentGoalData)
    {
        goalData->setParentID(m_parentGoalData->getGoalId());
    }
}

void GoalEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_dbObjectModel)
    {
        std::shared_ptr<UserGoalModel> goalData = std::dynamic_pointer_cast<UserGoalModel>(m_dbObjectModel);

        m_qt_description->setPlainText(QString::fromStdString(goalData->getDescription()));
        m_qt_priority->setText(QString::number(goalData->getPriority()));
    }
}

void GoalEditorDialog::createSharedPtrDBModelForAddObject()
{
    UserGoalModel_shp goalData = std::make_shared<UserGoalModel>();
    goalData->setUserId(m_userID);
    m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);
}
