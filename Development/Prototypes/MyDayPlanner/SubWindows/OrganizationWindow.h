#ifndef ORGANIZATIONWINDOW_H_
#define ORGANIZATIONWINDOW_H_

class OrganizationModel;

// Project Header Files
#include "ModelSubWindow.h"

// QT Header Files

// Standard C++ Header Files

class OrganizationWindow : public ModelSubWindow
{
public:
    OrganizationWindow(bool makeSubWindow = false, QWidget *parent = nullptr);
    void refresh() override;

Q_SIGNALS:

private Q_SLOTS:

protected:

};

#endif // ORGANIZATIONWINDOW_H_
