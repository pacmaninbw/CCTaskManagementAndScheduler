#ifndef GOALEDITORDIALOG_H
#define GOALEDITORDIALOG_H

class UserGoalModel;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

// Standard C++ Header Files
#include <memory>

class GoalEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoalEditorDialog(std::size_t userId, QWidget* parent = nullptr);
    bool getGoalFromDbInitFields(std::size_t goalId);
    ~GoalEditorDialog();

public Q_SLOTS:
    void accept() override;

private:
    void setUpGoalEditorDialogUI();
    QFormLayout* setUpGoalEditorDialogGroupBoxContents();
    QDialogButtonBox* setUpGoalEditorDialogButtonBox();
    void limitDialogRowth();
    void transferEditedDataToModel();
    void initEditorFieldsFromModel();

    std::size_t m_UserID;
    std::shared_ptr<UserGoalModel> m_GoalData;
    std::shared_ptr<UserGoalModel> m_ParentGoalData;
    QDialogButtonBox* buttonBox = nullptr;
    QGroupBox* editGoalGB = nullptr;
    QFormLayout* goalEditorDialogFormLayout = nullptr;
    QPlainTextEdit* editGoalDescriptionTE = nullptr;
    QLineEdit* editGoalPriorityLE = nullptr;
    QPushButton* editGoalSelectParentGoalPB = nullptr;
    int maxGroupBoxHeight;
    int maxButtonBoxHeight;

    const int goalDescriptionMinCharWidth = 40;
    const int goalDescriptionMaxCharWidth = 80;
    const int goalDescriptionLineCount = 3;
    const int goalPriorityMaxChar = 5;
    const int marginAndSpacing = 30;
//    const int goalDialogMaxWidth = goalDescriptionMaxCharWidth + marginAndSpacing;
};

#endif // GOALEDITORDIALOG_H
