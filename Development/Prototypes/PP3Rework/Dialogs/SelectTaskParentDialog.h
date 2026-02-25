#ifndef SELECTTASKPARENTDIALOG_H
#define SELECTTASKPARENTDIALOG_H

// Project Header Files
#include "GuiTaskModel.h"

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QAbstractTableModel>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QTableView>
#include <QVBoxLayout>

// Standard C++ Header Files

class SelectTaskParentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTaskParentDialog(QWidget *parent = nullptr);
    explicit SelectTaskParentDialog(GuiTaskModel* orphanTask, QWidget* parent = nullptr);
    ~SelectTaskParentDialog();
    GuiTaskModel* getParentTaskID() { return parentTaskDBID; };

private:
    void setupDialogUI();
    QGroupBox* setUpGroupBox();
    QDialogButtonBox* setUpDialogButtons();
    QTableView* setUpParentTaskView();
    QAbstractTableModel* setUpParentTaskTable();
    QVBoxLayout* selectTaskParentDialogLayout;
    QDialogButtonBox* buttonBox;
    QGroupBox* selectParentgroupBox;
    QFormLayout* selectParentGroupBoxLayout;
    QTableView* selectParentTableView;
    GuiTaskModel* childTaskData;
    GuiTaskModel* parentTaskDBID;

    const int defaultDialogWidth = 700;
    const int defaultDialogHeight = 450;
    const int minimumGroupBoxWidth = 320;
    const int minimumGroupBoxHeight = 220;
    const int maximumGroupBoxWidth = 920;
    const int maximumGroupBoxHeight = 520;
    const int parentTableMargin = 10;
    const int marginAllowance = parentTableMargin  * 2;
    const int minimumTableViewWidth = minimumGroupBoxWidth - marginAllowance;
    const int minimumTableViewHeight = minimumGroupBoxHeight - marginAllowance;
    const int maximumTableViewWidth = maximumGroupBoxWidth - marginAllowance;
    const int maximumTableViewHeight = maximumGroupBoxHeight - marginAllowance;
};

#endif // SELECTTASKPARENTDIALOG_H
