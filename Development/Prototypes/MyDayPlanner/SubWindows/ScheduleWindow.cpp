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
    tableViewReset(m_qt_modelTableView);
}

void ScheduleWindow::handleAddEvent()
{
    ScheduleItemEditorDialog addScheduleItemDialog(m_userData->getUserID(), 0, this);
    addScheduleItemDialog.initEditorFieldsFromDataBase();
    addScheduleItemDialog.exec();
    tableViewReset(m_qt_modelTableView);
}

void ScheduleWindow::handleScheduleItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t sheduleItemToEdit = static_cast<std::size_t>(index.internalId());

    ScheduleItemEditorDialog* editScheduleItemDialog;
    /*
     * If the event (schedule item) is in the database pass in the table row 
     * otherwise the user is creating a new event by clicking the schedule time.
     */
    if (sheduleItemToEdit)
    {
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_userData->getUserID(), sheduleItemToEdit, this);
    }
    else
    {
        std::chrono::system_clock::time_point startTime = m_scheduleTable->getScheduleItemStartTime(index);
        std::chrono::system_clock::time_point endTime = m_scheduleTable->getScheduleItemEndTime(index);
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_userData->getUserID(), startTime,  endTime, this);
    }

    editScheduleItemDialog->initEditorFieldsFromDataBase();
    editScheduleItemDialog->exec();

    tableViewReset(m_qt_modelTableView);
}

void ScheduleWindow::setUpWindowContentAndActions()
{
    m_qt_addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add Event to Schedule", "m_qt_addModelObject", this);
    connect(m_qt_addModelObject, &QPushButton::clicked, this, &ScheduleWindow::handleAddEvent);
    m_qt_modelWindowLayout->addWidget(m_qt_addModelObject);

    m_qt_modelTableView = new QTableView(this);
    m_qt_modelTableView->setObjectName("m_qt_modelTableView");
    tableViewReset(m_qt_modelTableView);

    connect(m_qt_modelTableView, &QTableView::clicked, this, &ScheduleWindow::handleScheduleItemClicked);
    connect(m_qt_modelTableView, &QTableView::doubleClicked, this, &ScheduleWindow::handleScheduleItemClicked);

    m_qt_modelWindowLayout->addWidget(m_qt_modelTableView);
}

void ScheduleWindow::tableViewReset(QTableView *tableView)
{
    if (m_scheduleTable)
    {
        delete m_scheduleTable;
    }

    m_scheduleTable = new GuiDashboardScheduleTable(m_userData->getUserID(), m_dateOfViewToDisplay, this);
    m_scheduleTable->setObjectName("m_scheduleTable");
    m_scheduleTable->fillSchedule();

    tableView->setModel(m_scheduleTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->clearSelection();
    tableView->clearFocus();
}


