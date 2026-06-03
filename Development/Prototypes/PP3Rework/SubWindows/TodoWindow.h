#ifndef TODOWINDOW_H_
#define TODOWINDOW_H_

class DashboardTaskViewer;
class UserModel;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

class TodoWindow : public ModelSubWindow
{
    Q_OBJECT

public:
    TodoWindow(bool makeSubWindow = false, QWidget *parent = nullptr);
    TodoWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddTodoItem();
    void handleTodoTableClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    DashboardTaskViewer* updateTodoList();

    DashboardTaskViewer* todoTableView = nullptr;
};

#endif // TODOWINDOW_H_
