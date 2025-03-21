#include "taskTodoListUI.h"
#include <QVariant>
#include <QAbstractButton>
#include <QDialog>
#include <QDialogButtonBox>

taskTodoListDialog::taskTodoListDialog(QWidget *parent) :
    QDialog(parent)
{
    setObjectName(QString::fromUtf8("taskTodoListDialog"));

    setUpDialogUI();

/*
 * QMetaObject::connectSlotsByName() does not connect Dialog Button Box buttons
 * or checkboxes. This makes it necessary to connect the buttons and checkboxes
 * explicitly.
 */
    connectDialogButtons();

    QMetaObject::connectSlotsByName(this);
}

void taskTodoListDialog::setUpDialogUI()
{
    QString dialogTitle = windowTitle();
    dialogTitle += "TITLE";

    setWindowTitle(dialogTitle);
}



void  taskTodoListDialog::connectDialogButtons()
{
}

/*
 * Slots for the widgets.
 */
void taskTodoListDialog::onAccept()
{
}


