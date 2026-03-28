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
    explicit GoalEditorDialog(std::size_t userId, std::size_t goalId, QWidget* parent = nullptr);
    ~GoalEditorDialog();

private:
    void setUpGoalEditorDialogUI();
    QFormLayout* setUpGoalEditorDialogGroupBoxContents();
    QDialogButtonBox* setUpGoalEditorDialogButtonBox();
    void limitDialogRowth();

    std::size_t m_UserID;
    std::shared_ptr<UserGoalModel> m_GoalData;
    QDialogButtonBox* buttonBox;
    QGroupBox* editGoalGB;
    QFormLayout* goalEditorDialogFormLayout;
    QPlainTextEdit* editGoalDescriptionTE;
    QLineEdit* editGoalPriorityLE;
    QPushButton* editGoalSelectParentGoalPB;
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
