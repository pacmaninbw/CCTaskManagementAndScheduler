#ifndef MODELSUBWINDOW_H_
#define MODELSUBWINDOW_H_

/*
 * The ModelSubWindow class is a SuperClass. It provides the functionality 
 * that every model window is expected to have such as providing an editor for
 * the model and providing a table to view all pertinent model items,
 * displaying a close button when the window is shown outside the user
 * dashboard and not displaying a close button when the window is shown within
 * the user dashboard.
 * 
 * The ModelSubWindow class is an Abstract class because some of the actions
 * must be defined by the subclasses because they can't be known by the super
 * class.
 * 
 * Actions that are common to all or most windows should be added to this class.
 *
 * The setUpWindowContentAndActions() function provided by the sub class
 * will add necessary visual components such as the table view and editors.
 */
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
    /*
     * The refresh() function is pure because there is no way to provide a
     * common way to refresh the table of models being displayed.
     */
    virtual void refresh() = 0;
    void changeWindowSize(int newWidth, int newHeight);

Q_SIGNALS:

protected Q_SLOTS:

protected:
    const int DefaultSubWindowWidth = 300;
    const int DefaultSubWindowHeight = 400;
    const int DefaultFullWindowWidth = 1000;
    const int DefaultFullWindowHeight = 720;

    virtual void setUpWindowContentAndActions();

    /*
     * The m_isSubWindow variable indicates if the window is being shown in
     * the user dash board or not. This controls what elements need to be
     * displayed.
     */
    bool m_isSubWindow;
    QVBoxLayout* m_qt_modelWindowLayout = nullptr;
    QPushButton* m_qt_addModelObject = nullptr;
    /*
     * To date all sub windows contain a table to be displayed, this may always
     * be true, but if not the m_qt_ModelTableView variable will remain null.
     */
    QTableView* m_qt_modelTableView = nullptr;
    QPushButton* m_qt_closeModelWindow = nullptr;
    QWidget* m_qt_centralwidget = nullptr;
    /*
     * There are 2 modes that this window may appear in, one is a stand alone
     * window and one as a window embedded in the user dashboard. The stand
     * alone window will have a title, as a window embedded in the user
     * dashboard the title will have to be added as a label.
     */
    QLabel* m_qt_alternateTitle = nullptr;

    std::shared_ptr<UserModel> m_userData;
    QDate m_dateOfViewToDisplay;
    QString m_windowTitleText;
    int m_width;
    int m_height;
    int m_lineEditWidth;
    int m_vSpacing = 10;
    int m_hSpacing = 10;
};

#endif // MODELSUBWINDOW_H_
