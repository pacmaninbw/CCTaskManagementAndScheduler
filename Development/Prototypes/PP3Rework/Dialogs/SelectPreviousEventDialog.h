#ifndef SELECTPREVIOUSEVENTDIALOG_H
#define SELECTPREVIOUSEVENTDIALOG_H

class ScheduleItemModel;

// Project Header Files

// QT Header Files
#include <QVariant>
#include <QAbstractButton>
#include <QApplication>
#include <QDialog>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QListView>
#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QVBoxLayout>

// Standard C++ Header Files
#include <string>
#include <vector>

class SelectPreviousEventDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectPreviousEventDialog(std::size_t userId, std::vector<std::string> previousEventTitles, QWidget* parent = nullptr);
    ~SelectPreviousEventDialog();
    void setupDialogUI();
    QString getSelectedEvent() { return m_SelectedEvent; };

private Q_SLOTS:
    void handleSelectedEvent_accepted();

private:
    QGroupBox* setUpGroupBox();
    QDialogButtonBox* setUpDialogButtons();
    QVBoxLayout* SelectPreviousEventDialogLayout;
    QDialogButtonBox* buttonBox;
    QGroupBox* selectPreviousEventGroupBox;
    QFormLayout* selectPreviousEventGroupBoxLayout;
    QListView* previousEventSelector;
    std::size_t m_UserID;
    QStringList m_PreviousTitles;
    QString m_SelectedEvent;

    const int defaultDialogWidth = 700;
    const int defaultDialogHeight = 450;
    const int minimumGroupBoxWidth = 320;
    const int minimumGroupBoxHeight = 220;
    const int maximumGroupBoxWidth = 920;
    const int maximumGroupBoxHeight = 520;
    const int parentTableMargin = 10;
    const int marginAllowance = parentTableMargin  * 2;
    const int minimumTableViewWidth = minimumGroupBoxWidth - marginAllowance;
    const int minimumTableViewHeight = minimumGroupBoxHeight - marginAllowance;
    const int maximumTableViewWidth = maximumGroupBoxWidth - marginAllowance;
    const int maximumTableViewHeight = maximumGroupBoxHeight - marginAllowance;
};

#endif // SELECTPREVIOUSEVENTDIALOG_H
