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
    m_parentGoalData{nullptr}
{
    setUpEditorUI();
}

GoalEditorDialog::~GoalEditorDialog()
{
}

void GoalEditorDialog::initEditorFieldsFromDataBase()
{
    if (m_dbModelId)
    {
        GoalQueryProcessor goalQueryProcessor;
        UserGoalModel_shp goalData = goalQueryProcessor.getGoalById(m_dbModelId);

        m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);

        std::size_t parentGoalId = goalData->getParentId();
        if (parentGoalId)
        {
            m_parentGoalData = goalQueryProcessor.getGoalById(parentGoalId);
        }

        transferDBModelDataToEditorFields();
    }
}

QGroupBox *GoalEditorDialog::setUpEditorDialogForm()
{
    QGroupBox* mainEditorGroupBox = new QGroupBox(m_editorTitleString, this);
    m_qt_editorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_qt_editorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    m_qt_description = cqtfa_flexiblePlainTextEditEbasedOnCharCount("m_qt_description", mainEditorGroupBox,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    m_qt_editorFormLayout->addRow("Goal Description:", m_qt_description);

    m_qt_priority = cqtfa_LineEditFixedWidthByCharCount("m_qt_priority",
        mainEditorGroupBox, goalPriorityMaxChar);
    m_qt_editorFormLayout->addRow("Priority:", m_qt_priority);

    m_qt_selectParentGoal = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "m_qt_selectParentGoal", mainEditorGroupBox);
    m_qt_editorFormLayout->addWidget(m_qt_selectParentGoal);

    m_maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_qt_editorFormLayout);

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
    m_dbObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);
}
