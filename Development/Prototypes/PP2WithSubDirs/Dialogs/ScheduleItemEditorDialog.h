#ifndef SCHEDULEITEMEDITORDIALOG_H
#define SCHEDULEITEMEDITORDIALOG_H

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QCheckBox>
#include <QDateEdit>
#include <QDateTimeEdit>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files


class ScheduleItemEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleItemEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, std::size_t siId=0);
    ~ScheduleItemEditorDialog();

public slots:

signals:

private slots:

private:
    void setUpScheduleItemEditorDialogUI();
    QGroupBox* setUpScheduleTimeControls();
    QDialogButtonBox* setUpScheduleItemButtonBox();
    QDateTimeEdit* createAndInitDateTimeEdit(const char* objName);
    void handleAddItemDate_DateChanged();

    std::size_t userID;
    std::size_t scheduleItemID;

    QVBoxLayout* seid_scheduleItemEditorDialogLayout;
    QDialogButtonBox* sied_buttonBox;
    QGroupBox* sied_scheduleTimeControls;
    QFormLayout* seid_groupBoxLayout;
    QDateEdit* seid_AddItemDate;
    QDateTimeEdit* sied_scheduleItemStartTimeDTEdit;
    QDateTimeEdit* sied_scheduleItemEndTimeDTEdit;
    QTextEdit* sied_scheduleItemTitleTE;
    QTextEdit* sied_locationTE;
    QCheckBox* sied_scheduleItemIsPersonalCB;

    const int sied_TextEditMinWidth = 300;
    const int sied_TextEditMaxWidth = 700;
};

#endif // SCHEDULEITEMEDITORDIALOG_H
