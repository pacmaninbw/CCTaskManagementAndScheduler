
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
    m_GoalDisplayTable->setUserRefillGoalTable(m_UserData->getUserID());

    m_GoalDisplayViewer->setModel(m_GoalDisplayTable);
    m_GoalDisplayViewer->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    m_GoalDisplayViewer->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}

void GoalWindow::handleAddGoalAction()
{
    qDebug() << "In GoalWindow::" << __func__;
    GoalEditorDialog addGoalDialog(m_UserData->getUserID(), this);

    addGoalDialog.exec();
    refresh();
}

void GoalWindow::handleEditGoalAction(const QModelIndex &index)
{
    qDebug() << "In GoalWindow::" << __func__;
    if (!index.isValid())
    {
        return;
    }

    std::size_t goalToEdit = index.internalId();

    GoalEditorDialog goalEditDialog(m_UserData->getUserID(), this);

    goalEditDialog.getGoalFromDbInitFields(goalToEdit);

    goalEditDialog.exec();
}

void GoalWindow::setUpWindowContentAndActions()
{
    qDebug() << "In GoalWindow::" << __func__;
    m_qt_AddModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a Goal", "m_qt_AddModelObject", this);
    connect(m_qt_AddModelObject, &QPushButton::clicked, this, &GoalWindow::handleAddGoalAction);

    m_qt_ModelWindowLayout->addWidget(m_qt_AddModelObject);

    m_GoalDisplayViewer = setUpGoalTableView();
    m_qt_ModelWindowLayout->addWidget(m_GoalDisplayViewer);
    connect(m_GoalDisplayViewer,  &QTableView::clicked, this, &GoalWindow::handleEditGoalAction);
    connect(m_GoalDisplayViewer,  &QTableView::doubleClicked, this, &GoalWindow::handleEditGoalAction);

}

QTableView *GoalWindow::setUpGoalTableView()
{
    qDebug() << "In GoalWindow::" << __func__;
    QTableView* goalTableView = new QTableView(this);

    m_GoalDisplayTable = new DefaultGoalDisplayTable(m_UserData->getUserID(), this);
    m_GoalDisplayTable->setObjectName("m_GoalDisplayTable");
    m_GoalDisplayTable->refillTable();

    goalTableView->setModel(m_GoalDisplayTable);
    goalTableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    goalTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    goalTableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);

    return goalTableView;
}
