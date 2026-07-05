#ifndef TODOWINDOW_H_
#define TODOWINDOW_H_

/*
 * Provide a window where the user can view and edit their To Do items.
 */
class GuiDashboardTaskTable;
class UserModel;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QString>
#include <QTableView>

// Standard C++ Header Files
#include <memory>

class TodoWindow : public ModelSubWindow
{
public:
    TodoWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddTodoItem();
    void handleTodoTableClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    void tableViewReset(QTableView* tableView);

    GuiDashboardTaskTable* m_todoTable = nullptr;
};

#endif // TODOWINDOW_H_
