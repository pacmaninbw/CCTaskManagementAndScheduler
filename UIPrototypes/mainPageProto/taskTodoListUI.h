#ifndef TASKTODOLISTUI_H_
#define TASKTODOLISTUI_H_

#include "createNamedQTWidget.h"
#include <QVariant>
#include <QDialog>
#include <QDialogButtonBox>

class taskTodoListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit taskTodoListDialog(QWidget* parent = nullptr);
    ~taskTodoListDialog() = default;

public slots:

signals:
    void validateTaskTodoListDialog();

private slots:
    void onAccept();

private:
    void setUpDialogUI();
    void connectDialogButtons();

};

#endif  // TASKTODOLISTUI_H_
