#ifndef USERWINDOW_H_
#define USERWINDOW_H_

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files

// Standard C++ Header Files
#include <memory>

class UserWindow : public ModelSubWindow
{
public:
    UserWindow(bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;


Q_SIGNALS:

private Q_SLOTS:

protected:

};

#endif // USERWINDOW_H_