#ifndef GOALWINDOW_H
#define GOALWINDOW_H

class DefaultGoalDisplayTable;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

class GoalWindow : public ModelSubWindow
{
public:
    explicit GoalWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddGoalAction();
    void handleEditGoalAction(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    void tableViewReset(QTableView* tableView);

    DefaultGoalDisplayTable* m_GoalTable = nullptr;
};

#endif // GOALWINDOW_H
