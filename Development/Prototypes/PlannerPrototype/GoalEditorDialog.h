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
    explicit GoalEditorDialog(QWidget* parent = nullptr, std::size_t goalId=0);
    ~GoalEditorDialog();

public slots:

signals:

private slots:

private:
    void setUpGoalEditorDialogUI();
    QFormLayout* setUpGoalEditorDialogGroupBoxContents();
    QTextEdit* setUpGoalDescriptionTextEdit();
    QDialogButtonBox* setUpGoalEditorDialogButtonBox();

    std::size_t goalID;
    QDialogButtonBox* buttonBox;
    QGroupBox* editGoalGB;
    QTextEdit* editGoalDescriptionTE;
    QLineEdit* editGoalPriorityLE;
    QPushButton* editGoalSelectParentGoalPB;

    const int goalEditorDialogWidth = 691;
    const int goalEditorDialogHeight = 400;
};

#endif // GOALEDITORDIALOG_H
