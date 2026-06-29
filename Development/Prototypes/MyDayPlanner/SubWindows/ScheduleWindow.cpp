// Project Header Files
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "GuiDashboardScheduleTable.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QWidget>

// Standard C++ Header Files
#include <chrono>
#include <memory>

ScheduleWindow::ScheduleWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Schedule:", makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void ScheduleWindow::refresh()
{
    tableViewReset(m_qt_ModelTableView);
}

void ScheduleWindow::handleAddEvent()
{
    ScheduleItemEditorDialog addScheduleItemDialog(m_UserData->getUserID(), 0, this);

    addScheduleItemDialog.initEditorFieldsFromDataBase();

    addScheduleItemDialog.exec();

    tableViewReset(m_qt_ModelTableView);
}

void ScheduleWindow::handleScheduleItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t sheduleItemToEdit = static_cast<std::size_t>(index.internalId());

    ScheduleItemEditorDialog* editScheduleItemDialog;
    if (sheduleItemToEdit)
    {
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_UserData->getUserID(), sheduleItemToEdit, this);
    }
    else
    {
        std::chrono::system_clock::time_point startTime = m_ScheduleTable->getScheduleItemStartTime(index);
        std::chrono::system_clock::time_point endTime = m_ScheduleTable->getScheduleItemEndTime(index);
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_UserData->getUserID(), startTime,  endTime, this);
    }

    editScheduleItemDialog->initEditorFieldsFromDataBase();

    editScheduleItemDialog->exec();

    tableViewReset(m_qt_ModelTableView);
}

void ScheduleWindow::setUpWindowContentAndActions()
{
    m_qt_AddModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add Event to Schedule", "udAddEventPB", this);
    connect(m_qt_AddModelObject, &QPushButton::clicked, this, &ScheduleWindow::handleAddEvent);
    m_qt_ModelWindowLayout->addWidget(m_qt_AddModelObject);

    m_qt_ModelTableView = new QTableView(this);
    m_qt_ModelTableView->setObjectName("m_qt_ModelTableView");
    tableViewReset(m_qt_ModelTableView);

    connect(m_qt_ModelTableView, &QTableView::clicked, this, &ScheduleWindow::handleScheduleItemClicked);
    connect(m_qt_ModelTableView, &QTableView::doubleClicked, this, &ScheduleWindow::handleScheduleItemClicked);

    m_qt_ModelWindowLayout->addWidget(m_qt_ModelTableView);
}

void ScheduleWindow::tableViewReset(QTableView *tableView)
{
    if (m_ScheduleTable)
    {
        delete m_ScheduleTable;
    }

    m_ScheduleTable = new GuiDashboardScheduleTable(m_UserData->getUserID(), m_DateOfViewToDisplay, this);
    m_ScheduleTable->setObjectName("m_ScheduleTable");
    m_ScheduleTable->fillSchedule();

    tableView->setModel(m_ScheduleTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->clearSelection();
    tableView->clearFocus();
}


