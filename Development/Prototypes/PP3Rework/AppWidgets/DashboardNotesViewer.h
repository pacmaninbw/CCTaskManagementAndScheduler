#ifndef DASHBOARDNOTESVIEWER_H
#define DASHBOARDNOTESVIEWER_H

class UserModel;
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
    explicit DashboardNotesViewer(std::size_t userDataPtr, QDate searchDate, QWidget *parent = nullptr);
    void setDate(QDate searchDate);
    void setUserId(std::size_t userDataPtr);
    void setUserIdAndDate(std::size_t userDataPtr, QDate searchDate);
    void updateNoteTable();


private:
    void createTable();
    std::size_t m_DBUserId;
    QDate m_SearchDate;
    DashboardNoteTable* m_NoteTable;
};

#endif // DASHBOARDNOTESVIEWER_H
