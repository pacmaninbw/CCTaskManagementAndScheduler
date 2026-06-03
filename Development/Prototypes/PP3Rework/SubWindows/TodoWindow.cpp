// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "DashboardTaskViewer.h"
#include "TodoWindow.h"
#include "TaskEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files


TodoWindow::TodoWindow(bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Prioritized To Do List:", makeSubWindow, parent)
{
}

TodoWindow::TodoWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : TodoWindow(makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void TodoWindow::refresh()
{
    updateTodoList();
}

void TodoWindow::handleAddTodoItem()
{
    TaskEditorDialog addTaskDialog(this, m_UserData);

    addTaskDialog.exec();

    updateTodoList();

}

void TodoWindow::handleTodoTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t taskToEditId = index.internalId();
    TaskEditorDialog editTaskDialog(this);
    if (editTaskDialog.setTaskDataAndInitDisplayFields(taskToEditId))
    {
        editTaskDialog.exec();
        updateTodoList();
    }

}

void TodoWindow::setUpWindowContentAndActions()
{
    addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a ToDo Item", "addModelObject", this);
    connect(addModelObject, &QPushButton::clicked, this, &TodoWindow::handleAddTodoItem);

    modelWindowLayout->addWidget(addModelObject);

    modelWindowLayout->addWidget(updateTodoList());

    if (!m_IsSubWindow)
    {
        closeModelWindow = cqtfa_QTWidgetWithText<QPushButton>("Close Todo Window", "closeModelWindow", this);
        modelWindowLayout->addWidget(closeModelWindow);
    }
}

DashboardTaskViewer *TodoWindow::updateTodoList()
{
    if (!todoTableView)
    {
        todoTableView = new DashboardTaskViewer(centralwidget);
        todoTableView->setObjectName("udTaskTableView");
        if (m_UserData)
        {
            todoTableView->setUserId(m_UserData->getUserID());
        }
        connect(todoTableView, &QTableView::clicked, this, &TodoWindow::handleTodoTableClicked);
        connect(todoTableView, &QTableView::doubleClicked, this, &TodoWindow::handleTodoTableClicked);
    }
    else
    {
        if (m_UserData)
        {
            todoTableView->setUserId(m_UserData->getUserID());
        }
        todoTableView->clearSelection();
        todoTableView->update();
    }

    todoTableView->clearFocus();

    return todoTableView;
}
