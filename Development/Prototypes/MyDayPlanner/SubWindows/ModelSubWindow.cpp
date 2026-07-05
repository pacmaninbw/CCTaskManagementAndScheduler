// Project Header Files
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
    m_isSubWindow{makeSubWindow},
    m_userData{nullptr},
    m_windowTitleText{QString::fromStdString(titleText)}
{
    if (makeSubWindow)
    {
        m_width = DefaultSubWindowWidth;
        m_height = DefaultSubWindowHeight;
        m_lineEditWidth = static_cast<int>(0.9 * m_width);
    }
    else
    {
        m_width = DefaultFullWindowWidth;
        m_height = DefaultFullWindowHeight;
        m_lineEditWidth = static_cast<int>(0.9 * m_width);
    }
}

void ModelSubWindow::setDate(QDate dateToShow)
{
    m_dateOfViewToDisplay = dateToShow;
}

void ModelSubWindow::setUser(std::shared_ptr<UserModel> user)
{
    m_userData = user;
}

void ModelSubWindow::changeDataRefreshTable(std::shared_ptr<UserModel> user, QDate dateToShow)
{
    m_dateOfViewToDisplay = dateToShow;
    m_userData = user;
    refresh();
}

void ModelSubWindow::setUpWindowUi()
{
    m_qt_centralwidget = new QWidget(this);
    m_qt_centralwidget->setObjectName(QString::fromUtf8("m_qt_centralwidget"));

    m_qt_modelWindowLayout = new QVBoxLayout(m_qt_centralwidget);
    m_qt_modelWindowLayout->setObjectName("modelWindowLayout");

    resize(m_width, m_height);

    if (m_isSubWindow)
    {
        m_qt_alternateTitle = cqtfa_QTWidgetWithText<QLabel>(m_windowTitleText.toUtf8().constData(), "m_qt_alternateTitle", this);
        m_qt_modelWindowLayout->addWidget(m_qt_alternateTitle, 0, Qt::AlignHCenter);
    }
    else {
        setWindowTitle(m_windowTitleText);
    }

    /*
     * The setUpWindowContentAndActions() function provided by the sub class
     * will add necessary visual components such as the table view and editors.
     */
    setUpWindowContentAndActions();

    if (!m_isSubWindow)
    {
        if (!m_qt_closeModelWindow)
        {
            m_qt_closeModelWindow = cqtfa_QTWidgetWithText<QPushButton>("Close Window", "m_qt_closeModelWindow", this);
            m_qt_modelWindowLayout->addWidget(m_qt_closeModelWindow);
            connect(m_qt_closeModelWindow, &QPushButton::clicked, this, &QWidget::close);
        }
    }

    setCentralWidget(m_qt_centralwidget);
}

void ModelSubWindow::changeWindowSize(int newWidth, int newHeight)
{
    m_width = newWidth;
    m_height = newHeight;
    m_lineEditWidth = static_cast<int>(m_width * 0.90);
}

void ModelSubWindow::setUpWindowContentAndActions()
{
    QString errorReport = "setUpWindowContentAndActions NOT IMPLEMENTED!!!\n";
    QMessageBox::critical(nullptr, "Critical Error", errorReport, QMessageBox::Ok);
}

