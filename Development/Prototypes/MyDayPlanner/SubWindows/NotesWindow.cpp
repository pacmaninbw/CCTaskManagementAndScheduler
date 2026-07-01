// Project Header Files
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "DashboardNoteTable.h"
#include "NoteEditorDialog.h"
#include "NotesWindow.h"
#include "UserModel.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

NotesWindow::NotesWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow, QWidget *parent)
    : ModelSubWindow("Notes:", makeSubWindow, parent)
{
    setUser(currentUser);
    setDate(dateToShow);
}

void NotesWindow::refresh()
{
    tableViewReset(m_qt_ModelTableView);
}

void NotesWindow::handleAddNoteAction()
{
    NoteEditorDialog addNoteDialog(this, m_UserData->getUserID());
    addNoteDialog.exec();
    tableViewReset(m_qt_ModelTableView);
}

void NotesWindow::handleNoteTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t noteToEdit = index.internalId();
    NoteEditorDialog editNoteDialog(this, m_UserData->getUserID(), noteToEdit);
    editNoteDialog.initEditorFieldsFromDataBase();
    editNoteDialog.exec();
    tableViewReset(m_qt_ModelTableView);
}

void NotesWindow::setUpWindowContentAndActions()
{
    m_qt_AddModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a Note", "udAddNotePB", this);
    connect(m_qt_AddModelObject, &QPushButton::clicked, this, &NotesWindow::handleAddNoteAction);

    m_qt_ModelWindowLayout->addWidget(m_qt_AddModelObject);

    m_qt_ModelTableView = new QTableView(this);
    m_qt_ModelTableView->setObjectName("m_qt_ModelTableView");
    tableViewReset(m_qt_ModelTableView);

    m_qt_ModelWindowLayout->addWidget(m_qt_ModelTableView);

    connect(m_qt_ModelTableView, &QTableView::clicked, this, &NotesWindow::handleNoteTableClicked);
    connect(m_qt_ModelTableView, &QTableView::doubleClicked, this, &NotesWindow::handleNoteTableClicked);
}

void NotesWindow::tableViewReset(QTableView* tableView)
{
    if (m_NoteTable)
    {
        delete m_NoteTable;
    }

    m_NoteTable = createTable();

    tableView->setModel(m_NoteTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->update();
    tableView->clearFocus();
}

DashboardNoteTable* NotesWindow::createTable()
{
    DashboardNoteTable* noteTable = new DashboardNoteTable(m_UserData->getUserID(), m_DateOfViewToDisplay, parent());
    noteTable->setObjectName("m_NoteTable");
    noteTable->fillTable();

    return noteTable;
}


