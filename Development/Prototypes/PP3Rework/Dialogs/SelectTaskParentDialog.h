#ifndef SELECTTASKPARENTDIALOG_H
#define SELECTTASKPARENTDIALOG_H

class TaskModel;

// Project Header Files

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
#include <QObject>
#include <QTableView>
#include <QVBoxLayout>

// Standard C++ Header Files

class SelectTaskParentDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectTaskParentDialog(std::size_t taskCreatorId, QWidget* parent = nullptr);
    ~SelectTaskParentDialog();
    std::size_t getParentTask();
    void setupDialogUI();

private Q_SLOTS:
    void handleParentTaskTableClicked(const QModelIndex &index);

private:
    QGroupBox* setUpGroupBox();
    QDialogButtonBox* setUpDialogButtons();
    QTableView* setUpParentTaskView();
    QAbstractTableModel* setUpParentTaskTable();
    QVBoxLayout* selectTaskParentDialogLayout = nullptr;
    QDialogButtonBox* buttonBox = nullptr;
    QGroupBox* selectParentgroupBox = nullptr;
    QFormLayout* selectParentGroupBoxLayout = nullptr;
    QTableView* selectParentTableView = nullptr;
    std::size_t m_ParentTaskID;
    std::size_t m_CreatorID;

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
