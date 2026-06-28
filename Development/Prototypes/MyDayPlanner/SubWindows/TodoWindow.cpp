// Project Header Files
#include "commonQTWidgetsForApp.h"
#include "GuiDashboardTaskTable.h"
#include "TodoWindow.h"
#include "TaskEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files


TodoWindow::TodoWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Prioritized To Do List:", makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void TodoWindow::refresh()
{
    tableViewReset(m_qt_ModelTableView);
}

void TodoWindow::handleAddTodoItem()
{
    TaskEditorDialog addTaskDialog(this, m_UserData);

    addTaskDialog.exec();

    tableViewReset(m_qt_ModelTableView);
}

void TodoWindow::handleTodoTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t taskToEditId = index.internalId();
    TaskEditorDialog editTaskDialog(this, nullptr, taskToEditId);
    editTaskDialog.initEditorFieldsFromDataBase();
    editTaskDialog.exec();
    tableViewReset(m_qt_ModelTableView);
}

void TodoWindow::setUpWindowContentAndActions()
{
    m_qt_AddModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a ToDo Item", "m_qt_AddModelObject", this);
    connect(m_qt_AddModelObject, &QPushButton::clicked, this, &TodoWindow::handleAddTodoItem);

    m_qt_ModelWindowLayout->addWidget(m_qt_AddModelObject);

    m_qt_ModelTableView = new QTableView(this);
    m_qt_ModelTableView->setObjectName("m_qt_ModelTableView");
    tableViewReset(m_qt_ModelTableView);

    m_qt_ModelWindowLayout->addWidget(m_qt_ModelTableView);

    connect(m_qt_ModelTableView, &QTableView::clicked, this, &TodoWindow::handleTodoTableClicked);
    connect(m_qt_ModelTableView, &QTableView::doubleClicked, this, &TodoWindow::handleTodoTableClicked);
}


void TodoWindow::tableViewReset(QTableView *tableView)
{
    if (m_TodoTable)
    {
        delete m_TodoTable;
    }
 
    m_TodoTable = new GuiDashboardTaskTable(m_UserData->getUserID(), parent());
    m_TodoTable->setObjectName("m_TodoTable");
    m_TodoTable->fillTable();

    tableView->setModel(m_TodoTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableView->clearSelection();
    tableView->clearFocus();
    tableView->update();
}
