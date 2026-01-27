// Project Header Files
#include "NoteModel.h"

// QT Header Files
#include "GuiNoteModel.h"
#include "stdChronoToQTConversions.h"

GuiNoteModel::GuiNoteModel(QObject *parent)
    : QObject{parent},
    m_UserId{0},
    m_DBNoteDataptr{nullptr},
    m_FieldsChangedValue{false}
{}

GuiNoteModel::GuiNoteModel(std::shared_ptr<NoteModel> dbNoteDataptr, QObject *parent)
    : GuiNoteModel(parent)
{
    m_DBNoteDataptr = dbNoteDataptr;
    m_UserId = dbNoteDataptr->getUserId();
    m_Content = QString::fromStdString(dbNoteDataptr->getContent());

    QDateTime tempqdt = chronoTimePointToQDateTime(dbNoteDataptr->getDateAdded());
    m_CreationDateTS = tempqdt.toString(Qt::ISODate);

    tempqdt = chronoTimePointToQDateTime(dbNoteDataptr->getLastModified());
    m_LastUpdateTS = tempqdt.toString(Qt::ISODate);

}

void GuiNoteModel::setContent(QString newContent)
{
    if (m_Content != newContent)
    {
        m_Content = newContent;
        m_FieldsChangedValue = true;
        emit contentChanged();
    }
}

void GuiNoteModel::setUserId(std::size_t userId)
{
    if (m_UserId != userId)
    {
        m_UserId = userId;
        m_FieldsChangedValue = true;
        emit userIdChanged();
    }
}

bool GuiNoteModel::addNote()
{
    NoteModel_shp newNote = std::make_shared<NoteModel>();
    newNote->setUserId(m_UserId);
    newNote->setContent(m_Content.toStdString());

    if (newNote->insert())
    {
        QDateTime tempqdt = chronoTimePointToQDateTime(newNote->getDateAdded());
        m_CreationDateTS = tempqdt.toString(Qt::ISODate);

        tempqdt = chronoTimePointToQDateTime(newNote->getLastModified());
        m_LastUpdateTS = tempqdt.toString(Qt::ISODate);

        m_DBNoteDataptr = newNote;

        return true;
    }

    return false;
}

bool GuiNoteModel::updateNote()
{
    if (!m_FieldsChangedValue)
    {
        return true;
    }

    m_DBNoteDataptr->setContent(m_Content.toStdString());

    if (m_DBNoteDataptr->update())
    {
        QDateTime tempqdt = chronoTimePointToQDateTime(m_DBNoteDataptr->getLastModified());
        m_LastUpdateTS = tempqdt.toString(Qt::ISODate);

        return true;
    }

    return false;
}
