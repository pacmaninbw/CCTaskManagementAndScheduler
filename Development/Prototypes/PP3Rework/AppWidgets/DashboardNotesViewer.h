#ifndef DASHBOARDNOTESVIEWER_H
#define DASHBOARDNOTESVIEWER_H

class GuiUserModel;
class DashboardNoteTable;

// Project Header Files

// QT Header Files
#include <QWidget>
#include <QTableView>
#include <QDate>

// Standard C++ Header Files

class DashboardNotesViewer : public QTableView
{
    Q_OBJECT
public:
    explicit DashboardNotesViewer(QWidget *parent = nullptr);
    explicit DashboardNotesViewer(GuiUserModel* userDataPtr, QDate searchDate, QWidget *parent = nullptr);
    void setDate(QDate searchDate);
    void setUserId(GuiUserModel* userDataPtr);
    void setUserIdAndDate(GuiUserModel* userDataPtr, QDate searchDate);
    void updateNoteTable();


private:
    void createTable();
    GuiUserModel* m_DBUserId;
    QDate m_SearchDate;
    DashboardNoteTable* m_NoteTable;
};

#endif // DASHBOARDNOTESVIEWER_H
