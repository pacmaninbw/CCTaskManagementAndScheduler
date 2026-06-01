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
    void setUpWindowUi();
    virtual void refresh() = 0;
    void changeWindowSize(int newWidth, int newHeight);

Q_SIGNALS:

protected Q_SLOTS:

protected:
    virtual void setUpWindowContentAndActions();

    bool m_IsSubWindow;
    QVBoxLayout* modelWindowLayout;
    QPushButton* addModelObject;
    QPushButton* closeModelWindow;
    QWidget* centralwidget = nullptr;
    QLabel* alternateTitle;

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
