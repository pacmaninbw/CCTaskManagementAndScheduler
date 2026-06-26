// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "GoalEditorDialog.h"
#include "UserGoalModel.h"
#include "GoalQueryProcessor.h"

// QT Header Files
#include <QVariant>
#include <QDialog>
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
    m_Qt_EditorFormLayout = new QFormLayout(mainEditorGroupBox);
    m_Qt_EditorFormLayout->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);

    editGoalDescriptionTE = cqtfa_flexiblePlainTextEditEbasedOnCharCount("editGoalDescriptionTE", mainEditorGroupBox,
        goalDescriptionMinCharWidth, goalDescriptionMaxCharWidth, goalDescriptionLineCount);
    m_Qt_EditorFormLayout->addRow("Goal Description:", editGoalDescriptionTE);

    editGoalPriorityLE = cqtfa_LineEditFixedWidthByCharCount("editGoalPriorityLE",
        mainEditorGroupBox, goalPriorityMaxChar);
    m_Qt_EditorFormLayout->addRow("Priority:", editGoalPriorityLE);

    editGoalSelectParentGoalPB = cqtfa_QTWidgetWithText<QPushButton>(
        "Select Parent Goal", "editGoalSelectParentGoalPB", mainEditorGroupBox);
    m_Qt_EditorFormLayout->addWidget(editGoalSelectParentGoalPB);

    maxGroupBoxHeight = cqtfa_calculateFormLayoutMaxHeight(m_Qt_EditorFormLayout);

    mainEditorGroupBox->setLayout(m_Qt_EditorFormLayout);

    return mainEditorGroupBox;
}

void GoalEditorDialog::transferEditorValuesToDBModel()
{
    std::shared_ptr<UserGoalModel> goalData = std::dynamic_pointer_cast<UserGoalModel>(m_DBObjectModel);
    goalData->setDescription(editGoalDescriptionTE->toPlainText().toStdString());
    goalData->setPriority(editGoalPriorityLE->text().toUInt());
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

        editGoalDescriptionTE->setPlainText(QString::fromStdString(goalData->getDescription()));
        editGoalPriorityLE->setText(QString::number(goalData->getPriority()));
    }
}

void GoalEditorDialog::createSharedPtrDBModelForAddObject()
{
    UserGoalModel_shp goalData = std::make_shared<UserGoalModel>();
    m_DBObjectModel = std::dynamic_pointer_cast<ModelDBInterface>(goalData);
}
