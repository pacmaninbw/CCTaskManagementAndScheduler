#ifndef NOTESWINDOW_H_
#define NOTESWINDOW_H_

/*
 * Provide a window where the user can view and edit their notes.
 */
class DashboardNoteTable;
class UserModel;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files
#include <QDate>
#include <QString>
#include <QWidget>

// Standard C++ Header Files
#include <memory>

class NotesWindow : public ModelSubWindow
{
public:
    NotesWindow(std::shared_ptr<UserModel> currentUser, QDate dateToShow, bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:
    void handleAddNoteAction();
    void handleNoteTableClicked(const QModelIndex &index);

protected:
    virtual void setUpWindowContentAndActions() override;
    void tableViewReset(QTableView* tableView);
    DashboardNoteTable* createTable();

    DashboardNoteTable* m_noteTable = nullptr;
};

#endif // NOTESWINDOW_H_
