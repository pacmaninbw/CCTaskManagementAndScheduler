#ifndef COMMONQTWIDGETSFORAPP_H_
#define COMMONQTWIDGETSFORAPP_H_

/* 
 * All functions provided by commonQTWidgetsForApp.h and commonQTWidgetsForApp.cpp
 * start with cqtfa_ to identify them.
 * 
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
#include <QTextEdit>
#include <QWidget>

template <typename WidgetType>
WidgetType* cqtfa_QTWidget(const char* objectName, QWidget* parent)
{
    WidgetType* objectPointer = new WidgetType(parent);
    objectPointer->setObjectName(objectName);

    return objectPointer;
}

/*
 * For buttons and checkboxes.
 */
template <typename WidgetType>
WidgetType *cqtfa_QTWidgetWithText(const char* textContent, const char* objectName, QWidget* parent)
{
    WidgetType* objectPointer = new WidgetType(textContent, parent);
    objectPointer->setObjectName(objectName);

    return objectPointer;
}

extern QLineEdit* cqtfa_LineEditWithWidthAndLength(const char* lineEditName, QWidget* parent, std::size_t width=200, std::size_t charCount=20);

/*
 * Provides a fixed width QLineEdit based on the maximum number of characters expected.
 */
extern QLineEdit* cqtfa_LineEditFixedWidthByCharCount(const char* objName, QWidget* parent, int charcount);

extern QFormLayout* cqtfa_FormLayoutWithPolicy(const char* formName, QWidget* parent,
    QFormLayout::FieldGrowthPolicy policy=QFormLayout::ExpandingFieldsGrow);

extern QDateEdit* cqtfa_DateEditWithCalendarPopUpCurrentDate(const char* formName, QWidget* parent);

extern QTextEdit* cqtfa_flexibleWidthTextEdit(const char* objName, QWidget* parent, int boxWidthMin, int boxWidthMax, int rowCount);

extern QTextEdit* cqtfa_flixbleTextEditEbasedOnCharCount(const char* objName, QWidget* parent, int minCharCount, int maxCharCount, int rowCount);

extern int cqtfa_getFormLayoutMaxWidth(QFormLayout* formToSize);

extern int cqtfa_calculateFormLayoutMaxHeight(QFormLayout* formToSize);

#endif // COMMONQTWIDGETSFORAPP_H_

