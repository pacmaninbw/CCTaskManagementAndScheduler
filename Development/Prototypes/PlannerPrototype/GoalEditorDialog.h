#ifndef GOALEDITORDIALOG_H
#define GOALEDITORDIALOG_H

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
#include <QPushButton>
#include <QTextEdit>

// Standard C++ Header Files

class GoalEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GoalEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t goalId=0);
    ~GoalEditorDialog();

public slots:

signals:

private slots:

private:
    void setUpGoalEditorDialogUI();
    QFormLayout* setUpGoalEditorDialogGroupBoxContents();
    QDialogButtonBox* setUpGoalEditorDialogButtonBox();
    void limitDialogRowth();

    std::size_t userID;
    std::size_t goalID;
    QDialogButtonBox* buttonBox;
    QGroupBox* editGoalGB;
    QTextEdit* editGoalDescriptionTE;
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
