// Project Header Files
#include "DashboardNoteTable.h"
#include "NoteList.h"
#include "NoteModel.h"
#include "stdChronoToQTConversions.h"
//#include "UserModel.h"

// QT Header Files
#include <QAbstractTableModel>
#include <QDateTime>
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <chrono>
#include <iostream>
#include <memory>
#include <vector>

DashboardNoteTable::DashboardNoteTable(std::size_t userID, QDate searchDate, QObject *parent)
    : QAbstractTableModel(parent),
    m_UserID{userID},
    m_SearchDate{searchDate}
{}

void DashboardNoteTable::setUserRefillTable(std::size_t userID)
{
    m_UserID = userID;

    fillTable();
}

void DashboardNoteTable::fillTable()
{
    if (m_UserID == 0)
    {
        NoteModel_shp noNote = std::make_shared<NoteModel>();
        noNote->setContent("Please log in to see today's notes");
        noNote->setDateAdded(std::chrono::system_clock::now());
        noNote->setLastModified(std::chrono::system_clock::now());
        append(noNote);
        return;
    }

    std::chrono::year_month_day searchDate = qDateToChrono(m_SearchDate);
    NoteList currentUserNoteList;
    NoteListValues userNotes = currentUserNoteList.getDashboardNoteTable(m_UserID, searchDate);

    if (userNotes.empty())
    {
        NoteModel_shp noNote = std::make_shared<NoteModel>();
        noNote->setContent("No notes entered yet");
        noNote->setDateAdded(std::chrono::system_clock::now());
        noNote->setLastModified(std::chrono::system_clock::now());
        append(noNote);
        return;
    }

    for (auto dbNotePtr: userNotes)
    {
        m_data.push_back(dbNotePtr);
    }
}

void DashboardNoteTable::append(std::shared_ptr<NoteModel> noteData)
{
    beginInsertRows(QModelIndex(), m_data.size(), m_data.size());

    m_data.push_back(noteData);

    endInsertRows();

}

void DashboardNoteTable::clearData()
{
    beginResetModel();

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

    return m_data.size();
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

    std::shared_ptr<NoteModel> note = m_data[index.row()];
    switch (index.column()) {
        case 0: {
            QDateTime tempTime(chronoTimePointToQDateTime(note->getDateAdded()));
            return tempTime.toLocalTime().toString("yyyy-MM-dd hh:mm");
        }
        case 1: return QString::fromStdString(note->getContent());
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
        m_data.insert(m_data.begin() + position, nullptr);
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
        NoteModel_shp data = m_data.at(position);
        data.reset();
        m_data.erase(m_data.begin() + position);
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

    NoteModel_shp noteModelItem = m_data[row];
    if (noteModelItem)
    {
        return createIndex(row, column, noteModelItem->getNoteId());
    }

    return QModelIndex();
}
