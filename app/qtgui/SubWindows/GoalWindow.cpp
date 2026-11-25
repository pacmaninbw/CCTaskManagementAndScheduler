// Project Header Files
#include "CommonWidgetExtensions.h"  // cqtfa_ functions
#include "DefaultGoalDisplayTable.h"
#include "GoalEditorDialog.h"
#include "GoalWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <string>
#include <memory>

GoalWindow::GoalWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Goals:", makeSubWindow, parent)
{
    qDebug() << "In GoalWindow::" << __func__;
    setUser(currentUser);
    setDate(dateToShow);
}

void GoalWindow::refresh()
{
    qDebug() << "In GoalWindow::" << __func__;
    tableViewReset(m_qt_modelTableView);
}

void GoalWindow::handleAddGoalAction()
{
    qDebug() << "In GoalWindow::" << __func__;
    GoalEditorDialog addGoalDialog(m_userData->getUserID(), nullptr, this);
    addGoalDialog.exec();
    tableViewReset(m_qt_modelTableView);
}

void GoalWindow::handleEditGoalAction(const QModelIndex &index)
{
    qDebug() << "In GoalWindow::" << __func__;
    if (!index.isValid())
    {
        return;
    }

    GoalEditorDialog goalEditDialog(m_userData->getUserID(), m_goalTable->getDatabaseObject(index), this);
    goalEditDialog.exec();
    tableViewReset(m_qt_modelTableView);
}

void GoalWindow::setUpWindowContentAndActions()
{
    qDebug() << "In GoalWindow::" << __func__;
    m_qt_addModelObject = common::namedWidgetWithText<QPushButton>("Add a Goal", "m_qt_addModelObject", this);
    connect(m_qt_addModelObject, &QPushButton::clicked, this, &GoalWindow::handleAddGoalAction);

    m_qt_modelWindowLayout->addWidget(m_qt_addModelObject);

    m_qt_modelTableView = new QTableView(this);
    m_qt_modelTableView->setObjectName("m_qt_modelTableView");

    tableViewReset(m_qt_modelTableView);

    connect(m_qt_modelTableView,  &QTableView::clicked, this, &GoalWindow::handleEditGoalAction);
    connect(m_qt_modelTableView,  &QTableView::doubleClicked, this, &GoalWindow::handleEditGoalAction);

    m_qt_modelWindowLayout->addWidget(m_qt_modelTableView);
}

void GoalWindow::tableViewReset(QTableView *tableView)
{
    qDebug() << "In GoalWindow::" << __func__;
    if (m_goalTable)
    {
        delete m_goalTable;
    }

    m_goalTable = new DefaultGoalDisplayTable(m_userData->getUserID(), this);
    m_goalTable->setObjectName("m_goalTable");
    m_goalTable->refillTable();

    tableView->setModel(m_goalTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}
