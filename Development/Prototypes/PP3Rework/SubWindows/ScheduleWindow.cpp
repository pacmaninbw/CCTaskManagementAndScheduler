
// Project Header Files
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "ScheduleItemEditorDialog.h"
#include "ScheduleTablerViewer.h"
#include "ScheduleWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <chrono>
#include <memory>

ScheduleWindow::ScheduleWindow(bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Schedule:", makeSubWindow, parent)
{

}

ScheduleWindow::ScheduleWindow(UserModel_shp currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : ScheduleWindow(makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void ScheduleWindow::refresh()
{
    updateSchedule();
}

void ScheduleWindow::handleAddEvent()
{
    ScheduleItemEditorDialog addScheduleItemDialog(m_UserData->getUserID(), this);

    addScheduleItemDialog.initEditFields();

    addScheduleItemDialog.exec();

    updateSchedule();

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
        std::chrono::system_clock::time_point startTime = m_ScheduleTableView->getScheduleItemStartTime(index);
        std::chrono::system_clock::time_point endTime = m_ScheduleTableView->getScheduleItemEndTime(index);
        editScheduleItemDialog = new ScheduleItemEditorDialog(m_UserData->getUserID(), startTime,  endTime, this);
    }

    editScheduleItemDialog->initEditFields();

    editScheduleItemDialog->exec();

    updateSchedule();
}

void ScheduleWindow::setUpWindowContentAndActions()
{
    addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add Event to Schedule", "udAddEventPB", this);
    connect(addModelObject, &QPushButton::clicked, this, &ScheduleWindow::handleAddEvent);
    modelWindowLayout->addWidget(addModelObject);

    modelWindowLayout->addWidget(updateSchedule());
}

ScheduleTablerViewer *ScheduleWindow::updateSchedule()
{
    if (!m_ScheduleTableView)
    {
        m_ScheduleTableView = new ScheduleTablerViewer(this);
        m_ScheduleTableView->setObjectName("udScheduleTableView");
        connect(m_ScheduleTableView, &QTableView::clicked, this, &ScheduleWindow::handleScheduleItemClicked);
        connect(m_ScheduleTableView, &QTableView::doubleClicked, this, &ScheduleWindow::handleScheduleItemClicked);
    }

    m_ScheduleTableView->clearSelection();
    m_ScheduleTableView->setUserIdAndDate(m_UserData->getUserID(), m_DateOfViewToDisplay);
    m_ScheduleTableView->updateSchedule();
    m_ScheduleTableView->clearFocus();

    return m_ScheduleTableView;
}
