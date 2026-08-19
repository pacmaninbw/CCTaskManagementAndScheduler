#ifndef COMMONWIDGETEXTENSIONS_H_
#define COMMONWIDGETEXTENSIONS_H_

/* 
 * This file provides functions that provide named QT Widgets. Many of the functions
 * provide layout information or other common features used by the Planner application.
 * 
 * These functions are provided to reduce code in the application GUI files.
 * 
 * Named QT Objects may provide an easier mechanism for connecting events with actions.
 */

#include <QDateEdit>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QWidget>

namespace commonW
{
    template <typename WidgetType>
    WidgetType* namedWidget(const char* objectName, QWidget* parent)
    {
        WidgetType* objectPointer = new WidgetType(parent);
        objectPointer->setObjectName(objectName);

        return objectPointer;
    }

    /*
    * For buttons and checkboxes.
    */
    template <typename WidgetType>
    WidgetType *namedWidgetWithText(const char* textContent, const char* objectName, QWidget* parent)
    {
        WidgetType* objectPointer = new WidgetType(textContent, parent);
        objectPointer->setObjectName(objectName);

        return objectPointer;
    }

    extern QLineEdit* lineEditWidthByLength(const char* lineEditName, QWidget* parent, std::size_t width=200, std::size_t charCount=20);

    /*
    * Provides a fixed width QLineEdit based on the maximum number of characters expected.
    */
    extern QLineEdit* lineEditWidthByCharCount(const char* objName, QWidget* parent, int charcount);

    extern QFormLayout* formLayoutWithPolicy(const char* formName, QWidget* parent,
        QFormLayout::FieldGrowthPolicy policy=QFormLayout::ExpandingFieldsGrow);

    extern QDateEdit* dateEditWithCalendar(const char* formName, QWidget* parent, QDate date = QDate::currentDate());

    extern QPlainTextEdit* flexWidthPlainTextEdit(const char* objName, QWidget* parent, int boxWidthMin, int boxWidthMax, int rowCount);

    extern QPlainTextEdit* flexPlainTextBasedOnCharCount(const char* objName, QWidget* parent, int minCharCount, int maxCharCount, int rowCount);

    extern QTextEdit* flexWidthTextEdit(const char* objName, QWidget* parent, int boxWidthMin, int boxWidthMax, int rowCount);

    extern QTextEdit* flexTextEditBasedOnCharCount(const char* objName, QWidget* parent, int minCharCount, int maxCharCount, int rowCount);

    extern int getMaxWidth(QFormLayout* formToSize);

    extern int getMaxHeight(QFormLayout* formToSize);
}

#endif // COMMONWIDGETEXTENSIONS_H_

