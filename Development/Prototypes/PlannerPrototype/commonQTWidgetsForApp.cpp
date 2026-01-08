// Project Header Files
#include "commonQTWidgetsForApp.h"

// QT Header Files
#include <QDateEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QSizePolicy>
#include <QStyleOptionFrame>
#include <QTextEdit>
#include <QWidget>
#include <QLabel>

// Standard C++ Header Files
#include <algorithm>
//#include <format>
//#include <iostream>

static const int maxWidthUndefined = 16777215;
static const int maxHeightUndefind = 16777215;

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
    int desiredWidth = fm.horizontalAdvance(QString(charCount, 'x'));

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

static int getRowMaximumWidth(QFormLayout* layout, int row)
{
    int maxLabelWidth = 0;
    int maxFieldWidth = 0;

    QLayoutItem* labelItem = layout->itemAt(row, QFormLayout::LabelRole);
    QLayoutItem* fieldItem = layout->itemAt(row, QFormLayout::FieldRole);

    if (labelItem) {
        maxLabelWidth = labelItem->maximumSize().width();
    }

    if (fieldItem) {
        maxFieldWidth = fieldItem->maximumSize().width();
    }

    if (maxLabelWidth >= maxWidthUndefined) { 
        maxLabelWidth = labelItem->sizeHint().width();
    }
    if (maxFieldWidth >= maxWidthUndefined) {
        maxFieldWidth = fieldItem->sizeHint().width();
    }

    int horizontalSpacing = layout->horizontalSpacing();
    if (horizontalSpacing < 0) {
        horizontalSpacing = 15;
    }

    return maxLabelWidth + maxFieldWidth + horizontalSpacing;
}

int cqtfa_getFormLayoutMaxWidth(QFormLayout* formToSize)
{
    int maxWidth = 0;

    for (int i = 0; i < formToSize->rowCount(); ++i)
    {
        int tempMaxWidth = getRowMaximumWidth(formToSize, i);
        if (tempMaxWidth > maxWidth)
        {
            maxWidth = tempMaxWidth;
        }
    }

    return maxWidth;
}

int cqtfa_calculateFormLayoutMaxHeight(QFormLayout *formToSize)
{
    int maxHeight = formToSize->verticalSpacing();

    for (int row = 0; row < formToSize->rowCount(); ++row)
    {
        QLayoutItem *fieldItem = formToSize->itemAt(row, QFormLayout::FieldRole);
        int fieldHeight = 0;
        if (fieldItem && fieldItem->widget()) {
            fieldHeight = fieldItem->widget()->maximumHeight();
            if (fieldHeight >= maxHeightUndefind)
            {
                fieldHeight = fieldItem->widget()->height();
            }
        }
        maxHeight += fieldHeight + formToSize->verticalSpacing();
    }

    return maxHeight;
}

