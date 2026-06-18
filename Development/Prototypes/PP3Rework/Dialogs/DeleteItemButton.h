#ifndef DELETEITEMBUTTON_H_
#define DELETEITEMBUTTON_H_

#include <QPushButton>
#include <QString>

class DeleteItemButton : public QPushButton
{
public:
    DeleteItemButton(QString itemType, QWidget* parent = nullptr);
};

#endif // DELETEITEMBUTTON_H_
