// Project Header
#include "CommandLineParser.h"
#include "CommonWidgetExtensions.h"  // cqtfa_ functions
#include "ControlPanel.h"
#include "LoginDialog.h"
#include "NotesWindow.h"
#include "ScheduleWindow.h"
#include "ModelSubWindow.h"
#include "TodoWindow.h"
#include "UserEditorDialog.h"
#include "UserModel.h"

// QT Header Files
#include <QFormLayout>
#include <QHBoxLayout>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files


ControlPanel::ControlPanel(std::shared_ptr<UserModel> loggedInUser, QWidget *parent)
    : QMainWindow(parent),
    m_userDataPtr{loggedInUser},
    m_currentDate{QDate::currentDate()}
{
    m_progNameStr = QString::fromStdString(programOptions.progName);
    m_progNameStr += " : Control Panel";

    setUpControlPanelUi();

    updateViewWindows();
}

ControlPanel::~ControlPanel()
{
}

void ControlPanel::setUpControlPanelUi()
{
    setUpMenuBar();

    m_qt_centralwidget = new QWidget(this);
    m_qt_centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    resize(controlPanelWidth, controlPanelHeight);
}

void ControlPanel::setUpMenuBar()
{

}

void ControlPanel::setUpUserMenu()
{

}

void ControlPanel::setUpViewsMenu()
{

}

void ControlPanel::setUpOrganizationMenu()
{

}

QHBoxLayout* ControlPanel::setUpSubWindowsLayout()
{
    QHBoxLayout* subWindowsLayout = new QHBoxLayout;
    subWindowsLayout->setObjectName("subWindowsLayout");

    for (auto subWindow: m_viewWindows)
    {
        subWindow->setUpWindowUi();
        subWindow->show();
        subWindowsLayout->addWidget(subWindow);
    }

    return subWindowsLayout;
}

void ControlPanel::initViewWindows()
{
    m_viewWindows.push_back(new ScheduleWindow(m_userDataPtr, m_currentDate, true, this));
}

void ControlPanel::updateViewWindows()
{

}

QGroupBox* ControlPanel::setUpUserIdBox()
{
    QGroupBox* userIdGroupBox = new QGroupBox("User ID:", m_qt_centralwidget);
    QHBoxLayout* uiBoxLaytout = new QHBoxLayout;
    uiBoxLaytout->setObjectName("uiBoxLaytout");

    m_qt_userFirstName = common::lineEditWidthByCharCount("m_qt_userFirstName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userFirstName);

    m_qt_userLastName = common::lineEditWidthByCharCount("m_qt_userLastName", m_qt_userIdGroupBox, userNameWidth);
    uiBoxLaytout->addWidget(m_qt_userLastName);

    m_qt_userIdGroupBox->setLayout(uiBoxLaytout);
    m_qt_userIdGroupBox->setGeometry(QRect(10, 10, maxOjectWidth, 90));
    m_qt_userIdGroupBox->setAlignment(Qt::AlignCenter);

    return userIdGroupBox;
}
