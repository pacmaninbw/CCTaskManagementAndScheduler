#include "createNamedQTWidget.h"

QLineEdit* createNamedLineEditWithWidthAndLength(const char* lineEditName, QWidget *parent, std::size_t width, std::size_t charCount)
{
    QLineEdit* newLineEdit = createNamedQTWidget<QLineEdit>(lineEditName, parent);
    std::string styleSheetStr(std::format("width: {}px;", width));
    newLineEdit->setStyleSheet(styleSheetStr.c_str());
    newLineEdit->setMaxLength(charCount);

    return newLineEdit;

}

QFormLayout* createNamedFormLayoutWithPolicy(const char* formName, QWidget* parent, QFormLayout::FieldGrowthPolicy policy)
{
    QFormLayout* newFormLayout = createNamedQTWidget<QFormLayout>(formName, parent);
    newFormLayout->setFieldGrowthPolicy(policy);

    return newFormLayout;

}

QDateEdit *createNamedDateEditWithCalendarPopUpCurrentDate(const char *formName, QWidget *parent)
{
    const int TwoWeeks = 14;
    const int OneYear = 365;

    QDateEdit* newDateEdit = new QDateEdit(QDate::currentDate(), parent);
    newDateEdit->setObjectName(formName);

    newDateEdit->setCalendarPopup(true);

    newDateEdit->setDisplayFormat("MM/dd/yyyy");
    
    newDateEdit->setMinimumDate(QDate::currentDate().addDays(-TwoWeeks));
    
    newDateEdit->setMaximumDate(QDate::currentDate().addDays(OneYear));

    return newDateEdit;
}
