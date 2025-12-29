#ifndef USERDASHBOARD_H
#define USERDASHBOARD_H

#include <QMainWindow>

namespace Ui {
class UserDashboard;
}

class UserDashboard : public QMainWindow
{
    Q_OBJECT

public:
    explicit UserDashboard(QWidget *parent = nullptr);
    ~UserDashboard();

private:
    Ui::UserDashboard *ui;
};

#endif // USERDASHBOARD_H
