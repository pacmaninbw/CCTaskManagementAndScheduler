// Project Header Files
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DashboardNotesViewer.h"
#include "NoteEditorDialog.h"
#include "NotesWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <chrono>
#include <memory>

NotesWindow::NotesWindow(bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Notes:", makeSubWindow, parent)
{}

NotesWindow::NotesWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : NotesWindow(makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void NotesWindow::refresh()
{
    updateNotes();
}

void NotesWindow::handleAddNoteAction()
{
    NoteEditorDialog addNoteDialog(this, m_UserData->getUserID());

    addNoteDialog.exec();

    updateNotes();
}

void NotesWindow::handleNoteTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t noteToEdit = index.internalId();
    NoteEditorDialog editNoteDialog(this, m_UserData->getUserID(), noteToEdit);

    editNoteDialog.exec();

    updateNotes();
}

void NotesWindow::setUpWindowContentAndActions()
{
    addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a Note", "udAddNotePB", this);
    connect(addModelObject, &QPushButton::clicked, this, &NotesWindow::handleAddNoteAction);

    modelWindowLayout->addWidget(addModelObject);

    m_NotesTableView = updateNotes();
    if (m_NotesTableView)
    {
        modelWindowLayout->addWidget(m_NotesTableView);
    }
}

DashboardNotesViewer *NotesWindow::updateNotes()
{
    if (!m_NotesTableView)
    {
        if (m_UserData == nullptr)
        {
            return nullptr;
        }

        m_NotesTableView = new DashboardNotesViewer(m_UserData->getUserID(), m_DateOfViewToDisplay, this);
        m_NotesTableView->setObjectName("m_NotesTableView");
        connect(m_NotesTableView, &QTableView::clicked, this, &NotesWindow::handleNoteTableClicked);
        connect(m_NotesTableView, &QTableView::doubleClicked, this, &NotesWindow::handleNoteTableClicked);
    }

    m_NotesTableView->clearSelection();
    m_NotesTableView->setUserIdAndDate(m_UserData->getUserID(), m_DateOfViewToDisplay);

    m_NotesTableView->update();
    m_NotesTableView->clearFocus();

    return m_NotesTableView;
}

