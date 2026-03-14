// Project Header Files
#include "NoteList.h"
#include "DashboardNoteTable.h"
#include "GuiNoteModel.h"
#include "GuiUserModel.h"
#include "stdChronoToQTConversions.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDateTime>
#include <QList>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <iostream>


DashboardNoteTable::DashboardNoteTable(GuiUserModel *userDataPtr, QDate searchDate, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserDataPtr{userDataPtr},
    m_SearchDate{searchDate}
{}

void DashboardNoteTable::setUserRefillTable(GuiUserModel *userDataPtr)
{
    m_UserDataPtr = userDataPtr;

    fillTable();
}

void DashboardNoteTable::fillTable()
{
    if (!m_UserDataPtr || m_UserDataPtr->getDbUserId() == 0)
    {
        NoteModel_shp noNote = std::make_shared<NoteModel>();
        noNote->setContent("Please log in to see today's notes");
        noNote->setDateAdded(std::chrono::system_clock::now());
        noNote->setLastModified(std::chrono::system_clock::now());
        GuiNoteModel* gNoNote = new GuiNoteModel(noNote, this->parent());
        append(gNoNote);
        return;
    }

    std::chrono::year_month_day searchDate = qDateToChrono(m_SearchDate);
    NoteList currentUserNoteList;
    NoteListValues userNotes = currentUserNoteList.getDashboardNoteTable(m_UserDataPtr->getDbUserId(), searchDate);

    if (userNotes.empty())
    {
        NoteModel_shp noNote = std::make_shared<NoteModel>();
        noNote->setContent("No notes entered yet");
        noNote->setDateAdded(std::chrono::system_clock::now());
        noNote->setLastModified(std::chrono::system_clock::now());
        GuiNoteModel* gNoNote = new GuiNoteModel(noNote, this->parent());
        append(gNoNote);
        return;
    }

    for (const auto& dbNotePtr: userNotes)
    {
        GuiNoteModel* modelData = new GuiNoteModel(dbNotePtr, this);
        m_data.append(modelData);
    }
}

void DashboardNoteTable::append(GuiNoteModel *noteData)
{
    beginInsertRows(QModelIndex(), m_data.count(), m_data.count());

    m_data.append(noteData);

    endInsertRows();

}

void DashboardNoteTable::clearData()
{
    beginResetModel();

    qDeleteAll(m_data.begin(), m_data.end());
    m_data.clear();

    endResetModel();
}

QVariant DashboardNoteTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
    switch (section) {
        case 0: return "Added";
        case 1: return "Content";
        default: return {};
    }
}

bool DashboardNoteTable::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        Q_EMIT headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}

int DashboardNoteTable::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return m_data.count();
}

int DashboardNoteTable::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
    {
        return 0;
    }

    return 2;
}

QVariant DashboardNoteTable::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }


    if (role != Qt::DisplayRole && role != Qt::EditRole)
    {
        return {};
    }

    const GuiNoteModel* note = m_data[index.row()];
    switch (index.column()) {
        case 0: return note->getDateAdded();
        case 1: return note->getContent();
        default: return {};
    }
}

bool DashboardNoteTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (data(index, role) != value) {
        // FIXME: Implement me!
        Q_EMIT dataChanged(index, index, {role});
        return true;
    }
    return false;
}

Qt::ItemFlags DashboardNoteTable::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return QAbstractItemModel::flags(index) | Qt::ItemIsEditable; // FIXME: Implement me!
}

bool DashboardNoteTable::insertRows(int position, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginInsertRows(QModelIndex(), position, position + count - 1);

    for (int row = 0; row < count; ++row)
    {
        m_data.insert(position, nullptr);
    }

    endInsertRows();

    return true;

}

bool DashboardNoteTable::removeRows(int position, int count, const QModelIndex &parent)
{
    Q_UNUSED(parent);
    beginRemoveRows(QModelIndex(), position, position + count - 1);

    for (int row = 0; row < count; ++row)
    {
        GuiNoteModel* data = m_data.at(position);
        delete data;
        m_data.removeAt(position);
    }

    endRemoveRows();

    return true;
}

QModelIndex DashboardNoteTable::index(int row, int column, const QModelIndex &parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    GuiNoteModel* noteModelItem = m_data.at(row);
    if (noteModelItem)
    {
        return createIndex(row, column, noteModelItem);
    }

    return QModelIndex();
}
