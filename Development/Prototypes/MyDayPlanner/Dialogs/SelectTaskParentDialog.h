#ifndef SELECTTASKPARENTDIALOG_H_
#define SELECTTASKPARENTDIALOG_H_

class TaskModel;

// Project Header Files

// QT Header Files
#include <QAbstractTableModel>
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
    explicit SelectTaskParentDialog(std::size_t taskCreatorId, QWidget* parent = nullptr);
    ~SelectTaskParentDialog();
    std::size_t getParentTask();
    void setupDialogUI();

private Q_SLOTS:
    void handleParentTaskTableClicked(const QModelIndex &index);

private:
    const int DefaultDialogWidth = 700;
    const int DefaultDialogHeight = 450;
    const int MinimumGroupBoxWidth = 320;
    const int MinimumGroupBoxHeight = 220;
    const int MaximumGroupBoxWidth = 920;
    const int MaximumGroupBoxHeight = 520;
    const int ParentTableMargin = 10;
    const int MarginAllowance = ParentTableMargin  * 2;
    const int MinimumTableViewWidth = MinimumGroupBoxWidth - MarginAllowance;
    const int MinimumTableViewHeight = MinimumGroupBoxHeight - MarginAllowance;
    const int MaximumTableViewWidth = MaximumGroupBoxWidth - MarginAllowance;
    const int MaximumTableViewHeight = MaximumGroupBoxHeight - MarginAllowance;

    QGroupBox* setUpGroupBox();
    QDialogButtonBox* setUpDialogButtons();
    QTableView* setUpParentTaskView();
    QAbstractTableModel* setUpParentTaskTable();

    QVBoxLayout* m_qt_selectParentDialogLayout = nullptr;
    QDialogButtonBox* m_qt_buttonBox = nullptr;
    QGroupBox* m_qt_selectParentGB = nullptr;
    QFormLayout* m_qt_selectParentGBLayout = nullptr;
    QTableView* m_qt_selectParentTableView = nullptr;

    std::size_t m_parentTaskId;
    std::size_t m_creatorId;
};

#endif // SELECTTASKPARENTDIALOG_H_
