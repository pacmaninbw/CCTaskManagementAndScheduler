#ifndef MODELSUBWINDOW_H_
#define MODELSUBWINDOW_H_

class UserModel;

// Project Header Files

// QT Header Files
#include <QDate>
#include <QLabel>
#include <QMainWindow>
#include <QPushButton>
#include <QString>
#include <QTableView>
#include <QVBoxLayout>
#include <QWidget>

// Standard C++ Header Files
#include <memory>
#include <string>

class ModelSubWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ModelSubWindow(std::string titleText, bool makeSubWindow = false, QWidget *parent = nullptr);
    void setDate(QDate dateToShow);
    void setUser(std::shared_ptr<UserModel> user);
    void changeDataRefreshTable(std::shared_ptr<UserModel> user, QDate dateToShow);
    void setUpWindowUi();
    virtual void refresh() = 0;
    void changeWindowSize(int newWidth, int newHeight);

Q_SIGNALS:

protected Q_SLOTS:

protected:
    virtual void setUpWindowContentAndActions();

    bool m_IsSubWindow;
    QVBoxLayout* m_qt_ModelWindowLayout = nullptr;
    QPushButton* m_qt_AddModelObject = nullptr;
    /*
     * To date all sub windows contain a table to be displayed, this may always
     * be true, but if not the m_qt_ModelTableView variable will remain null.
     */
    QTableView* m_qt_ModelTableView = nullptr;
    QPushButton* m_qt_CloseModelWindow = nullptr;
    QWidget* m_qt_centralwidget = nullptr;
    /*
     * There are 2 modes that this window may appear in, one is a stand alone
     * window and one as a window embedded in the user dashboard. The stand
     * alone window will have a title, as a window embedded in the user
     * dashboard the title will have to be added as a label.
     */
    QLabel* m_qt_AlternateTitle = nullptr;

    std::shared_ptr<UserModel> m_UserData;
    QDate m_DateOfViewToDisplay;
    QString m_WindowTitleText;
    int m_Width;
    int m_Height;
    int m_LineEditWidth;
    int m_vSpacing = 10;
    int m_hSpacing = 10;
    const int DefaultSubWindowWidth = 300;
    const int DefaultSubWindowHeight = 400;
    const int DefaultFullWindowWidth = 1000;
    const int DefaultFullWindowHeight = 720;
};

#endif // MODELSUBWINDOW_H_
