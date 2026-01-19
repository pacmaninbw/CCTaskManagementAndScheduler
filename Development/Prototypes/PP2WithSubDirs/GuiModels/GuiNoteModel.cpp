// Project Header Files
#include "NoteModel.h"

// QT Header Files
#include "GuiNoteModel.h"
#include "stdChronoToQTConversions.h"

GuiNoteModel::GuiNoteModel(QObject *parent)
    : QObject{parent},
    m_UserId{0},
    m_DBNoteDataptr{nullptr}
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
        emit contentChanged();
    }
}
