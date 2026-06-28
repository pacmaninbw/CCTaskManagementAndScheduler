
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

void ModelSubWindow::changeDataRefreshTable(std::shared_ptr<UserModel> user, QDate dateToShow)
{
    m_DateOfViewToDisplay = dateToShow;
    m_UserData = user;
    refresh();
}

void ModelSubWindow::setUpWindowUi()
{
    m_qt_centralwidget = new QWidget(this);
    m_qt_centralwidget->setObjectName(QString::fromUtf8("m_qt_centralwidget"));

    m_qt_ModelWindowLayout = new QVBoxLayout(m_qt_centralwidget);
    m_qt_ModelWindowLayout->setObjectName("modelWindowLayout");

    resize(m_Width, m_Height);

    if (m_IsSubWindow)
    {
        m_qt_AlternateTitle = cqtfa_QTWidgetWithText<QLabel>(m_WindowTitleText.toUtf8().constData(), "m_qt_AlternateTitle", this);
        m_qt_ModelWindowLayout->addWidget(m_qt_AlternateTitle, 0, Qt::AlignHCenter);
    }
    else {
        setWindowTitle(m_WindowTitleText);
    }

    setUpWindowContentAndActions();

    if (!m_IsSubWindow)
    {
        if (!m_qt_CloseModelWindow)
        {
            m_qt_CloseModelWindow = cqtfa_QTWidgetWithText<QPushButton>("Close Window", "m_qt_CloseModelWindow", this);
            m_qt_ModelWindowLayout->addWidget(m_qt_CloseModelWindow);
            connect(m_qt_CloseModelWindow, &QPushButton::clicked, this, &QWidget::close);
        }
    }

    setCentralWidget(m_qt_centralwidget);
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

