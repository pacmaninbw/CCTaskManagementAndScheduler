#ifndef SCHEDULEITEMEDITORDIALOG_H
#define SCHEDULEITEMEDITORDIALOG_H

// Project Header Files
#include "GuiScheduleItemModel.h"

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
#include <QPlainTextEdit>
#include <QVBoxLayout>

// Standard C++ Header Files


class ScheduleItemEditorDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleItemEditorDialog(QWidget* parent = nullptr, std::size_t userId=0, GuiScheduleItemModel* scheduleItemToEdit=nullptr);
    ~ScheduleItemEditorDialog();

public Q_SLOTS:
    void accept() override;

private Q_SLOTS:
    void handleAddItemDate_DateChanged();
    void handleScheduleItemTitle_TextChanged();

private:
    void setUpScheduleItemEditorDialogUI();
    QGroupBox* setUpScheduleTimeControls();
    QDialogButtonBox* setUpScheduleItemButtonBox();
    QDateTimeEdit* createAndInitDateTimeEdit(const char* objName, QDateTime initValue);
    bool addToDatabase();
    bool udpateDatabase();
    void transferFieldsToDataModel();
    void initEditFields();
    QDateTime initValidDateTime(QDateTime fieldData);

    std::size_t m_UserID;
    GuiScheduleItemModel* m_ScheduleItemData;

    QVBoxLayout* seid_scheduleItemEditorDialogLayout;
    QDialogButtonBox* sied_buttonBox;
    QGroupBox* sied_scheduleTimeControls;
    QFormLayout* seid_groupBoxLayout;
    QDateEdit* seid_AddItemDate;
    QDateTimeEdit* sied_scheduleItemStartTimeDTEdit;
    QDateTimeEdit* sied_scheduleItemEndTimeDTEdit;
    QPlainTextEdit* sied_scheduleItemTitleTE;
    QPlainTextEdit* sied_locationTE;
    QCheckBox* sied_scheduleItemIsPersonalCB;

    const int sied_TextEditMinWidth = 300;
    const int sied_TextEditMaxWidth = 700;
};

#endif // SCHEDULEITEMEDITORDIALOG_H
