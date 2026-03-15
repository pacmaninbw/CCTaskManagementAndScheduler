// Project Header Files
#include "DashboardNoteTable.h"
#include "UserModel.h"
#include "DashboardNotesViewer.h"

// QT Header Files
#include <QDate>
#include <QHeaderView>
#include <QTableView>
#include <QWidget>

// Standard C++ Header Files
#include <iostream>

DashboardNotesViewer::DashboardNotesViewer(QWidget *parent)
    : QTableView{parent},
    m_DBUserId{nullptr},
    m_SearchDate{QDate::currentDate()},
    m_NoteTable{nullptr}
{
    createTable();
}

DashboardNotesViewer::DashboardNotesViewer(UserModel *userDataPtr, QDate searchDate, QWidget *parent)
    : QTableView{parent},
    m_DBUserId{userDataPtr},
    m_SearchDate{searchDate},
    m_NoteTable{nullptr}
{
    createTable();
}

void DashboardNotesViewer::setDate(QDate searchDate)
{
    m_SearchDate = searchDate;

    updateNoteTable();
}

void DashboardNotesViewer::setUserId(UserModel *userDataPtr)
{
    m_DBUserId = userDataPtr;

    updateNoteTable();
}

void DashboardNotesViewer::setUserIdAndDate(UserModel *userDataPtr, QDate searchDate)
{
    m_DBUserId = userDataPtr;
    m_SearchDate = searchDate;

    updateNoteTable();
}

void DashboardNotesViewer::updateNoteTable()
{
    if (m_NoteTable)
    {
        delete m_NoteTable;
    }

    createTable();
}

void DashboardNotesViewer::createTable()
{
    m_NoteTable = new DashboardNoteTable(m_DBUserId, m_SearchDate, parent());
    m_NoteTable->setObjectName("m_NoteTable");
    m_NoteTable->fillTable();

    setModel(m_NoteTable);
    horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
}
