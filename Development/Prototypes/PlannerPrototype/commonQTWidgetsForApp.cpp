#include "commonQTWidgetsForApp.h"

QLineEdit* cqtfa_LineEditWithWidthAndLength(const char* lineEditName, QWidget *parent, std::size_t width, std::size_t charCount)
{
    QLineEdit* newLineEdit = cqtfa_QTWidget<QLineEdit>(lineEditName, parent);
    std::string styleSheetStr(std::format("width: {}px;", width));
    newLineEdit->setStyleSheet(styleSheetStr.c_str());
    newLineEdit->setMaxLength(charCount);

    return newLineEdit;
}

QLineEdit *cqtfa_LineEditFixedWidthByCharCount(const char *objName, QWidget *parent, int charCount)
{
    QLineEdit* newLineEdit = cqtfa_QTWidget<QLineEdit>(objName, parent);

    QFontMetrics fm(newLineEdit->font());
    // Using horizontalAdvance for accurate width calculation
    int desiredWidth = fm.horizontalAdvance(QString(charCount, 'x'));

    // Set fixed width and size policy
    newLineEdit->setFixedWidth(desiredWidth);
    newLineEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    newLineEdit->setMaxLength(charCount);

    return newLineEdit;
}

QFormLayout* cqtfa_FormLayoutWithPolicy(const char* formName, QWidget* parent, QFormLayout::FieldGrowthPolicy policy)
{
    QFormLayout* newFormLayout = cqtfa_QTWidget<QFormLayout>(formName, parent);
    newFormLayout->setFieldGrowthPolicy(policy);

    return newFormLayout;
}

QDateEdit *cqtfa_DateEditWithCalendarPopUpCurrentDate(const char *formName, QWidget *parent)
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

QTextEdit *cqtfa_flexibleWidthTextEdit(const char *objName, QWidget *parent, int boxWidthMin, int boxWidthMax, int rowCount)
{
    QTextEdit* newTextEdit = new QTextEdit(parent);

    newTextEdit->setObjectName(objName);

    newTextEdit->setMinimumHeight(newTextEdit->fontMetrics().height() * rowCount);

    newTextEdit->setMaximumHeight(newTextEdit->fontMetrics().height() * (rowCount + 2));

    newTextEdit->setMinimumWidth(boxWidthMin);

    newTextEdit->setMaximumWidth(boxWidthMax);

    newTextEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    return newTextEdit;
}

QTextEdit *cqtfa_flixbleTextEditEbasedOnCharCount(const char *objName, QWidget *parent, int minCharCount, int maxCharCount, int rowCount)
{
    QTextEdit* newTextEdit = new QTextEdit(parent);
    QFontMetrics qfm(newTextEdit->font());

    newTextEdit->setObjectName(objName);

    newTextEdit->setObjectName(objName);

    newTextEdit->setMinimumHeight(qfm.height() * rowCount);

    newTextEdit->setMaximumHeight(qfm.height() * (rowCount + 2));

    newTextEdit->setMinimumWidth(qfm.horizontalAdvance(QString(minCharCount, 'x')));

    newTextEdit->setMaximumWidth(qfm.horizontalAdvance(QString(maxCharCount, 'x')));

    newTextEdit->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);

    return newTextEdit;
}
