#ifndef DATABASECONNECTIONDIALOG_H
#define DATABASECONNECTIONDIALOG_H

// Project Header Files

// QT Header Files
#include <QDialog>

// Standard C++ Header Files

class DataBaseConnectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataBaseConnectionDialog(QWidget *parent = nullptr);
    ~DataBaseConnectionDialog();

private:

};

#endif // DATABASECONNECTIONDIALOG_H
