// Project Header Files
#include "CommonWidgetExtensions.h"  // cqtfa_ functions
#include "GuiDashboardScheduleTable.h"
#include "ScheduleItemEditorDialog.h"
#include "ScheduleWindow.h"
#include "ScheduleItemModel.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QWidget>

// Standard C++ Header Files
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
    ScheduleItemEditorDialog addScheduleItemDialog(m_userData->getUserID(), nullptr, this);
    // initCompletersFromDB requires database access, it should not be called in the ScheduleItemEditorDialog constructor.
    addScheduleItemDialog.initCompletersFromDB();
    addScheduleItemDialog.exec();

    tableViewReset(m_qt_modelTableView);
}

void ScheduleWindow::handleScheduleItemClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    ScheduleItemModel_shp sheduleItemToEdit = m_scheduleTable->getDatabaseObject(index);
    ScheduleItemEditorDialog editScheduleItemDialog(m_userData->getUserID(), sheduleItemToEdit, this);
    if (sheduleItemToEdit->isInDataBase() == false)
    {
        editScheduleItemDialog.initCompletersFromDB();
    }
    editScheduleItemDialog.exec();

    tableViewReset(m_qt_modelTableView);
}

void ScheduleWindow::setUpWindowContentAndActions()
{
    m_qt_addModelObject = common::namedWidgetWithText<QPushButton>("Add Event to Schedule", "m_qt_addModelObject", this);
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


