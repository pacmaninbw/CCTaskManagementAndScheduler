#ifndef NOTESWINDOW_H_
#define NOTESWINDOW_H_

class DashboardNotesViewer;
class UserModel;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QPushButton>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <memory>


class NotesWindow : public ModelSubWindow
{
    Q_OBJECT
public:
    NotesWindow(bool makeSubWindow = false, QWidget *parent = nullptr);
    NotesWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddNoteAction();
    void handleNoteTableClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    DashboardNotesViewer* updateNotes();

    DashboardNotesViewer* m_NotesTableView = nullptr;
};

#endif // NOTESWINDOW_H_
