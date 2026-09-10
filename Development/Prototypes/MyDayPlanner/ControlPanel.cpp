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
    : QMainWindow(parent)
{
    m_userDataPtr = loggedInUser;
}

ControlPanel::~ControlPanel()
{
}
