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
    tableViewReset(m_qt_modelTableView);
}

void TodoWindow::handleAddTodoItem()
{
    TaskEditorDialog addTaskDialog(this, m_userData);
    addTaskDialog.exec();
    tableViewReset(m_qt_modelTableView);
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
    tableViewReset(m_qt_modelTableView);
}

void TodoWindow::setUpWindowContentAndActions()
{
    m_qt_addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a ToDo Item", "m_qt_addModelObject", this);
    connect(m_qt_addModelObject, &QPushButton::clicked, this, &TodoWindow::handleAddTodoItem);

    m_qt_modelWindowLayout->addWidget(m_qt_addModelObject);

    m_qt_modelTableView = new QTableView(this);
    m_qt_modelTableView->setObjectName("m_qt_modelTableView");
    tableViewReset(m_qt_modelTableView);

    m_qt_modelWindowLayout->addWidget(m_qt_modelTableView);

    connect(m_qt_modelTableView, &QTableView::clicked, this, &TodoWindow::handleTodoTableClicked);
    connect(m_qt_modelTableView, &QTableView::doubleClicked, this, &TodoWindow::handleTodoTableClicked);
}

void TodoWindow::tableViewReset(QTableView *tableView)
{
    if (m_todoTable)
    {
        delete m_todoTable;
    }
 
    m_todoTable = new GuiDashboardTaskTable(m_userData->getUserID(), parent());
    m_todoTable->setObjectName("m_todoTable");
    m_todoTable->fillTable();

    tableView->setModel(m_todoTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    tableView->clearSelection();
    tableView->clearFocus();
    tableView->update();
}
