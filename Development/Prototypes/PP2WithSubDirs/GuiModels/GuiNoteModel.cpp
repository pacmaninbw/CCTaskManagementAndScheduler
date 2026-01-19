// Project Header Files
#include "NoteModel.h"

// QT Header Files
#include "GuiNoteModel.h"

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
}

void GuiNoteModel::setContent(QString newContent)
{
    if (m_Content != newContent)
    {
        m_Content = newContent;
        emit contentChanged();
    }
}
