#ifndef SCHEDULEDIALOGUI_H_
#define SCHEDULEDIALOGUI_H_

#include "createNamedQTWidget.h"
#include <QVariant>
#include <QDialog>
#include <QDialogButtonBox>

class ScheduleDialogUI : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleDialogUI(QWidget* parent = nullptr);
    ~ScheduleDialogUI() = default;

public slots:

signals:
    void validateScheduleDialogUI();

private slots:
    void onAccept();

private:
    void setUpDialogUI();
    void connectDialogButtons();

};

#endif  // SCHEDULEDIALOGUI_H_
