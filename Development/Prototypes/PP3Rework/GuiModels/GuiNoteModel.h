#ifndef GUINOTEMODEL_H
#define GUINOTEMODEL_H

class NoteModel;
// Project Header Files

// QT Header Files
#include <QObject>
#include <QString>

// Standard C++ Header Files
#include <memory>

class GuiNoteModel : public QObject
{
    Q_OBJECT
public:
    explicit GuiNoteModel(QObject *parent = nullptr);
    explicit GuiNoteModel(std::shared_ptr<NoteModel> dbNoteDataptr,QObject *parent = nullptr);
    QString getContent() const { return m_Content; };
    void setContent(QString newContent);
    void setUserId(std::size_t userId);
    bool addNote();
    bool updateNote();
    QString getErrorMessages() const { return m_DbErrorMessages; };

signals:
    void contentChanged();
    void userIdChanged();

private:
    std::size_t m_UserId;
    QString m_Content;
    QString m_CreationDateTS;
    QString m_LastUpdateTS;
    std::shared_ptr<NoteModel> m_DBNoteDataptr;
    bool m_FieldsChangedValue;
    QString m_DbErrorMessages;
};

#endif // GUINOTEMODEL_H
