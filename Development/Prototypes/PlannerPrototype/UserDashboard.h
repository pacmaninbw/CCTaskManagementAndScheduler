#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

// Project Header Files

// QT Header Files
#include <QMainWindow>

// Standard C++ Header Files

QT_BEGIN_NAMESPACE
namespace Ui {
class UserDashboard;
}
QT_END_NAMESPACE

class UserDashboard : public QMainWindow
{
    Q_OBJECT

public:
    UserDashboard(QWidget *parent = nullptr);
    ~UserDashboard();

private:

};
#endif // USERDASHBOARD_H
