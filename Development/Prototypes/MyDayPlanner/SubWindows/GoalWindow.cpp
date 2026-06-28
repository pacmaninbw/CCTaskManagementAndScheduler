
// Project Header
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DefaultGoalDisplayTable.h"
#include "GoalEditorDialog.h"
#include "GoalWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QMainWindow>
#include <QMessageBox>
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
    tableViewReset(m_qt_ModelTableView);
}

void GoalWindow::handleAddGoalAction()
{
    qDebug() << "In GoalWindow::" << __func__;
    GoalEditorDialog addGoalDialog(m_UserData->getUserID(), 0, this);

    addGoalDialog.exec();
    tableViewReset(m_qt_ModelTableView);
}

void GoalWindow::handleEditGoalAction(const QModelIndex &index)
{
    qDebug() << "In GoalWindow::" << __func__;
    if (!index.isValid())
    {
        return;
    }

    std::size_t goalToEdit = index.internalId();

    GoalEditorDialog goalEditDialog(m_UserData->getUserID(), goalToEdit, this);

    goalEditDialog.initEditorFieldsFromDataBase();

    goalEditDialog.exec();

    tableViewReset(m_qt_ModelTableView);
}

void GoalWindow::setUpWindowContentAndActions()
{
    qDebug() << "In GoalWindow::" << __func__;
    m_qt_AddModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a Goal", "m_qt_AddModelObject", this);
    connect(m_qt_AddModelObject, &QPushButton::clicked, this, &GoalWindow::handleAddGoalAction);

    m_qt_ModelWindowLayout->addWidget(m_qt_AddModelObject);

    m_qt_ModelTableView = new QTableView(this);
    m_qt_ModelTableView->setObjectName("m_qt_ModelTableView");

    tableViewReset(m_qt_ModelTableView);

    connect(m_qt_ModelTableView,  &QTableView::clicked, this, &GoalWindow::handleEditGoalAction);
    connect(m_qt_ModelTableView,  &QTableView::doubleClicked, this, &GoalWindow::handleEditGoalAction);

    m_qt_ModelWindowLayout->addWidget(m_qt_ModelTableView);
}

void GoalWindow::tableViewReset(QTableView *tableView)
{
    qDebug() << "In GoalWindow::" << __func__;
    if (m_GoalTable)
    {
        delete m_GoalTable;
    }

    m_GoalTable = new DefaultGoalDisplayTable(m_UserData->getUserID(), this);
    m_GoalTable->setObjectName("m_GoalTable");
    m_GoalTable->refillTable();

    tableView->setModel(m_GoalTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
}
