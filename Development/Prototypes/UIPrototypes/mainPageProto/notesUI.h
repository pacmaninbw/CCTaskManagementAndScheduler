#ifndef NOTESDIALOGUI_H_
#define NOTESDIALOGUI_H_

#include "createNamedQTWidget.h"
#include <QVariant>
#include <QDialog>
#include <QDialogButtonBox>

class NotesDialogUI : public QDialog
{
    Q_OBJECT

public:
    explicit NotesDialogUI(QWidget* parent = nullptr);
    ~NotesDialogUI() = default;

public slots:

signals:
    void validateNotesDialogUI();

private slots:
    void onAccept();

private:
    void setUpDialogUI();
    void connectDialogButtons();

};

#endif  // NOTESDIALOGUI_H_
