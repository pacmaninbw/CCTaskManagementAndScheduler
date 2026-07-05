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
    tableViewReset(m_qt_modelTableView);
}

void NotesWindow::handleAddNoteAction()
{
    NoteEditorDialog addNoteDialog(this, m_userData->getUserID());
    addNoteDialog.exec();
    tableViewReset(m_qt_modelTableView);
}

void NotesWindow::handleNoteTableClicked(const QModelIndex &index)
{
    if (!index.isValid())
    {
        return;
    }

    std::size_t noteToEdit = index.internalId();
    NoteEditorDialog editNoteDialog(this, m_userData->getUserID(), noteToEdit);
    editNoteDialog.initEditorFieldsFromDataBase();
    editNoteDialog.exec();
    tableViewReset(m_qt_modelTableView);
}

void NotesWindow::setUpWindowContentAndActions()
{
    m_qt_addModelObject = cqtfa_QTWidgetWithText<QPushButton>("Add a Note", "udAddNotePB", this);
    connect(m_qt_addModelObject, &QPushButton::clicked, this, &NotesWindow::handleAddNoteAction);

    m_qt_modelWindowLayout->addWidget(m_qt_addModelObject);

    m_qt_modelTableView = new QTableView(this);
    m_qt_modelTableView->setObjectName("m_qt_modelTableView");
    tableViewReset(m_qt_modelTableView);

    m_qt_modelWindowLayout->addWidget(m_qt_modelTableView);

    connect(m_qt_modelTableView, &QTableView::clicked, this, &NotesWindow::handleNoteTableClicked);
    connect(m_qt_modelTableView, &QTableView::doubleClicked, this, &NotesWindow::handleNoteTableClicked);
}

void NotesWindow::tableViewReset(QTableView* tableView)
{
    if (m_noteTable)
    {
        delete m_noteTable;
    }

    m_noteTable = createTable();

    tableView->setModel(m_noteTable);
    tableView->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    tableView->update();
    tableView->clearFocus();
}

DashboardNoteTable* NotesWindow::createTable()
{
    DashboardNoteTable* noteTable = new DashboardNoteTable(m_userData->getUserID(), m_dateOfViewToDisplay, parent());
    noteTable->setObjectName("m_noteTable");
    noteTable->fillTable();

    return noteTable;
}


