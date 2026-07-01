// Project Header Files
#include "commonQTWidgetsForApp.h"
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

GoalEditorDialog::GoalEditorDialog(std::size_t userId, std::size_t goalId, QWidget *parent)
    : BaseObjectEditorDialog("Goal", userId, goalId, parent),
    m_ParentGoalData{nullptr}
{
    setUpEditorUI();
}

GoalEditorDialog::~GoalEditorDialog()
{
}

void GoalEditorDialog::initEditorFieldsFromDataBase()
{
    if (m_DBModelID)
    {
        GoalQueryProcessor goalQueryProcessor;
        UserGoalModel_shp goalData = goalQueryProcessor.getGoalById(m_DBModelID);

        m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);

        std::size_t parentGoalId = goalData->getParentId();
        if (parentGoalId)
        {
            m_ParentGoalData = goalQueryProcessor.getGoalById(parentGoalId);
        }

        transferDBModelDataToEditorFields();
    }
}

QGroupBox *GoalEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* mainEditorGroupBox = new QGroupBox(m_EditorTitleString, this);
    m_qt_EditorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_qt_EditorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_Description = cqtfa_flexiblePlainTextEditEbasedOnCharCount("m_qt_Description", mainEditorGroupBox,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    m_qt_EditorFormLayout->addRow("Goal Description:", m_qt_Description);

    m_qt_Priority = cqtfa_LineEditFixedWidthByCharCount("m_qt_Priority",
        mainEditorGroupBox, goalPriorityMaxChar);
    m_qt_EditorFormLayout->addRow("Priority:", m_qt_Priority);

    m_qt_SelectParentGoal = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "m_qt_SelectParentGoal", mainEditorGroupBox);
    m_qt_EditorFormLayout->addWidget(m_qt_SelectParentGoal);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_qt_EditorFormLayout);

    mainEditorGroupBox->setLayout(m_qt_EditorFormLayout);

    return mainEditorGroupBox;
}

void GoalEditorDialog::transferEditorValuesToDBModel()
{
    std::shared_ptr<UserGoalModel> goalData = std::dynamic_pointer_cast<UserGoalModel>(m_DBObjectModel);
    goalData->setDescription(m_qt_Description->toPlainText().toStdString());
    goalData->setPriority(m_qt_Priority->text().toUInt());
    if (m_ParentGoalData)
    {
        goalData->setParentID(m_ParentGoalData->getGoalId());
    }
}

void GoalEditorDialog::transferDBModelDataToEditorFields()
{
    if (m_DBObjectModel)
    {
        std::shared_ptr<UserGoalModel> goalData = std::dynamic_pointer_cast<UserGoalModel>(m_DBObjectModel);

        m_qt_Description->setPlainText(QString::fromStdString(goalData->getDescription()));
        m_qt_Priority->setText(QString::number(goalData->getPriority()));
    }
}

void GoalEditorDialog::createSharedPtrDBModelForAddObject()
{
    UserGoalModel_shp goalData = std::make_shared<UserGoalModel>();
    m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);
}
