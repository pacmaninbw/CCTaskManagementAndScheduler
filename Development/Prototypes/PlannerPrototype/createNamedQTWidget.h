#ifndef CREATENAMEDQTWIDGET_H_
#define CREATENAMEDQTWIDGET_H_

#include <QWidget>
#include <QDateEdit>
#include <QFormLayout>
#include <QLineEdit>

template <typename WidgetType>
WidgetType *createNamedQTWidget(const char* objectName, QWidget* parent=nullptr)
{
    WidgetType* objectPointer = new WidgetType(parent);
    objectPointer->setObjectName(objectName);

    return objectPointer;
}

/*
 * For buttons and checkboxes.
 */
template <typename WidgetType>
WidgetType *createNameQTWidgetWithText(const char* textContent, const char* objectName, QWidget* parent=nullptr)
{
    WidgetType* objectPointer = new WidgetType(textContent, parent);
    objectPointer->setObjectName(objectName);

    return objectPointer;

}

extern QLineEdit* createNamedLineEditWithWidthAndLength(const char* lineEditName, QWidget* parent=nullptr, std::size_t width=200, std::size_t charCount=20);

extern QFormLayout* createNamedFormLayoutWithPolicy(const char* formName, QWidget* parent=nullptr, QFormLayout::FieldGrowthPolicy policy=QFormLayout::ExpandingFieldsGrow);

extern QDateEdit* createNamedDateEditWithCalendarPopUpCurrentDate(const char* formName, QWidget* parent=nullptr);

#endif // CREATENAMEDQTWIDGET_H_

