#include "notesUI.h"
#include <QVariant>
#include <QAbstractButton>
#include <QDialog>
#include <QDialogButtonBox>

NotesDialogUI::NotesDialogUI(QWidget *parent) :
    QDialog(parent)
{
    setObjectName(QString::fromUtf8("NotesDialogUI"));

    setUpDialogUI();

/*
 * QMetaObject::connectSlotsByName() does not connect Dialog Button Box buttons
 * or checkboxes. This makes it necessary to connect the buttons and checkboxes
 * explicitly.
 */
    connectDialogButtons();

    QMetaObject::connectSlotsByName(this);
}

void NotesDialogUI::setUpDialogUI()
{
    QString dialogTitle = windowTitle();
    dialogTitle += "TITLE";

    setWindowTitle(dialogTitle);
}


void  NotesDialogUI::connectDialogButtons()
{
}

/*
 * Slots for the widgets.
 */
void NotesDialogUI::onAccept()
{
}


