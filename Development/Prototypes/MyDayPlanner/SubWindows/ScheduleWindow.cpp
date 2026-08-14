// Project Header Files
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
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
#include <chrono>
#include <memory>
#include <optional>

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
        /*
         * If the user is clicking the schedule time provide the schedule item editor
         * with the proper start and end times.
         */
        std::shared_ptr<ScheduleItemModel> scheduledItem = m_scheduleTable->getScheduledItem(index);
        if (scheduledItem != nullptr)
        {
            std::optional<std::chrono::system_clock::time_point> startTime = scheduledItem->getOptionalStartTime();
            std::optional<std::chrono::system_clock::time_point> endTime = scheduledItem->getOptionalEndTime();
            if (startTime.has_value() && endTime.has_value())
            {
                editScheduleItemDialog = new ScheduleItemEditorDialog(m_userData->getUserID(), startTime.value(),  endTime.value(), this);
            }
            else {
                QString errorReport = "Start time or End time not set for empty hour.\n";
                QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
                return;
            }
        }
        else {
            QString errorReport = "No Empty Event at index.\n";
            QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
            return;
        }
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


