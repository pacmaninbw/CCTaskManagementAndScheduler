#ifndef CREATENAMEDQTWIDGET_H_
#define CREATENAMEDQTWIDGET_H_

/*
 * Providing object names makes using QMetaObject::connectSlotsByName() easier.
 * Properly named slots can be easily connected to their signals.
 */

class QWidget;
/*
 * General Widget types.
 */
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

#endif // CREATENAMEDQTWIDGET_H_

