#include "scheduleUI.h"
#include <QVariant>
#include <QAbstractButton>
#include <QDialog>
#include <QDialogButtonBox>

ScheduleDialogUI::ScheduleDialogUI(QWidget *parent) :
    QDialog(parent)
{
    setObjectName(QString::fromUtf8("ScheduleDialogUI"));

    setUpDialogUI();

/*
 * QMetaObject::connectSlotsByName() does not connect Dialog Button Box buttons
 * or checkboxes. This makes it necessary to connect the buttons and checkboxes
 * explicitly.
 */
    connectDialogButtons();

    QMetaObject::connectSlotsByName(this);
}

void ScheduleDialogUI::setUpDialogUI()
{
}



void  ScheduleDialogUI::connectDialogButtons()
{
}

/*
 * Slots for the widgets.
 */
void ScheduleDialogUI::onAccept()
{
}