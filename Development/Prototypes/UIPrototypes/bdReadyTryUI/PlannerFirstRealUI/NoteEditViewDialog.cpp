#include "NoteEditViewDialog.h"
#include "ui_NoteEditViewDialog.h"

NoteEditViewDialog::NoteEditViewDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::NoteEditViewDialog)
{
    ui->setupUi(this);
}

NoteEditViewDialog::~NoteEditViewDialog()
{
    delete ui;
}
