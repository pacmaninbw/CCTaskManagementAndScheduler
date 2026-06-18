// Project Header Files
#include "DeleteItemButton.h"

// QT Header Files
#include <QPalette>
#include <QPushButton>
#include <QString>

// Standard C++ Header Files

DeleteItemButton::DeleteItemButton(QString itemType, QWidget* parent)
    : QPushButton(parent)
{
    QString buttonText = "Delete " + itemType;
    setText(buttonText);
    setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::Button, Qt::red);
    setPalette(palette);
}
