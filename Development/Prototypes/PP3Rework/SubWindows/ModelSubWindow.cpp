
// Project Header
#include "commonQTWidgetsForApp.h"  // cqtfa_ functions
#include "ModelSubWindow.h"

// QT Header Files
#include <QMessageBox>
#include <QVBoxLayout>
#include <QString>

// Standard C++ Header Files
#include <string>

ModelSubWindow::ModelSubWindow(std::string titleText, bool makeSubWindow, QWidget *parent)
 :  QMainWindow{parent},
    m_IsSubWindow{makeSubWindow},
    modelWindowLayout{nullptr},
    addModelObject{nullptr},
    closeModelWindow{nullptr},
    m_UserData{nullptr},
    m_WindowTitleText{QString::fromStdString(titleText)}
{
    if (makeSubWindow)
    {
        m_Width = DefaultSubWindowWidth;
        m_Height = DefaultSubWindowHeight;
        m_LineEditWidth = static_cast<int>(0.9 * m_Width);
    }
    else
    {
        m_Width = DefaultFullWindowWidth;
        m_Height = DefaultFullWindowHeight;
        m_LineEditWidth = static_cast<int>(0.9 * m_Width);
    }
}

void ModelSubWindow::setDate(QDate dateToShow)
{
    m_DateOfViewToDisplay = dateToShow;
}

void ModelSubWindow::setUser(std::shared_ptr<UserModel> user)
{
    m_UserData = user;
}

void ModelSubWindow::setUpWindowUi()
{
    centralwidget = new QWidget(this);
    centralwidget->setObjectName(QString::fromUtf8("centralwidget"));

    modelWindowLayout = new QVBoxLayout(centralwidget);
    modelWindowLayout->setObjectName("modelWindowLayout");

    resize(m_Width, m_Height);

    if (m_IsSubWindow)
    {
        alternateTitle = cqtfa_QTWidgetWithText<QLabel>(m_WindowTitleText.toUtf8().constData(), "", this);
        modelWindowLayout->addWidget(alternateTitle, 0, Qt::AlignHCenter);
    }
    else {
        setWindowTitle(m_WindowTitleText);
    }

    setUpWindowContentAndActions();

    if (!m_IsSubWindow)
    {
        if (!closeModelWindow)
        {
            closeModelWindow = cqtfa_QTWidgetWithText<QPushButton>("Close Window", "closeModelWindow", this);
            modelWindowLayout->addWidget(closeModelWindow);
        }
    }

    setCentralWidget(centralwidget);
}

void ModelSubWindow::changeWindowSize(int newWidth, int newHeight)
{
    m_Width = newWidth;
    m_Height = newHeight;
    m_LineEditWidth = static_cast<int>(m_Width * 0.90);
}

void ModelSubWindow::setUpWindowContentAndActions()
{
    QString errorReport = "setUpWindowContentAndActions NOT IMPLEMENTED!!!\n";
    QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
}

