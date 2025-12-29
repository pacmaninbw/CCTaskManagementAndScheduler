#include "createNamedQTWidget.h"

QLineEdit* createNamedLineEditWithWidthAndLength(const char* lineEditName, QWidget *parent, std::size_t width, std::size_t charCount)
{
    QLineEdit* newLineEdit = createNamedQTWidget<QLineEdit>(lineEditName, parent);
    std::string styleSheetStr(std::format("width: {}px;", width));
    newLineEdit->setStyleSheet(styleSheetStr.c_str());
    newLineEdit->setMaxLength(charCount);

    return newLineEdit;

}
